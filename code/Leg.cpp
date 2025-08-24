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
  speed               = 0;

}

// Initialize the leg servos
bool Leg::init( uint8_t legIndex,
                uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, 
                float legBaseX, float legBaseY, float legBaseZ, 
                float legBaseR, uint8_t legSpeed,
                Driver* driver, Servo* servo) {

  this->legIndex      = legIndex;   // Set leg index
  this->legBaseX      = legBaseX;   // Set leg base X position
  this->legBaseY      = legBaseY;   // Set leg base Y position
  this->legBaseZ      = legBaseZ;   // Set leg base height
  this->legBaseR      = legBaseR;   // Set leg base rotation
  legServoIDs[Coxa]   = coxaID;     // Set coxa ID
  legServoIDs[Femur]  = femurID;    // Set femur ID
  legServoIDs[Tibia]  = tibiaID;    // Set tibia ID
  this->driver        = driver;     // Set the driver pointer
  this->servo         = servo;      // Set the servo pointer
  this->speed         = legSpeed;   // Set default speed

  if (!servo->init(legServoIDs[Coxa] , legSpeed, COXA_CW_LIMIT, COXA_CCW_LIMIT)) {   // Initialize coxa servo with velocity
    LOG_ERR("Failed to initialize coxa servo.");
    return false;
  }
  if (!servo->init(legServoIDs[Femur], legSpeed, FEMUR_CW_LIMIT, FEMUR_CCW_LIMIT)) {   // Initialize femur servo with velocity
    LOG_ERR("Failed to initialize femur servo.");
    return false;
  }
  if (!servo->init(legServoIDs[Tibia], legSpeed, TIBIA_CW_LIMIT, TIBIA_CCW_LIMIT)) {   // Initialize tibia servo with velocity
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
  // TODO: use syncWrite for setting speed
  if (speed < 0) speed = 0;
  if (speed > 1023) speed = 1023;
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

// Set servo positions
bool Leg::setServoPositions(uint16_t coxa, uint16_t femur, uint16_t tibia) {
  int32_t positions[LEG_SERVOS] = { static_cast<int32_t>(coxa), static_cast<int32_t>(femur), static_cast<int32_t>(tibia) };
  const uint8_t num_positions = 1;
  if (!driver->syncWrite(handler_index, legServoIDs, LEG_SERVOS, positions, num_positions)) {
    LOG_ERR("Failed to set servo positions via syncWrite.");
    return false;
  }
  return true;
}

// Get the current positions of the leg joints
bool Leg::getServoPositions(uint16_t* coxa, uint16_t* femur, uint16_t* tibia) {
  // TODO: use syncRead
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


// Set the leg tip position
bool Leg::setTipLocalPosition(float tip_local_x, float tip_local_y, float tip_local_z) {
  uint16_t positions[LEG_SERVOS];
  if (!getIKLocal(tip_local_x, tip_local_y, tip_local_z, positions)) {
    LOG_ERR("Failed to compute inverse kinematics.");
    return false;
  }
  return setServoPositions(positions[Coxa], positions[Femur], positions[Tibia]);
}

// Get the leg tip position
bool Leg::getTipLocalPosition(float* tip_local_x, float* tip_local_y, float* tip_local_z) {
  // TODO: Forward Kinematics to compute tip position from joint angles
  *tip_local_x = 1.0;
  *tip_local_y = 2.0;
  *tip_local_z = 3.0;
  return true;
}

// Forward Kinematics
bool Leg::getFKLocal(uint16_t coxa, uint16_t femur, uint16_t tibia, float* tip_local_x, float* tip_local_y, float* tip_local_z) {
  // Convert servo positions to angles in degrees
  float coxa_angle_deg  = coxa  * (300.0f / 1023.0f);
  float femur_angle_deg = femur * (300.0f / 1023.0f);
  float tibia_angle_deg = tibia * (300.0f / 1023.0f);

  // Convert degrees to radians
  float coxa_angle_rad  = coxa_angle_deg  * M_PI / 180.0f + legBaseR;
  float femur_angle_rad = femur_angle_deg * M_PI / 180.0f;
  float tibia_angle_rad = tibia_angle_deg * M_PI / 180.0f;

  // Planar FK for femur and tibia
  float planar_length = FEMUR_LENGTH * cos(femur_angle_rad) + TIBIA_LENGTH * cos(femur_angle_rad + tibia_angle_rad);
  float z = FEMUR_LENGTH * sin(femur_angle_rad) + TIBIA_LENGTH * sin(femur_angle_rad + tibia_angle_rad);

  // Tip position in leg base frame
  *tip_local_x = COXA_LENGTH + planar_length * cos(coxa_angle_rad);
  *tip_local_y = planar_length * sin(coxa_angle_rad);
  *tip_local_z = z;

  return true;
}

bool Leg::getFKGlobal(uint16_t coxa, uint16_t femur, uint16_t tibia, float* tip_global_x, float* tip_global_y, float* tip_global_z) {
  float tip_local_x = 0.0f, tip_local_y = 0.0f, tip_local_z = 0.0f;
  // Compute local tip position using FK
  if (!getFKLocal(coxa, femur, tibia, &tip_local_x, &tip_local_y, &tip_local_z)) {
    LOG_ERR("Failed to compute local FK.");
    return false;
  }

  // Transform local tip position to global coordinates
  // Apply yaw rotation (legBaseR) and add base position
  float cy = cos(legBaseR);
  float sy = sin(legBaseR);

  *tip_global_x = legBaseX + cy * tip_local_x - sy * tip_local_y;
  *tip_global_y = legBaseY + sy * tip_local_x + cy * tip_local_y;
  *tip_global_z = legBaseZ + tip_local_z;

  return true;
}

//---------------------------------------------------------------------------------------------

// IK in local coordinates (relative to leg base frame)
bool Leg::getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, uint16_t* positions)
{
    // 1. Coxa (rotation in XY plane)
    float coxa_angle_rad = atan2(tip_local_y, tip_local_x);
    float coxa_angle_deg = coxa_angle_rad * 180.0f / M_PI;

    float r = sqrt(tip_local_x * tip_local_x + tip_local_y * tip_local_y) - COXA_LENGTH;
    float z = tip_local_z;
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
    positions[Coxa]  = static_cast<uint16_t>(coxa_angle_deg_adjusted * (1023.0f / 300.0f));
    positions[Femur] = static_cast<uint16_t>(femur_angle_deg         * (1023.0f / 300.0f));
    positions[Tibia] = static_cast<uint16_t>(tibia_angle_deg         * (1023.0f / 300.0f));
    return true;
}

// IK in global coordinates (relative to body center)
bool Leg::getIKGlobal(float tip_global_x, float tip_global_y, float tip_global_z, uint16_t* positions)
{
    float tip_local_x, tip_local_y, tip_local_z;
    globalToLocal(tip_global_x, tip_global_y, tip_global_z, tip_local_x, tip_local_y, tip_local_z);
    return getIKLocal(tip_local_x, tip_local_y, tip_local_z, positions);
}

// Utility function to transform global (body) to local (leg base) coordinates
void Leg::globalToLocal( float global_x, float global_y, float global_z,
                float& local_x, float& local_y, float& local_z)
{
  // Subtract base position
  float x = global_x - legBaseX;
  float y = global_y - legBaseY;
  float z = global_z - legBaseZ;

  // Apply inverse yaw rotation only (around Z axis)
  float cy = cos(-legBaseR);
  float sy = sin(-legBaseR);

  local_x = cy * x - sy * y;
  local_y = sy * x + cy * y;
  local_z = z; // No change in Z for yaw-only rotation
}

//-------------------------------------------------------------------------------------

// Print current joint angles
bool Leg::printStatus() {
  uint16_t coxaAngle = 0, femurAngle = 0, tibiaAngle = 0;
  float legBaseX = 0, legBaseY = 0, legBaseZ = 0; float legBaseR = 0;
  float tip_local_X = 0, tip_local_Y = 0, tip_local_Z = 0;

  if (!getServoPositions(&coxaAngle, &femurAngle, &tibiaAngle)) {
    LOG_ERR("Failed to get joint angles.");
    return false;
  }

  if (!getBasePosition(&legBaseX, &legBaseY, &legBaseZ, &legBaseR)) {
    LOG_ERR("Failed to get leg base position.");
    return false;
  }

  if (!getTipLocalPosition(&tip_local_X, &tip_local_Y, &tip_local_Z)) {
    LOG_ERR("Failed to get leg tip position.");
    return false;
  }

  PRINTLN("Leg: " + String((int)legIndex));
  PRINTLN("Base         : X: " + String((float)legBaseX)
                   + "mm, Y: " + String((float)legBaseY) 
                   + "mm, Z: " + String((float)legBaseZ) 
                   + "mm, R: " + String((float)legBaseR) + "rad");
  PRINTLN("Servo IDs    : Coxa: " + String((int)legServoIDs[0]) 
              + ", Femur: " + String((int)legServoIDs[1]) 
              + ", Tibia: " + String((int)legServoIDs[2]));
  PRINTLN("Servo Pos.   : Coxa: " + String((int)coxaAngle) 
              + ", Femur: " + String((int)femurAngle) 
              + ", Tibia: " + String((int)tibiaAngle));
  PRINTLN("Speed        : " + String((int)speed));
  PRINTLN("Tip Local on : X: " + String((float)tip_local_X) 
                + "mm, Y: " + String((float)tip_local_Y) 
                + "mm, Z: " + String((float)tip_local_Z) + "mm");
  return true;
}


// -----------------------------------------------------------------------------------------------

// Process console commands for leg control
bool Leg::runConsoleCommands(const String& cmd, const String& args, int legIndex) {
    if (cmd == "ls") {
        printStatus();
        return true;

    } else if (cmd == "lss") {
        int newSpeed = LEG_SPEED; // Default speed

        if (args.length() > 0) {
            int count = 0, arg1 = 0, arg2 = 0;
            count = sscanf(args.c_str(), "%d %d", &arg1, &arg2);
            switch(count) {
              case 1:                 // only leg index is mentioned
                newSpeed = LEG_SPEED;
                break;
              case 2:
                newSpeed = arg2;      // leg index and speed are mentioned
                break;
              default:
                LOG_ERR("Invalid parameters");
                break;
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

    } else if (cmd == "lssp") {
        uint16_t coxaPos = COXA_DEFAULT, femurPos = FEMUR_DEFAULT, tibiaPos = TIBIA_DEFAULT;
        if (args.length() > 0) {
              int count = 0, arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0;
              count = sscanf(args.c_str(), "%d %d %d %d", &arg1, &arg2, &arg3, &arg4);
              switch(count) {
                case 1:                 // only leg index is mentioned
                  coxaPos = COXA_DEFAULT, femurPos = FEMUR_DEFAULT, tibiaPos = TIBIA_DEFAULT;
                  break;
                case 2:
                  coxaPos = arg2, femurPos = FEMUR_DEFAULT, tibiaPos = TIBIA_DEFAULT;
                  break;
                case 3:
                  coxaPos = arg2, femurPos = arg3, tibiaPos = TIBIA_DEFAULT;
                  break;
                case 4:
                  coxaPos = arg2, femurPos = arg3, tibiaPos = arg4;
                  break;
                default:
                  LOG_ERR("Invalid parameters");
                  break;
              }
        }
        setServoPositions(coxaPos, femurPos, tibiaPos);
        LOG_INF("Leg " + String(legIndex) + " servo positions set to: Coxa: " + String(coxaPos) + ", Femur: " + String(femurPos) + ", Tibia: " + String(tibiaPos));
        return true;

    } else if (cmd == "lgsp") {
        uint16_t coxaPos = 0, femurPos = 0, tibiaPos = 0;
        getServoPositions(&coxaPos, &femurPos, &tibiaPos);
        LOG_INF("Leg " + String(legIndex) + " servo positions: Coxa: " + String(coxaPos) + ", Femur: " + String(femurPos) + ", Tibia: " + String(tibiaPos));
        return true;

    } else if (cmd == "lstlp") {
        float tip_local_x = 0, tip_local_y = 0, tip_local_z = 0;
        // TODO:
        setTipLocalPosition(tip_local_x, tip_local_y, tip_local_z);
        return true;

    } else if (cmd == "lgtlp") {
        float tip_local_x = 0, tip_local_y = 0, tip_local_z = 0;
        getTipLocalPosition(&tip_local_x, &tip_local_y, &tip_local_z);
        LOG_INF("Leg " + String(legIndex) + " tip local position: X: " + String(tip_local_x) + ", Y: " + String(tip_local_y) + ", Z: " + String(tip_local_z));
        return true;

    } else if (cmd == "lgikl") {
        int count = 0, i = 0; float local_x = 0, local_y = 0, local_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &local_x, &local_y, &local_z);
        if (count == 4) {
            uint16_t positions[LEG_SERVOS];
            if (getIKLocal(local_x, local_y, local_z, positions)) {
                LOG_INF("Leg " + String(legIndex) + " IK Local Positions: Coxa: " + String(positions[Coxa]) + ", Femur: " + String(positions[Femur]) + ", Tibia: " + String(positions[Tibia]));
            } else {
                LOG_ERR("Failed to compute IK Local.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgikl. Usage: lgikl n x y z");
        }
        return true;

    } else if (cmd == "lgikg") {
        int count = 0, i = 0; float global_x = 0, global_y = 0, global_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &global_x, &global_y, &global_z);
        if (count == 4) {
            uint16_t positions[LEG_SERVOS];
            if (getIKGlobal(global_x, global_y, global_z, positions)) {
                LOG_INF("Leg " + String(legIndex) + " IK Global Positions: Coxa: " + String(positions[Coxa]) + ", Femur: " + String(positions[Femur]) + ", Tibia: " + String(positions[Tibia]));
            } else {
                LOG_ERR("Failed to compute IK Global.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgikg. Usage: lgikg n x y z");
        }
        return true;

    } else if (cmd == "lgfkl") {
        int count = 0, i = 0; uint16_t coxa = 0, femur = 0, tibia = 0;
        float local_x = 0, local_y = 0, local_z = 0;
        count = sscanf(args.c_str(), "%d %hu %hu %hu", &i, &coxa, &femur, &tibia);
        if (count == 4) {
            if (getFKLocal(coxa, femur, tibia, &local_x, &local_y, &local_z)) {
                LOG_INF("Leg " + String(legIndex) + " FK Local Position: X: " + String(local_x) + ", Y: " + String(local_y) + ", Z: " + String(local_z));
            } else {
                LOG_ERR("Failed to compute FK Local.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgfkl. Usage: lgfkl n c f t");
        }
        return true;

    } else if (cmd == "lgfkg") {
        int count = 0, i = 0; uint16_t coxa = 0, femur = 0, tibia = 0;
        float global_x = 0, global_y = 0, global_z = 0;
        count = sscanf(args.c_str(), "%d %hu %hu %hu", &i, &coxa, &femur, &tibia);
        if (count == 4) {
            if (getFKGlobal(coxa, femur, tibia, &global_x, &global_y, &global_z)) {
                LOG_INF("Leg " + String(legIndex) + " FK Global Position: X: " + String(global_x) + ", Y: " + String(global_y) + ", Z: " + String(global_z));
            } else {
                LOG_ERR("Failed to compute FK Global.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgfkg. Usage: lgfkg n c f t");
        }
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
    PRINTLN("  ls  [n]            - Print leg status (default: 0)");
    PRINTLN("  lss [n] [speed]    - Set leg speed (default: 0, " + String((int)LEG_SPEED) + ")");
    PRINTLN("");
    PRINTLN("  lpu [n]            - Move leg point up (default: 0)");
    PRINTLN("  lpd [n]            - Move leg point down (default: 0)");
    PRINTLN("  lpo [n]            - Move leg point out (default: 0)");
    PRINTLN("");
    PRINTLN("  lsu [n]            - Move leg to stand up position (default: 0)");
    PRINTLN("  lsd [n]            - Move leg to stand down position (default: 0)");
    PRINTLN("");
    PRINTLN("  lssp [n][c][f][t]  - Set leg servo positions (default: 0, " + String(COXA_DEFAULT) + ", " + String(FEMUR_DEFAULT) + ", " + String(TIBIA_DEFAULT) + ")");
    PRINTLN("  lgsp [n]           - Get leg servo positions (default: 0)");
    PRINTLN("  lstlp [n][x][y][z] - Set leg tip local position (default: 0)");
    PRINTLN("  lgtlp [n]          - Get leg tip local position (default: 0)");
    PRINTLN("");
    PRINTLN("  lgikl n x y z      - Compute IK in local coords (relative to leg base)");
    PRINTLN("  lgikg n x y z      - Compute IK in global coords (relative to body center)");
    PRINTLN("  lgfkl n c f t      - Compute FK in local coords (relative to leg base)");
    PRINTLN("  lgfkg n c f t      - Compute FK in global coords (relative to body center)");
    PRINTLN("");
    PRINTLN("  l?                 - Show this help");
    PRINTLN("");
    return true;
}


