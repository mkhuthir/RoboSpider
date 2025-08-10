#ifndef __GAITPOSES_H__
#define __GAITPOSES_H__ 

    #define HEXAPOD_LEGS   uint8_t(6)   // Maximum number of legs
    #define LEG_SERVOS     uint8_t(3)   // Number of servos per leg
    #define D 100                       // Coxa movement offset for all gaits

    // Wave Gait Poses
    uint8_t poseWaveGaitIDs[HEXAPOD_LEGS][LEG_SERVOS]     = {{1    , 2  , 3  }, {4    , 5  , 6  }, {7    , 8  , 9  }, {10   , 11 , 12 }, {13   , 14 , 15 }, {16   , 17 , 18 }};
    int32_t poseWaveGaitLegUp[HEXAPOD_LEGS][LEG_SERVOS]   = {{665  , 665, 972}, {358  , 665, 972}, {512  , 665, 972}, {512  , 665, 972}, {358  , 665, 972}, {665  , 665, 972}}; 
    int32_t poseWaveGaitLegDown[HEXAPOD_LEGS][LEG_SERVOS] = {{665-D, 358, 665}, {358+D, 358, 665}, {512-D, 358, 665}, {512+D, 358, 665}, {358-D, 358, 665}, {665+D, 358, 665}}; 

    // Ripple Gait Poses
    uint8_t poseRippleGaitIDs[HEXAPOD_LEGS/2][LEG_SERVOS*2]     = {{1    , 2  , 3  , 4    , 5  , 6  }, {7    , 8  , 9  , 10   , 11 , 12 }, {13   , 14 , 15 , 16   , 17 , 18 }}; 
    int32_t poseRippleGaitLegUp[HEXAPOD_LEGS/2][LEG_SERVOS*2]   = {{665  , 665, 972, 358  , 665, 972}, {512  , 665, 972, 512  , 665, 972}, {358  , 665, 972, 665  , 665, 972}}; 
    int32_t poseRippleGaitLegDown[HEXAPOD_LEGS/2][LEG_SERVOS*2] = {{665-D, 358, 665, 358+D, 358, 665}, {512-D, 358, 665, 512+D, 358, 665}, {358-D, 358, 665, 665+D, 358, 665}}; 

    // Tripod Gait Poses
    uint8_t poseTripodGaitIDs[HEXAPOD_LEGS/3][LEG_SERVOS*3]     = {{1    , 2  , 3  , 10   , 11 , 12 , 13   , 14 , 15 }, { 4   ,5  , 6  , 7    , 8  , 9  ,16   , 17 , 18 }}; 
    int32_t poseTripodGaitLegUp[HEXAPOD_LEGS/3][LEG_SERVOS*3]   = {{665  , 665, 972, 512  , 665, 972, 358  , 665, 972}, {358  ,665, 972, 512  , 665, 972,665  , 665, 972}}; 
    int32_t poseTripodGaitLegDown[HEXAPOD_LEGS/3][LEG_SERVOS*3] = {{665-D, 358, 665, 512+D, 358, 665, 358-D, 358, 665}, {358+D,358, 665, 512-D, 358, 665,665+D, 358, 665}}; 

#endif // __GAITPOSES_H__