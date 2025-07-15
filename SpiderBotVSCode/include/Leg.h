#ifndef LEG_H
#define LEG_H

#include "Servo.h"
#include "Config.h"

class Leg {
  public:
    Leg(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Servo* dxl); // Constructor
    void initialize();                                                                      // Initialize the leg servos
    void setJointAngles(float coxaAngle, float femurAngle, float tibiaAngle);               // Set angles for the leg joints
    void legUp();                                                                           // Move the leg up to a default position
    void legDown();                                                                         // Move the leg down to a default position
    void printJointAngles();                                                                // Print current joint angles to Serial
    float getCoxaAngle();                       // Get current coxa angle
    float getFemurAngle();                      // Get current femur angle
    float getTibiaAngle();                      // Get current tibia angle  

  private:
    uint8_t coxa, femur, tibia;                                                             // Servo IDs for the leg joints
    Servo* dxl;                                                               // Pointer to the Dynamixel controller instance
};

#endif // LEG_H