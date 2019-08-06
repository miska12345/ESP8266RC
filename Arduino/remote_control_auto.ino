#include <WiFi.h>

// Pulse-width-modulation pins
// These pins allow you to control motor speed
#define PWM1 23
#define PWM2 22

#define PWM_FREQ 30000
#define PWM_RES 8

// General Purpose Input/Output
// Control motor on/off and direction
#define A0 19
#define A1 18
#define A2 5
#define A3 17

#define DIR_L 19
#define DIR_R 18
// Motor Encoder
#define SA_L 36
#define SA_R 39

#define SB_L 34
#define SB_R 35

// Car specific (mm) (CHANGE!)
#define WHEEL_CIRCUM 60
#define COUNT_PER_REV 380
#define MOTOR_OFFSET 5

// Server config
// Port to listen for connection
// See https://docs.oracle.com/javase/tutorial/networking/sockets/definition.html
#define TCP_PORT 12345

// WiFi configuration
static const char *wfName = "SU-ECE-Lab";         // Your Wi-Fi name
static const char *wfPass = "B9fmvrfe";    // Your Wi-Fi password

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

// Data Field
// GPIOMatrix is a multi-level array
// It is equivalent to uint8_t[2][2]
// Matrix view: +-----------+
//              +-A0-----A1-+
//              +-A2-----A3-+
//              +-----------+
typedef uint8_t GPIOMatrix[2][2];

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

CarMgr *Manager;                      // A pointer to a CarMgr (which we defined earlier)
static WiFiServer Server(TCP_PORT);   // NodeMCU's server instance, the provided TCP_PORT is the port we listen for incoming connections

volatile unsigned long enc_l;  // Num ticks left motor
volatile unsigned long enc_r;  // Num ticks right motor

// These GPIOMatrices are pre-defined to help ease programming
static const GPIOMatrix MATRIX_STOP = {LOW, LOW, LOW, LOW};
static const GPIOMatrix MATRIX_FORWARD =  {HIGH, LOW, HIGH, LOW};
static const GPIOMatrix MATRIX_BACKWARD = {LOW, HIGH, LOW, HIGH};
static const GPIOMatrix MATRIX_FORWARD_LEFT = {LOW, LOW, HIGH, LOW};
static const GPIOMatrix MATRIX_FORWARD_RIGHT = {HIGH, LOW, LOW, LOW};
static const GPIOMatrix MATRIX_BACKWARD_LEFT = {LOW, LOW, LOW, HIGH};
static const GPIOMatrix MATRIX_BACKWARD_RIGHT = {LOW, HIGH, LOW, LOW};

void setup() {
  Serial.begin(115200);   // for Serial output
  Serial.println("Awake");
  WiFiInitialize();     // Initialize the WiFi module
  NodeMCUInitialize();  // Initialize the pins
  Initialize();         // Initialize data structures
  Serial.println("Initialization completed");
  //forward(100);


  while (true) {
    Serial.printf("%d | %d\n", enc_l, enc_r);
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
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  ledcSetup(0, PWM_FREQ, PWM_RES);
  ledcSetup(1, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM1, 0);
  ledcAttachPin(PWM2, 1);
  ledcWrite(0, 255);
  ledcWrite(1, 255);

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
  Manager = new CarMgr;
  if (Manager == NULL) {
    Serial.println("Not enough RAM");
    return;
  }
  Manager->Speed = 255;
  Manager->DIR = Direction::NONE;
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

    // Sample number of encoder ticks
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
  brake();
}

void drive(int power_l, int power_r) {
  power_l = constrain(power_l, -255, 255);
  power_r = constrain(power_r, -255, 255);

  if (power_l < 0) {
    /*
    digitalWrite(A0, LOW);
    digitalWrite(A1, HIGH);
    */
    digitalWrite(DIR_L, LOW);
  } else {
    /*
    digitalWrite(A0, HIGH);
    digitalWrite(A1, LOW);
    */
    digitalWrite(DIR_L, HIGH);
  }

  if (power_r < 0) {
    /*
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
    */
    digitalWrite(DIR_R, LOW);
  } else {
    /*
    digitalWrite(A2, HIGH);
    digitalWrite(A3, LOW);
    */
    digitalWrite(DIR_R, HIGH);
  }
  
  ledcWrite(0, abs(power_l));
  ledcWrite(1, abs(power_r));  
}

void brake() {
  InterpretIns(INS_NULL);  
}

void loop() {
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
      // Go back to 1 if client is still online
    }
    // Client is offline, stop the motors
    InterpretIns(INS_NULL);
  }
}

void InterpretIns(String ins) {
  // This function check to see if ins match any of the known instructions
  // If there is a matching instruction, motor states will be updated based on the instruction
  GPIOMatrix matrix;  // A scheme, used to hold matrix used in the function

  // Note: str.indexOf(b) is a function that returns positive value if a substring (b) appears in str, and -1 otherwise
  // By checking if the return value is not -1, we are certain that the instruction can be interpreted this way
  if (ins.indexOf(INS_FORWARD_PRESSED) != -1) {
    Set(matrix, MATRIX_FORWARD);
    Manager->DIR = Direction::FORWARD;
  } else if (ins.indexOf(INS_BACKWARD_PRESSED) != -1) {
    Set(matrix, MATRIX_BACKWARD);
    Manager->DIR = Direction::BACKWARD;
  } else if (ins.indexOf(INS_LEFT_PRESSED) != -1) {
    if (Manager->DIR != Direction::NONE) {
      Manager->DIR == Direction::FORWARD ? Set(matrix, MATRIX_FORWARD_LEFT) : Set(matrix, MATRIX_BACKWARD_LEFT);
    }
  } else if (ins.indexOf(INS_RIGHT_PRESSED) != -1) {
    if (Manager->DIR != Direction::NONE) {
      Manager->DIR == Direction::FORWARD ? Set(matrix, MATRIX_FORWARD_RIGHT) : Set(matrix, MATRIX_BACKWARD_RIGHT);
    }
  } else if (ins.indexOf(INS_LEFT_RELEASED) != -1 || ins.indexOf(INS_RIGHT_RELEASED) != -1) {
    if (Manager->DIR != Direction::NONE) {
        Manager->DIR == Direction::FORWARD ? Set(matrix,  MATRIX_FORWARD) : Set(matrix, MATRIX_BACKWARD);
    }
  } else if (ins.indexOf(INS_SET_SPEED) != -1) {
    int speed = ins.substring(strlen(INS_SET_SPEED)).toInt();
    Serial.printf("Speed = %d\n", speed);
    Manager->Speed = speed;
    ledcWrite(0, speed);
    ledcWrite(1, speed);
  } else {
    // Unknown instructions are treated as INS_NULL
    Set(matrix, MATRIX_STOP);
    Manager->DIR = Direction::NONE; 
  }
  Apply(matrix);  // Apply the current GPIO scheme to NodeMCU
}

void Apply(const GPIOMatrix matrix) {
    digitalWrite(A0, matrix[0][0]);
    digitalWrite(A1, matrix[0][1]);
    digitalWrite(A2, matrix[1][0]);
    digitalWrite(A3, matrix[1][1]);
}

void Set(GPIOMatrix matrix, const GPIOMatrix copyFrom) {
  matrix[0][0] = copyFrom[0][0];
  matrix[0][1] = copyFrom[0][1];
  matrix[1][0] = copyFrom[1][0];
  matrix[1][1] = copyFrom[1][1];
}
