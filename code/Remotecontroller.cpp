
#include "Remotecontroller.h"
#include "Console.h"
#include "Debug.h"


// Constructor for Remotecontroller
Remotecontroller::Remotecontroller() {}

bool Remotecontroller::begin(int serial_port, Microcontroller* mc, Hexapod* hexapod, Turret* turret, GaitController* gc) {

    rc.begin(serial_port);      // Initialize the RC100 remote controller

    this->mc        = mc;       // Store the Microcontroller instance
    this->hexapod   = hexapod;  // Store the hexapod instance
    this->turret    = turret;   // Store the turret instance
    this->gc        = gc;       // Store the GaitController instance
    
    LOG_INF("Remotecontroller initialized successfully");
    return true;                // Return true if initialization is successful

}

bool Remotecontroller::update() {
    if (rc.available()) {
        switch (rc.readData()) {
            case 00:
                LOG_DBG("RC Button depressed");
                break;

            case RC100_BTN_U:
                LOG_DBG("RC Button U pressed");
                turret->moveUp();
                break;

            case RC100_BTN_D:
                LOG_DBG("RC Button D pressed");
                turret->moveDown();
                break;

            case RC100_BTN_L:
                LOG_DBG("RC Button L pressed");
                turret->moveLeft();
                break;

            case RC100_BTN_R:
                LOG_DBG("RC Button R pressed");
                turret->moveRight();
                break;

            case RC100_BTN_1:
                LOG_DBG("RC Button 1 pressed");
                gc->setGaitType(GAIT_WAVE); // Set gait to WAVE
                break;

            case RC100_BTN_2:
                LOG_DBG("RC Button 2 pressed");
                gc->setGaitType(GAIT_RIPPLE); // Set gait to RIPPLE
                break;

            case RC100_BTN_3:
                LOG_DBG("RC Button 3 pressed");
                gc->setGaitType(GAIT_TRIPOD); // Set gait to TRIPOD
                break;

            case RC100_BTN_4:
                LOG_DBG("RC Button 4 pressed");
                gc->setGaitType(GAIT_ROTATE); // Set gait to ROTATE
                break;

            case RC100_BTN_5:
                LOG_DBG("RC Button 5 pressed");
                gc->setGaitType(GAIT_IDLE); // Set gait to IDLE
                break;

            case RC100_BTN_6:
                LOG_DBG("RC Button 6 pressed");
                turret->moveHome(); // Move turret to home position
                break;

            default:
                LOG_WRN("Unknown RC button pressed");
                return false;
        }
    }
    return true;

    //-----------------------------------------------------------------------------

    // Print the status of all legs
    bool Remotecontroller::printStatus() {
        PRINTLN("\nRemotecontroller Status:");
        return true;
    }

    // Process console commands for hexapod control
    bool Remotecontroller::runConsoleCommands(const String& cmd, const String& args) {
        if (cmd == "rs") {
            printStatus();
            return true;

        } else if (cmd == "r?") {
            printConsoleHelp();
            return true;

        }

        return false;
    }

    // Print hexapod-specific help information
    bool Remotecontroller::printConsoleHelp() {
        PRINTLN("Remotecontroller Commands:\n\r");
        PRINTLN("  rs               - Print remotecontroller status");
        PRINTLN("  r?               - Print this help information");
        PRINTLN("");
        return true;
    }

}