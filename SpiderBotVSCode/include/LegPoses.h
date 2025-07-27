#ifndef LEG_POSES_H
#define LEG_POSES_H

  // LEG SERVOS
  #define LEG_VELOCITY  500   // Default velocity for coxa servo
  #define COXA_MIN      0     // Min angle for COXA servo
  #define COXA_MAX      1023  // Max angle for COXA servo
  #define FEMUR_MIN     0     // Min angle for FEMUR servo
  #define FEMUR_MAX     1023  // Max angle for FEMUR servo
  #define TIBIA_MIN     0     // Min angle for TIBIA servo
  #define TIBIA_MAX     1023  // Max angle for TIBIA servo

  int32_t poseLegUp[LEG_SERVOS]   = {512, 819, 512};  // Leg pose for up position
  int32_t poseLegDown[LEG_SERVOS] = {512, 204, 512};  // Leg pose for down position
  int32_t poseLegOut[LEG_SERVOS]  = {512, 512, 512};  // Leg pose for out position

  // Wave Gait Poses
  int32_t poseLegWaveGaitUp[LEG_SERVOS]   = {512, 665, 972};  // Leg pose for wave gait up position
  int32_t poseLegWaveGaitDown[LEG_SERVOS] = {512, 358, 665};  // Leg pose for wave gait down position
  
#endif // LEG_POSES_H