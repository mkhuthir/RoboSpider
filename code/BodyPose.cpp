#include "BodyPose.h"

BodyPose::BodyPose()
    : x(0), y(0), z(0), roll(0), pitch(0), yaw(0)
{}

BodyPose::BodyPose(float x, float y, float z, float roll, float pitch, float yaw)
    : x(x), y(y), z(z), roll(roll), pitch(pitch), yaw(yaw)
{}

void BodyPose::setPosition(float nx, float ny, float nz) {
    x = nx; y = ny; z = nz;
}

void BodyPose::setOrientation(float r, float p, float y_) {
    roll = r; pitch = p; yaw = y_;
}

void BodyPose::setPose(float nx, float ny, float nz, float r, float p, float y_) {
    x = nx; y = ny; z = nz;
    roll = r; pitch = p; yaw = y_;
}

void BodyPose::getPosition(float &nx, float &ny, float &nz) const {
    nx = x; ny = y; nz = z;
}

void BodyPose::getOrientation(float &r, float &p, float &y_) const {
    r = roll; p = pitch; y_ = yaw;
}

void BodyPose::getPose(float &nx, float &ny, float &nz, float &r, float &p, float &y_) const {
    nx = x; ny = y; nz = z;
    r = roll; p = pitch; y_ = yaw;
}

void BodyPose::reset() {
    x = 0; y = 0; z = 0;
    roll = 0; pitch = 0; yaw = 0;
}