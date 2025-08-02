#include "Hexapod.h"
#include "Servo.h"
#include "Config.h"             // Include configuration header
#include "HexapodPoses.h"       // Include hexapod poses header

// Constructor for Hexapod class
Hexapod::Hexapod(){
  servo         = nullptr;              // Dynamixel controller not initialized
  for (int i = 0; i < HEXAPOD_LEGS; i++) {
    legs[i] = Leg();                    // instantiate each leg
  }
}

// Initialize the hexapod
bool Hexapod::begin(Servo* servo) {
  
  this->servo=servo;
  
  legs[0].init(1,  2,  3,  servo);                  // Initialize each leg with servo IDs
  legs[1].init(4,  5,  6,  servo);
  legs[2].init(7,  8,  9,  servo);
  legs[3].init(10, 11, 12, servo);
  legs[4].init(13, 14, 15, servo);
  legs[5].init(16, 17, 18, servo);

  if (!servo->addSyncWriteHandler(1, "Goal_Position")) {   // Add sync write handler
    return false;
  }

  moveStandUp();                                    // Move Hexapod to standing position
  return true;                                      // Initialization successful
}
// Move Hexapod
bool Hexapod::move(uint8_t *ids, uint8_t num_servos, int32_t *positions) {
  const uint8_t num_positions   = 1;
  servo->syncWrite(handler_index, ids, num_servos, positions, num_positions);
}

// Check if any leg is currently moving
bool Hexapod::isMoving() {
  for (int i = 0; i < HEXAPOD_LEGS; ++i) {
    if (legs[i].isMoving()) {
      return true;  // If any leg is moving, return true
    }
  }
  return false;  // If no legs are moving, return false
}

// Move Hexapod Up
bool Hexapod::moveStandUp() {
  move(poseHexapodIDs, HEXAPOD_SERVOS, poseHexapodStandUP);
}

// Move Hexapod Down
bool Hexapod::moveStandDown() {
  move(poseHexapodIDs, HEXAPOD_SERVOS, poseHexapodStandDown);
}

// Print the status of all legs
void Hexapod::printStatus(Stream& stream) {
  stream.println("\nHexapod Legs Status:");
  for (int i = 0; i < 6; i++) {
    stream.print("Leg ");
    stream.print(i + 1);
    stream.print(": ");
    legs[i].printStatus(stream);  // Print leg angles of each leg
  }
}




