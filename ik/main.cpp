#include <iostream>
#include <cmath>
#include <cstdint>


#define COXA_LENGTH       float(52)       // Length of the coxa segment in mm
#define FEMUR_LENGTH      float(82)       // Length of the femur segment in mm
#define TIBIA_LENGTH      float(142)      // Length of the tibia segment in mm


// -------------------- Servo mapping --------------------
static constexpr float SERVO_MIN_DEG = 30.0f;    
static constexpr float SERVO_MAX_DEG = 330.0f;   
static constexpr float SERVO_SPAN_DEG = (SERVO_MAX_DEG - SERVO_MIN_DEG); // 300 deg
static constexpr float TICKS_MAX = 1023.0f;

// -------------------- Result struct --------------------
struct LegIKResult {
    uint16_t    coxaTick;
    uint16_t    femurTick;
    uint16_t    tibiaTick;
    float       coxaRad;
    float       femurRad;
    float       tibiaRad;
    uint16_t    coxaDeg;
    uint16_t    femurDeg;
    uint16_t    tibiaDeg;
};

// -------------------- Helpers --------------------
inline float rad2Deg(float r){ return r * 180.0f / M_PI; }
inline float deg2Rad(float d){ return d * M_PI / 180.0f; }

static float wrap360(float aDeg) {
    float a = fmodf(aDeg, 360.0f);
    if (a < 0) a += 360.0f;
    return a;
}

static bool deg2Tick(float deg, uint16_t &tick) {
    if (deg < SERVO_MIN_DEG || deg > SERVO_MAX_DEG) return false;
    float t = (deg - SERVO_MIN_DEG) * (TICKS_MAX / SERVO_SPAN_DEG);
    if (t < 0) t = 0;
    if (t > TICKS_MAX) t = TICKS_MAX;
    tick = static_cast<uint16_t>(lroundf(t));
    return true;
}

// -------------------- IK Solver --------------------
bool solveHexapodLegIK(float x, float y, float z, float coxaMountDeg, LegIKResult &out)
{
    // Mechanical reference: femur forward = 180°, tibia straight = 180°
    const float femurZeroDeg = 180.0f;
    const float tibiaZeroDeg = 180.0f;

    // 1) Coxa yaw
    float theta0 = atan2f(y, x);

    // 2) Planar reduction
    float r  = sqrtf(x*x + y*y);
    float Xp = r - COXA_LENGTH;
    float Zp = z;

    float d2 = Xp*Xp + Zp*Zp;
    float d  = sqrtf(d2);
    float Lsum = FEMUR_LENGTH + TIBIA_LENGTH;
    float Ldiff = fabsf(FEMUR_LENGTH - TIBIA_LENGTH);
    if (!(d >= Ldiff - 1e-5f && d <= Lsum + 1e-5f)) {
        return false;
    }

    float D = (d2 - FEMUR_LENGTH*FEMUR_LENGTH - TIBIA_LENGTH*TIBIA_LENGTH) / (2.0f * FEMUR_LENGTH * TIBIA_LENGTH);
    if (D < -1.0f) D = -1.0f;
    if (D >  1.0f) D =  1.0f;

    float sin_knee = sqrtf(fmaxf(0.0f, 1.0f - D*D));
    float theta2 = -atan2f(sin_knee, D);
    float theta1 = atan2f(Zp, Xp) - atan2f(TIBIA_LENGTH*sinf(theta2), FEMUR_LENGTH + TIBIA_LENGTH*cosf(theta2));

    // Servo absolute angles
    float coxaServoDeg  = wrap360(rad2Deg(theta0) - coxaMountDeg);
    float femurServoDeg = wrap360(rad2Deg(theta1) + femurZeroDeg);
    float tibiaServoDeg = wrap360(rad2Deg(theta2) + tibiaZeroDeg);

    // Convert to ticks
    uint16_t coxaTick, femurTick, tibiaTick;
    if (!deg2Tick(coxaServoDeg,  coxaTick))  return false;
    if (!deg2Tick(femurServoDeg, femurTick)) return false;
    if (!deg2Tick(tibiaServoDeg, tibiaTick)) return false;

    out.coxaTick    = coxaTick;
    out.femurTick   = femurTick;
    out.tibiaTick   = tibiaTick;
    out.coxaRad     = theta0;
    out.femurRad    = theta1;
    out.tibiaRad    = theta2;
    out.coxaDeg     = wrap360(rad2Deg(theta0) - coxaMountDeg);
    out.femurDeg    = wrap360(rad2Deg(theta1) + femurZeroDeg);
    out.tibiaDeg    = wrap360(rad2Deg(theta2) + tibiaZeroDeg);
    return true;
}

// -------------------- Main CLI --------------------
int main(int argc, char** argv) {

    float x, y, z, coxaMountDeg;
    if (argc == 5) {
        x = std::stof(argv[1]);
        y = std::stof(argv[2]);
        z = std::stof(argv[3]);
        coxaMountDeg = std::stof(argv[4]);
    } else {
        x = 0.0f;
        y = 276.0f;
        z = 0.0f;
        coxaMountDeg = 0.0f;
    }
    
    LegIKResult result;
    if (!solveHexapodLegIK(x, y, z, coxaMountDeg, result)) {
        std::cerr << "Target not reachable or out of servo range.\n";
        return 2;
    }

    std::cout << "IK Solution:\n";
    std::cout << "  Coxa:  ticks=" << result.coxaTick
              << " angle(rad)=" << result.coxaRad 
              << " angle(deg)=" << result.coxaDeg << "\n";
    std::cout << "  Femur: ticks=" << result.femurTick
              << " angle(rad)=" << result.femurRad
              << " angle(deg)=" << result.femurDeg << "\n";
    std::cout << "  Tibia: ticks=" << result.tibiaTick
              << " angle(rad)=" << result.tibiaRad
              << " angle(deg)=" << result.tibiaDeg << "\n";

    return 0;
}
