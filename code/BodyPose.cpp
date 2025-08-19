#include "BodyPose.h"
#include "Console.h"
#include "Debug.h"


BodyPose::BodyPose()
    : x(0), y(0), z(0), roll(0), pitch(0), yaw(0)
{}

BodyPose::BodyPose(float x, float y, float z, float roll, float pitch, float yaw)
        :   x(clamp(x, X_MIN, X_MAX)),
            y(clamp(y, Y_MIN, Y_MAX)),
            z(clamp(z, Z_MIN, Z_MAX)),
            roll(clamp(roll, ROLL_MIN, ROLL_MAX)),
            pitch(clamp(pitch, PITCH_MIN, PITCH_MAX)),
            yaw(clamp(yaw, YAW_MIN, YAW_MAX))
{}

void BodyPose::setPosition(float x, float y, float z) {
    this->x = clamp(x, X_MIN, X_MAX);
    this->y = clamp(y, Y_MIN, Y_MAX);
    this->z = clamp(z, Z_MIN, Z_MAX);
}

void BodyPose::setOrientation(float roll, float pitch, float yaw) {
    this->roll = clamp(roll, ROLL_MIN, ROLL_MAX);
    this->pitch = clamp(pitch, PITCH_MIN, PITCH_MAX);
    this->yaw = clamp(yaw, YAW_MIN, YAW_MAX);
}

void BodyPose::setPose(float x, float y, float z, float roll, float pitch, float yaw) {
    this->x = clamp(x, X_MIN, X_MAX);
    this->y = clamp(y, Y_MIN, Y_MAX);
    this->z = clamp(z, Z_MIN, Z_MAX);
    this->roll = clamp(roll, ROLL_MIN, ROLL_MAX);
    this->pitch = clamp(pitch, PITCH_MIN, PITCH_MAX);
    this->yaw = clamp(yaw, YAW_MIN, YAW_MAX);
}

void BodyPose::getPosition(float &x, float &y, float &z) const {
    x = this->x; 
    y = this->y; 
    z = this->z;
}

void BodyPose::getOrientation(float &roll, float &pitch, float &yaw) const {
    roll = this->roll; 
    pitch = this->pitch; 
    yaw = this->yaw;
}

void BodyPose::getPose(float &x, float &y, float &z, float &roll, float &pitch, float &yaw) const {
    x = this->x; 
    y = this->y; 
    z = this->z;
    roll = this->roll; 
    pitch = this->pitch; 
    yaw = this->yaw;
}

void BodyPose::resetPose() {
    x = 0; 
    y = 0; 
    z = 0;
    roll = 0; 
    pitch = 0; 
    yaw = 0;
}

// Clamp function implementation
float BodyPose::clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

bool BodyPose::printStatus() {
    PRINTLN("Body Pose Status:\n\r");
    PRINT("Position    : X = " + String(x) + ", Y = " + String(y) + ", Z = " + String(z));
    PRINTLN("");
    PRINT("Orientation : Roll = " + String(roll) + ", Pitch = " + String(pitch) + ", Yaw = " + String(yaw));
    PRINTLN("");
    return true;
}


bool BodyPose::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "bs") {
        printStatus();
        return true;

    } else if (cmd == "bsp") {
        float x, y, z, roll, pitch, yaw;
        if (sscanf(args.c_str(), "%f %f %f %f %f %f", &x, &y, &z, &roll, &pitch, &yaw) == 6) {
            setPose(x, y, z, roll, pitch, yaw);
        } else {
            LOG_ERR("Invalid arguments. Usage: bsp [x y z roll pitch yaw]");
        }   

    } else if (cmd == "brp") {
        resetPose();
        return true;
    } else if (cmd == "bspos") {
        float x, y, z;
        if (sscanf(args.c_str(), "%f %f %f", &x, &y, &z) == 3) {
            setPosition(x, y, z);
        } else {
            LOG_ERR("Invalid arguments. Usage: bspos [x y z]");
        }
        return true;
        
    } else if (cmd == "bso") {
        float roll, pitch, yaw;
        if (sscanf(args.c_str(), "%f %f %f", &roll, &pitch, &yaw) == 3) {
            setOrientation(roll, pitch, yaw);
        } else {
            LOG_ERR("Invalid arguments. Usage: bso [roll pitch yaw]");
        }
        return true;

    } else if (cmd == "b?") {
        printConsoleHelp();
        return true;

    }
    return false;
}

bool BodyPose::printConsoleHelp() const {
    PRINTLN("BodyPose Console Commands:\n\r");
    PRINTLN("  bs                           - Print body pose status");
    PRINTLN("  bsp [x y z roll pitch yaw]   - Set the body pose");
    PRINTLN("  brp                          - Reset the body pose");
    PRINTLN("  bspos [x y z]                - Set the body position");
    PRINTLN("  bso [roll pitch yaw]         - Set the body orientation");
    PRINTLN("  b?                           - Show this help");
    return true;
}