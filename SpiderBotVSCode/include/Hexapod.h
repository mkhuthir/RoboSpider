#ifndef HEXAPOD_H
#define HEXAPOD_H

  #include "Leg.h"
  #include "HexapodPoses.h"
  
  // Make HEXAPOD_LEGS, LEG_SERVOS, and HEXAPOD_SERVOS available globally
  static constexpr uint8_t  HEXAPOD_LEGS   = 6;                           // Maximum number of legs
  static constexpr uint8_t  LEG_SERVOS     = 3;                           // Number of servos per leg
  static constexpr uint8_t  HEXAPOD_SERVOS = HEXAPOD_LEGS * LEG_SERVOS;   // Maximum number of servos
  
  static constexpr uint8_t  handler_index  = 0;                           // Index for sync write handler

  class Hexapod {
    public:
      Hexapod(); // Constructor
      void begin(Servo* servo);               // Initialize the hexapod

      bool move(uint8_t *ids, uint8_t num_servos, int32_t *positions); // Move Hexapod
      bool moveUp();                                                    // Move Hexapod Up
      bool moveDown();                                                  // Move Hexapod Down
      
      void printStatus(Stream& stream);       // Print the status of all legs
  
    private:
      Leg                       legs[HEXAPOD_LEGS];                           // Array of legs
      Servo*                    servo;                                        // Dynamixel controller instance
  };

#endif // HEXAPOD_H