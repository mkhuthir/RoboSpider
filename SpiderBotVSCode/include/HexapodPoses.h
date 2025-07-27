#ifndef HEXAPOD_POSES_H
#define HEXAPOD_POSES_H

    // Hexapod Poses
    uint8_t poseHexapodIDs[HEXAPOD_SERVOS]       = {1  , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18 }; // Servo IDs for hexapod
    int32_t poseHexapodStandUP[HEXAPOD_SERVOS]   = {665, 358, 665, 358, 358, 665, 512, 358, 665, 512, 358, 665, 358, 358, 665, 665, 358, 665}; // Hexapod pose for standing up
    int32_t poseHexapodStandDown[HEXAPOD_SERVOS] = {665, 665, 972, 358, 665, 972, 512, 665, 972, 512, 665, 972, 358, 665, 972, 665, 665, 972}; // Hexapod pose for standing down

#endif // HEXAPOD_POSES_H