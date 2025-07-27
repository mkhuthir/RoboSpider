#include "GaitController.h"
#include "Hexapod.h"
#include "Config.h"             // Include configuration header

// Constructor for GaitController class
GaitController::GaitController() {
    hexapod         = nullptr;
    gaitType        = GAIT_IDLE;
    lastUpdate      = 0;
    currentPhase    = 0;
    stepInterval    = 1000;  // Default 1000ms between steps
}

// Initialize the GaitController with a Hexapod instance
void GaitController::begin(Hexapod* hexapod){
    this->hexapod   = hexapod;
    gaitType        = GAIT_IDLE;    // Start with idle gait
    lastUpdate      = millis();     // Initialize last update time
    currentPhase    = 0;
    stepInterval    = 1000;          // Default 1000ms between steps
}

// Set the current gait type    
void GaitController::setGait(GaitType newGait) {
    gaitType        = newGait;
    currentPhase    = 0;
    lastUpdate      = millis();
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
    if (gaitType == GAIT_IDLE) return;              // Do nothing if in idle gait

    unsigned long now = millis();
    if (now - lastUpdate < stepInterval) return;    // Check if enough time has passed since the last update

    lastUpdate = now;
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
    stream.print(" | Interval: ");
    stream.print(stepInterval);
    stream.print(" | Phase: ");
    stream.println(currentPhase);
}

// Perform the wave gait
// In wave gait, one leg swings at a time
// This creates a smooth wave-like motion across the hexapod
void GaitController::doWaveGait() {
    // One leg swings at a time
    hexapod->legs[currentPhase].moveWaveGaitUp();
    delay(500);  // Delay to simulate step time
    hexapod->legs[currentPhase].moveWaveGaitDown();
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