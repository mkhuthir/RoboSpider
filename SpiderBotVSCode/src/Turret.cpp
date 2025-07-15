#include "Turret.h"

Turret::Turret(){
  #ifdef DEBUG
    Serial.println("Turret initialized with Pan ID: " + String(panID) + " and Tilt ID: " + String(tiltID));
  #endif // DEBUG
  }

// Initialize the turret servos
void Turret::init(uint8_t pan, uint8_t tilt, Servo* dxlCtrl) {
  panID=pan;
  tiltID=tilt;
  dxl=dxlCtrl;

  dxl->initServo(panID);      // Initialize pan servo with limits
  dxl->initServo(tiltID);     // Initialize tilt servo with limits
  rotateTurretHome();             // Rotate turret to home position
}

// Rotate the turret to specified angles
void Turret::rotateTurret(float panAngle, float tiltAngle) {
  dxl->goalPosition(panID, panAngle);                              // Set pan angle
  dxl->goalPosition(tiltID, tiltAngle);                            // Set tilt angle 
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
  //Serial.print(dxl->getGoalPosition(panID));
  Serial.print(" | Tilt: ");
  //Serial.println(dxl->getGoalPosition(tiltID));
  Serial.println("---");
}