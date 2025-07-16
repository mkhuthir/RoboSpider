#include "Hexapod.h"
#include "Servo.h"

Hexapod::Hexapod(){
  #ifdef DEBUG
    Serial.println("Hexapod instance created with 6 legs.");
  #endif // DEBUG
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

// Set the gait type
void Hexapod::setGaitType(int Type) {
  gaitType = Type;
}

// Set the gait speed
void Hexapod::setGaitSpeed(float Speed) {
  gaitSpeed = Speed;
}

// Set the servo speed
void Hexapod::setServoSpeed(float Speed) {
  servoSpeed = Speed;
}

// Stand up the hexapod
void Hexapod::standUp() {
  for(int i=0; i<6; i++)
    legs[i].legUp();
}

// Sit down the hexapod
void Hexapod::sitDown() {
  for(int i=0; i<6; i++)
    legs[i].legDown();
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




