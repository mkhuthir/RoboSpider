#ifndef __GAITPOSES_H__
#define __GAITPOSES_H__ 

    #define HEXAPOD_LEGS   uint8_t(6)                         // Maximum number of legs
    #define LEG_SERVOS     uint8_t(3)                         // Number of servos per leg
    
    #define D 100    // delta value for wave gait poses

    // Wave Gait Poses
    int32_t poseLegWaveGaitUp[HEXAPOD_LEGS][LEG_SERVOS]   = {{665  , 665, 972}, {358  , 665, 972}, {512  , 665, 972}, {512  , 665, 972}, {358  , 665, 972}, {665  , 665, 972}};  // Leg pose for wave gait up position
    int32_t poseLegWaveGaitDown[HEXAPOD_LEGS][LEG_SERVOS] = {{665-D, 358, 665}, {358+D, 358, 665}, {512-D, 358, 665}, {512+D, 358, 665}, {358-D, 358, 665}, {665+D, 358, 665}};  // Leg pose for wave gait down position
    

#endif // __GAITPOSES_H__