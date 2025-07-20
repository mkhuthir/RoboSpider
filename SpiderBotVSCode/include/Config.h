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

  // Legs (IDs 1 to 18)
  #define LEG_SERVO_ID_START    1
  #define LEG_SERVO_ID_END      18

  // Turret servos
  #define TURRET_PAN_ID         19
  #define TURRET_TILT_ID        20

  // AX-S1 Sensor
  #define AXS1_SENSOR_ID        100

  // ------------------------
  // SERVO LIMITS
  // Define safe limits to avoid mechanical overrun
  // AX-18A typical physical limit ~0-300 deg

  #define MAX_NO_OF_SERVOS            256       // Max number of servos supported by Dynamixel2Arduino
  #define DEFAULT_SERVO_VELOCITY      200   // Default velocity for servos
  
  // ------------------------
  // TURRET SERVOS

  #define TURRET_VELOCITY       200   // Default velocity for turret servos
  #define TURRET_PAN_MIN_DEG    0     // Min angle for turret pan servo
  #define TURRET_PAN_MAX_DEG    1023  // Max angle for turret pan servo
  #define TURRET_PAN_HOME_DEG   512   // Home position for turret pan servo
  #define TURRET_PAN_LEFT_DEG   819   // Left position for turret pan servo
  #define TURRET_PAN_RIGHT_DEG  204   // Right position for turret pan servo
  #define TURRET_PAN_DOWN_DEG   512   // Down position for turret pan servo
  #define TURRET_PAN_UP_DEG     512   // Up position for turret pan servo

  #define TURRET_TILT_MIN_DEG   0     // Min angle for turret tilt servo
  #define TURRET_TILT_MAX_DEG   1023  // Max angle for turret tilt servo
  #define TURRET_TILT_HOME_DEG  204   // Home position for turret tilt servo
  #define TURRET_TILT_LEFT_DEG  204   // Left position for turret tilt servo
  #define TURRET_TILT_RIGHT_DEG 204   // Right position for turret tilt servo
  #define TURRET_TILT_DOWN_DEG  100   // Down position for turret tilt servo
  #define TURRET_TILT_UP_DEG    512   // Up position for turret tilt servo

  // ------------------------
  // LEG SERVOS
  #define COXA_VELOCITY         200   // Default velocity for coxa servo
  #define COXA_MIN_DEG          0     // Min angle for COXA servo
  #define COXA_MAX_DEG          1023  // Max angle for COXA servo
  #define COXA_HOME_DEG         512   // Home position for COXA servo
  #define COXA_UP_DEG           512   // Default up angle for COXA servo
  #define COXA_DOWN_DEG         512   // Default down angle for COXA servo

  #define FEMUR_VELOCITY        200   // Default velocity for femur servo
  #define FEMUR_MIN_DEG         0     // Min angle for FEMUR servo
  #define FEMUR_MAX_DEG         1023  // Max angle for FEMUR servo
  #define FEMUR_HOME_DEG        358   // Home position for FEMUR servo
  #define FEMUR_UP_DEG          665   // Default up angle for FEMUR servo
  #define FEMUR_DOWN_DEG        358   // Default down angle for FEMUR servo

  #define TIBIA_VELOCITY        200   // Default velocity for tibia servo
  #define TIBIA_MIN_DEG         0     // Min angle for TIBIA servo
  #define TIBIA_MAX_DEG         1023  // Max angle for TIBIA servo
  #define TIBIA_HOME_DEG        665   // Home position for TIBIA servo
  #define TIBIA_UP_DEG          819   // Default up angle for TIBIA servo
  #define TIBIA_DOWN_DEG        665   // Default down angle for TIBIA servo

  // ------------------------
  // GAIT TIMING
  // ------------------------
  #define DEFAULT_GAIT_CYCLE_MS 1000

#endif  // CONFIG_H