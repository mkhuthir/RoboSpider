#include "Leg.h"
#include "Servo.h"

// Default constructor for Leg class
Leg::Leg(){
  coxa = 0;
  femur = 0;
  tibia = 0;
  dxl = nullptr;
}

// Initialize the leg servos
void Leg::init(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Servo* dxlCtrl) {
  coxa  = coxaID;
  femur = femurID;
  tibia = tibiaID;
  dxl   = dxlCtrl;

  dxl->initServo(coxa);     // Initialize coxa servo with limits
  dxl->initServo(femur);    // Initialize femur servo with limits
  dxl->initServo(tibia);    // Initialize tibia servo with limits
}

// Set angles for the leg joints
void Leg::setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle) {
  dxl->goalPosition(coxa, coxaAngle);      // Set coxa angle
  dxl->goalPosition(femur, femurAngle);    // Set femur angle
  dxl->goalPosition(tibia, tibiaAngle);    // Set tibia angle
}

float Leg::getCoxaAngle() {
  // TODO: Replace with real implementation reading from servo
  return 0.0f;
}

float Leg::getFemurAngle() {
  // TODO: Replace with real implementation reading from servo
  return 0.0f;
}

float Leg::getTibiaAngle() {
  // TODO: Replace with real implementation reading from servo
  return 0.0f;
}

void Leg::printJointAngles() {
  Serial.print("Coxa: ");
  Serial.print(getCoxaAngle());
  Serial.print(" | Femur: ");
  Serial.print(getFemurAngle());
  Serial.print(" | Tibia: ");
  Serial.println(getTibiaAngle());
}

// Lift the leg to a default position
void Leg::legUp() {
  setJointAngles(COXA_UP_ANGLE_DEG, FEMUR_UP_ANGLE_DEG, TIBIA_UP_ANGLE_DEG);        // Lift the leg by setting specific angles
}

// Move the leg forward to a default position
void Leg::legDown() {
  setJointAngles(COXA_DOWN_ANGLE_DEG, FEMUR_DOWN_ANGLE_DEG, TIBIA_DOWN_ANGLE_DEG);  // Move the leg down by setting specific angles
}
