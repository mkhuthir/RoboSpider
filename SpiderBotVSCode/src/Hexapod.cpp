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
void Hexapod::begin(Servo* servo) {
  this->servo=servo;
  legs[0].init(1,  2,  3,  servo);
  legs[1].init(4,  5,  6,  servo);
  legs[2].init(7,  8,  9,  servo);
  legs[3].init(10, 11, 12, servo);
  legs[4].init(13, 14, 15, servo);
  legs[5].init(16, 17, 18, servo);
                                
  servo->addSyncWriteHandler(1, "Goal_Position");   // Add sync write handler for pan
  moveUp(); // Move all legs to home position

}
// Move Hexapod Up
bool Hexapod::moveUp() {
  const uint8_t num_positions   = 1;                                      // Number of positions to write
  servo->syncWrite(handler_index, hexapod_ids, HEXAPOD_SERVOS, poseStandUP, num_positions);   // Write target positions to servos
}

// Move Hexapod Down
bool Hexapod::moveDown() {
  const uint8_t num_positions   = 1;                                        // Number of positions to write
  servo->syncWrite(handler_index, hexapod_ids, HEXAPOD_SERVOS, poseStandDown, num_positions);   // Write target positions to servos
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




