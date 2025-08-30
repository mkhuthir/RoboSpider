#include "Servo.h"
#include "Leg.h"
#include "LegPoses.h"
#include "Console.h"
#include "Debug.h"


// Default constructor for Leg class
Leg::Leg(){
  // Initialize leg IDs to zero
  index            = 0;
  baseX            = 0.0;
  baseY            = 0.0;
  baseZ            = 0.0;
  baseR            = 0.0;
  servoIDs[Coxa]   = 0;
  servoIDs[Femur]  = 0;
  servoIDs[Tibia]  = 0;
  driver           = nullptr;
  servo            = nullptr;
  speed            = 0;

}

// Initialize the leg servos
bool Leg::init( uint8_t index,
                uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, 
                float baseX, float baseY, float baseZ, 
                float baseR, uint8_t speed,
                Driver* driver, Servo* servo) {

  this->index      = index;      // Set leg index
  this->baseX      = baseX;      // Set leg base X position
  this->baseY      = baseY;      // Set leg base Y position
  this->baseZ      = baseZ;      // Set leg base height
  this->baseR      = baseR;      // Set leg base rotation
  servoIDs[Coxa]   = coxaID;     // Set coxa ID
  servoIDs[Femur]  = femurID;    // Set femur ID
  servoIDs[Tibia]  = tibiaID;    // Set tibia ID
  this->driver     = driver;     // Set the driver pointer
  this->servo      = servo;      // Set the servo pointer
  this->speed      = speed;      // Set default speed

  if (!servo->init(servoIDs[Coxa] , speed, COXA_CW_LIMIT, COXA_CCW_LIMIT)) {   // Initialize coxa servo with velocity
    LOG_ERR("Failed to initialize coxa servo.");
    return false;
  }
  if (!servo->init(servoIDs[Femur], speed, FEMUR_CW_LIMIT, FEMUR_CCW_LIMIT)) {   // Initialize femur servo with velocity
    LOG_ERR("Failed to initialize femur servo.");
    return false;
  }
  if (!servo->init(servoIDs[Tibia], speed, TIBIA_CW_LIMIT, TIBIA_CCW_LIMIT)) {   // Initialize tibia servo with velocity
    LOG_ERR("Failed to initialize tibia servo.");
    return false;
  }

  LOG_INF("Leg initialized successfully. (Servo IDs: " + String(servoIDs[Coxa]) + ", " + String(servoIDs[Femur]) + ", " + String(servoIDs[Tibia]) + ")");
  return true;

}

// Update the leg state
bool Leg::update() {
  // Update each servo
  for (int i = 0; i < LEG_SERVOS; i++) {
    servo->update(servoIDs[i]);
  }
  return true;
}

// Move the leg to the specified positions
bool Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  if(!driver->syncWrite(handler_index, servoIDs, LEG_SERVOS, positions, num_positions)) {
    LOG_ERR("Failed to move leg.");
    return false;
  }
  return true;
}

