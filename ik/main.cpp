#include <iostream>
#include <cmath>
#include <cstdint>


#define COXA_LENGTH       float(52)       // Length of the coxa segment in mm
#define FEMUR_LENGTH      float(82)       // Length of the femur segment in mm
#define TIBIA_LENGTH      float(142)      // Length of the tibia segment in mm
#define FEMUR_FORWARD     float(180)      // Femur forward angle in degrees
#define TIBIA_STRAIGHT    float(180)      // Tibia straight angle in degrees

// AX-18A mapping
#define SERVO_MIN_DEG    float(30)                          // start angle
#define SERVO_MAX_DEG    float(330)                         // end angle
#define SERVO_SPAN_DEG   (SERVO_MAX_DEG - SERVO_MIN_DEG)    // 300 degrees span (30-300 CCW)
#define SERVO_MIN_TICK   uint16_t(0)                        // min ticks 0 = 30 degrees
#define SERVO_MAX_TICK   uint16_t(1023)                     // max ticks 1023 = 300 degrees

#define baseR            float(-90.0)                       // Coxa mount angle in degrees

// -------------------- Helpers --------------------
inline float rad2Deg(float r) {         // convert radians to degrees
     return r * 180.0f / M_PI; 
}

inline float deg2Rad(float d) {         // convert degrees to radians
    return d * M_PI / 180.0f;
}

float wrap360(float aDeg) {      // convert degree range from (-180 to 180) to (0 to 360)
    return fmodf(aDeg+360.0f, 360.0f);
}

bool deg2Tick(float deg, uint16_t &tick) {
    if (deg < SERVO_MIN_DEG || deg > SERVO_MAX_DEG) return false;
    float t = (deg - SERVO_MIN_DEG) * (SERVO_MAX_TICK / SERVO_SPAN_DEG);
    if (t < SERVO_MIN_TICK) t = SERVO_MIN_TICK;
    if (t > SERVO_MAX_TICK) t = SERVO_MAX_TICK;
    tick = static_cast<uint16_t>(lroundf(t));
    return true;
}

// -------------------- IK Solver --------------------
bool getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, uint16_t* positions)
{

    // 1. Coxa yaw (rotation in XY plane)
    float coxa_angle_rad = atan2f(tip_local_x, tip_local_y);

    // 2) Planar reduction
    float r  = sqrtf(powf(tip_local_x, 2) + powf(tip_local_y, 2));
    float Xp = r - COXA_LENGTH;
    float Zp = tip_local_z;

    float d2 = powf(Xp, 2) + powf(Zp, 2);
    float d  = sqrtf(d2);
    float lenSum = FEMUR_LENGTH + TIBIA_LENGTH;
    float lenDiff = fabsf(FEMUR_LENGTH - TIBIA_LENGTH);
    if (!(d >= lenDiff - 1e-5f && d <= lenSum + 1e-5f)) {
        return false;
    }

    float D = (d2 - FEMUR_LENGTH*FEMUR_LENGTH - TIBIA_LENGTH*TIBIA_LENGTH) / (2.0f * FEMUR_LENGTH * TIBIA_LENGTH);
    if (D < -1.0f) D = -1.0f;
    if (D >  1.0f) D =  1.0f;

    float sin_knee = sqrtf(fmaxf(0.0f, 1.0f - D*D));
    float tibia_angle_rad = -atan2f(sin_knee, D);
    float femur_angle_rad = atan2f(Zp, Xp) - atan2f(TIBIA_LENGTH*sinf(tibia_angle_rad), FEMUR_LENGTH + TIBIA_LENGTH*cosf(tibia_angle_rad));

    // Servo absolute angles
    float coxa_angle_deg  = wrap360(rad2Deg(coxa_angle_rad)  - baseR);
    float femur_angle_deg = wrap360(rad2Deg(femur_angle_rad) + FEMUR_FORWARD);
    float tibia_angle_deg = wrap360(rad2Deg(tibia_angle_rad) + TIBIA_STRAIGHT);

    // Convert to ticks
    uint16_t tibia_angle_tick, femur_angle_tick, coxa_angle_tick;
    if (!deg2Tick(coxa_angle_deg,  coxa_angle_tick))  return false;
    if (!deg2Tick(femur_angle_deg, femur_angle_tick)) return false;
    if (!deg2Tick(tibia_angle_deg, tibia_angle_tick)) return false;

    positions[0] = coxa_angle_tick;
    positions[1] = femur_angle_tick;
    positions[2] = tibia_angle_tick;

    return true;
}

// -------------------- Main CLI --------------------
int main(int argc, char** argv) {

    float x, y, z;
    if (argc == 4) {
        x = std::stof(argv[1]);
        y = std::stof(argv[2]);
        z = std::stof(argv[3]);
    } else {
        x = 0.0f;
        y = 276.0f;
        z = 0.0f;
    }

    uint16_t result[3];
    if (!getIKLocal(x, y, z, result)) {
        std::cerr << "Target not reachable or out of servo range.\n";
        return 2;
    }

    std::cout << "IK Solution:\n";
    std::cout << " Coxa:  ticks=" << result[0] << "\n";
    std::cout << " Femur: ticks=" << result[1] << "\n";
    std::cout << " Tibia: ticks=" << result[2] << "\n";
    return 0;
}
