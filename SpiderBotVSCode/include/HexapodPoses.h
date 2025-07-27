#ifndef HEXAPOD_POSES_H
#define HEXAPOD_POSES_H

    // Make HEXAPOD_LEGS, LEG_SERVOS, and HEXAPOD_SERVOS available globally
    #define handler_index  0                                                // Index for sync write handler
    static constexpr uint8_t  HEXAPOD_LEGS   = 6;                           // Maximum number of legs
    static constexpr uint8_t  LEG_SERVOS     = 3;                           // Number of servos per leg
    static constexpr uint8_t  HEXAPOD_SERVOS = HEXAPOD_LEGS * LEG_SERVOS;   // Maximum number of servos

    // Hexapod Poses
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
                                            512, 665, 972,
                                            512, 665, 972,
                                            512, 665, 972,
                                            512, 665, 972,
                                            512, 665, 972,
                                            512, 665, 972
                                        };

        

#endif // HEXAPOD_POSES_H