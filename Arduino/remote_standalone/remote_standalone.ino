#include <WiFi.h>
#include <HCSR04.h>

// Pulse-width-modulation pins
// These pins allow you to control motor speed
#define PWM1 23
#define PWM2 22

#define PWM_FREQ 30000
#define PWM_RES 8

#define DIR_L 19
#define DIR_R 18
// Motor Encoder
#define SA_L 36
#define SA_R 39

#define SB_L 34
#define SB_R 35

// Car specific (mm) (CHANGE!)
#define WHEEL_CIRCUM (65 * PI)
#define COUNT_PER_REV 120
#define MOTOR_OFFSET 5

// Ultra-Sonic sensors
#define SEN_FRONT_TRIG 16
#define SEN_FRONT_ECHO 4

#define SEN_RIGHT_TRIG 14
#define SEN_RIGHT_ECHO 12

#define SEN_LEFT_TRIG 26
#define SEN_LEFT_ECHO 27

#define SPEED 160

#define MIN_DIS_WALL 5
#define MIN_DIS_MOVE 15 

#define LEFT_FORWARD HIGH
#define LEFT_BACKWARD LOW

#define RIGHT_FORWARD LOW
#define RIGHT_BACKWARD HIGH

volatile unsigned long enc_l;  // Num ticks left motor
volatile unsigned long enc_r;  // Num ticks right motor

UltraSonicDistanceSensor sensor_front(SEN_FRONT_TRIG, SEN_FRONT_ECHO);
UltraSonicDistanceSensor sensor_right(SEN_RIGHT_TRIG, SEN_RIGHT_ECHO);
UltraSonicDistanceSensor sensor_left(SEN_LEFT_TRIG, SEN_LEFT_ECHO);

void setup() {
  Serial.begin(115200);   // for Serial output
  Serial.println("Awake");
  NodeMCUInitialize();  // Initialize the pins
  Serial.println("Initialization completed");
}

void NodeMCUInitialize() {
  pinMode(SA_L, INPUT_PULLUP);
  pinMode(SA_R, INPUT_PULLUP);
  pinMode(SB_L, INPUT_PULLUP);
  pinMode(SB_R, INPUT_PULLUP);
  pinMode(DIR_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  ledcSetup(0, PWM_FREQ, PWM_RES);
  ledcSetup(1, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM1, 0);
  ledcAttachPin(PWM2, 1);
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  
  attachInterrupt(digitalPinToInterrupt(SA_L), countLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SA_R), countRight, CHANGE);

  attachInterrupt(digitalPinToInterrupt(SB_L), countLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SB_R), countRight, CHANGE);
}

void countLeft() {
  enc_l++;  
}

void countRight() {
  enc_r++;
}

// From Fred Bot
void forward() {
  unsigned long num_ticks_l;
  unsigned long num_ticks_r;

  bool flag = false;
  bool stuck = false;
  
  // Set initial motor power
  int power_l = SPEED;
  int power_r = SPEED;

  // Used to determine which way to turn to adjust
  unsigned long diff_l;
  unsigned long diff_r;

  // Reset encoder counts
  enc_l = 0;
  enc_r = 0;

  // Remember previous encoder counts
  unsigned long enc_l_prev = enc_l;
  unsigned long enc_r_prev = enc_r;

  int val_f = sensor_front.measureDistanceCm();

  while (val_f < MIN_DIS_MOVE) {
    SetMotorEx(LEFT_BACKWARD, RIGHT_BACKWARD, SPEED, SPEED);
    val_f = sensor_front.measureDistanceCm();
  }
  
  // Drive until one of the encoders reaches desired count
  while (val_f >= MIN_DIS_MOVE) {
    num_ticks_l = enc_l;
    num_ticks_r = enc_r;

    // Drive
    drive(power_l, power_r);

    // Number of ticks counted since last time
    diff_l = num_ticks_l - enc_l_prev;
    diff_r = num_ticks_r - enc_r_prev;

    if (enc_l_prev == num_ticks_l && enc_r_prev == num_ticks_r) {
      if (!flag) {
        flag = true;
      } else {
        BackUnStuck();
      }
    }
    if (enc_l_prev == num_ticks_l) {
      if (!flag) {
        flag = true;
      } else {
        LeftUnStuck();
      }
    }
    if (enc_r_prev == num_ticks_r) {
      if (!flag) {
        flag = true;
      } else {
        RightUnStuck();
      }
    }

    // Store current tick counter for next time
    enc_l_prev = num_ticks_l;
    enc_r_prev = num_ticks_r;

    // If left is faster, slow it down and speed up right
    if ( diff_l > diff_r ) {
      power_l -= MOTOR_OFFSET;
      power_r += MOTOR_OFFSET;
    }

    // If right is faster, slow it down and speed up left
    if ( diff_l < diff_r ) {
      power_l += MOTOR_OFFSET;
      power_r -= MOTOR_OFFSET;
    }

    // Brief pause to let motors respond
    val_f = sensor_front.measureDistanceCm();
    delay(20);
  }
  
  // Brake
  Brake();
  MakeTurn();
}

