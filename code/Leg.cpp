#include "Servo.h"
#include "Leg.h"
#include "LegPoses.h"
#include "Console.h"
#include "Debug.h"


// Default constructor for Leg class
Leg::Leg(){
  // Initialize leg IDs to zero
  legIndex            = 0;
  legBaseX            = 0.0;
  legBaseY            = 0.0;
  legBaseZ            = 0.0;
  legBaseR            = 0.0;
  legServoIDs[Coxa]   = 0;
  legServoIDs[Femur]  = 0;
  legServoIDs[Tibia]  = 0;
  driver              = nullptr;
  servo               = nullptr;
  speed               = LEG_SPEED;

}

// Initialize the leg servos
bool Leg::init( uint8_t legIndex,
                uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, 
                float legBaseX, float legBaseY, float legBaseZ, 
                float legBaseR,
                Driver* driver, Servo* servo) {

  this->legIndex      = legIndex;
  this->legBaseX      = legBaseX;
  this->legBaseY      = legBaseY;
  this->legBaseZ      = legBaseZ;
  this->legBaseR      = legBaseR;
  legServoIDs[Coxa]   = coxaID;   // Set coxa ID
  legServoIDs[Femur]  = femurID;  // Set femur ID
  legServoIDs[Tibia]  = tibiaID;  // Set tibia ID
  this->driver        = driver;    // Set the driver pointer
  this->servo         = servo;      // Set the servo pointer
  this->speed         = LEG_SPEED;  // Set default speed

  if (!servo->init(legServoIDs[Coxa] , LEG_SPEED, COXA_CW_LIMIT, COXA_CCW_LIMIT)) {   // Initialize coxa servo with velocity
    LOG_ERR("Failed to initialize coxa servo.");
    return false;
  }
  if (!servo->init(legServoIDs[Femur], LEG_SPEED, FEMUR_CW_LIMIT, FEMUR_CCW_LIMIT)) {   // Initialize femur servo with velocity
    LOG_ERR("Failed to initialize femur servo.");
    return false;
  }
  if (!servo->init(legServoIDs[Tibia], LEG_SPEED, TIBIA_CW_LIMIT, TIBIA_CCW_LIMIT)) {   // Initialize tibia servo with velocity
    LOG_ERR("Failed to initialize tibia servo.");
    return false;
  }

  LOG_INF("Leg initialized successfully. (Servo IDs: " + String(legServoIDs[Coxa]) + ", " + String(legServoIDs[Femur]) + ", " + String(legServoIDs[Tibia]) + ")");
  return true;

}

// Update the leg state
bool Leg::update() {
  // Update each servo
  for (int i = 0; i < LEG_SERVOS; i++) {
    servo->update(legServoIDs[i]);
  }
  return true;
}

// Move the leg to the specified positions
bool Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  if(!driver->syncWrite(handler_index, legServoIDs, LEG_SERVOS, positions, num_positions)) {
    LOG_ERR("Failed to move leg.");
    return false;
  }
  return true;
}

