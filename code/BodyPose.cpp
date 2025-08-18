#include "BodyPose.h"

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

void BodyPose::reset() {
    x = 0; 
    y = 0; 
    z = 0;
    roll = 0; 
    pitch = 0; 
    yaw = 0;
}

static float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}