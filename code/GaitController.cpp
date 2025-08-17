#include "Hexapod.h"
#include "GaitController.h"
#include "GaitPoses.h"
#include "Console.h"
#include "Log.h"

// Constructor for GaitController class
GaitController::GaitController() {
    hexapod         = nullptr;
    gaitType        = GAIT_IDLE;
    currentPhase    = 0;
    currentStep     = 0;
}

// Initialize the GaitController with a Hexapod instance
bool GaitController::begin(Hexapod* hexapod){
    this->hexapod   = hexapod;
    gaitType        = GAIT_IDLE;    // Start with idle gait
    currentPhase    = 0;
    currentStep     = 0;            // Reset current step
    LOG_INF("GaitController initialized successfully.");
    return true;
}

// Update the gait controller, called periodically
bool GaitController::update() {
    if (gaitType == GAIT_IDLE) return false;  // Do nothing if in idle gait

    switch (gaitType) {
        case GAIT_WAVE:                 // Perform wave gait
            doWaveGait();
            break;
        case GAIT_RIPPLE:               // Perform ripple gait
            doRippleGait();
            break;
        case GAIT_TRIPOD:               // Perform tripod gait
            doTripodGait();
            break;
        default:                        // If an unknown gait type is set, do nothing               
            LOG_ERR("Unknown gait type, no action taken.");
            break;
    }
    return true;
}

// Set the current gait type
bool GaitController::setGait(GaitType newGait) {
    gaitType        = newGait;      // Set the new gait type
    currentPhase    = 0;            // Reset current phase for the new gait
    currentStep     = 0;            // Reset current step for the new gait
    hexapod->moveStandUp();         // Reset hexapod position when changing gait
    LOG_DBG("Gait set to: ");
    switch (gaitType) {
        case GAIT_IDLE:
            LOG_DBG("Idle");
            break;
        case GAIT_WAVE:
            LOG_DBG("Wave");
            break;
        case GAIT_RIPPLE:
            LOG_DBG("Ripple");
            break;
        case GAIT_TRIPOD:
            LOG_DBG("Tripod");
            break;
        default:
            LOG_DBG("Unknown");
            break;
    }
    return true;
    
}

// Get the current gait type
GaitType GaitController::getGait() const {
    return gaitType;
}

// Perform the wave gait, one leg swings at a time
bool GaitController::doWaveGait() {

    if(hexapod->isMoving()) return false;      // If hexapod is already moving, do nothing

    if (currentPhase == (HEXAPOD_LEGS)) {                                           // if end of the wave gait cycle
        hexapod->moveStandUp();                                                          // Move hexapod to standing position
        currentPhase = 0;                                                           // Reset phase for the next cycle                                     
        currentStep  = 0;                                                           // Reset step for the next cycle

    } else {                                                                        // If not at the end of the wave gait cycle
        switch(currentStep) {
            case 0:                                                                 // If current step is 0, move the current leg up
                hexapod->move(poseWaveGaitIDs[currentPhase], LEG_SERVOS, poseWaveGaitLegUp[currentPhase]);
                break;
            case 1:                                                                 // If current step is 1, move the current leg down
                hexapod->move(poseWaveGaitIDs[currentPhase], LEG_SERVOS, poseWaveGaitLegDown[currentPhase]);
                currentPhase    = (currentPhase + 1) % (HEXAPOD_LEGS+1);            // Increment phase with wrap-around
                break;
        }   
        currentStep     = (currentStep  + 1) % 2;                                   // Toggle between up and down poses
    }
    return true;                                                                   // Return true to indicate gait was performed
}


