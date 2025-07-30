#include "Leg.h"
#include "Servo.h"
#include "Config.h"     // Include configuration header
#include "LegPoses.h"   // Include leg poses header

// Default constructor for Leg class
Leg::Leg(){
  // Initialize leg IDs to zero
  legIDs[Coxa]  = 0;
  legIDs[Femur] = 0;
  legIDs[Tibia] = 0;
  servo = nullptr;
}

// Initialize the leg servos
void Leg::init(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Servo* servo) {
  legIDs[Coxa]  = coxaID;   // Set coxa ID
  legIDs[Femur] = femurID;  // Set femur ID
  legIDs[Tibia] = tibiaID;  // Set tibia ID

  this->servo = servo;  // Set the servo pointer

  servo->init(legIDs[Coxa] , LEG_VELOCITY);   // Initialize coxa servo with velocity
  servo->init(legIDs[Femur], LEG_VELOCITY);   // Initialize femur servo with velocity
  servo->init(legIDs[Tibia], LEG_VELOCITY);   // Initialize tibia servo with velocity
}

// Move the leg to the specified positions
void Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  servo->syncWrite(handler_index, legIDs, LEG_SERVOS, positions, num_positions);
}

// Move leg up
void Leg::moveUp() {
  move(poseLegUp);
}

// Move leg down
void Leg::moveDown() {
  move(poseLegDown);
}

// Move leg out
void Leg::moveOut() {
  move(poseLegOut);
}

// Check if any servo in the leg is currently moving
bool Leg::isMoving() {
  uint8_t moving = 0;
  for (int i = 0; i < LEG_SERVOS; ++i) {
    if (servo->isMoving(legIDs[i])) {
      return true;  // If any servo is moving, return true
    }
  }
  return false;
}

// Get current coxa angle
int32_t Leg::getCoxa() {
  int32_t angle = 0;
  servo->getPresentPositionData(legIDs[Coxa], &angle);
  return angle;
}

// Get current femur angle
int32_t Leg::getFemur() {
  int32_t angle = 0;
  servo->getPresentPositionData(legIDs[Femur], &angle);
  return angle;
}

// Get current tibia angle
int32_t Leg::getTibia() {
  int32_t angle = 0;
  servo->getPresentPositionData(legIDs[Tibia], &angle);
  return angle;
}

// Print current joint angles
void Leg::printStatus(Stream& stream) {
  stream.print("Coxa: ");
  stream.print(getCoxa());
  stream.print(" | Femur: ");
  stream.print(getFemur());
  stream.print(" | Tibia: ");
  stream.println(getTibia());
}



