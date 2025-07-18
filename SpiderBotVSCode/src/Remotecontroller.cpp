
#include "Remotecontroller.h" 
#include "Config.h"             // Include configuration header

// Constructor for Remotecontroller
Remotecontroller::Remotecontroller() {}

void Remotecontroller::begin(int serial_port) {
    rc.begin(serial_port); // Initialize the remote controller with the specified serial port
}

void Remotecontroller::update() {
 if (rc.available())
  {
    int RCRx = rc.readData();
    switch (RCRx) {
        case 00:
            #ifdef DEBUG
                Serial.println("RC Button depressed");
            #endif // DEBUG

            break;

        case RC100_BTN_U:
            #ifdef DEBUG
                Serial.println("RC Button U pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_D:
            #ifdef DEBUG
                Serial.println("RC Button D pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_L:
            #ifdef DEBUG
                Serial.println("RC Button L pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_R:
            #ifdef DEBUG
                Serial.println("RC Button R pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_1:
            #ifdef DEBUG
                Serial.println("RC Button 1 pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_2:
            #ifdef DEBUG
                Serial.println("RC Button 2 pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_3:
            #ifdef DEBUG
                Serial.println("RC Button 3 pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_4:
            #ifdef DEBUG
                Serial.println("RC Button 4 pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_5:
            #ifdef DEBUG
                Serial.println("RC Button 5 pressed");
            #endif // DEBUG

            break;

        case RC100_BTN_6:
            #ifdef DEBUG
                Serial.println("RC Button 6 pressed");
            #endif // DEBUG

            break;
    }
   
  }
}