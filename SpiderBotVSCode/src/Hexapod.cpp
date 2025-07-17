#include "Hexapod.h"
#include "Servo.h"

// Constructor for Hexapod class
Hexapod::Hexapod(){
  servoSpeed  = 1.0;            // Default servo speed
  dxl         = nullptr;        // Dynamixel controller not initialized
  for (int i = 0; i < 6; i++) {
    legs[i] = Leg();            // Initialize each leg
  }
}

// Initialize the hexapod
void Hexapod::init(Servo* dxlCtrl) {
  dxl=dxlCtrl;
  legs[0].init(1,  2,  3,  dxl);
  legs[1].init(4,  5,  6,  dxl);
  legs[2].init(7,  8,  9,  dxl);
  legs[3].init(10, 11, 12, dxl);
  legs[4].init(13, 14, 15, dxl);
  legs[5].init(16, 17, 18, dxl);

//  for(int i=0; i<6; i++)
//    legs[i]->init();
}

// Set the servo speed
void Hexapod::setServoSpeed(float Speed) {
  servoSpeed = Speed;
}

// Print the status of all legs
void Hexapod::printLegsStatus() {
  Serial.println("\nHexapod Legs Status:");
  for (int i = 0; i < 6; i++) {
    Serial.print("Leg ");
    Serial.print(i + 1);
    Serial.print(": ");
    legs[i].printJointAngles();  // Print joint angles of each leg 
  }
  Serial.println("---");

}




