#include "GaitController.h"
#include "Hexapod.h"
#include "Config.h"             // Include configuration header
#include "GaitPoses.h"        // Include gait poses header


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
    return true;
}

// Set the current gait type    
void GaitController::setGait(GaitType newGait) {
    gaitType        = newGait;      // Set the new gait type
    currentPhase    = 0;            // Reset current phase for the new gait
    currentStep     = 0;            // Reset current step for the new gait
    hexapod->moveStandUp();              // Reset hexapod position when changing gait

    #ifdef DEBUG
        Serial.print("Gait set to: ");
        switch (gaitType) {
            case GAIT_IDLE:
                Serial.println("Idle");
                break;
            case GAIT_WAVE:
                Serial.println("Wave");
                break;
            case GAIT_RIPPLE:
                Serial.println("Ripple");
                break;
            case GAIT_TRIPOD:
                Serial.println("Tripod");
                break;
            default:
                Serial.println("Unknown");
                break;
        }
    #endif // DEBUG
}

// Get the current gait type
GaitType GaitController::getGait() const {
    return gaitType;
}

// Update the gait controller, called periodically
void GaitController::update() {
    if (gaitType == GAIT_IDLE) return;  // Do nothing if in idle gait

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
            #ifdef DEBUG
                    Serial.println("Unknown gait type, no action taken.");
            #endif // DEBUG
            break;
    }
}

// Perform the wave gait, one leg swings at a time
void GaitController::doWaveGait() {

    if(hexapod->isMoving()) return;                                                 // If hexapod is already moving, do nothing

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
}


// Perform the ripple gait, two legs swing with a phase offset
void GaitController::doRippleGait() {

    if(hexapod->isMoving()) return;                                                 // If hexapod is already moving, do nothing
    
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
}

// Perform the tripod gait, three legs swing at a time
void GaitController::doTripodGait() {

    if(hexapod->isMoving()) return;                                                 // If hexapod is already moving, do nothing

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

}

// Print the current gait status to Serial
void GaitController::printStatus(Stream& stream) {
    stream.print("\nGaitController Status: ");
    stream.print("Gait: ");
    switch (gaitType) {
        case GAIT_IDLE:
            stream.print("Idle");
            break;
        case GAIT_WAVE:
            stream.print("Wave");
            break;
        case GAIT_RIPPLE:
            stream.print("Ripple");
            break;
        case GAIT_TRIPOD:
            stream.print("Tripod");
            break;
        default:
            stream.print("Unknown");
            break;
    }
    stream.print(" | Step: ");
    stream.print(currentStep);
    stream.print(" | Phase: ");
    stream.println(currentPhase);
}
// GaitController.cpp