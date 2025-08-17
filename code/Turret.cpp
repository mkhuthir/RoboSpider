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
  if (!servo->init(turret_ids[0], TURRET_SPEED, TURRET_PAN_CW_LIMIT, TURRET_PAN_CCW_LIMIT)) return false;
  if (!servo->init(turret_ids[1], TURRET_SPEED, TURRET_TILT_CW_LIMIT, TURRET_TILT_CCW_LIMIT)) return false;
  moveHome();
  LOG_INF("Turret initialized successfully. (Servo IDs: " + String(turret_ids[0]) + ", " + String(turret_ids[1]) + ")");
  return true;
}

// Update turret state
bool Turret::update() {
  for (int i = 0; i < TURRET_SERVOS; i++) {
    servo->update(turret_ids[i]);
  }
  return true;
}

// Rotate the turret to specified angles
bool Turret::move(int32_t *positions) {
  const uint8_t num_positions = 1;                                                              // Number of positions to write
  if (servo == nullptr) {
    LOG_ERR("Servo controller not initialized. Call begin() first.");
    return false;
  }
  return servo->syncWrite(handler_index, turret_ids, TURRET_SERVOS, positions, num_positions);  // Write target positions to servos
}

// Rotate the turret to home position
bool Turret::moveHome() {
  return move(poseTurretHome); 
}

// Rotate turret to the right
bool Turret::moveRight() {
  return move(poseTurretRight);
} 

// Rotate turret to the left
bool Turret::moveLeft() {
  return move(poseTurretLeft);
}

// Rotate turret up
bool Turret::moveUp() {
  return move(poseTurretUp);
}

// Rotate turret down
bool Turret::moveDown() {
  return move(poseTurretDown);
} 

// Print current turret angles to Serial
bool Turret::printStatus() {
  uint16_t panPosition = 0, tiltPosition = 0;
  if (!servo->getPresentPosition(turret_ids[0], &panPosition)) return false;
  if (!servo->getPresentPosition(turret_ids[1], &tiltPosition)) return false;
  PRINTLN("Turret Status: Pan: " + String(panPosition) + " | Tilt: " + String(tiltPosition));
  return true;
}

// Print turret-specific help information
bool Turret::printConsoleHelp() {
    PRINTLN("Turret Commands:\n\r");
    PRINTLN("  ts               - Print current turret angles");
    PRINTLN("");
    PRINTLN("  tu               - Move turret up");
    PRINTLN("  td               - Move turret down");
    PRINTLN("  tl               - Move turret left");
    PRINTLN("  tr               - Move turret right");
    PRINTLN("  th               - Move turret to home position");
    PRINTLN("");
    PRINTLN("  t?               - Print this help message");
    PRINTLN("");
    return true;
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
    return false; // Command not recognized
}