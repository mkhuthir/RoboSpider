#ifndef HEXAPOD_POSES_H
#define HEXAPOD_POSES_H

    
    // TURRET SERVOS

    #define TURRET_VELOCITY   200   // Default velocity for turret servos
    #define TURRET_PAN_MIN    0     // Min angle for turret pan servo
    #define TURRET_PAN_MAX    1023  // Max angle for turret pan servo
    #define TURRET_PAN_HOME   512   // Home position for turret pan servo
    #define TURRET_PAN_LEFT   819   // Left position for turret pan servo
    #define TURRET_PAN_RIGHT  204   // Right position for turret pan servo
    #define TURRET_PAN_DOWN   512   // Down position for turret pan servo
    #define TURRET_PAN_UP     512   // Up position for turret pan servo

    #define TURRET_TILT_MIN   0     // Min angle for turret tilt servo
    #define TURRET_TILT_MAX   1023  // Max angle for turret tilt servo
    #define TURRET_TILT_HOME  204   // Home position for turret tilt servo
    #define TURRET_TILT_LEFT  204   // Left position for turret tilt servo
    #define TURRET_TILT_RIGHT 204   // Right position for turret tilt servo
    #define TURRET_TILT_DOWN  100   // Down position for turret tilt servo
    #define TURRET_TILT_UP    512   // Up position for turret tilt servo
    
    // LEG SERVOS
    #define COXA_VELOCITY     200   // Default velocity for coxa servo
    #define COXA_MIN          0     // Min angle for COXA servo
    #define COXA_MAX          1023  // Max angle for COXA servo

    #define FEMUR_VELOCITY    200   // Default velocity for femur servo
    #define FEMUR_MIN         0     // Min angle for FEMUR servo
    #define FEMUR_MAX         1023  // Max angle for FEMUR servo

    #define TIBIA_VELOCITY    200   // Default velocity for tibia servo
    #define TIBIA_MIN         0     // Min angle for TIBIA servo
    #define TIBIA_MAX         1023  // Max angle for TIBIA servo

    // Hexapod Poses
    #define HEXAPOD_SERVOS    18    // Total number of servos in hexapod 

    uint8_t hexapod_ids[HEXAPOD_SERVOS] = {                 // Servo IDs for all legs
                                            1, 2, 3,          
                                            4, 5, 6,
                                            7, 8, 9,
                                            10, 11, 12,
                                            13, 14, 15,
                                            16, 17, 18
                                        };

    int32_t poseStandUP[HEXAPOD_SERVOS] = {                 // Stand up pose for hexapod
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665
                                        }; 

    int32_t poseStandDown[HEXAPOD_SERVOS] = {               // Stand down pose for hexapod
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665,
                                            512, 358, 665
                                        };

        

#endif // HEXAPOD_POSES_H