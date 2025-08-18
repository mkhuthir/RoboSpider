#ifndef LEGPOSES_H
#define LEGPOSES_H

#include <stdint.h>
#include "BodyPose.h"

class LegPose {
public:
    LegPose();
    LegPose(int32_t coxa, int32_t femur, int32_t tibia);

    void setPose(int32_t coxa, int32_t femur, int32_t tibia);
    void getPose(int32_t &coxa, int32_t &femur, int32_t &tibia) const;
    void reset();

    // Kinematic transformation based on body pose
    void transformWithBody(const BodyPose& bodyPose);

    int32_t coxa;
    int32_t femur;
    int32_t tibia;
};

// Predefined poses
extern LegPose poseLegPointUp;
extern LegPose poseLegPointDown;
extern LegPose poseLegPointOut;
extern LegPose poseLegStandUp;
extern LegPose poseLegStandDown;

#endif // LEGPOSES_H
