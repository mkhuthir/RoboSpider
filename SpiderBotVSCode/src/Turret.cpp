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
  moveHome();                                           // Move turret to home position

}

// Rotate the turret to specified angles
void Turret::move(int32_t panAngle, int32_t tiltAngle) {
  const uint8_t num_positions   = 1;                                                  // Number of positions to write
  int32_t positions[num_servos] = {panAngle, tiltAngle};                              // Target positions for pan and tilt
  servo->syncWrite(handler_index, turret_ids, num_servos, positions, num_positions);  // Write target positions to servos
}

// Rotate the turret to home position
void Turret::moveHome() {
  move(TURRET_PAN_HOME, TURRET_TILT_HOME);    // Reset turret to home position
}

// Rotate turret to the right
void Turret::moveRight() {
  move(TURRET_PAN_RIGHT, TURRET_TILT_RIGHT); // Rotate turret to the right
} 

// Rotate turret to the left
void Turret::moveLeft() {
  move(TURRET_PAN_LEFT, TURRET_TILT_LEFT);  // Rotate turret to the left
}

// Rotate turret up
void Turret::moveUp() {
  move(TURRET_PAN_UP, TURRET_TILT_UP);  // Rotate turret up
}

// Rotate turret down
void Turret::moveDown() {
  move(TURRET_PAN_DOWN, TURRET_TILT_DOWN); // Rotate turret down
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