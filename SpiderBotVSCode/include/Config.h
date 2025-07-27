#ifndef CONFIG_H
#define CONFIG_H

  // Config.h - Configuration file for SpiderBot project

  // Define DEBUG to enable debug output
  //#define DEBUG 
  #undef DEBUG 

  // ------------------------
  // SERIAL PORTS
  // ------------------------

  #define DEBUG_SERIAL Serial     // Serial port for debugging output
  #define RC100_SERIAL Serial1    // Serial port for RC100 remote controller
  #define DXL_SERIAL ""           // OpenCR Dynamixel is on Serial1(USART1)

  // ------------------------
  // SERIAL BAUD RATES
  // ------------------------

  #define DXL_BAUD_RATE         1000000   // Default baud rate for Dynamixel servos
  #define DEBUG_BAUD_RATE       115200    // Baud rate for debugging output
  #define RC100_BAUD_RATE       115200    // Baud rate for RC controller communication

  // ------------------------
  // Dynamixel PROTOCOL VERSION
  // ------------------------

  #define PROTOCOL_VERSION 1.0 // Protocol version for Dynamixel servos

  // ------------------------
  // Servos & Sensors IDs
  // ------------------------
  
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




  // AX-S1 Sensor
  #define AXS1_SENSOR_ID        100

  // ------------------------
  // SERVO LIMITS
  // Define safe limits to avoid mechanical overrun
  // AX-18A typical physical limit ~0-300 deg

  #define DEFAULT_SERVO_VELOCITY      200   // Default velocity for servos


#endif  // CONFIG_H