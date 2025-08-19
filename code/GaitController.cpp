#include "Hexapod.h"
#include "GaitController.h"
#include "GaitPoses.h"
#include "Console.h"
#include "Debug.h"

// Constructor for GaitController class
GaitController::GaitController() {
    hexapod             = nullptr;
    currentPhase        = 0;
    currentStep         = 0;
    
    gaitType            = GAIT_IDLE;
    gaitWalkDirection   = 0;
    gaitRotateDirection = ROTATE_CW;
    gaitSpeed           = 300;
    gaitStepSize        = 100;
    
}

// Initialize the GaitController with a Hexapod instance
bool GaitController::begin(Hexapod* hexapod){
    
    this->hexapod       = hexapod;
    gaitType            = GAIT_IDLE;    // Start with idle gait
    currentPhase        = 0;            // Reset current phase
    currentStep         = 0;            // Reset current step

    gaitWalkDirection   = 0;            // Default direction
    gaitRotateDirection = ROTATE_CW;    // Default rotation direction
    gaitSpeed           = 300;          // Default speed
    gaitStepSize        = 100;          // Default step size

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
        case GAIT_ROTATE:               // Perform rotate gait
            doRotateGait();
            break;
        default:                        // If an unknown gait type is set, do nothing
            LOG_ERR("Unknown gait type, no action taken.");
            break;
    }
    return true;
}

// Set the current gait type
bool GaitController::setGaitType(GaitType newGait) {
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
        case GAIT_ROTATE:
            LOG_DBG("Rotate");
            break;
        default:
            LOG_DBG("Unknown");
            break;
    }
    return true;
    
}

// Get the current gait type
GaitType GaitController::getGaitType() const {
    return gaitType;
}

// Setters and getters
void GaitController::setWalkDirection(int8_t w_dir) {
    if (w_dir < -180) w_dir = -180;
    if (w_dir > 180) w_dir = 180;
    gaitWalkDirection = w_dir;
}
int8_t GaitController::getWalkDirection() const {
    return gaitWalkDirection;
}

bool GaitController::setGaitSpeed(uint16_t speed) {
    if (speed < 0) speed = 0;
    if (speed > 1023) speed = 1023;
    gaitSpeed = speed;
    return hexapod->setSpeed(speed);
}
uint16_t GaitController::getGaitSpeed() const {
    return gaitSpeed;
}

void GaitController::setGaitStepSize(uint16_t step_size) {
    if (step_size < 0) step_size = 0;
    if (step_size > 1023) step_size = 1023;
    gaitStepSize = step_size;
}
uint16_t GaitController::getGaitStepSize() const {
    return gaitStepSize;
}



// Perform the wave gait, one leg swings at a time
bool GaitController::doWaveGait() {

    if(hexapod->isMoving()) return false;      // If hexapod is already moving, do nothing

    if (currentPhase == (HEXAPOD_LEGS)) {                                           // if end of the wave gait cycle
        hexapod->moveStandUp();                                                     // Move hexapod to standing position
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
    return true;                                                                    // Return true to indicate gait was performed
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
                currentPhase    = (currentPhase + 1) % (HEXAPOD_LEGS/3+1);          // Increment phase with wrap-around
                break;
        }
        currentStep     = (currentStep + 1) % 2;                                    // Toggle between up and down poses
    }
    return true;
}

// Perform the rotate gait
bool GaitController::doRotateGait() {
    if(hexapod->isMoving()) return false;                                                 // If hexapod is already moving, do nothing
    // TODO: Implement rotate gait logic
    return true;

};

// Print the current gait status to Serial
bool GaitController::printStatus() {
    PRINTLN("GaitController Status: \n\r");
    PRINT("Gait Type ");
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
        case GAIT_ROTATE:
            PRINT("Rotate");
            break;
        default:
            PRINT("Unknown");
            break;
    }
    PRINT(" | Step " + String((int)currentStep));
    PRINTLN(" | Phase " + String((int)currentPhase));
    PRINTLN("Walk Direction   : " + String((int)gaitWalkDirection));
    PRINTLN("Rotate Direction : " + String(gaitRotateDirection == ROTATE_CW ? "CW" : "CCW"));
    PRINTLN("Gait Speed       : " + String((int)gaitSpeed));
    PRINTLN("Gait Step Size   : " + String((int)gaitStepSize));
    return true;
}

// Process console commands for gait control
bool GaitController::runConsoleCommands(const String& cmd, const String& args) {
    
    if (cmd == "gs") {
        if(!printStatus()) LOG_ERR("Failed to print status");
        return true;

    } else if (cmd == "gw") {
        setGaitType(GAIT_WAVE);
        LOG_INF("Gait set to WAVE");
        return true;

    } else if (cmd == "gr") {
        setGaitType(GAIT_RIPPLE);
        LOG_INF("Gait set to RIPPLE");
        return true;

    } else if (cmd == "gt") {
        setGaitType(GAIT_TRIPOD);
        LOG_INF("Gait set to TRIPOD");
        return true;

    } else if (cmd == "gi") {
        setGaitType(GAIT_IDLE);
        LOG_INF("Gait set to IDLE");
        return true;

    } else if (cmd == "grt") {
        setGaitType(GAIT_ROTATE);
        LOG_INF("Gait set to ROTATE");
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
    PRINTLN("  gw               - start Wave gait");
    PRINTLN("  gr               - start Ripple gait");
    PRINTLN("  gt               - start Tripod gait");
    PRINTLN("  gi               - start Idle gait");
    PRINTLN("  grt              - start Rotate gait");
    PRINTLN("");
    PRINTLN("  gswd [dir]       - Set walk direction -180 to 180 (default 0)");
    PRINTLN("  gsrd [dir]       - Set rotate direction CW or CCW (default CW)");
    PRINTLN("  gss [speed]      - Set gait speed 0 to 1023 (default 300)");
    PRINTLN("  gsz [size]       - Set gait step size (default 100)");
    PRINTLN("  g?               - Show this help");
    PRINTLN("");
    return true;
}
// GaitController.cpp
