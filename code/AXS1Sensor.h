#ifndef AXS1_SENSOR_H
#define AXS1_SENSOR_H

    // Sensor ID for AX-S1
    #define AXS1_SENSOR_ID                      100
    #define AXS1_OBSTACLE_DETECTED              8  // Obstacle detected threshold
    #define AXS1_LIGHT_DETECTED                 8  // Light detected threshold

    // EEPROM Area (Section 4.6 https://emanual.robotis.com/docs/en/parts/sensor/ax-s1/)
    #define AXS1_Model_Number_L                 0   // access=R  , initial value=13(0x0D)
    #define AXS1_Model_Number_H                 1   // access=R  , initial value=0(0x00)
    #define AXS1_Firmware_Version               2   // access=R
    #define AXS1_ID                             3   // access=RW , initial value=100(0x64)
    #define AXS1_Baud_Rate                      4   // access=RW , initial value=1(0x01)
    #define AXS1_Return_Delay_Time              5   // access=RW , initial value=250(0xFA)
    #define AXS1_Status_Return_Level            16  // access=RW , initial value=2(0x02)

    // RAM Area (Section 4.7 https://emanual.robotis.com/docs/en/parts/sensor/ax-s1/)
    #define AXS1_Left_Distance_Data             26  // access=R
    #define AXS1_Center_Distance_Data           27  // access=R
    #define AXS1_Right_Distance_Data            28  // access=R
    #define AXS1_Light_Left_Data                29  // access=R
    #define AXS1_Light_Center_Data              30  // access=R
    #define AXS1_Light_Right_Data               31  // access=R
    #define AXS1_IR_Obstacle_Detected           32  // access=R
    #define AXS1_Light_Detected                 33  // access=R
    #define AXS1_Sound_Data                     35  // access=R
    #define AXS1_Sound_Data_Max_Hold            36  // access=RW
    #define AXS1_Sound_Detected_Count           37  // access=RW
    #define AXS1_Sound_Detected_Time_L          38  // access=RW
    #define AXS1_Sound_Detected_Time_H          39  // access=RW
    #define AXS1_Buzzer_Data_0                  40  // access=RW
    #define AXS1_Buzzer_Data_1                  41  // access=RW
    #define AXS1_Registered                     44  // access=RW , initial value=0(0x00)
    #define AXS1_IR_Remocon_Arrived             46  // access=R  , initial value=0(0x00)
    #define AXS1_Lock                           47  // access=RW , initial value=0(0x00)
    #define AXS1_Remocon_RX_Data_0              48  // access=R
    #define AXS1_Remocon_RX_Data_1              49  // access=R
    #define AXS1_Remocon_TX_Data_0              50  // access=RW
    #define AXS1_Remocon_TX_Data_1              51  // access=RW
    #define AXS1_IR_Obstacle_Detect_Compare     52  // access=RW
    #define AXS1_Light_Detect_Compare           53  // access=RW

    #include "Servo.h"

    class AXS1Sensor {
    public:
        AXS1Sensor();

        bool begin(Servo* servo, uint8_t sensor_id);
        bool update();
        
        bool ping();

        // EEPROM Read Functions
        bool getModelNumber(uint32_t* model_number);
        int getFirmwareVersion();
        int getID();
        int getBaudRate();
        int getReturnDelayTime();
        int getStatusReturnLevel();
        
        // IR Sensors
        int getDistanceLeft();
        int getDistanceCenter();
        int getDistanceRight();

        int getIRLeft();
        int getIRCenter();
        int getIRRight();

        int setObstacleCompare(uint8_t value);
        int getObstacleCompare();
        int ObstacleDetected();
        
        int setLightCompare(uint8_t value);
        int getLightCompare();
        int LightDetected();

        // Sound Detection
        int getSoundData();
        int getSoundDataMaxHold();
        int getSoundDetectedCount();
        int getSoundDetectedTime();
        bool resetSoundDataMaxHold();
        bool resetSoundDetectedCount();
        bool resetSoundDetectedTime();
        
        // Buzzer Control
        bool playTone(uint8_t note, uint8_t duration);
        bool playMelody(uint8_t melody);
        bool startTone(uint8_t note);
        bool stopTone();

        // Remote Control
        bool RemoconArrived();
        int getRemoconRX();
        int getRemoconTX();
        bool setRemoconTX(uint32_t value);

        // Status and Helper Functions
        void printStatus();
        bool runConsoleCommands(const String& cmd, const String& args);  // Process console commands for sensor control
        void printConsoleHelp();                                                // Print sensor-specific help information

    private:
        Servo*      servo;                     // Pointer to Servo instance
        uint8_t     id = AXS1_SENSOR_ID;       // ID of the AX-S1 sensor
    };

#endif // AXS1_SENSOR_H
