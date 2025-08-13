#ifndef LEG_POSES_H
#define LEG_POSES_H

  // LEG SERVOS
  #define LEG_SPEED           300  // Default speed for coxa servo
  #define COXA_CW_LIMIT       225  // Min angle for COXA servo
  #define COXA_CCW_LIMIT      798  // Max angle for COXA servo
  #define FEMUR_CW_LIMIT      204  // Min angle for FEMUR servo
  #define FEMUR_CCW_LIMIT     819  // Max angle for FEMUR servo
  #define TIBIA_CW_LIMIT      30   // Min angle for TIBIA servo
  #define TIBIA_CCW_LIMIT     993  // Max angle for TIBIA servo

  int32_t poseLegPointUp[LEG_SERVOS]   = {512, 819, 512};  // Leg pose for pointing up
  int32_t poseLegPointDown[LEG_SERVOS] = {512, 204, 512};  // Leg pose for pointing down
  int32_t poseLegPointOut[LEG_SERVOS]  = {512, 512, 512};  // Leg pose for pointing out
  int32_t poseLegStandUp[LEG_SERVOS]   = {512, 358, 665};  // Leg pose for standing up
  int32_t poseLegStandDown[LEG_SERVOS] = {512, 665, 972};  // Leg pose for standing down

  
#endif // LEG_POSES_H