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
void Leg::setLeg(int32_t coxaAngle, int32_t femurAngle, int32_t tibiaAngle) {
  servo->goalPosition(coxa, coxaAngle);      // Set coxa angle
  servo->goalPosition(femur, femurAngle);    // Set femur angle
  servo->goalPosition(tibia, tibiaAngle);    // Set tibia angle
}

int32_t Leg::getCoxa() {
  int32_t angle = 0;
  servo->getPresentPositionData(coxa, &angle);
  return angle;
}

int32_t Leg::getFemur() {
  int32_t angle = 0;
  servo->getPresentPositionData(femur, &angle);
  return angle;
}

int32_t Leg::getTibia() {
  int32_t angle = 0;
  servo->getPresentPositionData(tibia, &angle);
  return angle;
}

void Leg::printStatus(Stream& stream) {
  stream.print("Coxa: ");
  stream.print(getCoxa());
  stream.print(" | Femur: ");
  stream.print(getFemur());
  stream.print(" | Tibia: ");
  stream.println(getTibia());
}

