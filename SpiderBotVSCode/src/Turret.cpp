#include "Turret.h"
#include "Config.h"             // Include configuration header

Turret::Turret(){
  panID   = 0;         // Initialize pan ID
  tiltID  = 0;         // Initialize tilt ID
  dxl     = nullptr;   // Dynamixel controller not initialized
}

// Initialize the turret servos
void Turret::begin(uint8_t pan, uint8_t tilt, Servo* dxl) {
  panID     = pan;
  tiltID    = tilt;
  this->dxl = dxl;

  dxl->init(panID);      // Initialize pan servo with limits
  dxl->init(tiltID);     // Initialize tilt servo with limits
  rotateTurretHome();             // Rotate turret to home position
}

// Rotate the turret to specified angles
void Turret::rotateTurret(float panAngle, float tiltAngle) {
  dxl->goalPosition(panID, panAngle);                              // Set pan angle
  dxl->goalPosition(tiltID, tiltAngle);                            // Set tilt angle 
}

// Rotate the turret to home position
void Turret::rotateTurretHome() {
  rotateTurret(TURRET_PAN_HOME_DEG, TURRET_TILT_HOME_DEG);    // Reset turret to home position
}

// Rotate turret to the right
void Turret::rotateTurretRight() {
  rotateTurret(TURRET_PAN_RIGHT_DEG, TURRET_TILT_RIGHT_DEG); // Rotate turret to the right
} 

// Rotate turret to the left
void Turret::rotateTurretLeft() {
  rotateTurret(TURRET_PAN_LEFT_DEG, TURRET_TILT_LEFT_DEG);  // Rotate turret to the left
}

// Rotate turret up
void Turret::rotateTurretUp() {
  rotateTurret(TURRET_PAN_UP_DEG, TURRET_TILT_UP_DEG);  // Rotate turret up
}

// Rotate turret down
void Turret::rotateTurretDown() {
  rotateTurret(TURRET_PAN_DOWN_DEG, TURRET_TILT_DOWN_DEG); // Rotate turret down
} 

// Print current turret angles to Serial
void Turret::printTurretStatus() {
  Serial.println("\nTurret Status:");
  Serial.print("Pan: ");
  //Serial.print(dxl->getGoalPosition(panID));
  Serial.print(" | Tilt: ");
  //Serial.println(dxl->getGoalPosition(tiltID));
  Serial.println("---");
}