#ifndef BODYPPOSE_H
#define BODYPPOSE_H

#include <stdint.h>
#include <math.h>

class BodyPose {
public:
    BodyPose();
    BodyPose(float x, float y, float z, float roll, float pitch, float yaw);

    // Setters
    void setPosition(float x, float y, float z);
    void setOrientation(float roll, float pitch, float yaw);
    void setPose(float x, float y, float z, float roll, float pitch, float yaw);

    // Getters
    void getPosition(float &x, float &y, float &z) const;
    void getOrientation(float &roll, float &pitch, float &yaw) const;
    void getPose(float &x, float &y, float &z, float &roll, float &pitch, float &yaw) const;

    // Utility
    void reset(); // Reset to default pose

    // Members
    float x;      // Position in mm (forward/back)
    float y;      // Position in mm (sideways)
    float z;      // Position in mm (vertical)
    float roll;   // Rotation in radians
    float pitch;  // Rotation in radians
    float yaw;    // Rotation in radians
};

#endif // BODYPPOSE_H