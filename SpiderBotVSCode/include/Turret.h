#ifndef TURRET_H
#define TURRET_H

  #include "Servo.h"

  class Turret {
    public:
      Turret();    // Constructor
      void begin(uint8_t pan, uint8_t tilt, Servo* servo);    // Initialize the turret servos

      void move(int32_t panAngle, int32_t tiltAngle);         // Rotate the turret to specified angles
      void moveHome();                                        // Rotate the turret to home position
      void moveRight();                                       // Rotate turret to the right
      void moveLeft();                                        // Rotate turret to the left
      void moveUp();                                          // Rotate turret up
      void moveDown();                                        // Rotate turret down
      void printStatus(Stream& stream);                       // Print current turret angles to Serial

    private:
      static constexpr uint8_t handler_index   = 0;           // Index for sync write handler
      static constexpr uint8_t num_servos      = 2;           // Number of servos to control
      
      Servo*  servo;                                          // Pointer to the servo instance
      uint8_t turret_ids[num_servos] = {19, 20};              // Servo IDs for pan and tilt
  };

#endif // TURRET_H