// Perform the ripple gait, two legs swing with a phase offset
bool GaitController::doRippleGait() {

    if(hexapod->isMoving()) return false;                                           // If hexapod is already moving, do nothing

    if (currentPhase == HEXAPOD_LEGS/2) {                                           // If end of the ripple gait cycle
        hexapod->moveStandUp();                                                     // Move hexapod to standing position
        currentPhase = 0;                                                           // Reset phase for the next cycle
        currentStep  = 0;                                                           // Reset step for the next cycle

    } else {                                                                        // If not at the end of the ripple gait cycle
        switch(currentStep) {
            case 0:                                                                 // If current step is 0, move the two legs up
                hexapod->move(poseRippleGaitIDs[currentPhase], LEG_SERVOS*2, poseRippleGaitLegUp[currentPhase]);
                break;

            case 1:                                                                 // If current step is 1, move the two legs down
                hexapod->move(poseRippleGaitIDs[currentPhase], LEG_SERVOS*2, poseRippleGaitLegDown[currentPhase]);
                currentPhase    = (currentPhase + 1) % (HEXAPOD_LEGS/2+1);            // Increment phase with wrap-around
                break;
        }
        currentStep     = (currentStep + 1) % 2;                                    // Toggle between up and down poses
    }
    return true;
}

// Perform the tripod gait, three legs swing at a time
bool GaitController::doTripodGait() {

    if(hexapod->isMoving()) return false;                                                 // If hexapod is already moving, do nothing

    if (currentPhase == HEXAPOD_LEGS/3) {                                           // If end of the tripod gait cycle
        hexapod->moveStandUp();                                                     // Move hexapod to standing position
        currentPhase = 0;                                                           // Reset phase for the next cycle
        currentStep  = 0;                                                           // Reset step for the next cycle

    } else {                                                                        // If not at the end of the tripod gait cycle
        switch(currentStep) {
            case 0:                                                                 // If current step is 0, move the two legs up
                hexapod->move(poseTripodGaitIDs[currentPhase], LEG_SERVOS*3, poseTripodGaitLegUp[currentPhase]);
                break;

            case 1:                                                                 // If current step is 1, move the two legs down
                hexapod->move(poseTripodGaitIDs[currentPhase], LEG_SERVOS*3, poseTripodGaitLegDown[currentPhase]);
                currentPhase    = (currentPhase + 1) % (HEXAPOD_LEGS/3+1);            // Increment phase with wrap-around
                break;
        }
        currentStep     = (currentStep + 1) % 2;                                    // Toggle between up and down poses
    }
    return true;
}

// Print the current gait status to Serial
bool GaitController::printStatus() {
    PRINT("\nGaitController Status: \n\r");
    PRINT("Gait: ");
    switch (gaitType) {
        case GAIT_IDLE:
            PRINT("Idle");
            break;
        case GAIT_WAVE:
            PRINT("Wave");
            break;
        case GAIT_RIPPLE:
            PRINT("Ripple");
            break;
        case GAIT_TRIPOD:
            PRINT("Tripod");
            break;
        default:
            PRINT("Unknown");
            break;
    }
    PRINT(" | Step: ");
    PRINT((int)currentStep);
    PRINT(" | Phase: ");
    PRINTLN((int)currentPhase);
    return true;
}

// Process console commands for gait control
bool GaitController::runConsoleCommands(const String& cmd, const String& args) {
    
    if (cmd == "gs") {
        if(!printStatus()) LOG_ERR("Failed to print status");
        return true;

    } else if (cmd == "gw") {
        setGait(GAIT_WAVE);
        LOG_INF("Gait set to WAVE");
        return true;

    } else if (cmd == "gr") {
        setGait(GAIT_RIPPLE);
        LOG_INF("Gait set to RIPPLE");
        return true;

    } else if (cmd == "gt") {
        setGait(GAIT_TRIPOD);
        LOG_INF("Gait set to TRIPOD");
        return true;

    } else if (cmd == "gi") {
        setGait(GAIT_IDLE);
        LOG_INF("Gait set to IDLE");
        return true;

    } else if (cmd == "g?") {
        printConsoleHelp();
        return true;
    }

    return false;
}

// Print gait-specific help information
bool GaitController::printConsoleHelp() {
    PRINTLN("Gait Commands:\n\r");
    PRINTLN("  gs               - Show current gait status");
    PRINTLN("");
    PRINTLN("  gw               - Set wave gait");
    PRINTLN("  gr               - Set ripple gait");
    PRINTLN("  gt               - Set tripod gait");
    PRINTLN("  gi               - Set idle gait");
    PRINTLN("");
    PRINTLN("  g?               - Show this help");
    PRINTLN("");
    return true;
}
// GaitController.cpp