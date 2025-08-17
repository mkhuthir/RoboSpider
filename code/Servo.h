#ifndef SERVO_H
#define SERVO_H

    #include <DynamixelWorkbench.h> // Include DynamixelWorkbench for managing servos

    class Servo {
        public:
            Servo();
//---------------------------------------------------------------------------------------------------------------------------------------------------            
            bool                begin(  const char* device_name,                            // initialize the controller with device name and baudrate
                                        uint32_t baudrate,
                                        float protocol_version);          
                                        
            bool                setPortHandler(const char *device_name);                    // set the port handler for the controller  
            bool                setBaudrate(uint8_t baud_rate);                            // set the baudrate for the controller
            bool                setPacketHandler(float protocol_version);                   // set the packet handler with the protocol version
            
            float               getProtocolVersion(void);                                   // get the protocol version being used
            uint32_t            getBaudrate(void);                                          // get the current baudrate
//---------------------------------------------------------------------------------------------------------------------------------------------------
            bool                readRegister(uint8_t id, uint16_t address, uint16_t length, uint32_t *data);
            bool                writeRegister(uint8_t id, uint16_t address, uint16_t length, uint8_t* data);

            bool                readRegister(uint8_t id, const char *item_name, uint32_t *data);
            bool                writeRegister(uint8_t id, const char *item_name, uint32_t data);

            bool                addSyncWriteHandler(uint16_t address, uint16_t length);
            bool                addSyncWriteHandler(uint8_t id, const char *item_name);
            bool                syncWrite(uint8_t index, int32_t *data);
            bool                syncWrite(uint8_t index, uint8_t *id, uint8_t id_num, int32_t *data, uint8_t data_num_for_each_id);
//---------------------------------------------------------------------------------------------------------------------------------------------------
            bool                ping(uint8_t dxl_id);                                       // ping a servo to check if it is connected
            const char *        getModelName(uint8_t id);                                   // get the model name of a servo by its ID
            
            // EEPROM Area
            bool                getModelNumber(uint8_t id, uint16_t* model_number);          // get the model number of a servo by its ID
            bool                getFirmwareVersion(uint8_t id, uint8_t* version);
            bool                getBaudRate(uint8_t id, uint8_t* baud_rate);
            bool                getReturnDelayTime(uint8_t id, uint8_t* return_delay_time);
            bool                getAngleLimits(uint8_t id, uint16_t* CW_angle, uint16_t* CCW_angle);
            bool                setAngleLimits(uint8_t id, uint16_t CW_angle, uint16_t CCW_angle);
            bool                getTemperatureLimit(uint8_t id, uint8_t* max_temp);
            bool                getVoltageLimit(uint8_t id, uint8_t* min_voltage, uint8_t* max_voltage);
            bool                getMaxTorque(uint8_t id, uint16_t* max_torque);
            bool                getStatusReturnLevel(uint8_t id, uint8_t* level);
            bool                getAlarmLED(uint8_t id, uint8_t* alarm_led);
            bool                getShutdown(uint8_t id, uint8_t* shutdown);    

            // RAM Area
            bool                isTorqueOn(uint8_t id);                                     // check if the torque is enabled for a servo
            bool                torqueOn(uint8_t id);                                       // turn on the torque of a servo
            bool                torqueOff(uint8_t id);                                      // turn off the torque of a servo
            bool                isLedOn(uint8_t id);                                        // check if the LED is enabled for a servo
            bool                ledOn(uint8_t dxl_id);                                      // turn on the LED of a servo
            bool                ledOff(uint8_t dxl_id);                                     // turn off the LED of a servo

            bool                getComplianceMargin(uint8_t id, uint8_t* CW_margin, uint8_t* CCW_margin);           // get the compliance margin of a servo
            bool                setComplianceMargin(uint8_t id, uint8_t CW_margin, uint8_t CCW_margin);              // set the compliance margin of a servo
            bool                getComplianceSlope(uint8_t id, uint8_t* CW_slope, uint8_t* CCW_slope);                // get the compliance slope of a servo
            bool                setComplianceSlope(uint8_t id, uint8_t CW_slope, uint8_t CCW_slope);                // set the compliance slope of a servo

            bool                setGoalPosition(uint8_t id, int16_t position);                  // set the position of a servo
            bool                setGoalSpeed(uint8_t id, int16_t speed);                        // set the speed of a servo

            bool                getTorqueLimit(uint8_t id, uint16_t* torque_limit);              // get the torque limit of a servo
            bool                setTorqueLimit(uint8_t id, uint16_t torque_limit);              // set the torque limit of a servo

            bool                getPresentPosition(uint8_t id, uint16_t* pos);                      // get the present position of a servo
            bool                getPresentSpeed(uint8_t id, uint16_t* speed);                       // get the present speed of a servo
            bool                getPresentLoad(uint8_t id, uint16_t* load);                         // get the present load of a servo
            bool                getPresentVoltage(uint8_t id, uint8_t* voltage);                   // get the present voltage of a servo
            bool                getPresentTemperature(uint8_t id, uint8_t* temperature);           // get the present temperature of a servo

            bool                isMoving(uint8_t id);                                       // check if a servo is currently moving

//---------------------------------------------------------------------------------------------------------------------------------------------------
            bool                init(   uint8_t dxl_id,                                     // initialize a servo with default settings            
                                        int32_t speed,
                                        int32_t CW_angle, 
                                        int32_t CCW_angle);

            bool                update(uint8_t id);                                         // update the state of a servo

            bool                runConsoleCommands(const String& cmd, const String& args);  // Process console commands for servo control
            bool                printStatus(uint8_t id);                                    // print the status of a servo for debugging            
            bool                printConsoleHelp();                                         // Print servo-specific help information
            DynamixelWorkbench* getWorkbench();                                             // if you need to expose the workbench pointer
//---------------------------------------------------------------------------------------------------------------------------------------------------
        private:
            const char*         log = NULL;         // Log string for debugging 
            DynamixelWorkbench  dxl;                // DynamixelWorkbench instance for managing servos

            
    };

#endif // SERVO_H
