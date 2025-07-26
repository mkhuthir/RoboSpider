#ifndef HEXAPOD_POSES_H
#define HEXAPOD_POSES_H


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