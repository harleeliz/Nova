#include <Arduino.h>
#include <SerialBT.h>

/***** Motor & Fan Pin Definitions *****/
// ----- Motor A -----
int MotorA_Pin1   = 3;    // GP3
int MotorA_Pin2   = 4;    // GP4
int MotorA_Speed  = 5;    // GP5 (PWM)

// ----- Motor B -----
int MotorB_Pin1   = 6;    // GP6
int MotorB_Pin2   = 7;    // GP7
int MotorB_Speed  = 1;    // GP1 (PWM)

// ----- Motor C -----
int MotorC_Pin1   = 9;    // GP9
int MotorC_Pin2   = 10;   // GP10
int MotorC_Speed  = 2;    // GP2 (PWM)

// ----- FAN CONTROL via Motor Controller Channel -----
int Fan_ENA_Pin = 12;    // GP12
int Fan_IN1_Pin = 13;    // GP13
int Fan_IN2_Pin = 14;    // GP14

int currentMotorSpeed = 180;  // Default Motor Speed
int currentFanSpeed = 180;    // Default Fan Speed

const int MAX_SPEED = 255;  // Maximum PWM value (0-255)

/***** Function Prototypes *****/
void moveForward();
void moveBackward();
void moveRight();
void moveLeft();         
void stopMotors();
void stopMotorC();       
void setMotorSpeed(int speed);
void setFanSpeed(int speed);
void handleCommand(char command);

void setup() {
  Serial.begin(9600);  // USB serial for debugging
  SerialBT.begin();    // Bluetooth device name
  SerialBT.setName("NovaBT");

  pinMode(MotorA_Pin1, OUTPUT);
  pinMode(MotorA_Pin2, OUTPUT);
  pinMode(MotorA_Speed, OUTPUT);

  pinMode(MotorB_Pin1, OUTPUT);
  pinMode(MotorB_Pin2, OUTPUT);
  pinMode(MotorB_Speed, OUTPUT);

  pinMode(MotorC_Pin1, OUTPUT);
  pinMode(MotorC_Pin2, OUTPUT);
  pinMode(MotorC_Speed, OUTPUT);

  pinMode(Fan_ENA_Pin, OUTPUT);
  pinMode(Fan_IN1_Pin, OUTPUT);
  pinMode(Fan_IN2_Pin, OUTPUT);

  digitalWrite(Fan_IN1_Pin, HIGH);
  digitalWrite(Fan_IN2_Pin, LOW);

  stopMotors();
  setFanSpeed(0);
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(command);
    handleCommand(command);
  }
}

void moveForward() {
  digitalWrite(MotorA_Pin1, LOW); 
  digitalWrite(MotorA_Pin2, HIGH);

  digitalWrite(MotorB_Pin1, LOW); 
  digitalWrite(MotorB_Pin2, HIGH);

  stopMotorC();  // Stop Motor C while going straight
  setMotorSpeed(currentMotorSpeed);
}

void moveBackward() {
  digitalWrite(MotorB_Pin1, HIGH); 
  digitalWrite(MotorB_Pin2, LOW);

  digitalWrite(MotorA_Pin1, HIGH); 
  digitalWrite(MotorA_Pin2, LOW);

  stopMotorC();  // Stop Motor C while moving straight
  setMotorSpeed(currentMotorSpeed);
}

void moveRight() {

  
  digitalWrite(MotorB_Pin1, HIGH); 
  digitalWrite(MotorB_Pin2, LOW);

  digitalWrite(MotorC_Pin1, LOW); 
  digitalWrite(MotorC_Pin2, HIGH);

  digitalWrite(MotorA_Pin1, LOW); 
  digitalWrite(MotorA_Pin2, HIGH);

  
  setMotorSpeed(currentMotorSpeed);
}

void moveLeft() {
  
  digitalWrite(MotorA_Pin1, HIGH); 
  digitalWrite(MotorA_Pin2, LOW);

  digitalWrite(MotorB_Pin1, LOW); 
  digitalWrite(MotorB_Pin2, HIGH);

  digitalWrite(MotorC_Pin1, HIGH); 
  digitalWrite(MotorC_Pin2, LOW);

  setMotorSpeed(currentMotorSpeed);
}



//FUNCTION: Stop only Motor C
void stopMotorC() {
  analogWrite(MotorC_Speed, 0);
  digitalWrite(MotorC_Pin1, LOW);
  digitalWrite(MotorC_Pin2, LOW);
}

void stopMotors() {
  analogWrite(MotorA_Speed, 0);
  analogWrite(MotorB_Speed, 0);
  analogWrite(MotorC_Speed, 0);
}

void setMotorSpeed(int speed) {
  analogWrite(MotorA_Speed, speed);
  analogWrite(MotorB_Speed, speed);
  analogWrite(MotorC_Speed, speed);
}

void setFanSpeed(int speed) {
  speed = constrain(speed, 0, MAX_SPEED);
  analogWrite(Fan_ENA_Pin, speed);
}

void handleCommand(char command) {
  if (command == 'w') {
    moveForward();
  } else if (command == 's') {
    moveBackward();
  } else if (command == 'r') {
    moveRight();
  } else if (command == 'l') {    
    moveLeft();
  } else if (command == 'a') {
    setFanSpeed(currentFanSpeed);
  } else if (command == 'd') {
    setFanSpeed(0);
  } else if (command == 'q') {
    stopMotors();
  }
}