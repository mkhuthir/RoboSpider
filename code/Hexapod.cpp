#include "Servo.h"
#include "Hexapod.h"
#include "Debug.h"
#include "HexapodPoses.h"       // Include hexapod poses header
#include "Console.h"        // Add this include for logging macros


// Constructor for Hexapod class
Hexapod::Hexapod(){
  driver  = nullptr;                    // Dynamixel controller not initialized
  servo   = nullptr;                    // Servo controller not initialized
  speed   = HEXAPOD_SPEED;              // Speed not initialized

  for (int i = 0; i < HEXAPOD_LEGS; i++) {
    legs[i] = Leg();                    // instantiate each leg
  }
}

// Initialize the hexapod
bool Hexapod::begin(Driver* driver , Servo* servo) {
  if (driver == nullptr || servo == nullptr) {
    LOG_ERR("Driver or Servo is not initialized.");
    return false;
  }

  this->driver = driver;  // Set the driver pointer
  this->servo = servo;    // Set the servo pointer
  this->speed = HEXAPOD_SPEED;

  //           i  S1  S2  S3  X  Y  Z  Roll  Pitch  Yaw
  legs[0].init(0, 1,  2,  3,  0, 0, 0, 0, 0, 0, driver, servo); // Initialize each leg with servo IDs
  legs[1].init(1, 4,  5,  6,  0, 0, 0, 0, 0, 0, driver, servo);
  legs[2].init(2, 7,  8,  9,  0, 0, 0, 0, 0, 0, driver, servo);
  legs[3].init(3, 10, 11, 12, 0, 0, 0, 0, 0, 0, driver, servo);
  legs[4].init(4, 13, 14, 15, 0, 0, 0, 0, 0, 0, driver, servo);
  legs[5].init(5, 16, 17, 18, 0, 0, 0, 0, 0, 0, driver, servo);

  if (!driver->addSyncWriteHandler(1, "Goal_Position")) {   // Add sync write handler
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
  return driver->syncWrite(handler_index, ids, num_servos, positions, num_positions);
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

// Set the speed of the hexapod
bool Hexapod::setSpeed(uint16_t speed) {
  if (speed < 0) speed = 0;
  if (speed > 1023) speed = 1023;
  for (int i = 0; i < HEXAPOD_LEGS; i++) {
    legs[i].setSpeed(speed);
  }
  return true;
}

// Get the speed of the hexapod
uint16_t Hexapod::getSpeed() const {
  return speed;
}

// Get

// Print the status of all legs
bool Hexapod::printStatus() {
  PRINTLN("\nHexapod Legs Status:");
  for (int i = 0; i < HEXAPOD_LEGS; i++) {
    legs[i].printStatus();  // Print leg angles of each leg
  }
  PRINTLN("Hexapod Speed: " + String(speed));
  return true;
}

// Process console commands for hexapod control
bool Hexapod::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "hs") {
        printStatus();
        return true;

    } else if (cmd == "hss") {
        if (args.length() > 0) {
            int newSpeed = args.toInt();
            setSpeed(newSpeed);
            speed = newSpeed;
            LOG_INF("Hexapod speed set to " + String(newSpeed));
        } else {
            setSpeed(HEXAPOD_SPEED);
            speed = HEXAPOD_SPEED;
            LOG_INF("Hexapod speed: " + String((int)HEXAPOD_SPEED));
        }
        return true;

    } else if (cmd == "hgs") {
        PRINTLN("Hexapod Speed: " + String(speed));
        return true;

    } else if (cmd == "hsu") {
        if (moveStandUp()) LOG_INF("Hexapod standing up");
        return true;

    } else if (cmd == "hsd") {
        if (moveStandDown()) LOG_INF("Hexapod standing down");
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
    PRINTLN("");
    PRINTLN("  hss [speed]      - Set hexapod speed (default: " + String(HEXAPOD_SPEED) + ")");
    PRINTLN("  hgs              - Get hexapod speed");
    PRINTLN("");
    PRINTLN("  hsu              - Hexapod stand up");
    PRINTLN("  hsd              - Hexapod stand down");
    PRINTLN("  h?               - Print this help information");
    PRINTLN("");
    return true;
}


