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

bool Hexapod::moveLeg(int legIndex, int32_t coxa, int32_t femur, int32_t tibia) {
  if (legIndex < 0 || legIndex >= 6) return false;  // Invalid leg index
  legs[legIndex].setLeg(coxa, femur, tibia);
  delay(1000);  // Wait for the movement to complete
  return true;
}

// Print the status of all legs
void Hexapod::printStatus(Stream& stream) {
  stream.println("\nHexapod Legs Status:");
  for (int i = 0; i < 6; i++) {
    stream.print("Leg ");
    stream.print(i + 1);
    stream.print(": ");
    legs[i].printStatus(stream);  // Print leg angles of each leg
  }
}




