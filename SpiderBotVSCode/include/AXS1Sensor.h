#ifndef AXS1_SENSOR_H
#define AXS1_SENSOR_H

    // Hardcoded Sensor ID for AX-S1
    #define AXS1_SENSOR_ID                      100
    // EEPROM Area (Section 4.6 https://emanual.robotis.com/docs/en/parts/sensor/ax-s1/)
    #define AXS1_Model_Number_L                 0
    #define AXS1_Model_Number_H                 1
    #define AXS1_Firmware_Version               2
    #define AXS1_ID                             3
    #define AXS1_Baud_Rate                      4
    #define AXS1_Return_Delay_Time              5
    #define AXS1_Status_Return_Level            16
    // RAM Area (Section 4.7 https://emanual.robotis.com/docs/en/parts/sensor/ax-s1/)
    #define AXS1_Left_Distance_Data             26
    #define AXS1_Center_Distance_Data           27
    #define AXS1_Right_Distance_Data            28
    #define AXS1_Light_Left_Data                29
    #define AXS1_Light_Center_Data              30
    #define AXS1_Light_Right_Data               31
    #define AXS1_IR_Obstacle_Detected           32
    #define AXS1_Light_Detected                 33
    #define AXS1_Sound_Data                     35
    #define AXS1_Sound_Data_Max_Hold            36
    #define AXS1_Sound_Detected_Count           37
    #define AXS1_Sound_Detected_Time_L          38
    #define AXS1_Sound_Detected_Time_H          39
    #define AXS1_Buzzer_Data_0                  40
    #define AXS1_Buzzer_Data_1                  41
    #define AXS1_Registered                     44
    #define AXS1_IR_Remocon_Arrived             46
    #define AXS1_Lock                           47
    #define AXS1_Remocon_RX_Data_0              48
    #define AXS1_Remocon_RX_Data_1              49
    #define AXS1_Remocon_TX_Data_0              50
    #define AXS1_Remocon_TX_Data_1              51
    #define AXS1_IR_Obstacle_Detect_Compare     52
    #define AXS1_Light_Detect_Compare           53

    #include "Servo.h"

    class AXS1Sensor {
    public:
        AXS1Sensor();

        bool begin(Servo* servo, uint8_t sensor_id);
        bool update();
        
        bool ping();

        bool setBuzzer(uint8_t note, uint8_t duration);

        int getIRLeft();
        int getIRCenter();
        int getIRRight();

        int getLightLeft();
        int getLightCenter();
        int getLightRight();

        int getSoundLevel();
        int getSoundCount();

        float getVoltage();
        float getTemperature();

    private:
        Servo*      servo;                     // Pointer to Servo instance
        uint8_t     id = AXS1_SENSOR_ID;       // ID of the AX-S1 sensor
    };

#endif // AXS1_SENSOR_H
