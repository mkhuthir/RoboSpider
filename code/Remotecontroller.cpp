
#include "Remotecontroller.h"
#include "Console.h"
#include "Debug.h"


// Constructor for Remotecontroller
Remotecontroller::Remotecontroller() {}

bool Remotecontroller::begin(int serial_port, Microcontroller* mc, Hexapod* hexapod, Turret* turret, GaitController* gc) {

    rc.begin(serial_port);      // Initialize the RC100 remote controller

    lastButtonPressed = 0;
    lastButtonDepressed = 0;

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
                lastButtonDepressed = lastButtonPressed;
                break;

            case RC100_BTN_U:
                LOG_DBG("RC Button U pressed");
                lastButtonPressed = RC100_BTN_U;
                turret->moveUp();
                break;

            case RC100_BTN_D:
                LOG_DBG("RC Button D pressed");
                lastButtonPressed = RC100_BTN_D;
                turret->moveDown();
                break;

            case RC100_BTN_L:
                LOG_DBG("RC Button L pressed");
                lastButtonPressed = RC100_BTN_L;
                turret->moveLeft();
                break;

            case RC100_BTN_R:
                LOG_DBG("RC Button R pressed");
                lastButtonPressed = RC100_BTN_R;
                turret->moveRight();
                break;

            case RC100_BTN_1:
                LOG_DBG("RC Button 1 pressed");
                lastButtonPressed = RC100_BTN_1;
                gc->setGaitType(GAIT_WAVE); // Set gait to WAVE
                break;

            case RC100_BTN_2:
                LOG_DBG("RC Button 2 pressed");
                lastButtonPressed = RC100_BTN_2;
                gc->setGaitType(GAIT_RIPPLE); // Set gait to RIPPLE
                break;

            case RC100_BTN_3:
                LOG_DBG("RC Button 3 pressed");
                lastButtonPressed = RC100_BTN_3;
                gc->setGaitType(GAIT_TRIPOD); // Set gait to TRIPOD
                break;

            case RC100_BTN_4:
                LOG_DBG("RC Button 4 pressed");
                lastButtonPressed = RC100_BTN_4;
                gc->setGaitType(GAIT_ROTATE); // Set gait to ROTATE
                break;

            case RC100_BTN_5:
                LOG_DBG("RC Button 5 pressed");
                lastButtonPressed = RC100_BTN_5;
                gc->setGaitType(GAIT_IDLE); // Set gait to IDLE
                break;

            case RC100_BTN_6:
                LOG_DBG("RC Button 6 pressed");
                lastButtonPressed = RC100_BTN_6;
                turret->moveHome(); // Move turret to home position
                break;

            default:
                LOG_WRN("Unknown RC button pressed");
                return false;
        }
    }
    return true;
}
 
// get last button pressed
uint16_t Remotecontroller::getLastButtonPressed() {
    return lastButtonPressed;
}

// get last button depressed
uint16_t Remotecontroller::getLastButtonDepressed() {
    return lastButtonDepressed;
}

//-----------------------------------------------------------------------------------------

// Print the status of the remote controller
bool Remotecontroller::printStatus() {
    PRINTLN("\nRemotecontroller Status:");
    PRINTLN("Last button pressed   : " + String(lastButtonPressed) + " (0b" + String(lastButtonPressed, BIN) + ")");
    PRINTLN("Last button depressed : " + String(lastButtonDepressed) + " (0b" + String(lastButtonDepressed, BIN) + ")");
    return true;
}

// Handle console commands specific to the remote controller
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

// Print remotecontroller-specific help information
bool Remotecontroller::printConsoleHelp() {
    PRINTLN("Remotecontroller Commands:\n\r");
    PRINTLN("  rs               - Print remotecontroller status");
    PRINTLN("  r?               - Print this help information");
    PRINTLN("");
    return true;
}

