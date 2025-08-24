#ifndef LEG_H
#define LEG_H

  #include "Servo.h"
  #include "Driver.h"

  #define handler_index   0                 // Index for sync write handler
  
  #define LEG_SERVOS        uint8_t(3)      // Number of servos per leg
  #define HEXAPOD_LEGS      uint8_t(6)      // Maximum number of legs
  #define LEG_SPEED         uint16_t(100)   // Default speed for leg movement
  
  #define COXA_LENGTH       float(52)       // Length of the coxa segment in mm
  #define COXA_CW_LIMIT     uint16_t(225)   // Min position for COXA servo
  #define COXA_CCW_LIMIT    uint16_t(798)   // Max position for COXA servo
  #define COXA_DEFAULT      uint16_t(512)   // Default position for COXA servo

  #define FEMUR_LENGTH      float(82)       // Length of the femur segment in mm
  #define FEMUR_CW_LIMIT    uint16_t(204)   // Min position for FEMUR servo
  #define FEMUR_CCW_LIMIT   uint16_t(819)   // Max position for FEMUR servo
  #define FEMUR_DEFAULT     uint16_t(358)   // Default position for FEMUR servo

  #define TIBIA_LENGTH      float(142)      // Length of the tibia segment in mm
  #define TIBIA_CW_LIMIT    uint16_t(30)    // Min position for TIBIA servo
  #define TIBIA_CCW_LIMIT   uint16_t(993)   // Max position for TIBIA servo
  #define TIBIA_DEFAULT     uint16_t(665)   // Default position for TIBIA servo

  class Leg {
    public:
      Leg();                                  // Constructor
      bool    init( uint8_t legIndex,         // Initialize the leg servos
                    uint8_t coxaID,           
                    uint8_t femurID, 
                    uint8_t tibiaID,
                    float   legBaseX,
                    float   legBaseY,
                    float   legBaseZ,
                    float   legBaseR,
                    Driver* driver,
                    Servo*  servo);

      bool      update();                         // Update the leg state

      bool      move(int32_t *positions);         // Move the leg to the specified positions
      bool      isMoving();                       // Check if any servo in the leg is currently moving 

      bool      movePointUp();                    // Move leg up
      bool      movePointDown();                  // Move leg down
      bool      movePointOut();                   // Move leg out
      bool      moveStandUp();                    // Move leg to stand up position
      bool      moveStandDown();                  // Move leg to stand down position

      bool      setSpeed(uint16_t speed);         // Set the speed of the leg
      uint16_t  getSpeed() const;                 // Get the current speed of the leg

      bool      getBasePosition(float* base_x, float* base_y, float* base_z, float* base_r);

      bool      setServoPositions(uint16_t coxa, uint16_t femur, uint16_t tibia);
      bool      getServoPositions(uint16_t* coxa, uint16_t* femur, uint16_t* tibia);

      bool      setTipLocalPosition(float tip_local_x, float tip_local_y, float tip_local_z);
      bool      getTipLocalPosition(float* tip_local_x, float* tip_local_y, float* tip_local_z);

      bool      printStatus();                    // Print current joint angles to Serial
      bool      runConsoleCommands( const String& cmd, 
                                    const String& args, 
                                    int legIndex);// Process console commands for leg control
      bool      printConsoleHelp();               // Print leg-specific help information
      
    private:
      Driver*   driver        = nullptr;          // Pointer to the driver instance
      Servo*    servo         = nullptr;          // Pointer to the servo instance
      uint16_t  speed         = 0;                // Speed of the leg

      uint8_t   legIndex      = 0;                // Index of the leg (0-5)
      float     legBaseX      = 0.0;              // Base X position from body center
      float     legBaseY      = 0.0;              // Base Y position from body center
      float     legBaseZ      = 0.0;              // Base Z position from body center
      float     legBaseR      = 0.0;              // Base Rotation position from body center

      uint8_t   legServoIDs[LEG_SERVOS]={0,0,0};  // Servo IDs for the leg joints

      enum LegJoint { Coxa  = 0,                  // Enum for leg joints
                      Femur = 1, 
                      Tibia = 2 };


      bool      getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, uint16_t* positions);
      bool      getIKGlobal(float tip_global_x, float tip_global_y, float tip_global_z, uint16_t* positions);

      void      globalToLocal( float global_x, float global_y, float global_z,
                                    float& local_x, float& local_y, float& local_z);
  };

#endif // LEG_H