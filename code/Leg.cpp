#include "Servo.h"
#include "Leg.h"
#include "LegPoses.h"
#include "Console.h"
#include "Debug.h"


// Default constructor for Leg class
Leg::Leg(){
  // Initialize leg IDs to zero
  legIDs[Coxa]  = 0;
  legIDs[Femur] = 0;
  legIDs[Tibia] = 0;
  servo = nullptr;
}

// Initialize the leg servos
bool Leg::init(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Driver* driver, Servo* servo) {
  legIDs[Coxa]  = coxaID;   // Set coxa ID
  legIDs[Femur] = femurID;  // Set femur ID
  legIDs[Tibia] = tibiaID;  // Set tibia ID

  this->driver = driver;    // Set the driver pointer
  this->servo = servo;      // Set the servo pointer

  if (!servo->init(legIDs[Coxa] , LEG_SPEED, COXA_CW_LIMIT, COXA_CCW_LIMIT)) {   // Initialize coxa servo with velocity
    LOG_ERR("Failed to initialize coxa servo.");
    return false;
  }
  if (!servo->init(legIDs[Femur], LEG_SPEED, FEMUR_CW_LIMIT, FEMUR_CCW_LIMIT)) {   // Initialize femur servo with velocity
    LOG_ERR("Failed to initialize femur servo.");
    return false;
  }
  if (!servo->init(legIDs[Tibia], LEG_SPEED, TIBIA_CW_LIMIT, TIBIA_CCW_LIMIT)) {   // Initialize tibia servo with velocity
    LOG_ERR("Failed to initialize tibia servo.");
    return false;
  }

  LOG_INF("Leg initialized successfully. (Servo IDs: " + String(legIDs[Coxa]) + ", " + String(legIDs[Femur]) + ", " + String(legIDs[Tibia]) + ")");
  return true;

}

// Update the leg state
bool Leg::update() {
  // Update each servo
  for (int i = 0; i < LEG_SERVOS; i++) {
    servo->update(legIDs[i]);
  }
  return true;
}

// Move the leg to the specified positions
bool Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  if(!driver->syncWrite(handler_index, legIDs, LEG_SERVOS, positions, num_positions)) {
    LOG_ERR("Failed to move leg.");
    return false;
  }
  return true;
}

// Check if any servo in the leg is currently moving
bool Leg::isMoving() {
  uint8_t moving = 0;
  for (int i = 0; i < LEG_SERVOS; ++i) {
    if (servo->isMoving(legIDs[i])) {
      return true;  // If any servo is moving, return true
    }
  }
  return false;
}

// Move leg up
bool Leg::movePointUp() {
  return move(poseLegPointUp);
}

// Move leg down
bool Leg::movePointDown() {
  return move(poseLegPointDown);
}

// Move leg out
bool Leg::movePointOut() {
  return move(poseLegPointOut);
}

// Move leg to stand up position
bool Leg::moveStandUp() {
  return move(poseLegStandUp);
}

// Move leg to stand down position
bool Leg::moveStandDown() {
  return move(poseLegStandDown);
}

// Get current coxa angle
bool Leg::getCoxa(uint16_t* angle) {
  if (!servo->getPresentPosition(legIDs[Coxa], (uint16_t*)angle)) {
    LOG_ERR("Failed to get coxa position.");
    return false;
  }
  return true;
}

// Get current femur angle
bool Leg::getFemur(uint16_t* angle) {
  if (!servo->getPresentPosition(legIDs[Femur], (uint16_t*)angle)) {
    LOG_ERR("Failed to get femur position.");
    return false;
  }
  return true;
}

// Get current tibia angle
bool Leg::getTibia(uint16_t* angle) {
  if (!servo->getPresentPosition(legIDs[Tibia], (uint16_t*)angle)) {
    LOG_ERR("Failed to get tibia position.");
    return false;
  }
  return true;
}

// Print current joint angles
bool Leg::printStatus() {
  uint16_t coxaAngle = 0, femurAngle = 0, tibiaAngle = 0;
  if (!getCoxa(&coxaAngle) || !getFemur(&femurAngle) || !getTibia(&tibiaAngle)) {
    LOG_ERR("Failed to get joint angles.");
    return false;
  }
  PRINT("Coxa: ");
  PRINT((int)coxaAngle);
  PRINT(" | Femur: ");
  PRINT((int)femurAngle);
  PRINT(" | Tibia: ");
  PRINTLN((int)tibiaAngle);
  return true;
}

// Process console commands for leg control
bool Leg::runConsoleCommands(const String& cmd, const String& args, int legIndex) {
    if (cmd == "lpu") {
        movePointUp();
        LOG_INF("Leg " + String(legIndex) + " point moving up");
        return true;

    } else if (cmd == "lpd") {
        movePointDown();
        LOG_INF("Leg " + String(legIndex) + " point moving down");
        return true;

    } else if (cmd == "lpo") {
        movePointOut();
        LOG_INF("Leg " + String(legIndex) + " point moving out");
        return true;

    } else if (cmd == "lsu") {
        moveStandUp();
        LOG_INF("Leg " + String(legIndex) + " standing up");
        return true;

    } else if (cmd == "lsd") {
        moveStandDown();
        LOG_INF("Leg " + String(legIndex) + " standing down");
        return true;

    } else if (cmd == "ls") {
        printStatus();
        return true;

    } else if (cmd == "l?") {
        printConsoleHelp();
        return true;

    }

    return false;
}

// Print leg-specific help information
bool Leg::printConsoleHelp() {
    PRINTLN("Leg Commands:\n\r");
    PRINTLN("  ls  [n]          - Print leg status (default: 0)");
    PRINTLN("");
    PRINTLN("  lpu [n]          - Move leg point up (default: 0)");
    PRINTLN("  lpd [n]          - Move leg point down (default: 0)");
    PRINTLN("  lpo [n]          - Move leg point out (default: 0)");
    PRINTLN("");
    PRINTLN("  lsu [n]          - Move leg to stand up position (default: 0)");
    PRINTLN("  lsd [n]          - Move leg to stand down position (default: 0)");
    PRINTLN("");
    PRINTLN("  l?               - Show this help");
    PRINTLN("");
    return true;
}


