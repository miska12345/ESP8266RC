#include <ESP8266WiFi.h>

// Pulse-width-modulation pins
// These pins allow you to control motor speed
#define PWM1 D1
#define PWM2 D2

// General Purpose Input/Output
// Control motor on/off and direction
#define A0 D8
#define A1 D7
#define A2 D6
#define A3 D5

#define LED_L D0
#define LED_R D3

#define AUDIO D4

// GPIOMatrix is a multi-level array
// It is equivalent to uint8_t[2][2]
// Matrix view: +-----------+
//              +-A0-----A1-+
//              +-A2-----A3-+
//              +-----------+
typedef uint8_t GPIOMatrix[2][2];

// Server config
// Port to listen for connection
// See https://docs.oracle.com/javase/tutorial/networking/sockets/definition.html
#define TCP_PORT 12345
#define MAX_FREQUENCY 1000
#define MIN_FREQUENCY 100
#define AUDIO_CHANGE_INTERVAL 800
// WiFi configuration
static const char *wfName = "NAME";         // Your Wi-Fi name
static const char *wfPass = "PASSWORD";    // Your Wi-Fi password

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

#define INS_HORN_PRESSED "HORN_P"
#define INS_HORN_RELEASED "HORN_R"

// Data Field
#define BLINK_INTERVAL 500  // How many sec per blink

// Direction is an abstraction for the direction of motors
// There are 3 states for the car's Direction
//  1. Forward: State where motors roll forward (relative)
//  2. Backward: State where motors roll backward (relative)
//  3. None: An idle state, also a default state
enum Direction {FORWARD, BACKWARD, NONE };

// Struct is a bundle of data

typedef struct horn_st {
  bool Enabled;
  int Frequency;
  int Sign;
  unsigned long LastChange;
} HornMgr;

typedef struct blinker_st {
  bool Instance[2];
  unsigned long PreviousBlink;
  bool LastOn;
} LEDBlinker;

// CarMgr is a name we use for the struct manager_st
// This struct currently holds a field of type Direction
// It is considered best pratice to place related data together in a data structure such as a struct
// Read more: https://www.geeksforgeeks.org/structures-c/
typedef struct manager_st {
  Direction DIR;
  LEDBlinker *Blinker;
  HornMgr *Horn;
} CarMgr;

// Function Declaration

// A function that initialize NodeMCU WiFi
//  Upon return, NodeMCU has been successfully connected to the WiFi
void WiFiInitialize();

// A function that defines INPUT/OUTPUT for various pins
void NodeMCUInitialize();

// A function that initialize Manager(*see below)
void Initialize();

// A function that takes a GPIOMatrix and apply the corresponding change to motors
// @requires matrix != null
void Apply(const GPIOMatrix matrix);

// A function that takes a source GPIOMatrix and a dst GPIOMatrix and copy the content in dst to src
// The content in dst will not be modified while the function executes
void Set(GPIOMatrix src, const GPIOMatrix dst);

// A function that handles remote request based on the parameter ins
// @requires ins != null
void InterpretIns(String ins);

void UpdateBlinker();

void UpdateHorn();

CarMgr *Manager;                      // A pointer to a CarMgr (which we defined earlier)
static WiFiServer server(TCP_PORT);   // NodeMCU's server instance, the provided TCP_PORT is the port we listen for incoming connections

// These GPIOMatrices are pre-defined to help ease programming
static const GPIOMatrix MATRIX_STOP = {LOW, LOW, LOW, LOW};
static const GPIOMatrix MATRIX_FORWARD =  {HIGH, LOW, HIGH, LOW};
static const GPIOMatrix MATRIX_BACKWARD = {LOW, HIGH, LOW, HIGH};
static const GPIOMatrix MATRIX_FORWARD_LEFT = {LOW, LOW, HIGH, LOW};
static const GPIOMatrix MATRIX_FORWARD_RIGHT = {HIGH, LOW, LOW, LOW};
static const GPIOMatrix MATRIX_BACKWARD_LEFT = {LOW, LOW, LOW, HIGH};
static const GPIOMatrix MATRIX_BACKWARD_RIGHT = {LOW, HIGH, LOW, LOW};

void setup() {
  Serial.begin(9600);   // for Serial output
  WiFiInitialize();     // Initialize the WiFi module
  NodeMCUInitialize();  // Initialize the pins
  Initialize();         // Initialize data structures
  Serial.println("Initialization completed");
}

void WiFiInitialize() {
  WiFi.begin(wfName, wfPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());  // Print IP to console
  server.begin();
}

void NodeMCUInitialize() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(LED_L, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(AUDIO, OUTPUT);

}

