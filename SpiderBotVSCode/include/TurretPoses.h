#ifndef TURRET_POSES_H
#define TURRET_POSES_H


   // Turret servos
  #define TURRET_PAN_ID     19
  #define TURRET_TILT_ID    20

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

  uint8_t turret_ids[TURRET_SERVOS] = {TURRET_PAN_ID, TURRET_TILT_ID}; // Servo IDs for pan and tilt


#endif // TURRET_POSES_H