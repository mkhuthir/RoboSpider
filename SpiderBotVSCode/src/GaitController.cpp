#include "GaitController.h"
#include "Hexapod.h"
#include "Config.h"             // Include configuration header

// Constructor for GaitController class
GaitController::GaitController() {
    robot           = nullptr;
    gait            = GAIT_IDLE;
    lastUpdate      = 0;
    currentPhase    = 0;
    stepInterval    = 500;  // Default 500ms between steps
}

// Initialize the GaitController with a Hexapod instance
void GaitController::init(Hexapod* hexapod){
    robot           = hexapod;
    gait            = GAIT_IDLE;
    lastUpdate      = millis();
    currentPhase    = 0;
    stepInterval    = 500;  // Default 500ms between steps
}

// Set the current gait type    
void GaitController::setGait(GaitType newGait) {
    gait            = newGait;
    currentPhase    = 0;
    lastUpdate      = millis();
}

// Get the current gait type
GaitType GaitController::getGait() const {
    return gait;
}

// Set the step interval for the gait
void GaitController::setStepInterval(unsigned long interval) {
    stepInterval    = interval;
}

// Get the current step interval
unsigned long GaitController::getStepInterval() const {
    return stepInterval;
}

// Update the gait controller, called periodically
void GaitController::update() {
    if (gait == GAIT_IDLE) return;

    unsigned long now = millis();
    if (now - lastUpdate < stepInterval) return;

    lastUpdate = now;

    switch (gait) {
        case GAIT_WAVE:
            doWaveGait();
            break;
        case GAIT_RIPPLE:
            doRippleGait();
            break;
        case GAIT_TRIPOD:
            doTripodGait();
            break;
        default:
            break;
    }
}

// Perform the wave gait
// In wave gait, one leg swings at a time
// This creates a smooth wave-like motion across the hexapod
void GaitController::doWaveGait() {
    // One leg swings at a time
    //robot->moveLeg(currentPhase, 512, 300, 700);  // swing phase
    delay(50);
    //robot->moveLeg(currentPhase, 512, 512, 512);  // stance phase

    currentPhase = (currentPhase + 1) % 6;
}

// Perform the ripple gait
// In ripple gait, two legs swing with a phase offset
// This creates a wave-like motion across the hexapod
void GaitController::doRippleGait() {
    // Two legs with phase offset
    int swingLegs[2] = {
        currentPhase,
        (currentPhase + 3) % 6
    };

    for (int i = 0; i < 2; ++i) {
        //robot->moveLeg(swingLegs[i], 512, 300, 700);
    }
    delay(50);
    for (int i = 0; i < 2; ++i) {
        //robot->moveLeg(swingLegs[i], 512, 512, 512);
    }

    currentPhase = (currentPhase + 1) % 3;
}

// Perform the tripod gait
// In tripod gait, three legs swing while the other three are in stance
void GaitController::doTripodGait() {
    // Tripod groups:
    // Group A: 0, 3, 4
    // Group B: 1, 2, 5
    int groupA[3] = {0, 3, 4};
    int groupB[3] = {1, 2, 5};

    int* swingGroup;
    if (currentPhase % 2 == 0) {
        swingGroup = groupA;
    } else {
        swingGroup = groupB;
    }

    for (int i = 0; i < 3; ++i) {
        //robot->moveLeg(swingGroup[i], 512, 300, 700);
    }
    delay(50);
    for (int i = 0; i < 3; ++i) {
        //robot->moveLeg(swingGroup[i], 512, 512, 512);
    }

    currentPhase = (currentPhase + 1) % 2;
}
// GaitController.cpp