void Initialize() {
  Manager = (CarMgr*)malloc(sizeof(CarMgr));
  Manager->Blinker = (LEDBlinker*)malloc(sizeof(LEDBlinker));
  Manager->Horn = (HornMgr*)malloc(sizeof(HornMgr));
  Manager->Blinker->PreviousBlink = millis();
  Manager->Blinker->LastOn = false;
  Manager->Blinker->Instance[0] = false;
  Manager->Blinker->Instance[1] = false;
  Manager->Horn->Enabled = false;
  Manager->Horn->Frequency = MAX_FREQUENCY;
  Manager->Horn->LastChange = 0;
  Manager->Horn->Sign = 1;
  Manager->DIR = Direction::NONE;
}

void loop() {
  WiFiClient c = server.available();
  if (c) {
    // There is a connection
    while (c.connected()) {
      // 1. While the client is connected, we continuously receive data from the client
      while (c.available() > 0) {
        // 2. As soon as we reach the end of instruction (by seeing \r), we handle the instruction
        String data = c.readStringUntil('\r');
        c.flush();
        InterpretIns(data);
      }
      UpdateBlinker();
      UpdateHorn();
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
      Manager->Blinker->Instance[0] = true;
      Manager->DIR == Direction::FORWARD ? Set(matrix, MATRIX_FORWARD_LEFT) : Set(matrix, MATRIX_BACKWARD_LEFT);
    }
  } else if (ins.indexOf(INS_RIGHT_PRESSED) != -1) {
    if (Manager->DIR != Direction::NONE) {
      Manager->Blinker->Instance[1] = true;
      Manager->DIR == Direction::FORWARD ? Set(matrix, MATRIX_FORWARD_RIGHT) : Set(matrix, MATRIX_BACKWARD_RIGHT);
    }
  } else if (ins.indexOf(INS_LEFT_RELEASED) != -1 || ins.indexOf(INS_RIGHT_RELEASED) != -1) {
    Manager->Blinker->Instance[0] = false;
    Manager->Blinker->Instance[1] = false;
    if (Manager->DIR != Direction::NONE) {
        Manager->DIR == Direction::FORWARD ? Set(matrix,  MATRIX_FORWARD) : Set(matrix, MATRIX_BACKWARD);
    }
  } else if (ins.indexOf(INS_HORN_PRESSED) != -1) {
    Manager->Horn->Enabled = true;
    if (Manager->DIR != Direction::NONE) {
      Manager->DIR == Direction::FORWARD ? Set(matrix, MATRIX_FORWARD) : Set(matrix, MATRIX_BACKWARD);
    }
  } else if (ins.indexOf(INS_HORN_RELEASED) != -1) {
    Manager->Horn->Enabled = false;
    if (Manager->DIR != Direction::NONE) {
      Manager->DIR == Direction::FORWARD ? Set(matrix, MATRIX_FORWARD) : Set(matrix, MATRIX_BACKWARD);
    }
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

void UpdateBlinker() {
  unsigned long curTime = millis();
  if (!Manager->Blinker->Instance[0]) {
    digitalWrite(LED_L, LOW);
  }
  if (!Manager->Blinker->Instance[1]) {
    digitalWrite(LED_R, LOW);
  }
  if (curTime - Manager->Blinker->PreviousBlink < BLINK_INTERVAL) {
    return;
  }
  if (Manager->Blinker->Instance[0]) {
    if (Manager->Blinker->LastOn) {
      digitalWrite(LED_L, LOW);
      Manager->Blinker->LastOn = false;
    } else {
      digitalWrite(LED_L, HIGH);
      Manager->Blinker->LastOn = true;
    }
    Manager->Blinker->PreviousBlink = curTime;
  }
  if (Manager->Blinker->Instance[1]) {
    if (Manager->Blinker->LastOn) {
      Manager->Blinker->LastOn = false;
      digitalWrite(LED_R, LOW);
    } else {
      Manager->Blinker->LastOn = true;
      digitalWrite(LED_R, HIGH);
    }
    Manager->Blinker->PreviousBlink = curTime;
  }
}

void UpdateHorn() {
  if (!Manager->Horn->Enabled) {
    analogWrite(AUDIO, 0);
    return;
  }
  unsigned long curTime = millis();
  if (curTime - Manager->Horn->LastChange >= AUDIO_CHANGE_INTERVAL) {
    if (Manager->Horn->Frequency < MIN_FREQUENCY) {
      Manager->Horn->Frequency = MIN_FREQUENCY;
      Manager->Horn->Sign = -1;
    } else if (Manager->Horn->Frequency > MAX_FREQUENCY) {
      Manager->Horn->Frequency = MAX_FREQUENCY;
      Manager->Horn->Sign = 1;
    }
    Manager->Horn->Frequency -= Manager->Horn->Sign * 50;
    Manager->Horn->LastChange = curTime;
    analogWrite(AUDIO, Manager->Horn->Frequency);
  }
}
