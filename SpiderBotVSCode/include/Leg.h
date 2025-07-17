#ifndef LEG_H
#define LEG_H

  #include "Config.h"
  #include "Servo.h"

  class Leg {
    public:
      Leg(); // Constructor
      void  init(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Servo* dxlCtrl); // Initialize the leg servos
      void  setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle);    // Set angles for the leg joints
      float getCoxaAngle();                                                         // Get current coxa angle
      float getFemurAngle();                                                        // Get current femur angle
      float getTibiaAngle();                                                        // Get current tibia angle
      void  printJointAngles();                                                     // Print current joint angles to Serial

    private:
      Servo* dxl;                                                                   // Pointer to the Dynamixel controller instance
      uint8_t coxa, femur, tibia;                                                   // Servo IDs for the leg joints
  };

#endif // LEG_H