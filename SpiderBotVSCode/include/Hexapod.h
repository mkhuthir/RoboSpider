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
      bool isMoving();                                                  // Check if any leg is currently moving
      
      void printStatus(Stream& stream);       // Print the status of all legs
      
      Leg  legs[HEXAPOD_LEGS];                           // Array of legs
  
    private:
      
      Servo*                    servo;                                        // Dynamixel controller instance
  };

#endif // HEXAPOD_H