// Check if any servo in the leg is currently moving
bool Leg::isMoving() {
  uint8_t moving = 0;
  for (int i = 0; i < LEG_SERVOS; ++i) {
    if (servo->isMoving(legServoIDs[i])) {
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

// Set the speed of the leg
bool Leg::setSpeed(uint16_t speed) {
  for (int i = 0; i < LEG_SERVOS; i++) {
    if (!servo->setGoalSpeed(legServoIDs[i], speed)) {
      LOG_ERR("Failed to set speed for leg servo.");
      return false;
    }
  }
  this->speed = speed;
  return true;
}

// Get the current speed of the leg
uint16_t Leg::getSpeed() const {
  return speed;
}

// Get the leg base position
bool Leg::getBasePosition(float* base_x, float* base_y, float* base_z, float* base_r) {
  *base_x = legBaseX;
  *base_y = legBaseY;
  *base_z = legBaseZ;
  *base_r = legBaseR;
  return true;
}

// Get the current positions of the leg joints
bool Leg::getServoPositions(uint16_t* coxa, uint16_t* femur, uint16_t* tibia) {
  if (!servo->getPresentPosition(legServoIDs[Coxa], coxa)) {
    LOG_ERR("Failed to get coxa position.");
    return false;
  }
  if (!servo->getPresentPosition(legServoIDs[Femur], femur)) {
    LOG_ERR("Failed to get femur position.");
    return false;
  }
  if (!servo->getPresentPosition(legServoIDs[Tibia], tibia)) {
    LOG_ERR("Failed to get tibia position.");
    return false;
  }
  return true;
}

// Get the leg tip position
bool Leg::getTipPosition(float* tip_x, float* tip_y, float* tip_z) {
  // TODO: Forward Kinematics to compute tip position from joint angles
  *tip_x = 1.0;
  *tip_y = 2.0;
  *tip_z = 3.0;
  return true;
}

// IK in local coordinates (relative to leg base frame)
bool Leg::getIKLocal(float tip_x, float tip_y, float tip_z, int16_t* positions)
{
    // 1. Coxa (rotation in XY plane)
    float coxa_angle_rad = atan2(tip_y, tip_x);
    float coxa_angle_deg = coxa_angle_rad * 180.0f / M_PI;

    float r = sqrt(tip_x * tip_x + tip_y * tip_y) - COXA_LENGTH;
    float z = tip_z;
    float leg_length = sqrt(r * r + z * z);

    if (leg_length > (FEMUR_LENGTH + TIBIA_LENGTH) || leg_length < fabs(FEMUR_LENGTH - TIBIA_LENGTH)) {
        return false;
    }

    float a1 = atan2(z, r);
    float a2 = acos((FEMUR_LENGTH*FEMUR_LENGTH + leg_length*leg_length - TIBIA_LENGTH*TIBIA_LENGTH) / (2 * FEMUR_LENGTH * leg_length));
    float femur_angle_rad = a1 + a2;
    float femur_angle_deg = femur_angle_rad * 180.0f / M_PI;

    float tibia_angle_rad = acos((FEMUR_LENGTH*FEMUR_LENGTH + TIBIA_LENGTH*TIBIA_LENGTH - leg_length*leg_length) / (2 * FEMUR_LENGTH * TIBIA_LENGTH));
    float tibia_angle_deg = 180.0f - tibia_angle_rad * 180.0f / M_PI;

    // --- Servo mapping: 0-300 deg → 0-1023 ---
    float coxa_angle_deg_adjusted = coxa_angle_deg - (legBaseR * 180.0f / M_PI);        // Adjust coxa angle based on legBaseR (convert to degrees)
    positions[Coxa]  = static_cast<int16_t>(coxa_angle_deg_adjusted * (1023.0f / 300.0f));
    positions[Femur] = static_cast<int16_t>(femur_angle_deg         * (1023.0f / 300.0f));
    positions[Tibia] = static_cast<int16_t>(tibia_angle_deg         * (1023.0f / 300.0f));
    return true;
}

// IK in global coordinates (relative to body center)
bool Leg::getIKGlobal(float tip_x_global, float tip_y_global, float tip_z_global, int16_t* positions)
{
    float x_local, y_local, z_local;
    transGlobalToLocal(tip_x_global, tip_y_global, tip_z_global, x_local, y_local, z_local);
    return getIKLocal(x_local, y_local, z_local, positions);
}

// Utility function to transform global (body) to local (leg base) coordinates
void Leg::transGlobalToLocal( float x_global, float y_global, float z_global,
                float& x_local, float& y_local, float& z_local)
{
  // Subtract base position
  float x = x_global - legBaseX;
  float y = y_global - legBaseY;
  float z = z_global - legBaseZ;

  // Apply inverse yaw rotation only (around Z axis)
  float cy = cos(-legBaseR);
  float sy = sin(-legBaseR);

  x_local = cy * x - sy * y;
  y_local = sy * x + cy * y;
  z_local = z; // No change in Z for yaw-only rotation
}

// Print current joint angles
bool Leg::printStatus() {
  uint16_t coxaAngle = 0, femurAngle = 0, tibiaAngle = 0;
  float legBaseX = 0, legBaseY = 0, legBaseZ = 0; float legBaseR = 0;
  float tipX = 0, tipY = 0, tipZ = 0;

  if (!getServoPositions(&coxaAngle, &femurAngle, &tibiaAngle)) {
    LOG_ERR("Failed to get joint angles.");
    return false;
  }

  if (!getBasePosition(&legBaseX, &legBaseY, &legBaseZ, &legBaseR)) {
    LOG_ERR("Failed to get leg base position.");
    return false;
  }

  if (!getTipPosition(&tipX, &tipY, &tipZ)) {
    LOG_ERR("Failed to get leg tip position.");
    return false;
  }

  PRINTLN("Leg: " + String((int)legIndex));
  PRINTLN("Base         : X: " + String((int)legBaseX) + "mm, Y: " + String((int)legBaseY) + "mm, Z: " + String((int)legBaseZ) + "mm, R: " + String((int)legBaseR) + "°");
  PRINTLN("Servo IDs    : Coxa: " + String((int)legServoIDs[0]) + ", Femur: " + String((int)legServoIDs[1]) + ", Tibia: " + String((int)legServoIDs[2]));
  PRINTLN("Servo Pos.   : Coxa: " + String((int)coxaAngle) + ", Femur: " + String((int)femurAngle) + ", Tibia: " + String((int)tibiaAngle));
  PRINTLN("Speed        : " + String((int)speed));
  PRINTLN("Tip Position : X: " + String((int)tipX) + "mm, Y: " + String((int)tipY) + "mm, Z: " + String((int)tipZ) + "mm");


  return true;
}

// Process console commands for leg control
bool Leg::runConsoleCommands(const String& cmd, const String& args, int legIndex) {
    if (cmd == "ls") {
        printStatus();
        return true;

    } else if (cmd == "lss") {
        int newSpeed = LEG_SPEED; // Default speed
        if (args.length() > 0) {
            int spaceIdx = args.indexOf(' ');
            if (spaceIdx >= 0) {
              newSpeed = args.substring(spaceIdx + 1).toInt();
            }
        }
        setSpeed(newSpeed);
        speed = newSpeed;
        LOG_INF("Leg " + String(legIndex) + " speed: " + String((int)speed));
        return true;

    } else if (cmd == "lpu") {
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
    PRINTLN("  lss [n] [speed]  - Set leg speed (default: " + String((int)LEG_SPEED) + ")");
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


