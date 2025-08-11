#ifndef TURRET_H
#define TURRET_H

  #include "Servo.h"
  
  #define handler_index  0                           // Index for sync write handler
  static constexpr uint8_t TURRET_SERVOS      = 2;   // Number of servos to control

  class Turret {
    public:
      Turret();                         // Constructor
      bool begin(Servo* servo);         // Initialize the turret servos
      bool update();                    // Update turret state

      bool move(int32_t *positions);    // Rotate the turret to specified angles
      bool moveHome();                  // Rotate the turret to home position
      bool moveRight();                 // Rotate turret to the right
      bool moveLeft();                  // Rotate turret to the left
      bool moveUp();                    // Rotate turret up
      bool moveDown();                  // Rotate turret down
      bool printStatus();               // Print current turret angles to Serial

      bool runConsoleCommands(const String& cmd, const String& args); // Process console commands for turret control
      bool printConsoleHelp();                                        // Print turret-specific help information

    private:
      Servo*  servo;                    // Pointer to the servo instance
  };

#endif // TURRET_H