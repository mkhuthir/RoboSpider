
#include "Remotecontroller.h" c/ Include RemoteController class for managing remote control input


// Constructor for Remotecontroller
Remotecontroller::Remotecontroller() {}

void Remotecontroller::begin(int serial_port) {
    rc.begin(serial_port); // Initialize the remote controller with the specified serial port and baud race
}

void Remotecontroller::update() {
 if (rc.available())
  {
    int RCRx = rc.readData();
    switch (RCRx) {
        case RC100_BTN_U:
            Serial.println("Button U pressed");
            break;
        case RC100_BTN_D:
            Serial.println("Button D pressed");
            break;
        case RC100_BTN_L:
            Serial.println("Button L pressed");
            break;
        case RC100_BTN_R:
            Serial.println("Button R pressed");
            break;
        case RC100_BTN_1:
            Serial.println("Button 1 pressed");
            break;
        case RC100_BTN_2:
            Serial.println("Button 2 pressed");
            break;
        case RC100_BTN_3:
            Serial.println("Button 3 pressed");
            break;
        case RC100_BTN_4:
            Serial.println("Button 4 pressed");
            break;
        case RC100_BTN_5:
            Serial.println("Button 5 pressed");
            break;
        case RC100_BTN_6:
            Serial.println("Button 6 pressed");
            break;
    }
  }
}