#ifndef HEXAPOD_H
#define HEXAPOD_H

  #include "Leg.h"

  class Hexapod {
    public:
      Hexapod(); // Constructor
      void begin(Servo* servo);                                   // Initialize the hexapod
      bool moveLeg(int legIndex, int32_t coxa, int32_t femur, int32_t tibia); // Move a specific leg

      void printStatus();                                     // Print the status of all legs
  
    private:
      Leg                   legs[6];                              // Array of legs
      Servo*                servo;                                // Dynamixel controller instance
  };

#endif // HEXAPOD_H