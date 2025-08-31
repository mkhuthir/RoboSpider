#include "Kinematics.h"

namespace IK {

    // Wrap degrees from [180, -180] to [0, 360]
    float wrap360(float deg) {
        return fmodf(deg + 360.0f, 360.0f);
    }

    bool deg2Tick(float deg, uint16_t &tick) {
        if (deg < SERVO_MIN_DEG || deg > SERVO_MAX_DEG) return false;
        float t = (deg - SERVO_MIN_DEG) * (SERVO_MAX_TICK / SERVO_SPAN_DEG);
        if (t < SERVO_MIN_TICK) t = SERVO_MIN_TICK;
        if (t > SERVO_MAX_TICK) t = SERVO_MAX_TICK;
        tick = static_cast<uint16_t>(lroundf(t));
        return true;
    }

    bool tick2Deg(uint16_t tick, float &deg) {
        if (tick < SERVO_MIN_TICK || tick > SERVO_MAX_TICK) return false;
        deg = SERVO_MIN_DEG + (static_cast<float>(tick) * SERVO_SPAN_DEG / SERVO_MAX_TICK);
        return true;
    }

    void global2Local(float global_x, float global_y, float global_z, float baseX, float baseY, float baseZ, float* local_x, float* local_y, float* local_z) {
        *local_x = global_x - baseX;
        *local_y = global_y - baseY;
        *local_z = global_z - baseZ;
    }

    void local2Global(float local_x, float local_y, float local_z, float baseX, float baseY, float baseZ, float* global_x, float* global_y, float* global_z) {
        *global_x = baseX + local_x;
        *global_y = baseY + local_y;
        *global_z = baseZ + local_z;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------
    // Local Inverse Kinematics
    bool getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, float baseR, uint16_t* positions) {
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

        positions[0] = coxa_angle_tick;
        positions[1] = femur_angle_tick;
        positions[2] = tibia_angle_tick;

        return true;
    }

    // Global Inverse Kinematics
    bool getIKGlobal(float tip_global_x, float tip_global_y, float tip_global_z, float baseX, float baseY, float baseZ, float baseR, uint16_t* positions) {
        float tip_local_x, tip_local_y, tip_local_z;
        global2Local(tip_global_x, tip_global_y, tip_global_z, baseX, baseY, baseZ, &tip_local_x, &tip_local_y, &tip_local_z);
        return getIKLocal(tip_local_x, tip_local_y, tip_local_z, baseR, positions);
    }

    // Local Forward Kinematics
    bool getFKLocal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseR, float* tip_local_x, float* tip_local_y, float* tip_local_z) {
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

    bool getFKGlobal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseX, float baseY, float baseZ, float baseR, float* tip_global_x, float* tip_global_y, float* tip_global_z) {
        float tip_local_x = 0.0f, tip_local_y = 0.0f, tip_local_z = 0.0f;
        // Compute local tip position using FK
        if (!getFKLocal(coxa, femur, tibia, baseR, &tip_local_x, &tip_local_y, &tip_local_z)) {
            return false;
        }
        local2Global(tip_local_x, tip_local_y, tip_local_z, baseX, baseY, baseZ, tip_global_x, tip_global_y, tip_global_z);
        return true;
    }

} // namespace IK
