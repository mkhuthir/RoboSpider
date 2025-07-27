#ifndef LEG_H
#define LEG_H

  #include "Servo.h"
  #include "LegPoses.h"

  #define handler_index  0                    // Index for sync write handler

  class Leg {
    public:
      Leg();                                  // Constructor
      void    init( uint8_t coxaID,           // Initialize the leg servos
                    uint8_t femurID, 
                    uint8_t tibiaID, 
                    Servo* servo);            

      void    move(int32_t *positions);       // Move the leg to the specified positions
      
      int32_t getCoxa();                      // Get current coxa angle
      int32_t getFemur();                     // Get current femur angle
      int32_t getTibia();                     // Get current tibia angle
      void    printStatus(Stream& stream);    // Print current joint angles to Serial

    private:
      Servo* servo;                           // Pointer to the servo instance
      uint8_t coxa, femur, tibia;             // Servo IDs for the leg joints
  };

#endif // LEG_H