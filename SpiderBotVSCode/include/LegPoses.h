#ifndef LEG_POSES_H
#define LEG_POSES_H

  // LEG SERVOS
  #define COXA_VELOCITY   200   // Default velocity for coxa servo
  #define COXA_MIN        0     // Min angle for COXA servo
  #define COXA_MAX        1023  // Max angle for COXA servo

  #define FEMUR_VELOCITY  200   // Default velocity for femur servo
  #define FEMUR_MIN       0     // Min angle for FEMUR servo
  #define FEMUR_MAX       1023  // Max angle for FEMUR servo

  #define TIBIA_VELOCITY  200   // Default velocity for tibia servo
  #define TIBIA_MIN       0     // Min angle for TIBIA servo
  #define TIBIA_MAX       1023  // Max angle for TIBIA servo

  int32_t poseLegUp[LEG_SERVOS]   = {512, 204, 512};  // Leg pose for up position
  int32_t poseLegDown[LEG_SERVOS] = {512, 204, 512};  // Leg pose for down position
  int32_t poseLegOut[LEG_SERVOS]  = {512, 512, 512};  // Leg pose for out position

#endif // LEG_POSES_H