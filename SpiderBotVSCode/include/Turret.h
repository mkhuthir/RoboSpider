#ifndef TURRET_H
#define TURRET_H

  #include "Servo.h"

  class Turret {
    public:
      Turret();    // Constructor
      void begin(uint8_t pan, uint8_t tilt, Servo* servo);    // Initialize the turret servos

      void moveTurret(int32_t panAngle, int32_t tiltAngle);       // Rotate the turret to specified angles
      void moveHome();                                        // Rotate the turret to home position
      void moveRight();                                       // Rotate turret to the right
      void moveLeft();                                        // Rotate turret to the left
      void moveUp();                                          // Rotate turret up
      void moveDown();                                        // Rotate turret down
      void printStatus(Stream& stream);                       // Print current turret angles to Serial

    private:
      Servo*  servo;                                          // Pointer to the servo instance
      uint8_t panID = 19, tiltID =20;                                  // Servo IDs for pan, tilt and sensor
  };

#endif // TURRET_H