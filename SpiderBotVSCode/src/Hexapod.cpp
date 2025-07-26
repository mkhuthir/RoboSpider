#include "Hexapod.h"
#include "Servo.h"
#include "Config.h"             // Include configuration header

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
  int32_t positions[HEXAPOD_SERVOS] = { COXA_UP, FEMUR_UP, TIBIA_UP,      // Target positions
                                        COXA_UP, FEMUR_UP, TIBIA_UP,
                                        COXA_UP, FEMUR_UP, TIBIA_UP,
                                        COXA_UP, FEMUR_UP, TIBIA_UP,
                                        COXA_UP, FEMUR_UP, TIBIA_UP,
                                        COXA_UP, FEMUR_UP, TIBIA_UP
                                      };

  servo->syncWrite(handler_index, hexapod_ids, HEXAPOD_SERVOS, positions, num_positions);   // Write target positions to servos
}

// Move Hexapod Down
bool Hexapod::moveDown() {
  const uint8_t num_positions   = 1;                                        // Number of positions to write
  int32_t positions[HEXAPOD_SERVOS] = { COXA_DOWN, FEMUR_DOWN, TIBIA_DOWN,  // Target positions
                                        COXA_DOWN, FEMUR_DOWN, TIBIA_DOWN,
                                        COXA_DOWN, FEMUR_DOWN, TIBIA_DOWN,
                                        COXA_DOWN, FEMUR_DOWN, TIBIA_DOWN,
                                        COXA_DOWN, FEMUR_DOWN, TIBIA_DOWN,
                                        COXA_DOWN, FEMUR_DOWN, TIBIA_DOWN
                                      };

  servo->syncWrite(handler_index, hexapod_ids, HEXAPOD_SERVOS, positions, num_positions);   // Write target positions to servos
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




