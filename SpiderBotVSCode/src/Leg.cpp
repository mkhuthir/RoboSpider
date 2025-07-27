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

  servo->init(coxa , COXA_VELOCITY );   // Initialize coxa servo with velocity
  servo->init(femur, FEMUR_VELOCITY);   // Initialize femur servo with velocity
  servo->init(tibia, TIBIA_VELOCITY);   // Initialize tibia servo with velocity
}

// Move the leg to the specified positions
void Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  uint8_t ids[3]      = {coxa, femur, tibia};   // Array of servo IDs
  uint8_t num_servos  = 3;                      // Number of servos in the leg
  servo->syncWrite(handler_index, ids, num_servos, positions, num_positions);
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

