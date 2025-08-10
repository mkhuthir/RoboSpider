#include "Servo.h"
#include "Leg.h"
#include "LegPoses.h"   // Include leg poses header
#include "Console.h"        // Add this include for logging macros


// Default constructor for Leg class
Leg::Leg(){
  // Initialize leg IDs to zero
  legIDs[Coxa]  = 0;
  legIDs[Femur] = 0;
  legIDs[Tibia] = 0;
  servo = nullptr;
}

// Initialize the leg servos
void Leg::init(uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, Servo* servo) {
  legIDs[Coxa]  = coxaID;   // Set coxa ID
  legIDs[Femur] = femurID;  // Set femur ID
  legIDs[Tibia] = tibiaID;  // Set tibia ID

  this->servo = servo;  // Set the servo pointer

  servo->init(legIDs[Coxa] , LEG_VELOCITY);   // Initialize coxa servo with velocity
  servo->init(legIDs[Femur], LEG_VELOCITY);   // Initialize femur servo with velocity
  servo->init(legIDs[Tibia], LEG_VELOCITY);   // Initialize tibia servo with velocity

  LOG_INF("Leg initialized successfully. (Servo IDs: " + String(legIDs[Coxa]) + ", " + String(legIDs[Femur]) + ", " + String(legIDs[Tibia]) + ")");
}

// Move the leg to the specified positions
void Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  servo->syncWrite(handler_index, legIDs, LEG_SERVOS, positions, num_positions);
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
void Leg::movePointUp() {
  move(poseLegPointUp);
}

// Move leg down
void Leg::movePointDown() {
  move(poseLegPointDown);
}

// Move leg out
void Leg::movePointOut() {
  move(poseLegPointOut);
}

// Move leg to stand up position
void Leg::moveStandUp() {
  move(poseLegStandUp);
}

// Move leg to stand down position
void Leg::moveStandDown() {
  move(poseLegStandDown);
}

// Get current coxa angle
int32_t Leg::getCoxa() {
  int32_t angle = 0;
  servo->getPosition(legIDs[Coxa], &angle);
  return angle;
}

// Get current femur angle
int32_t Leg::getFemur() {
  int32_t angle = 0;
  servo->getPosition(legIDs[Femur], &angle);
  return angle;
}

// Get current tibia angle
int32_t Leg::getTibia() {
  int32_t angle = 0;
  servo->getPosition(legIDs[Tibia], &angle);
  return angle;
}

// Print current joint angles
void Leg::printStatus() {
  PRINT("Coxa: ");
  PRINT((int)getCoxa());
  PRINT(" | Femur: ");
  PRINT((int)getFemur());
  PRINT(" | Tibia: ");
  PRINTLN((int)getTibia());
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
void Leg::printConsoleHelp() {
    PRINTLN("Leg Commands (add leg number 0-5 as argument, default=0):");
    PRINTLN("  lpu [n]          - Move leg point up");
    PRINTLN("  lpd [n]          - Move leg point down");
    PRINTLN("  lpo [n]          - Move leg point out");
    PRINTLN("  lsu [n]          - Move leg to stand up position");
    PRINTLN("  lsd [n]          - Move leg to stand down position");
    PRINTLN("  ls  [n]          - Print leg status (angles)");
    PRINTLN("  l?               - Show this help");
    PRINTLN("");
}


