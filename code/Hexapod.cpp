#include "Servo.h"
#include "Hexapod.h"
#include "Debug.h"
#include "HexapodPoses.h"       // Include hexapod poses header
#include "Console.h"        // Add this include for logging macros


// Constructor for Hexapod class
Hexapod::Hexapod(){
  servo         = nullptr;              // Dynamixel controller not initialized
  for (int i = 0; i < HEXAPOD_LEGS; i++) {
    legs[i] = Leg();                    // instantiate each leg
  }
}

// Initialize the hexapod
bool Hexapod::begin(Servo* servo) {
  
  this->servo=servo;
  
  legs[0].init(1,  2,  3,  servo);                  // Initialize each leg with servo IDs
  legs[1].init(4,  5,  6,  servo);
  legs[2].init(7,  8,  9,  servo);
  legs[3].init(10, 11, 12, servo);
  legs[4].init(13, 14, 15, servo);
  legs[5].init(16, 17, 18, servo);

  if (!servo->addSyncWriteHandler(1, "Goal_Position")) {   // Add sync write handler
    LOG_ERR("Failed to add sync write handler for Goal_Position");
    return false;
  }

  if (!moveStandUp()) {                                    // Move Hexapod to standing position
    LOG_ERR("Failed to move Hexapod to standing position");
    return false;
  }

  LOG_INF("Hexapod initialized successfully.");
  return true;                                      // Initialization successful
}

// Hexapod update
bool Hexapod::update() {
  // Update each leg
  for (int i = 0; i < HEXAPOD_LEGS; i++) {
    legs[i].update();
  }
  return true;
}

// Move Hexapod
bool Hexapod::move(uint8_t *ids, uint8_t num_servos, int32_t *positions) {
  const uint8_t num_positions   = 1;
  return servo->syncWrite(handler_index, ids, num_servos, positions, num_positions);
}

// Check if any leg is currently moving
bool Hexapod::isMoving() {
  for (int i = 0; i < HEXAPOD_LEGS; ++i) {
    if (legs[i].isMoving()) {
      return true;  // If any leg is moving, return true
    }
  }
  return false;  // If no legs are moving, return false
}

// Move Hexapod Up
bool Hexapod::moveStandUp() {
  if (!move(poseHexapodIDs, HEXAPOD_SERVOS, poseHexapodStandUP)) {
    LOG_ERR("Failed to move Hexapod to standing position");
    return false;
  }
  return true;
}

// Move Hexapod Down
bool Hexapod::moveStandDown() {
  if (!move(poseHexapodIDs, HEXAPOD_SERVOS, poseHexapodStandDown)) {
    LOG_ERR("Failed to move Hexapod to sitting position");
    return false;
  }
  return true;
}

// Print the status of all legs
bool Hexapod::printStatus() {
  PRINTLN("\nHexapod Legs Status:");
  for (int i = 0; i < 6; i++) {
    PRINT("Leg ");
    PRINT(i + 1);
    PRINT(": ");
    legs[i].printStatus();  // Print leg angles of each leg
  }
  return true;
}

// Process console commands for hexapod control
bool Hexapod::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "hsu") {
        if (moveStandUp()) LOG_INF("Hexapod standing up");
        return true;

    } else if (cmd == "hsd") {
        if (moveStandDown()) LOG_INF("Hexapod standing down");
        return true;

    } else if (cmd == "hs") {
        printStatus();
        return true;
        
    } else if (cmd == "h?") {
        printConsoleHelp();
        return true;

    }

    return false;
}

// Print hexapod-specific help information
bool Hexapod::printConsoleHelp() {
    PRINTLN("Hexapod Commands:\n\r");
    PRINTLN("  hs               - Print hexapod legs status");
    PRINTLN("  hsu              - Hexapod stand up");
    PRINTLN("  hsd              - Hexapod stand down");
    PRINTLN("  h?               - Print this help information");
    PRINTLN("");
    return true;
}


