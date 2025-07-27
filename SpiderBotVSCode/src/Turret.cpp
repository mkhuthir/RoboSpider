#include "Turret.h"
#include "TurretPoses.h"

Turret::Turret(){
  turret_ids[0] = TURRET_PAN_ID;  // Set pan servo ID
  turret_ids[1] = TURRET_TILT_ID; // Set tilt servo ID
  servo         = nullptr;        // Dynamixel controller not initialized
}

// Initialize the turret servos
void Turret::begin(Servo* servo) {
  this->servo = servo;  // Set the servo pointer
  servo->init(turret_ids[0], TURRET_VELOCITY);   // Initialize pan servo with default velocity
  servo->init(turret_ids[1], TURRET_VELOCITY);   // Initialize tilt servo with default velocity
  moveHome();                           // Move turret to home position
}

// Rotate the turret to specified angles
void Turret::move(int32_t *positions) {
  const uint8_t num_positions   = 1;                                                  // Number of positions to write
  servo->syncWrite(handler_index, turret_ids, TURRET_SERVOS, positions, num_positions);  // Write target positions to servos
}

// Rotate the turret to home position
void Turret::moveHome() {
  move(poseTurretHome);    // Reset turret to home position
}

// Rotate turret to the right
void Turret::moveRight() {
  move(poseTurretRight); // Rotate turret to the right
} 

// Rotate turret to the left
void Turret::moveLeft() {
  move(poseTurretLeft);  // Rotate turret to the left
}

// Rotate turret up
void Turret::moveUp() {
  move(poseTurretUp);  // Rotate turret up
}

// Rotate turret down
void Turret::moveDown() {
  move(poseTurretDown); // Rotate turret down
} 

// Print current turret angles to Serial
void Turret::printStatus(Stream& stream) {
  int32_t panPosition = 0, tiltPosition = 0;

  servo->getPresentPositionData(turret_ids[0], &panPosition);
  servo->getPresentPositionData(turret_ids[1], &tiltPosition);

  stream.print("\nTurret Status:");
  stream.print("Pan: ");
  stream.print(panPosition);
  stream.print(" | Tilt: ");
  stream.println(tiltPosition);
}