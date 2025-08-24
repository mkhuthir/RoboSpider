#ifndef TURRET_H
#define TURRET_H

  #include "Servo.h"
  #include "Driver.h"

  #define handler_index  0                           // Index for sync write handler

  #define TURRET_PAN_ID         19
  #define TURRET_TILT_ID        20

  #define TURRET_PAN_CW_LIMIT   0
  #define TURRET_PAN_CCW_LIMIT  1023
  #define TURRET_TILT_CW_LIMIT  72
  #define TURRET_TILT_CCW_LIMIT 952

  #define TURRET_SPEED          100   // Default speed for turret servos
  #define TURRET_PAN_MIN        0     // Min angle for turret pan servo
  #define TURRET_PAN_MAX        1023  // Max angle for turret pan servo
  #define TURRET_TILT_MIN       0     // Min angle for turret tilt servo
  #define TURRET_TILT_MAX       1023  // Max angle for turret tilt servo

  static constexpr uint8_t TURRET_SERVOS      = 2;   // Number of servos to control

  class Turret {
    public:
      Turret();                               // Constructor
      bool begin(Driver* driver, Servo* servo);         // Initialize the turret servos
      bool update();                          // Update turret state

      bool      move(int32_t *positions);     // Rotate the turret to specified angles
      bool      moveHome();                   // Rotate the turret to home position
      bool      moveRight();                  // Rotate turret to the right
      bool      moveLeft();                   // Rotate turret to the left
      bool      moveUp();                     // Rotate turret up
      bool      moveDown();                   // Rotate turret down

      bool      setSpeed(int16_t speed);      // Set turret rotation speed
      uint16_t  getSpeed() const;             // Get current turret rotation speed

      bool      printStatus();                // Print current turret angles to Serial
      bool      runConsoleCommands(const String& cmd, const String& args); // Process console commands for turret control
      bool      printConsoleHelp();                                        // Print turret-specific help information

    private:
      Driver*   driver;                       // Pointer to the driver instance
      Servo*    servo;                        // Pointer to the servo instance
      uint16_t  speed;
  }; 

#endif // TURRET_H