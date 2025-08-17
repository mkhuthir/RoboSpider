#ifndef LEG_H
#define LEG_H

  #include "Servo.h"
  #include "Driver.h"

  #define handler_index   0                   // Index for sync write handler
  #define LEG_SERVOS      uint8_t(3)          // Number of servos per leg
  #define HEXAPOD_LEGS    uint8_t(6)          // Maximum number of legs

  class Leg {
    public:
      Leg();                                  // Constructor
      bool    init( uint8_t coxaID,           // Initialize the leg servos
                    uint8_t femurID, 
                    uint8_t tibiaID,
                    Driver* driver,
                    Servo* servo);

      bool    update();                       // Update the leg state

      bool    move(int32_t *positions);       // Move the leg to the specified positions
      bool    isMoving();                     // Check if any servo in the leg is currently moving 

      bool    movePointUp();                  // Move leg up
      bool    movePointDown();                // Move leg down
      bool    movePointOut();                 // Move leg out
      bool    moveStandUp();                  // Move leg to stand up position
      bool    moveStandDown();                // Move leg to stand down position

      bool    getCoxa(uint16_t* angle);       // Get current coxa angle
      bool    getFemur(uint16_t* angle);      // Get current femur angle
      bool    getTibia(uint16_t* angle);      // Get current tibia angle

      bool    printStatus();                  // Print current joint angles to Serial
      bool    runConsoleCommands(const String& cmd, const String& args, int legIndex);  // Process console commands for leg control
      bool    printConsoleHelp();             // Print leg-specific help information
      
    private:
      Driver* driver;                         // Pointer to the driver instance
      Servo* servo;                           // Pointer to the servo instance
      uint8_t legIDs[LEG_SERVOS]={0,0,0};     // Servo IDs for the leg joints

      enum LegJoint { Coxa  = 0,              // Enum for leg joints
                      Femur = 1, 
                      Tibia = 2 };
  };

#endif // LEG_H