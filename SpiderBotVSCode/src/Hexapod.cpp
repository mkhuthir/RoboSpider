#include "Hexapod.h"
#include "Servo.h"
#include "Config.h"             // Include configuration header

// Constructor for Hexapod class
Hexapod::Hexapod(){
  servo         = nullptr;        // Dynamixel controller not initialized
  for (int i = 0; i < 6; i++) {
    legs[i] = Leg();            // Initialize each leg
  }
}

// Initialize the hexapod
void Hexapod::begin(Servo* servo) {
  this->servo=servo;
  legs[0].init(1,  2,  3,  servo);
  legs[1].init(4,  5,  6,  servo);
  legs[2].init(7,  8,  9,  servo);
  legs[3].init(10, 11, 12, servo);
  legs[4].init(13, 14, 15, servo);
  legs[5].init(16, 17, 18, servo);

//  for(int i=0; i<6; i++)
//    legs[i]->init();
}

bool Hexapod::moveLeg(int legIndex, int coxa, int femur, int tibia) {
  if (legIndex < 0 || legIndex >= 6) return false;  // Invalid leg index
  legs[legIndex].setJointAngles(coxa, femur, tibia);
  delay(1000);  // Wait for the movement to complete
  return true;
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




