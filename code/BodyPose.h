#ifndef BODYPPOSE_H
#define BODYPPOSE_H

#define ROLL_MIN   -1.57f   // -90 degrees in radians
#define ROLL_MAX    1.57f   //  90 degrees in radians
#define PITCH_MIN  -1.57f   // -90 degrees in radians
#define PITCH_MAX   1.57f   //  90 degrees in radians
#define YAW_MIN    -3.14f   // -180 degrees in radians
#define YAW_MAX     3.14f   //  180 degrees in radians
#define X_MIN     -100.0f   // min in mm
#define X_MAX      100.0f   // max in mm
#define Y_MIN     -100.0f   // min in mm
#define Y_MAX      100.0f   // max in mm
#define Z_MIN     -50.0f    // min in mm
#define Z_MAX      50.0f    // max in mm

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
private:
    static float clamp(float value, float min, float max);
};

#endif // BODYPPOSE_H