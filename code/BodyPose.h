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

    void setPosition(float x, float y, float z);
    void getPosition(float &x, float &y, float &z) const;
    void setOrientation(float roll, float pitch, float yaw);
    void getOrientation(float &roll, float &pitch, float &yaw) const;
    void setPose(float x, float y, float z, float roll, float pitch, float yaw);
    void getPose(float &x, float &y, float &z, float &roll, float &pitch, float &yaw) const;
    void resetPose();

    bool printStatus();
    bool runConsoleCommands(const String& cmd, const String& args);
    bool printConsoleHelp() const;

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