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

  servo->init(panID, TURRET_PAN_HOME_DEG, TURRET_VELOCITY);         // Initialize pan servo with limits
  servo->init(tiltID, TURRET_TILT_HOME_DEG, TURRET_VELOCITY);       // Initialize tilt servo with limits
}

// Rotate the turret to specified angles
void Turret::moveTurret(int32_t panAngle, int32_t tiltAngle) {
  servo->goalPosition(panID, panAngle);                              // Set pan angle
  servo->goalPosition(tiltID, tiltAngle);                            // Set tilt angle 
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