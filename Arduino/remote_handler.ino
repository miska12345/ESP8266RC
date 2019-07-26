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

// Server config
// Port to listen for connection
// See https://docs.oracle.com/javase/tutorial/networking/sockets/definition.html
#define TCP_PORT 12345

// WiFi configuration
static const char *wfName = "name";         // Your Wi-Fi name
static const char *wfPass = "pass";    // Your Wi-Fi password

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
  Direction DIR;
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

CarMgr *Manager;                      // A pointer to a CarMgr (which we defined earlier)
static WiFiServer Server(TCP_PORT);   // NodeMCU's server instance, the provided TCP_PORT is the port we listen for incoming connections

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
  Server.begin();
}

void NodeMCUInitialize() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  analogWrite(PWM1, 1024);
  analogWrite(PWM2, 1024);
}

void Initialize() {
  Manager = new CarMgr;
  if (Manager == NULL) {
    Serial.println("Not enough RAM");
    return;
  }
  Manager->DIR = Direction::NONE;
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
