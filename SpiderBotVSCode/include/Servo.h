#ifndef SERVO_H
#define SERVO_H

    #include <DynamixelWorkbench.h> // Include DynamixelWorkbench for managing servos
    
    #define PROTOCOL_VERSION 1.0    // Protocol version for Dynamixel servos


    class Servo {
        public:
            Servo();
            bool                begin(const char* device_name, uint32_t baudrate);  // initialize the controller with device name and baudrate
            
            bool                setPortHandler(const char *device_name);                    // set the port handler for the controller  
            bool                setBaudrate(uint32_t baud_rate);                            // set the baudrate for the controller
            bool                setPacketHandler(float protocol_version);                   // set the packet handler with the protocol version

            float               getProtocolVersion(void);                                   // get the protocol version being used
            uint32_t            getBaudrate(void);                                          // get the current baudrate

            const char *        getModelName(uint8_t id);                                   // get the model name of a servo by its ID
            uint16_t            getModelNumber(uint8_t id);                                 // get the model number of a servo by its ID
            
            bool                ping(uint8_t dxl_id);                                       // ping a servo to check if it is connected

            bool                writeRegister(uint8_t id, uint16_t address, uint16_t length, uint8_t* data);
            bool                writeRegister(uint8_t id, const char *item_name, int32_t data);
            bool                readRegister(uint8_t id, uint16_t address, uint16_t length, uint32_t *data);
            bool                readRegister(uint8_t id, const char *item_name, int32_t *data);


            bool                addSyncWriteHandler(uint16_t address, uint16_t length);
            bool                addSyncWriteHandler(uint8_t id, const char *item_name);
            bool                syncWrite(uint8_t index, int32_t *data);
            bool                syncWrite(uint8_t index, uint8_t *id, uint8_t id_num, int32_t *data, uint8_t data_num_for_each_id);

            bool                torqueOn(uint8_t id);                                       // turn on the torque of a servo
            bool                torqueOff(uint8_t id);                                      // turn off the torque of a servo

            bool                itemWrite(uint8_t id, const char *item_name, int32_t data); // write an item to a servo
            bool                itemRead(uint8_t id, const char *item_name, int32_t *data); // read an item from a servo

            bool                ledOn(uint8_t dxl_id);                                      // turn on the LED of a servo
            bool                ledOff(uint8_t dxl_id);                                     // turn off the LED of a servo

            bool                setNormalDirection(uint8_t id);                             // set the servo direction to normal
            bool                setReverseDirection(uint8_t id);                            // set the servo direction to reverse

            bool                jointMode(uint8_t dxl_id);                                  // set a servo to joint mode

            bool                goalPosition(uint8_t id, int32_t value);
            bool                goalPosition(uint8_t id, float radian);

            bool                goalVelocity(uint8_t id, int32_t value);
            bool                goalVelocity(uint8_t id, float velocity);

            bool                getPresentPositionData(uint8_t id, int32_t* data);
            bool                getRadian(uint8_t id, float* radian);

            bool                getPresentVelocityData(uint8_t id, int32_t* data);
            bool                getVelocity(uint8_t id, float* velocity);
//---------------------------------------------------------------------------------------------------------------------------------------------------
            bool                init(uint8_t dxl_id, int32_t velocity);                     // initialize a servo with default settings
            bool                isMoving(uint8_t id);                                       // check if a servo is currently moving
            bool                printStatus(uint8_t id);                                    // print the status of a servo for debugging
            
            bool                runConsoleCommands(const String& cmd, const String& args);  // Process console commands for servo control
            void                printConsoleHelp();                                         // Print servo-specific help information

            DynamixelWorkbench* getWorkbench();                                             // if you need to expose the workbench pointer

        private:
            DynamixelWorkbench  dxl;                // DynamixelWorkbench instance for managing servos
            const char*         log = NULL;         // Log string for debugging 
            
    };

#endif // SERVO_H
