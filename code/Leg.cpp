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

  legBaseX      = 0.0;
  legBaseY      = 0.0;
  legBaseZ      = 0.0;
  legBaseRoll   = 0.0;
  legBasePitch  = 0.0;
  legBaseYaw    = 0.0;

  driver        = nullptr;
  servo         = nullptr;
  speed         = LEG_SPEED;

}

// Initialize the leg servos
bool Leg::init( uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, 
                float legBaseX, float legBaseY, float legBaseZ, 
                float legBaseRoll, float legBasePitch, float legBaseYaw, 
                Driver* driver, Servo* servo) {
  
  this->legBaseX      = legBaseX;
  this->legBaseY      = legBaseY;
  this->legBaseZ      = legBaseZ;
  this->legBaseRoll   = legBaseRoll;
  this->legBasePitch  = legBasePitch;
  this->legBaseYaw    = legBaseYaw;

  legIDs[Coxa]  = coxaID;   // Set coxa ID
  legIDs[Femur] = femurID;  // Set femur ID
  legIDs[Tibia] = tibiaID;  // Set tibia ID

  this->driver = driver;    // Set the driver pointer
  this->servo = servo;      // Set the servo pointer
  this->speed = LEG_SPEED;  // Set default speed

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

// Set the speed of the leg
bool Leg::setSpeed(uint16_t speed) {
  for (int i = 0; i < LEG_SERVOS; i++) {
    if (!servo->setGoalSpeed(legIDs[i], speed)) {
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
    if (legBaseYaw == -180) { // TODO:
        positions[Coxa]  = static_cast<int16_t>(coxa_angle_deg * (1023.0f / 300.0f));
    } else {
        positions[Coxa]  = static_cast<int16_t>((300.0f - coxa_angle_deg) * (1023.0f / 300.0f));
    }
    positions[Femur] = static_cast<int16_t>(femur_angle_deg * (1023.0f / 300.0f));
    positions[Tibia] = static_cast<int16_t>(tibia_angle_deg * (1023.0f / 300.0f));
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

    // Apply inverse rotation (roll, pitch, yaw)
    // Rotation order: yaw (Z), pitch (Y), roll (X)
    // Build rotation matrix from roll, pitch, yaw (intrinsic Tait-Bryan angles)
    float cy = cos(-legBaseYaw)   , sy = sin(-legBaseYaw);
    float cp = cos(-legBasePitch) , sp = sin(-legBasePitch);
    float cr = cos(-legBaseRoll)  , sr = sin(-legBaseRoll);

    // Rotation matrix: R = Rz(yaw) * Ry(pitch) * Rx(roll)
    float x_rot = cy*cp*x + (cy*sp*sr - sy*cr)*y + (cy*sp*cr + sy*sr)*z;
    float y_rot = sy*cp*x + (sy*sp*sr + cy*cr)*y + (sy*sp*cr - cy*sr)*z;
    float z_rot = -sp*x    + cp*sr*y              + cp*cr*z;

    x_local = x_rot;
    y_local = y_rot;
    z_local = z_rot;
}

// Print current joint angles
bool Leg::printStatus() {
  uint16_t coxaAngle = 0, femurAngle = 0, tibiaAngle = 0;
  if (!getCoxa(&coxaAngle) || !getFemur(&femurAngle) || !getTibia(&tibiaAngle)) {
    LOG_ERR("Failed to get joint angles.");
    return false;
  }
  PRINT("Coxa: " + String((int)coxaAngle));
  PRINT(" | Femur: " + String((int)femurAngle));
  PRINT(" | Tibia: " + String((int)tibiaAngle));
  PRINTLN(" | Speed: " + String((int)speed));

  return true;
}

// Process console commands for leg control
bool Leg::runConsoleCommands(const String& cmd, const String& args, int legIndex) {
    if (cmd == "ls") {
        printStatus();
        return true;

    } else if (cmd == "lss") {
        if (args.length() > 0) {
            int newSpeed = args.toInt();
            setSpeed(newSpeed);
            speed = newSpeed;
            LOG_INF("Leg " + String(legIndex) + " speed set to " + String(newSpeed));
        } else {
            setSpeed(LEG_SPEED);
            speed = LEG_SPEED;  
            LOG_INF("Leg " + String(legIndex) + " speed: " + String((int)LEG_SPEED));
        }
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


