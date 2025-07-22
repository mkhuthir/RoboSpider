#ifndef LEG_H
#define LEG_H

  #include "Servo.h"

  class Leg {
    public:
      Leg(); // Constructor
      void    init( uint8_t coxaID, 
                    uint8_t femurID, 
                    uint8_t tibiaID, 
                    Servo* servo);            // Initialize the leg servos
      void    setLeg( int32_t coxaAngle, 
                      int32_t femurAngle, 
                      int32_t tibiaAngle);    // Set angles for the leg joints
      int32_t getCoxa();                      // Get current coxa angle
      int32_t getFemur();                     // Get current femur angle
      int32_t getTibia();                     // Get current tibia angle
      void    printStatus(Stream& stream);    // Print current joint angles to Serial

    private:
      Servo* servo;                           // Pointer to the servo instance
      uint8_t coxa, femur, tibia;             // Servo IDs for the leg joints
  };

#endif // LEG_H