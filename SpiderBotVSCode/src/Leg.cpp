#include "Leg.h"
#include "Servo.h"
#include "Config.h"             // Include configuration header

// Default constructor for Leg class
Leg::Leg(){
  coxa  = 0;
  femur = 0;
  tibia = 0;
  dxl   = nullptr;
}

// Initialize the leg servos
void Leg::init(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Servo* dxlCtrl) {
  coxa  = coxaID;
  femur = femurID;
  tibia = tibiaID;
  dxl   = dxlCtrl;

  dxl->init(coxa);     // Initialize coxa servo with limits
  dxl->init(femur);    // Initialize femur servo with limits
  dxl->init(tibia);    // Initialize tibia servo with limits
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

