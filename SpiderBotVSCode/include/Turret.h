#ifndef TURRET_H
#define TURRET_H

#include "Config.h"
#include "Servo.h"

class Turret {
  public:
    Turret();    // Constructor
    void begin(uint8_t pan, uint8_t tilt, Servo* servo);    // Initialize the turret servos

    void rotateTurret(float panAngle, float tiltAngle);   // Rotate the turret to specified angles
    void rotateTurretHome();                              // Rotate the turret to home position
    void rotateTurretRight();                             // Rotate turret to the right
    void rotateTurretLeft();                              // Rotate turret to the left
    void rotateTurretUp();                                // Rotate turret up
    void rotateTurretDown();                              // Rotate turret down
    void printTurretStatus();                             // Print current turret angles to Serial 
            
  private:
    Servo*  servo;                                        // Pointer to the servo instance
    uint8_t panID, tiltID;                                // Servo IDs for pan, tilt and sensor
    
};

#endif // TURRET_H