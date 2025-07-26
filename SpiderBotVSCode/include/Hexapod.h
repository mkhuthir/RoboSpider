#ifndef HEXAPOD_H
#define HEXAPOD_H

  #include "Leg.h"

  class Hexapod {
    public:
      Hexapod(); // Constructor
      void begin(Servo* servo);               // Initialize the hexapod
      
      bool moveUp();                          // Move Hexapod Up
      bool moveDown();                        // Move Hexapod Down
      
      void printStatus(Stream& stream);       // Print the status of all legs
  
    private:
      static constexpr uint8_t handler_index  = 0;                           // Index for sync write handler
      static constexpr uint8_t HEXAPOD_LEGS   = 6;                           // Maximum number of legs
      static constexpr uint8_t LEG_SERVOS     = 3;                           // Number of servos per leg
      static constexpr uint8_t HEXAPOD_SERVOS = HEXAPOD_LEGS * LEG_SERVOS;   // Maximum number of servos

      Leg                   legs[HEXAPOD_LEGS];                              // Array of legs
      uint8_t               hexapod_ids[HEXAPOD_SERVOS]={ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };                     // Servo IDs for all legs
      Servo*                servo;                                           // Dynamixel controller instance
  };

#endif // HEXAPOD_H