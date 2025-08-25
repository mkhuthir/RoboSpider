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
#define TICKS_MAX        uint16_t(1023)                     // max ticks 1023 = 300 degrees

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

    // 1) Coxa yaw
    float coxaRad = atan2f(y, x);

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
    float tibiaRad = -atan2f(sin_knee, D);
    float femurRad = atan2f(Zp, Xp) - atan2f(TIBIA_LENGTH*sinf(tibiaRad), FEMUR_LENGTH + TIBIA_LENGTH*cosf(tibiaRad));

    // Servo absolute angles
    float coxaDeg  = wrap360(rad2Deg(coxaRad)  - coxaMountDeg);
    float femurDeg = wrap360(rad2Deg(femurRad) + FEMUR_FORWARD);
    float tibiaDeg = wrap360(rad2Deg(tibiaRad) + TIBIA_STRAIGHT);

    // Convert to ticks
    uint16_t coxaTick, femurTick, tibiaTick;
    if (!deg2Tick(coxaDeg,  coxaTick))  return false;
    if (!deg2Tick(femurDeg, femurTick)) return false;
    if (!deg2Tick(tibiaDeg, tibiaTick)) return false;

    out.coxaTick    = coxaTick;
    out.femurTick   = femurTick;
    out.tibiaTick   = tibiaTick;
    out.coxaRad     = coxaRad;
    out.femurRad    = femurRad;
    out.tibiaRad    = tibiaRad;
    out.coxaDeg     = coxaDeg;
    out.femurDeg    = femurDeg;
    out.tibiaDeg    = tibiaDeg;
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
        coxaMountDeg = -90.0f;
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
