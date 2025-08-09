#ifndef TURRET_POSES_H
#define TURRET_POSES_H


   // Turret servos
  #define TURRET_PAN_ID     19
  #define TURRET_TILT_ID    20

  #define TURRET_VELOCITY   500   // Default velocity for turret servos
  #define TURRET_PAN_MIN    0     // Min angle for turret pan servo
  #define TURRET_PAN_MAX    1023  // Max angle for turret pan servo
  #define TURRET_TILT_MIN   0     // Min angle for turret tilt servo
  #define TURRET_TILT_MAX   1023  // Max angle for turret tilt servo

  uint8_t turret_ids[TURRET_SERVOS]       = {TURRET_PAN_ID, TURRET_TILT_ID};  // Servo IDs for pan and tilt
  int32_t poseTurretHome[TURRET_SERVOS]   = {512, 204};                       // Turret pose for home position
  int32_t poseTurretUp[TURRET_SERVOS]     = {512, 512};                       // Turret pose for looking up
  int32_t poseTurretDown[TURRET_SERVOS]   = {512, 100};                       // Turret pose for looking down 
  int32_t poseTurretRight[TURRET_SERVOS]  = {204, 204};                       // Turret pose for looking right
  int32_t poseTurretLeft[TURRET_SERVOS]   = {819, 204};                       // Turret pose for looking left

#endif // TURRET_POSES_H