void LeftUnStuck() {
  unsigned long pre = millis();
  while (sensor_left.measureDistanceCm() < MIN_DIS_MOVE) {
    unsigned long cur = millis();
    if (cur - pre > 3 * 1000) {
      break;
    }
    SetMotorEx(LEFT_BACKWARD, RIGHT_BACKWARD, 0, SPEED);
  }
}

void RightUnStuck() {
  unsigned long pre = millis();
  while (sensor_right.measureDistanceCm() < MIN_DIS_MOVE) {
    unsigned long cur = millis();
    if (cur - pre > 3 * 1000) {
      break;
    }
    SetMotorEx(LEFT_BACKWARD, RIGHT_FORWARD, SPEED, 0);
  }
}

void BackUnStuck() {
  unsigned long pre = millis();
  while (sensor_front.measureDistanceCm() < MIN_DIS_MOVE) {
    unsigned long cur = millis();
    if (cur - pre > 3 * 1000) {
      break;
    }
    SetMotorEx(LEFT_BACKWARD, RIGHT_FORWARD, SPEED, SPEED);
  }
}

void MakeTurn() {
  int val_l = sensor_left.measureDistanceCm();
  int val_r = sensor_right.measureDistanceCm();
   unsigned long pre = millis();
  if (val_l >= val_r) {
    // Turn left
    while (sensor_front.measureDistanceCm() < MIN_DIS_MOVE) {
      unsigned long cur = millis();
      if (cur - pre > 3 * 1000) {
        break;
      }
      SetMotorEx(LEFT_FORWARD, RIGHT_FORWARD, 0, SPEED);
    }
  } else {
    while (sensor_front.measureDistanceCm() < MIN_DIS_MOVE) {
      unsigned long cur = millis();
      if (cur - pre > 3 * 1000) {
        break;
      }
      SetMotorEx(LEFT_FORWARD, RIGHT_FORWARD, SPEED, 0);
    }
  }
  Brake();
}

void drive(int power_l, int power_r) {
  power_l = constrain(power_l, -255, 255);
  power_r = constrain(power_r, -255, 255);

  if (power_l < 0) {
    digitalWrite(DIR_L, LEFT_BACKWARD);
  } else {
    digitalWrite(DIR_L, LEFT_FORWARD);
  }

  if (power_r < 0) {
    digitalWrite(DIR_R, RIGHT_BACKWARD);
  } else {
    digitalWrite(DIR_R, RIGHT_FORWARD);
  }
  
  ledcWrite(0, abs(power_l));
  ledcWrite(1, abs(power_r));  
}

void loop() {
  
  int val_f = sensor_front.measureDistanceCm();
  Serial.printf("front: %d ", val_f);

  int val_l = sensor_left.measureDistanceCm();
  Serial.printf("left: %d ", val_l);

  int val_r = sensor_right.measureDistanceCm();
  Serial.printf("right: %d\n", val_r);

  forward();
}

void SetForward() {
  SetMotorEx(LEFT_FORWARD, RIGHT_FORWARD);
}

void SetBackward() {
  SetMotorEx(LEFT_BACKWARD, RIGHT_BACKWARD);
}

void SetMotorEx(uint8_t left, uint8_t right) {
  digitalWrite(DIR_L, left);
  digitalWrite(DIR_R, right);
}

void SetMotorEx(uint8_t left, uint8_t right, int speed_left, int speed_right) {
  ledcWrite(0, speed_left);
  ledcWrite(1, speed_right);
  digitalWrite(DIR_L, left);
  digitalWrite(DIR_R, right);
}

void UpdateCurSpeed() {
  ledcWrite(0, SPEED);
  ledcWrite(1, SPEED);
}

void Brake() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  digitalWrite(DIR_L, LEFT_BACKWARD);
  digitalWrite(DIR_R, RIGHT_BACKWARD);
}
