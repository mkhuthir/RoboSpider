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
void GaitController::begin(Hexapod* hexapod){
    this->hexapod   = hexapod;
    gaitType        = GAIT_IDLE;    // Start with idle gait
    currentPhase    = 0;
    currentStep     = 0;            // Reset current step
}

// Set the current gait type    
void GaitController::setGait(GaitType newGait) {
    gaitType        = newGait;      // Set the new gait type
    currentPhase    = 0;            // Reset current phase for the new gait
    currentStep     = 0;            // Reset current step for the new gait
    hexapod->moveUp();              // Reset hexapod position when changing gait

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
    if (gaitType == GAIT_IDLE) return;              // Do nothing if in idle gait

    switch (gaitType) {
        case GAIT_WAVE:             // Perform wave gait
            doWaveGait();
            break;
        case GAIT_RIPPLE:           // Perform ripple gait
            doRippleGait();
            break;
        case GAIT_TRIPOD:           // Perform tripod gait
            doTripodGait();
            break;
        default:                    // If an unknown gait type is set, do nothing               
            #ifdef DEBUG
                    Serial.println("Unknown gait type, no action taken.");
            #endif // DEBUG
            break;
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

// Perform the wave gait
// In wave gait, one leg swings at a time
void GaitController::doWaveGait() {

    // If the current leg or the previous leg is moving, skip update
    if (hexapod->legs[currentPhase].isMoving() || hexapod->legs[(currentPhase - 1) % HEXAPOD_LEGS].isMoving()) {
        return;
    }
    delay(500); // Add a small delay to allow leg movement to complete
    // One leg swings at a time
    switch(currentStep) {
        case 0:                                                                 // Move the current leg up
            hexapod->legs[currentPhase].move(poseLegWaveGaitUp[currentPhase]);
            break;
        case 1:                                                                 // Move the current leg down
            hexapod->legs[currentPhase].move(poseLegWaveGaitDown[currentPhase]);
            currentPhase    = (currentPhase + 1) % (HEXAPOD_LEGS+1);            //
            break;
    }
    
    currentStep     = (currentStep  + 1) % 2;                                   // Toggle between up and down poses

    if (currentPhase == (HEXAPOD_LEGS)) {
        delay(500);
        currentPhase = 0;                                                        // Reset hexapod position after a full cycle
        hexapod->moveUp();
    }
}

// Perform the ripple gait
// In ripple gait, two legs swing with a phase offset
void GaitController::doRippleGait() {
    // Two legs with phase offset
    int swingLegs[2] = {
        currentPhase,
        (currentPhase + 3) % 6
    };

    for (int i = 0; i < 2; ++i) {
        //hexapod->moveLeg(swingLegs[i], COXA_UP_DEG, FEMUR_UP_DEG, TIBIA_UP_DEG);
    }
    for (int i = 0; i < 2; ++i) {
        //hexapod->moveLeg(swingLegs[i], COXA_HOME_DEG, FEMUR_HOME_DEG, TIBIA_HOME_DEG);
    }

    currentPhase = (currentPhase + 1) % 3;
}

// Perform the tripod gait
// In tripod gait, three legs swing while the other three are in stance
void GaitController::doTripodGait() {
    // Tripod groups:
    int groupA[3] = {0, 3, 4};
    int groupB[3] = {1, 2, 5};

    int* swingGroup;
    if (currentPhase % 2 == 0) {
        swingGroup = groupA;
    } else {
        swingGroup = groupB;
    }
    for (int i = 0; i < 3; ++i) {
        //hexapod->moveLeg(swingGroup[i], COXA_UP_DEG, FEMUR_UP_DEG, TIBIA_UP_DEG);
    }
    for (int i = 0; i < 3; ++i) {
        //hexapod->moveLeg(swingGroup[i], COXA_HOME_DEG, FEMUR_HOME_DEG, TIBIA_HOME_DEG);
    }
    currentPhase = (currentPhase + 1) % 2;
}
// GaitController.cpp