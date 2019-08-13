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
#define WHEEL_CIRCUM 60
#define COUNT_PER_REV 100
#define MOTOR_OFFSET 5

// Ultra-Sonic sensors
#define SEN_FRONT_TRIG 16
#define SEN_FRONT_ECHO 4


// Server config
// Port to listen for connection
// See https://docs.oracle.com/javase/tutorial/networking/sockets/definition.html
#define TCP_PORT 12345

// WiFi configuration
static const char *wfName = "LINT";         // Your Wi-Fi name
static const char *wfPass = "wassup123";    // Your Wi-Fi password

// InsCode Definition
// InsCode are all instruction send by remote client (i.e. App)
// Values defined here are of type String
// Other encodings, such as integer, are possible
#define INS_FORWARD_PRESSED "FORWARD_P"
#define INS_BACKWARD_PRESSED "BACKWARD_P"
#define INS_LEFT_PRESSED "LEFT_P"
#define INS_LEFT_RELEASED "LEFT_R"
#define INS_RIGHT_PRESSED "RIGHT_P"
#define INS_RIGHT_RELEASED "RIGHT_R"
#define INS_NULL "STOP"

#define INS_SET_SPEED "SPEED="

#define INS_UPDATE "UPDATE"

#define INIT_SPEED 200
// Direction is an abstraction for the direction of motors
// There are 3 states for the car's Direction
//  1. Forward: State where motors roll forward (relative)
//  2. Backward: State where motors roll backward (relative)
//  3. None: An idle state, also a default state
enum Direction {FORWARD, BACKWARD, NONE };

// Struct is a bundle of data
// CarMgr is a name we use for the struct manager_st
// This struct currently holds a field of type Direction
// It is considered best pratice to place related data together in a data structure such as a struct
// Read more: https://www.geeksforgeeks.org/structures-c/
typedef struct manager_st {
  int Speed;
  Direction DIR;
} CarMgr;

#define LEFT_FORWARD HIGH
#define LEFT_BACKWARD LOW

#define RIGHT_FORWARD LOW
#define RIGHT_BACKWARD HIGH

CarMgr *Manager;                      // A pointer to a CarMgr (which we defined earlier)
static WiFiServer Server(TCP_PORT);   // NodeMCU's server instance, the provided TCP_PORT is the port we listen for incoming connections

volatile unsigned long enc_l;  // Num ticks left motor
volatile unsigned long enc_r;  // Num ticks right motor

UltraSonicDistanceSensor sensor_front(SEN_FRONT_TRIG, SEN_FRONT_ECHO);
int avg_front_dis = 0;
int front_samples = 0;

void setup() {
  Serial.begin(115200);   // for Serial output
  Serial.println("Awake");
  WiFiInitialize();     // Initialize the WiFi module
  NodeMCUInitialize();  // Initialize the pins
  Initialize();         // Initialize data structures
  Serial.println("Initialization completed");
  //forward(100);

  while (true) {
    MoveUntiilWall();
  }
  
}

