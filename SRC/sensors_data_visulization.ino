#include <AFMotor.h>
#include <Wire.h>

// --- MOTOR SETUP ---
AF_DCMotor leftFront(1); AF_DCMotor leftRear(2);
AF_DCMotor rightFront(3); AF_DCMotor rightRear(4);

#define LDR_FRONT A0
#define LDR_BACK  A1
#define LDR_LEFT  A2
#define LDR_RIGHT A3
#define SLAVE_ADDRESS 0x08 

uint8_t speed = 200;      
float threshold = 2.5; 

// We use an array to store the 4 float voltages
// Volatile is important because it's accessed in the I2C interrupt
volatile float voltages[4] = {0.0, 0.0, 0.0, 0.0}; 

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent); 
  stopAll();
}

void loop() {
  // 1. Read and Convert to Voltage directly
  float f = analogRead(LDR_FRONT) * (5.0 / 1023.0);
  float b = analogRead(LDR_BACK)  * (5.0 / 1023.0);
  float l = analogRead(LDR_LEFT)  * (5.0 / 1023.0);
  float r = analogRead(LDR_RIGHT) * (5.0 / 1023.0);

  // 2. Update the shared array safely
  noInterrupts();
  voltages[0] = f; // Front
  voltages[1] = b; // Back
  voltages[2] = l; // Left
  voltages[3] = r; // Right
  interrupts();
  
  // --- MOVEMENT LOGIC ---
  float maxVal = f;
  if (b > maxVal) maxVal = b;
  if (l > maxVal) maxVal = l;
  if (r > maxVal) maxVal = r;

  if (maxVal < threshold) stopAll();
  else if (maxVal == f) {
    if (l > threshold) diagForwardLeft();
    else if (r > threshold) diagForwardRight();
    else forward();
  }
  else if (maxVal == b) {
    if (l > threshold) diagBackwardLeft();
    else if (r > threshold) diagBackwardRight();
    else backward();
  }
  else if (maxVal == l) {
    if (f > threshold) diagForwardLeft();
    else if (b > threshold) diagBackwardLeft();
    else strafeLeft();
  }
  else if (maxVal == r) {
    if (f > threshold) diagForwardRight();
    else if (b > threshold) diagBackwardRight();
    else strafeRight();
  }

  delay(50); 
}

void requestEvent() {
  // Send the 16 bytes representing the 4 floats
  Wire.write((byte*)voltages, 16);
}

// ... Keep your motor functions (forward, backward, etc.) exactly as they were ...
void forward() { setSpeedAll(speed); leftFront.run(FORWARD); leftRear.run(FORWARD); rightFront.run(FORWARD); rightRear.run(FORWARD); }
void backward() { setSpeedAll(speed); leftFront.run(BACKWARD); leftRear.run(BACKWARD); rightFront.run(BACKWARD); rightRear.run(BACKWARD); }
void strafeLeft() { setSpeedAll(speed); leftFront.run(BACKWARD); leftRear.run(FORWARD); rightFront.run(FORWARD); rightRear.run(BACKWARD); }
void strafeRight() { setSpeedAll(speed); leftFront.run(FORWARD); leftRear.run(BACKWARD); rightFront.run(BACKWARD); rightRear.run(FORWARD); }
void diagForwardLeft() { setSpeedAll(speed); leftFront.run(RELEASE); leftRear.run(FORWARD); rightFront.run(FORWARD); rightRear.run(RELEASE); }
void diagForwardRight() { setSpeedAll(speed); leftFront.run(FORWARD); leftRear.run(RELEASE); rightFront.run(RELEASE); rightRear.run(FORWARD); }
void diagBackwardLeft() { setSpeedAll(speed); leftFront.run(RELEASE); leftRear.run(BACKWARD); rightFront.run(BACKWARD); rightRear.run(RELEASE); }
void diagBackwardRight() { setSpeedAll(speed); leftFront.run(BACKWARD); leftRear.run(RELEASE); rightFront.run(RELEASE); rightRear.run(BACKWARD); }
void stopAll() { leftFront.run(RELEASE); leftRear.run(RELEASE); rightFront.run(RELEASE); rightRear.run(RELEASE); }
void setSpeedAll(uint8_t s) { leftFront.setSpeed(s); leftRear.setSpeed(s); rightFront.setSpeed(s); rightRear.setSpeed(s); }