// Check if any servo in the leg is currently moving
bool Leg::isMoving() {
  uint8_t moving = 0;
  for (int i = 0; i < LEG_SERVOS; ++i) {
    if (servo->isMoving(servoIDs[i])) {
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
    if (!servo->setGoalSpeed(servoIDs[i], speed)) {
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
  *base_x = baseX;
  *base_y = baseY;
  *base_z = baseZ;
  *base_r = baseR;
  return true;
}

// Set servo positions
bool Leg::setServoPositions(uint16_t coxa, uint16_t femur, uint16_t tibia) {
  int32_t positions[LEG_SERVOS] = { static_cast<int32_t>(coxa), static_cast<int32_t>(femur), static_cast<int32_t>(tibia) };
  const uint8_t num_positions = 1;
  if (!driver->syncWrite(handler_index, servoIDs, LEG_SERVOS, positions, num_positions)) {
    LOG_ERR("Failed to set servo positions via syncWrite.");
    return false;
  }
  return true;
}

// Get the current positions of the leg joints
bool Leg::getServoPositions(uint16_t* coxa, uint16_t* femur, uint16_t* tibia) {
  // TODO: use syncRead
  if (!servo->getPresentPosition(servoIDs[Coxa], coxa)) {
    LOG_ERR("Failed to get coxa position.");
    return false;
  }
  if (!servo->getPresentPosition(servoIDs[Femur], femur)) {
    LOG_ERR("Failed to get femur position.");
    return false;
  }
  if (!servo->getPresentPosition(servoIDs[Tibia], tibia)) {
    LOG_ERR("Failed to get tibia position.");
    return false;
  }
  return true;
}

//---------------------------------------------------------------------------------------------

// Set the leg tip local position
bool Leg::setTipLocalPosition(float tip_local_x, float tip_local_y, float tip_local_z) {
  uint16_t positions[LEG_SERVOS];
  if (!getIKLocal(tip_local_x, tip_local_y, tip_local_z, positions)) {
    LOG_ERR("Failed to compute inverse kinematics.");
    return false;
  }
  return setServoPositions(positions[Coxa], positions[Femur], positions[Tibia]);
}

// Get the leg tip local position
bool Leg::getTipLocalPosition(float* tip_local_x, float* tip_local_y, float* tip_local_z) {
  uint16_t coxa = 0, femur = 0, tibia = 0;
  if (!getServoPositions(&coxa, &femur, &tibia)) {
    LOG_ERR("Failed to get servo positions.");
    return false;
  }
  return getFKLocal(coxa, femur, tibia, tip_local_x, tip_local_y, tip_local_z);
}

// Set the leg tip global position
bool Leg::setTipGlobalPosition(float tip_global_x, float tip_global_y, float tip_global_z) {
  float tip_local_x, tip_local_y, tip_local_z;
  global2Local(tip_global_x, tip_global_y, tip_global_z, &tip_local_x, &tip_local_y, &tip_local_z);
  return setTipLocalPosition(tip_local_x, tip_local_y, tip_local_z);
}

// Get the leg tip global position
bool Leg::getTipGlobalPosition(float* tip_global_x, float* tip_global_y, float* tip_global_z) {
  float tip_local_x, tip_local_y, tip_local_z;
  if (!getTipLocalPosition(&tip_local_x, &tip_local_y, &tip_local_z)) {
    LOG_ERR("Failed to get local tip position.");
    return false;
  }
  local2Global(tip_local_x, tip_local_y, tip_local_z, tip_global_x, tip_global_y, tip_global_z);
  return true;
}

//---------------------------------------------------------------------------------------------

// Get inverse kinematics in local coordinates
bool Leg::getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, uint16_t* positions)
{
  // Coxa yaw (rotation in XY plane)
  float coxa_angle_rad = atan2f(tip_local_y, tip_local_x);

  // Planar reduction
  float r  = sqrtf(powf(tip_local_x, 2) + powf(tip_local_y, 2));
  float Xp = r - COXA_LENGTH;
  float Zp = tip_local_z;

  float d2 = powf(Xp, 2) + powf(Zp, 2);
  float d  = sqrtf(d2);
  float lenSum = FEMUR_LENGTH + TIBIA_LENGTH;
  float lenDiff = fabsf(FEMUR_LENGTH - TIBIA_LENGTH);

  // Check if the point is reachable
  if (!(d >= lenDiff - 1e-5f && d <= lenSum + 1e-5f)) return false;

  float D = (d2 - powf(FEMUR_LENGTH, 2) - powf(TIBIA_LENGTH, 2)) / (2.0f * FEMUR_LENGTH * TIBIA_LENGTH);
  // Clamp D to the range [-1, 1]
  if (D < -1.0f) D = -1.0f;
  if (D >  1.0f) D =  1.0f;

  float sin_knee = sqrtf(fmaxf(0.0f, 1.0f - powf(D, 2)));
  float tibia_angle_rad = atan2f(sin_knee, D);
  float femur_angle_rad = atan2f(Zp, Xp) - atan2f(TIBIA_LENGTH*sinf(tibia_angle_rad), FEMUR_LENGTH + TIBIA_LENGTH*cosf(tibia_angle_rad));

  float femur_angle_deg = wrap360((FEMUR_UP_DIR) * rad2Deg(femur_angle_rad) + FEMUR_H_POS);
  float tibia_angle_deg = wrap360((-TIBIA_UP_DIR) * rad2Deg(tibia_angle_rad) + TIBIA_H_POS);
  float coxa_angle_deg  = wrap360(rad2Deg(coxa_angle_rad)  - baseR);

  // Convert to ticks
  uint16_t tibia_angle_tick, femur_angle_tick, coxa_angle_tick;
  if (!deg2Tick(coxa_angle_deg,  coxa_angle_tick))  return false;
  if (!deg2Tick(femur_angle_deg, femur_angle_tick)) return false;
  if (!deg2Tick(tibia_angle_deg, tibia_angle_tick)) return false;

  positions[Coxa] = coxa_angle_tick;
  positions[Femur] = femur_angle_tick;
  positions[Tibia] = tibia_angle_tick;

  return true;
}

// Get inverse kinematics in global coordinates
bool Leg::getIKGlobal(float tip_global_x, float tip_global_y, float tip_global_z, uint16_t* positions)
{
    float tip_local_x, tip_local_y, tip_local_z;
    global2Local(tip_global_x, tip_global_y, tip_global_z, &tip_local_x, &tip_local_y, &tip_local_z);
    return getIKLocal(tip_local_x, tip_local_y, tip_local_z, positions);
}

// Get forward kinematics in local coordinates
bool Leg::getFKLocal(uint16_t coxa, uint16_t femur, uint16_t tibia, float* tip_local_x, float* tip_local_y, float* tip_local_z) {
  // Convert servo positions to angles in degrees
  float coxa_angle_deg  = coxa  * (300.0f / 1023.0f);
  float femur_angle_deg = femur * (300.0f / 1023.0f);
  float tibia_angle_deg = tibia * (300.0f / 1023.0f);

  // Convert degrees to radians
  float coxa_angle_rad  = coxa_angle_deg  * M_PI / 180.0f + baseR;
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

// Get forward kinematics in global coordinates
bool Leg::getFKGlobal(uint16_t coxa, uint16_t femur, uint16_t tibia, float* tip_global_x, float* tip_global_y, float* tip_global_z) {
  float tip_local_x = 0.0f, tip_local_y = 0.0f, tip_local_z = 0.0f;
  // Compute local tip position using FK
  if (!getFKLocal(coxa, femur, tibia, &tip_local_x, &tip_local_y, &tip_local_z)) {
    LOG_ERR("Failed to compute local FK.");
    return false;
  }
  local2Global(tip_local_x, tip_local_y, tip_local_z, tip_global_x, tip_global_y, tip_global_z);
  return true;
}

// Utility function to transform global (body) to local (leg base) coordinates
void Leg::global2Local( float global_x, float global_y, float global_z, float* local_x, float* local_y, float* local_z)
{
  *local_x = global_x - baseX;
  *local_y = global_y - baseY;
  *local_z = global_z - baseZ;
}

// Local to Global transformation
void Leg::local2Global(float local_x, float local_y, float local_z, float* global_x, float* global_y, float* global_z)
{
  *global_x = baseX + local_x;
  *global_y = baseY + local_y;
  *global_z = baseZ + local_z;
}

inline float Leg::rad2Deg(float r) {
     return r * 180.0f / M_PI; 
}

inline float Leg::deg2Rad(float d) {
    return d * M_PI / 180.0f;
}

float Leg::wrap360(float aDeg) {
    float a = fmodf(aDeg, 360.0f);
    if (a < 0) a += 360.0f;
    return a;
}

bool Leg::deg2Tick(float deg, uint16_t &tick) {
    if (deg < SERVO_MIN_DEG || deg > SERVO_MAX_DEG) return false;
    float t = (deg - SERVO_MIN_DEG) * (SERVO_MAX_TICK / SERVO_SPAN_DEG);
    if (t < SERVO_MIN_TICK) t = SERVO_MIN_TICK;
    if (t > SERVO_MAX_TICK) t = SERVO_MAX_TICK;
    tick = static_cast<uint16_t>(lroundf(t));
    return true;
}

bool Leg::tick2Deg(uint16_t tick, float &deg) {
    if (tick < SERVO_MIN_TICK || tick > SERVO_MAX_TICK) return false;
    deg = SERVO_MIN_DEG + (static_cast<float>(tick) * SERVO_SPAN_DEG / SERVO_MAX_TICK);
    return true;
}

//-------------------------------------------------------------------------------------

// Print leg status
bool Leg::printStatus() {
  uint16_t coxaAngle = 0, femurAngle = 0, tibiaAngle = 0;
  float baseX = 0, baseY = 0, baseZ = 0; float baseR = 0;
  float tip_local_X = 0, tip_local_Y = 0, tip_local_Z = 0;

  if (!getServoPositions(&coxaAngle, &femurAngle, &tibiaAngle)) {
    LOG_ERR("Failed to get joint angles.");
    return false;
  }

  if (!getBasePosition(&baseX, &baseY, &baseZ, &baseR)) {
    LOG_ERR("Failed to get leg base position.");
    return false;
  }

  if (!getTipLocalPosition(&tip_local_X, &tip_local_Y, &tip_local_Z)) {
    LOG_ERR("Failed to get leg tip position.");
    return false;
  }

  PRINTLN("Leg: " + String((int)index));
  PRINTLN("Base         : X: " + String((float)baseX)
                   + "mm, Y: " + String((float)baseY) 
                   + "mm, Z: " + String((float)baseZ) 
                   + "mm, R: " + String((float)baseR) + " deg");
  PRINTLN("Servo IDs    : Coxa: " + String((int)servoIDs[0]) 
              + ", Femur: " + String((int)servoIDs[1]) 
              + ", Tibia: " + String((int)servoIDs[2]));
  PRINTLN("Servo Pos.   : Coxa: " + String((int)coxaAngle) 
              + ", Femur: " + String((int)femurAngle) 
              + ", Tibia: " + String((int)tibiaAngle));
  PRINTLN("Speed        : " + String((int)speed));
  PRINTLN("Tip Local on : X: " + String((float)tip_local_X) 
                + "mm, Y: " + String((float)tip_local_Y) 
                + "mm, Z: " + String((float)tip_local_Z) + "mm");
  return true;
}

// Process console commands for leg control
bool Leg::runConsoleCommands(const String& cmd, const String& args, int index) {
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
        LOG_INF("Leg " + String(index) + " speed: " + String((int)speed));
        return true;

    } else if (cmd == "lpu") {
        movePointUp();
        LOG_INF("Leg " + String(index) + " point moving up");
        return true;

    } else if (cmd == "lpd") {
        movePointDown();
        LOG_INF("Leg " + String(index) + " point moving down");
        return true;

    } else if (cmd == "lpo") {
        movePointOut();
        LOG_INF("Leg " + String(index) + " point moving out");
        return true;

    } else if (cmd == "lsu") {
        moveStandUp();
        LOG_INF("Leg " + String(index) + " standing up");
        return true;

    } else if (cmd == "lsd") {
        moveStandDown();
        LOG_INF("Leg " + String(index) + " standing down");
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
        LOG_INF("Leg " + String(index) + " servo positions set to: Coxa: " + String(coxaPos) + ", Femur: " + String(femurPos) + ", Tibia: " + String(tibiaPos));
        return true;

    } else if (cmd == "lgsp") {
        uint16_t coxaPos = 0, femurPos = 0, tibiaPos = 0;
        getServoPositions(&coxaPos, &femurPos, &tibiaPos);
        LOG_INF("Leg " + String(index) + " servo positions: Coxa: " + String(coxaPos) + ", Femur: " + String(femurPos) + ", Tibia: " + String(tibiaPos));
        return true;

    } else if (cmd == "lstpl") {
        if (args.length() > 0) {
            int count = 0, i = 0; float local_x = 0, local_y = 0, local_z = 0;
            count = sscanf(args.c_str(), "%d %f %f %f", &i, &local_x, &local_y, &local_z);
            if (count == 4) {
                setTipLocalPosition(local_x, local_y, local_z);
                LOG_INF("Leg " + String(index) + " tip local position set to: X: " + String(local_x) + ", Y: " + String(local_y) + ", Z: " + String(local_z));
            } else {
                LOG_ERR("Invalid parameters for lstlp. Usage: lstlp n x y z");
            }
        } else {
            LOG_ERR("Invalid parameters for lstlp. Usage: lstlp n x y z");
        }
        return true;

    } else if (cmd == "lgtpl") {
        float tip_local_x = 0, tip_local_y = 0, tip_local_z = 0;
        getTipLocalPosition(&tip_local_x, &tip_local_y, &tip_local_z);
        LOG_INF("Leg " + String(index) + " tip local position: X: " + String(tip_local_x) + ", Y: " + String(tip_local_y) + ", Z: " + String(tip_local_z));
        return true;

    } else if (cmd == "lstpg") {
        if (args.length() > 0) {
            int count = 0, i = 0; float global_x = 0, global_y = 0, global_z = 0;
            count = sscanf(args.c_str(), "%d %f %f %f", &i, &global_x, &global_y, &global_z);
            if (count == 4) {
                setTipGlobalPosition(global_x, global_y, global_z);
                LOG_INF("Leg " + String(index) + " tip global position set to: X: " + String(global_x) + ", Y: " + String(global_y) + ", Z: " + String(global_z));
            } else {
                LOG_ERR("Invalid parameters for lstgp. Usage: lstgp n x y z");
            }
        } else {
            LOG_ERR("Invalid parameters for lstgp. Usage: lstgp n x y z");
        }
        return true;

    } else if (cmd == "lgtpg") {
        float tip_global_x = 0, tip_global_y = 0, tip_global_z = 0;
        getTipGlobalPosition(&tip_global_x, &tip_global_y, &tip_global_z);
        LOG_INF("Leg " + String(index) + " tip global position: X: " + String(tip_global_x) + ", Y: " + String(tip_global_y) + ", Z: " + String(tip_global_z));
        return true;

    } else if (cmd == "lgikl") {
        int count = 0, i = 0; float local_x = 0, local_y = 0, local_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &local_x, &local_y, &local_z);
        if (count == 4) {
            uint16_t positions[LEG_SERVOS];
            if (getIKLocal(local_x, local_y, local_z, positions)) {
                LOG_INF("Leg " + String(index) + " IK Local Positions: Coxa: " + String(positions[Coxa]) + ", Femur: " + String(positions[Femur]) + ", Tibia: " + String(positions[Tibia]));
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
                LOG_INF("Leg " + String(index) + " IK Global Positions: Coxa: " + String(positions[Coxa]) + ", Femur: " + String(positions[Femur]) + ", Tibia: " + String(positions[Tibia]));
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
                LOG_INF("Leg " + String(index) + " FK Local Position: X: " + String(local_x) + ", Y: " + String(local_y) + ", Z: " + String(local_z));
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
                LOG_INF("Leg " + String(index) + " FK Global Position: X: " + String(global_x) + ", Y: " + String(global_y) + ", Z: " + String(global_z));
            } else {
                LOG_ERR("Failed to compute FK Global.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgfkg. Usage: lgfkg n c f t");
        }
        return true;
    } else if (cmd == "lltg") {
        int count = 0, i = 0; float local_x = 0, local_y = 0, local_z = 0;
        float global_x = 0, global_y = 0, global_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &local_x, &local_y, &local_z);
        if (count == 4) {
            local2Global(local_x, local_y, local_z, &global_x, &global_y, &global_z);
            LOG_INF("Leg " + String(index) + " Local to Global: X: " + String(global_x) + ", Y: " + String(global_y) + ", Z: " + String(global_z));
        } else {
            LOG_ERR("Invalid parameters for lltg. Usage: lltg n x y z");
        }
        return true;
        
    } else if (cmd == "lgtl") {
        int count = 0, i = 0; float global_x = 0, global_y = 0, global_z = 0;
        float local_x = 0, local_y = 0, local_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &global_x, &global_y, &global_z);
        if (count == 4) {
            global2Local(global_x, global_y, global_z, &local_x, &local_y, &local_z);
            LOG_INF("Leg " + String(index) + " Global to Local: X: " + String(local_x) + ", Y: " + String(local_y) + ", Z: " + String(local_z));
        } else {
            LOG_ERR("Invalid parameters for lgtl. Usage: lgtl n x y z");
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
    PRINTLN("");
    PRINTLN("  lstpl n x y z      - Set leg tip local position");
    PRINTLN("  lgtpl [n]          - Get leg tip local position (default: 0)");
    PRINTLN("");
    PRINTLN("  lstpg n x y z      - Set leg tip global position");
    PRINTLN("  lgtpg [n]          - Get leg tip global position (default: 0)");
    PRINTLN("");
    PRINTLN("  lgikl n x y z      - Compute IK in local coords (relative to leg base)");
    PRINTLN("  lgikg n x y z      - Compute IK in global coords (relative to body center)");
    PRINTLN("");
    PRINTLN("  lgfkl n c f t      - Compute FK in local coords (relative to leg base)");
    PRINTLN("  lgfkg n c f t      - Compute FK in global coords (relative to body center)");
    PRINTLN("");
    PRINTLN("  lltg n x y z       - Compute global coordinates from local coordinates");
    PRINTLN("  lgtl n x y z       - Compute local coordinates from global coordinates");
    PRINTLN("");
    PRINTLN("  l?                 - Show this help");
    PRINTLN("");
    return true;
}


