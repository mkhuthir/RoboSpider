#ifndef LEG_H
#define LEG_H

  #include "Servo.h"
  
  #define handler_index   0                   // Index for sync write handler
  #define LEG_SERVOS      uint8_t(3)          // Number of servos per leg
  #define HEXAPOD_LEGS    uint8_t(6)          // Maximum number of legs

  class Leg {
    public:
      Leg();                                  // Constructor
      void    init( uint8_t coxaID,           // Initialize the leg servos
                    uint8_t femurID, 
                    uint8_t tibiaID, 
                    Servo* servo);            

      void    move(int32_t *positions);       // Move the leg to the specified positions
      bool    isMoving();                     // Check if any servo in the leg is currently moving 

      void    movePointUp();                  // Move leg up
      void    movePointDown();                // Move leg down
      void    movePointOut();                 // Move leg out
      void    moveStandUp();                  // Move leg to stand up position
      void    moveStandDown();                // Move leg to stand down position
 
      int32_t getCoxa();                      // Get current coxa angle
      int32_t getFemur();                     // Get current femur angle
      int32_t getTibia();                     // Get current tibia angle
      void    printStatus();                  // Print current joint angles to Serial
      
      
      bool    runConsoleCommands(const String& cmd, const String& args, int legIndex);  // Process console commands for leg control
      void    printConsoleHelp();                    // Print leg-specific help information
      
    private:
      Servo* servo;                           // Pointer to the servo instance
      uint8_t legIDs[LEG_SERVOS]={0,0,0};     // Servo IDs for the leg joints

      enum LegJoint { Coxa  = 0,               // Enum for leg joints
                      Femur = 1, 
                      Tibia = 2 };
  };

#endif // LEG_H