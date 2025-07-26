#include "Turret.h"
#include "Config.h"             // Include configuration header

Turret::Turret(){
  panID   = 0;         // Initialize pan ID
  tiltID  = 0;         // Initialize tilt ID
  servo   = nullptr;   // Dynamixel controller not initialized
}

// Initialize the turret servos
void Turret::begin(uint8_t pan, uint8_t tilt, Servo* servo) {
  panID     = pan;
  tiltID    = tilt;
  this->servo = servo;  // Set the servo pointer

  servo->init(panID, TURRET_VELOCITY);                  // Initialize pan servo with default velocity
  servo->init(tiltID, TURRET_VELOCITY);                 // Initialize tilt servo with default velocity
  servo->addSyncWriteHandler(panID, "Goal_Position");   // Add sync write handler for pan
  moveHome();                                           // Move turret to home position

}

// Rotate the turret to specified angles
void Turret::moveTurret(int32_t panAngle, int32_t tiltAngle) {
  const uint8_t handler_index   = 0;                                            // Index for sync write handler
  const uint8_t num_servos      = 2;                                            // Number of servos to control
  const uint8_t num_positions   = 1;                                            // Number of positions to write
  uint8_t ids[num_servos]       = {panID, tiltID};                              // Servo IDs for pan and tilt
  int32_t positions[num_servos] = {panAngle, tiltAngle};                        // Target positions for pan and tilt
  servo->syncWrite(handler_index, ids, num_servos, positions, num_positions);   // Write target positions to servos
}

// Rotate the turret to home position
void Turret::moveHome() {
  moveTurret(TURRET_PAN_HOME_DEG, TURRET_TILT_HOME_DEG);    // Reset turret to home position
}

// Rotate turret to the right
void Turret::moveRight() {
  moveTurret(TURRET_PAN_RIGHT_DEG, TURRET_TILT_RIGHT_DEG); // Rotate turret to the right
} 

// Rotate turret to the left
void Turret::moveLeft() {
  moveTurret(TURRET_PAN_LEFT_DEG, TURRET_TILT_LEFT_DEG);  // Rotate turret to the left
}

// Rotate turret up
void Turret::moveUp() {
  moveTurret(TURRET_PAN_UP_DEG, TURRET_TILT_UP_DEG);  // Rotate turret up
}

// Rotate turret down
void Turret::moveDown() {
  moveTurret(TURRET_PAN_DOWN_DEG, TURRET_TILT_DOWN_DEG); // Rotate turret down
} 

// Print current turret angles to Serial
void Turret::printStatus(Stream& stream) {
  int32_t panPosition = 0, tiltPosition = 0;
  
  servo->getPresentPositionData(panID, &panPosition);
  servo->getPresentPositionData(tiltID, &tiltPosition);

  stream.print("\nTurret Status:");
  stream.print("Pan: ");
  stream.print(panPosition);
  stream.print(" | Tilt: ");
  stream.println(tiltPosition);
}