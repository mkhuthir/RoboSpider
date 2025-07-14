#ifndef TURRET_H
#define TURRET_H

#include "Servo.h"
#include "Config.h"


class Turret {
  public:
    Turret(uint8_t panID, uint8_t tiltID, Servo* controller); // Constructor
    void initialize();                                                      // Initialize the turret servos

    void rotateTurret(float panAngle, float tiltAngle);                     // Rotate the turret to specified angles
    void rotateTurretHome();                                                // Rotate the turret to default position
    void rotateTurretRight();                                               // Rotate turret to the right
    void rotateTurretLeft();                                                // Rotate turret to the left
    void rotateTurretUp();                                                  // Rotate turret up
    void rotateTurretDown();                                                // Rotate turret down
    void printTurretStatus();                                               // Print current turret angles to Serial 
            
  private:
    uint8_t pan, tilt;                                                      // Servo IDs for pan, tilt and sensor
    Servo* dxl_wb;                                           // Pointer to the Dynamixel controller instance
};

#endif // TURRET_H