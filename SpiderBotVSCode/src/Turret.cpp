#include "Turret.h"

Turret::Turret(uint8_t panID, uint8_t tiltID, Servo* dxl)
  : pan(panID), tilt(tiltID), dxl_wb(dxl) {}

// Initialize the turret servos
void Turret::initialize() {
  dxl_wb->initServo(pan);      // Initialize pan servo with limits
  dxl_wb->initServo(tilt);     // Initialize tilt servo with limits
  rotateTurretHome();             // Rotate turret to home position
}

// Rotate the turret to specified angles
void Turret::rotateTurret(float panAngle, float tiltAngle) {
  dxl_wb->goalPosition(pan, panAngle);                              // Set pan angle
  dxl_wb->goalPosition(tilt, tiltAngle);                            // Set tilt angle 
}

// Rotate the turret to home position
void Turret::rotateTurretHome() {
  rotateTurret(TURRET_PAN_DEFAULT_DEG, TURRET_TILT_DEFAULT_DEG);    // Reset turret to default position
}

// Rotate turret to the right
void Turret::rotateTurretRight() {
  rotateTurret(270,60); // Rotate turret to the right
} 

// Rotate turret to the left
void Turret::rotateTurretLeft() {
  rotateTurret(60,60);  // Rotate turret to the left
}

// Rotate turret up
void Turret::rotateTurretUp() {
  rotateTurret(60, 30);  // Rotate turret up
}

// Rotate turret down
void Turret::rotateTurretDown() {
  rotateTurret(60, 90); // Rotate turret down
} 

// Print current turret angles to Serial
void Turret::printTurretStatus() {
  Serial.println("\nTurret Status:");
  Serial.print("Pan: ");
  //Serial.print(dxl_wb->getGoalPosition(pan));
  Serial.print(" | Tilt: ");
  //Serial.println(dxl_wb->getGoalPosition(tilt));
  Serial.println("---");
}