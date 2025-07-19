#ifndef HEXAPOD_H
#define HEXAPOD_H

  #include "Leg.h"

  class Hexapod {
    public:
      Hexapod(); // Constructor
      void begin(Servo* servo);                                   // Initialize the hexapod
      bool moveLeg(int legIndex, int coxa, int femur, int tibia); // Move a specific leg

      void printLegsStatus();                                     // Print the status of all legs
  
    private:
      Leg                   legs[6];                              // Array of legs
      Servo*                servo;                                // Dynamixel controller instance
  };

#endif // HEXAPOD_H