void WiFiInitialize() {
  WiFi.begin(wfName, wfPass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());  // Print IP to console
  Server.begin();
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

void Initialize() {
  Manager = (CarMgr*)malloc(sizeof(CarMgr));
  if (Manager == NULL) {
    Serial.println("Not enough RAM");
    return;
  }
  Manager->Speed = INIT_SPEED;
  Manager->DIR = Direction::NONE;
}

void MoveUntiilWall() {
  bool outlier = false;
    int dis = sensor_front.measureDistanceCm();
    while (dis > 20) {
      forward(2);
      dis = sensor_front.measureDistanceCm();
    }
    Serial.println("done");
}

// From Fred Bot
void forward(float dist) {
  unsigned long num_ticks_l;
  unsigned long num_ticks_r;

  // Set initial motor power
  int power_l = Manager->Speed;
  int power_r = Manager->Speed;

  // Used to determine which way to turn to adjust
  unsigned long diff_l;
  unsigned long diff_r;

  // Reset encoder counts
  enc_l = 0;
  enc_r = 0;

  // Remember previous encoder counts
  unsigned long enc_l_prev = enc_l;
  unsigned long enc_r_prev = enc_r;

  // Calculate target number of ticks
  float num_rev = (dist * 10) / WHEEL_CIRCUM;  // Convert to mm
  unsigned long target_count = num_rev * COUNT_PER_REV;
  
  // Debug
  Serial.print("Driving for ");
  Serial.print(dist);
  Serial.print(" cm (");
  Serial.print(target_count);
  Serial.print(" ticks)");

  // Drive until one of the encoders reaches desired count
  while ( (enc_l < target_count) && (enc_r < target_count) ) {
    int dis = sensor_front.measureDistanceCm();
    if (dis <= 10) {
      break;
    }
    num_ticks_l = enc_l;
    num_ticks_r = enc_r;

    // Print out current number of ticks
    Serial.print(num_ticks_l);
    Serial.print("\t");
    Serial.println(num_ticks_r);

    // Drive
    drive(power_l, power_r);

    // Number of ticks counted since last time
    diff_l = num_ticks_l - enc_l_prev;
    diff_r = num_ticks_r - enc_r_prev;

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
    delay(20);
  }

  // Brake
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
  //Serial.println(sensor_front.measureDistanceCm());
  //delay(20);
  WiFiClient c = Server.available();
  if (c) {
    // There is a connection
    while (c.connected()) {
      // 1. While the client is connected, we continuously receive data from the client
      while (c.available() > 0) {
        // 2. As soon as we reach the end of instruction (by seeing \r), we handle the instruction
        String data = c.readStringUntil('\r');
        InterpretIns(data); 
      }
      if (Manager->DIR != Direction::NONE && !CanMoveForward()) {
        Brake();
      }
      // Go back to 1 if client is still online
    }
    // Client is offline, stop the motors
    InterpretIns(INS_NULL);
  }
}

void InterpretIns(String ins) {
  // Note: str.indexOf(b) is a function that returns positive value if a substring (b) appears in str, and -1 otherwise
  // By checking if the return value is not -1, we are certain that the instruction can be interpreted this way
  if (ins.indexOf(INS_FORWARD_PRESSED) != -1) {
    if (CanMoveForward()) {
      UpdateCurSpeed();
      SetForward();
      Manager->DIR = Direction::FORWARD;  
    }
    
  } else if (ins.indexOf(INS_BACKWARD_PRESSED) != -1) {
    UpdateCurSpeed();
    SetBackward();
    Manager->DIR = Direction::BACKWARD;
  } else if (ins.indexOf(INS_LEFT_PRESSED) != -1) {
    if (Manager->DIR != Direction::NONE) {
      Manager->DIR == Direction::FORWARD ? SetMotorEx(LEFT_FORWARD, RIGHT_FORWARD, 0, Manager->Speed) : SetMotorEx(LEFT_FORWARD, RIGHT_BACKWARD, 0, Manager->Speed);
    }
  } else if (ins.indexOf(INS_RIGHT_PRESSED) != -1) {
    if (Manager->DIR != Direction::NONE) {
      Manager->DIR == Direction::FORWARD ? SetMotorEx(LEFT_FORWARD, RIGHT_BACKWARD, Manager->Speed, 0) : SetMotorEx(LEFT_BACKWARD, RIGHT_BACKWARD, Manager->Speed, 0);
    }
  } else if (ins.indexOf(INS_LEFT_RELEASED) != -1 || ins.indexOf(INS_RIGHT_RELEASED) != -1) {
    if (Manager->DIR != Direction::NONE) {
        UpdateCurSpeed();
        Manager->DIR == Direction::FORWARD ? SetForward() : SetBackward();
    }
  } else if (ins.indexOf(INS_SET_SPEED) != -1) {
    int speed = ins.substring(strlen(INS_SET_SPEED)).toInt();
    Serial.printf("Speed = %d\n", speed);
    Manager->Speed = speed;
    UpdateCurSpeed();
  } else {
    // Unknown instructions are treated as INS_NULL
    Brake();
    Manager->DIR = Direction::NONE; 
  }
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
  ledcWrite(0, Manager->Speed);
  ledcWrite(1, Manager->Speed);
}

void Brake() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

bool CanMoveForward() {
  return true;
}
