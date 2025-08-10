#include "Turret.h"
#include "TurretPoses.h"
#include "Console.h"        // Add this include for logging macros

Turret::Turret(){
  turret_ids[0] = TURRET_PAN_ID;  // Set pan servo ID
  turret_ids[1] = TURRET_TILT_ID; // Set tilt servo ID
  servo         = nullptr;        // Dynamixel controller not initialized
}

// Initialize the turret servos
bool Turret::begin(Servo* servo) {
  
  this->servo = servo;  // Set the servo pointer
  
  if (!servo->init(turret_ids[0], TURRET_VELOCITY)) {   // Initialize pan servo with default velocity
    LOG_ERR("Failed to initialize turret pan servo ID: " + String(turret_ids[0]));
    return false;
  }

  if (!servo->init(turret_ids[1], TURRET_VELOCITY)) {   // Initialize tilt servo with default velocity
    LOG_ERR("Failed to initialize turret tilt servo ID: " + String(turret_ids[1]));
    return false;
  }

  moveHome();                           // Move turret to home position
  LOG_INF("Turret initialized successfully. (Servo IDs: " + String(turret_ids[0]) + ", " + String(turret_ids[1]) + ")");
  return true;                          // Return true if initialization is successful
}

// Update turret state
bool Turret::update() {
 
  return true;      // Return true if update is successful
}

// Rotate the turret to specified angles
void Turret::move(int32_t *positions) {
  if (servo == nullptr) {
    LOG_ERR("Cannot move turret: servo not initialized");
    return;
  }
  
  const uint8_t num_positions = 1;                                                        // Number of positions to write
  servo->syncWrite(handler_index, turret_ids, TURRET_SERVOS, positions, num_positions);   // Write target positions to servos
  LOG_DBG("Turret moved to position - Pan: " + String(positions[0]) + ", Tilt: " + String(positions[1]));
}

// Rotate the turret to home position
void Turret::moveHome() {
  move(poseTurretHome);    // Reset turret to home position
}

// Rotate turret to the right
void Turret::moveRight() {
  move(poseTurretRight); // Rotate turret to the right
} 

// Rotate turret to the left
void Turret::moveLeft() {
  move(poseTurretLeft);  // Rotate turret to the left
}

// Rotate turret up
void Turret::moveUp() {
  move(poseTurretUp);  // Rotate turret up
}

// Rotate turret down
void Turret::moveDown() {
  move(poseTurretDown); // Rotate turret down
} 

// Print current turret angles to Serial
void Turret::printStatus() {
  if (servo == nullptr) {
    LOG_ERR("Turret Status: Not initialized");
    return;
  }
  
  int32_t panPosition = 0, tiltPosition = 0;

  if (!servo->getPosition(turret_ids[0], &panPosition)) {
    LOG_ERR("Failed to read pan servo position");
  }

  if (!servo->getPosition(turret_ids[1], &tiltPosition)) {
    LOG_ERR("Failed to read tilt servo position");
  }

  PRINT("Turret Status: Pan: ");
  PRINT((int)panPosition);
  PRINT(" | Tilt: ");
  PRINTLN((int)tiltPosition);
}

// Print turret-specific help information
void Turret::printConsoleHelp() {
    PRINTLN("Turret Commands:");
    PRINTLN("  tu               - Move turret up");
    PRINTLN("  td               - Move turret down");
    PRINTLN("  tl               - Move turret left");
    PRINTLN("  tr               - Move turret right");
    PRINTLN("  th               - Move turret to home position");
    PRINTLN("  ts               - Print current turret angles");
    PRINTLN("  t?               - Print this help message");
    PRINTLN("");
}

// Process console commands for turret control
bool Turret::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "tu") {
        moveUp();
        return true;

    } else if (cmd == "td") {
        moveDown();
        return true;

    } else if (cmd == "tl") {
        moveLeft();
        return true;

    } else if (cmd == "tr") {
        moveRight();
        return true;

    } else if (cmd == "th") {
        moveHome();
        return true;

    } else if (cmd == "ts") {
        printStatus();
        return true;

    } else if (cmd == "t?") {
        printConsoleHelp();
        return true;

    }
    return false;
}