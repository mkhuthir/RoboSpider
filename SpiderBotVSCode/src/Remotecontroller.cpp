
#include "Remotecontroller.h" 
#include "Config.h"             // Include configuration header

// Constructor for Remotecontroller
Remotecontroller::Remotecontroller() {}

void Remotecontroller::begin(int serial_port, Hexapod* hexapod, Turret* turret, GaitController* gc, Microcontroller* mc) {
    rc.begin(serial_port);      // Initialize the remote controller with the specified serial port
    this->hexapod   = hexapod;   // Store the hexapod instance
    this->turret    = turret;   // Store the turret instance
    this->gc        = gc;       // Store the GaitController instance
    this->mc        = mc;       // Store the Microcontroller instance

}

void Remotecontroller::update() {
    if (rc.available()) {
        switch (rc.readData()) {
            case 00:
                #ifdef DEBUG
                Serial.println("RC Button depressed");
            #endif // DEBUG

            break;

        case RC100_BTN_U:
            #ifdef DEBUG
                Serial.println("RC Button U pressed");
            #endif // DEBUG
            turret->moveUp();
            break;

        case RC100_BTN_D:
            #ifdef DEBUG
                Serial.println("RC Button D pressed");
            #endif // DEBUG
            turret->moveDown();
            break;

        case RC100_BTN_L:
            #ifdef DEBUG
                Serial.println("RC Button L pressed");
            #endif // DEBUG
            turret->moveLeft();
            break;

        case RC100_BTN_R:
            #ifdef DEBUG
                Serial.println("RC Button R pressed");
            #endif // DEBUG
            turret->moveRight();
            break;

        case RC100_BTN_1:
            #ifdef DEBUG
                Serial.println("RC Button 1 pressed");
            #endif // DEBUG
            gc->setGait(GAIT_WAVE); // Set gait to WAVE
            break;

        case RC100_BTN_2:
            #ifdef DEBUG
                Serial.println("RC Button 2 pressed");
            #endif // DEBUG
            gc->setGait(GAIT_RIPPLE); // Set gait to RIPPLE
            break;

        case RC100_BTN_3:
            #ifdef DEBUG
                Serial.println("RC Button 3 pressed");
            #endif // DEBUG
            gc->setGait(GAIT_TRIPOD); // Set gait to TRIPOD
            break;

        case RC100_BTN_4:
            #ifdef DEBUG
                Serial.println("RC Button 4 pressed");
            #endif // DEBUG
            turret->printStatus(); // Print turret status
            hexapod->printStatus(); // Print hexapod status
            break;

        case RC100_BTN_5:
            #ifdef DEBUG
                Serial.println("RC Button 5 pressed");
            #endif // DEBUG
            gc->setGait(GAIT_IDLE); // Set gait to IDLE
            break;

        case RC100_BTN_6:
            #ifdef DEBUG
                Serial.println("RC Button 6 pressed");
            #endif // DEBUG
            turret->moveHome(); // Move turret to home position
            break;
        }
    }
}   