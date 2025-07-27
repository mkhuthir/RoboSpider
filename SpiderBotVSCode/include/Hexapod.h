#ifndef HEXAPOD_H
#define HEXAPOD_H

  #include "Leg.h"

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