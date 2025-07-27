#ifndef HEXAPOD_H
#define HEXAPOD_H

  #include "Leg.h"

  #define handler_index  uint8_t(0)                         // Index for sync write handler
  #define HEXAPOD_LEGS   uint8_t(6)                         // Maximum number of legs
  #define LEG_SERVOS     uint8_t(3)                         // Number of servos per leg
  #define HEXAPOD_SERVOS uint8_t(HEXAPOD_LEGS * LEG_SERVOS) // Maximum number of servos

  class Hexapod {
    public:
      Hexapod(); // Constructor
      void begin(Servo* servo);               // Initialize the hexapod

      bool move(uint8_t *ids, uint8_t num_servos, int32_t *positions);  // Move Hexapod
      bool moveUp();                                                    // Move Hexapod Up
      bool moveDown();                                                  // Move Hexapod Down

      bool moveLeg(uint8_t leg, int32_t *positions);                    // Move a specific leg
      bool moveLegUp(uint8_t leg);                                      // Move a specific leg up
      bool moveLegDown(uint8_t leg);                                    // Move a specific leg down
      bool moveLegOut(uint8_t leg);                                     // Move a specific leg out
      
      void printStatus(Stream& stream);       // Print the status of all legs
  
    private:
      Leg                       legs[HEXAPOD_LEGS];                           // Array of legs
      Servo*                    servo;                                        // Dynamixel controller instance
  };

#endif // HEXAPOD_H