#include "Leg.h"
#include "Servo.h"
#include "Config.h"     // Include configuration header
#include "LegPoses.h"   // Include leg poses header

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

  servo->init(coxa , LEG_VELOCITY );   // Initialize coxa servo with velocity
  servo->init(femur, LEG_VELOCITY);   // Initialize femur servo with velocity
  servo->init(tibia, LEG_VELOCITY);   // Initialize tibia servo with velocity
}

// Move the leg to the specified positions
void Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  uint8_t ids[3]      = {coxa, femur, tibia};   // Array of servo IDs
  servo->syncWrite(handler_index, ids, LEG_SERVOS, positions, num_positions);
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
  
  uint32_t coxaVel = 0, femurVel = 0, tibiaVel = 0;
  //servo->readRegister(coxa,  43, 1, &coxaVel);
  //servo->readRegister(femur, 43, 1, &femurVel);
  //servo->readRegister(tibia, 43, 1, &tibiaVel);
  //#ifdef DEBUG
    Serial.print("Leg moving status - Coxa: ");
    Serial.print(coxaVel);
    Serial.print(" | Femur: ");
    Serial.print(femurVel);
    Serial.print(" | Tibia: ");
    Serial.println(tibiaVel);
  //#endif // DEBUG
  return (coxaVel == 1 || femurVel == 1 || tibiaVel == 1);
}

// Get current coxa angle
int32_t Leg::getCoxa() {
  int32_t angle = 0;
  servo->getPresentPositionData(coxa, &angle);
  return angle;
}

// Get current femur angle
int32_t Leg::getFemur() {
  int32_t angle = 0;
  servo->getPresentPositionData(femur, &angle);
  return angle;
}

// Get current tibia angle
int32_t Leg::getTibia() {
  int32_t angle = 0;
  servo->getPresentPositionData(tibia, &angle);
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



