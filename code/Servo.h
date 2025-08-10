#ifndef SERVO_H
#define SERVO_H

    #include <DynamixelWorkbench.h> // Include DynamixelWorkbench for managing servos
    
    #define PROTOCOL_VERSION 1.0    // Protocol version for Dynamixel servos


    class Servo {
        public:
            Servo();
//---------------------------------------------------------------------------------------------------------------------------------------------------            
            bool                begin(const char* device_name, uint32_t baudrate);          // initialize the controller with device name and baudrate
            bool                setPortHandler(const char *device_name);                    // set the port handler for the controller  
            bool                setBaudrate(uint32_t baud_rate);                            // set the baudrate for the controller
            bool                setPacketHandler(float protocol_version);                   // set the packet handler with the protocol version
            float               getProtocolVersion(void);                                   // get the protocol version being used
            uint32_t            getBaudrate(void);                                          // get the current baudrate
//---------------------------------------------------------------------------------------------------------------------------------------------------
            bool                readRegister(uint8_t id, uint16_t address, uint16_t length, uint32_t *data);
            bool                writeRegister(uint8_t id, uint16_t address, uint16_t length, uint8_t* data);

            bool                readRegister(uint8_t id, const char *item_name, int32_t *data);
            bool                writeRegister(uint8_t id, const char *item_name, int32_t data);

            bool                addSyncWriteHandler(uint16_t address, uint16_t length);
            bool                addSyncWriteHandler(uint8_t id, const char *item_name);
            bool                syncWrite(uint8_t index, int32_t *data);
            bool                syncWrite(uint8_t index, uint8_t *id, uint8_t id_num, int32_t *data, uint8_t data_num_for_each_id);
//---------------------------------------------------------------------------------------------------------------------------------------------------
            bool                ping(uint8_t dxl_id);                                       // ping a servo to check if it is connected

            const char *        getModelName(uint8_t id);                                   // get the model name of a servo by its ID
            uint16_t            getModelNumber(uint8_t id);                                 // get the model number of a servo by its ID
            bool                getPosition(uint8_t id, int32_t* pos);                      // get the present position of a servo
            bool                getSpeed(uint8_t id, int32_t* speed);                       // get the present speed of a servo
            bool                getLoad(uint8_t id, int32_t* load);                         // get the present load of a servo
            bool                getVoltage(uint8_t id, int32_t* voltage);                   // get the present voltage of a servo
            bool                getTemperature(uint8_t id, int32_t* temperature);           // get the present temperature of a servo

            bool                setJointMode(uint8_t dxl_id);                               // set a servo to joint mode
            bool                setPosition(uint8_t id, int32_t position);                  // set the position of a servo
            bool                setSpeed(uint8_t id, int32_t speed);                        // set the speed of a servo

            bool                isMoving(uint8_t id);                                       // check if a servo is currently moving
            bool                isTorqueOn(uint8_t id);                                     // check if the torque is enabled for a servo
            bool                isLedOn(uint8_t id);                                        // check if the LED is enabled for a servo

            bool                torqueOn(uint8_t id);                                       // turn on the torque of a servo
            bool                torqueOff(uint8_t id);                                      // turn off the torque of a servo
            bool                ledOn(uint8_t dxl_id);                                      // turn on the LED of a servo
            bool                ledOff(uint8_t dxl_id);                                     // turn off the LED of a servo
//---------------------------------------------------------------------------------------------------------------------------------------------------
            bool                init(uint8_t dxl_id, int32_t speed);                        // initialize a servo with default settings            
            bool                runConsoleCommands(const String& cmd, const String& args);  // Process console commands for servo control
            bool                printStatus(uint8_t id);                                    // print the status of a servo for debugging            
            void                printConsoleHelp();                                         // Print servo-specific help information
            DynamixelWorkbench* getWorkbench();                                             // if you need to expose the workbench pointer
//---------------------------------------------------------------------------------------------------------------------------------------------------
        private:
            DynamixelWorkbench  dxl;                // DynamixelWorkbench instance for managing servos
            const char*         log = NULL;         // Log string for debugging 
            
    };

#endif // SERVO_H
