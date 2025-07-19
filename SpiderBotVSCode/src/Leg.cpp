#include "Leg.h"
#include "Servo.h"
#include "Config.h"             // Include configuration header

// Default constructor for Leg class
Leg::Leg(){
  coxa  = 0;
  femur = 0;
  tibia = 0;
  servo = nullptr;
}

// Initialize the leg servos
void Leg::init(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Servo* servo) {
  coxa      = coxaID;
  femur     = femurID;
  tibia     = tibiaID;
  this->servo = servo;  // Set the servo pointer

  servo->init(coxa,   COXA_HOME_DEG,  COXA_VELOCITY);     // Initialize coxa servo with limits
  servo->init(femur,  FEMUR_HOME_DEG, FEMUR_VELOCITY);   // Initialize femur servo with limits
  servo->init(tibia,  TIBIA_HOME_DEG, TIBIA_VELOCITY);   // Initialize tibia servo with limits
}

// Set angles for the leg joints
void Leg::setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle) {
  servo->goalPosition(coxa, coxaAngle);      // Set coxa angle
  servo->goalPosition(femur, femurAngle);    // Set femur angle
  servo->goalPosition(tibia, tibiaAngle);    // Set tibia angle
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

