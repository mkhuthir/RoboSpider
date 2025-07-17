#ifndef SERVO_H
#define SERVO_H

    #define PROTOCOL_VERSION 1.0 // Protocol version for Dynamixel servos

    #include <DynamixelWorkbench.h> // Include DynamixelWorkbench for managing servos

    class Servo {
        public:
            Servo();
            bool            begin(const char* device_name, uint32_t baudrate);  // initialize the controller with device name and baudrate
            
            bool            setPortHandler(const char *device_name, const char **log = NULL);
            bool            setBaudrate(uint32_t baud_rate, const char **log = NULL);
            bool            setPacketHandler(float protocol_version, const char **log = NULL);

            float           getProtocolVersion(void);
            uint32_t        getBaudrate(void);
            const char *    getModelName(uint8_t id, const char **log = NULL);
            uint16_t        getModelNumber(uint8_t id, const char **log = NULL);

            bool            ping(uint8_t dxl_id);                               // ping a servo to check if it is connected
            bool            jointMode(uint8_t dxl_id);                          // set a servo to joint mode
            bool            init(uint8_t dxl_id);                               // initialize a servo with default settings

            bool            goalPosition(uint8_t dxl_id, int32_t position);     // set the goal position of a servo
            bool            goalVelocity(uint8_t dxl_id, int32_t velocity);     // set the goal velocity of a servo

            bool            LEDOn(uint8_t dxl_id);                         // turn on the LED of a servo
            bool            LEDOff(uint8_t dxl_id);                        // turn off the LED of a servo

            DynamixelWorkbench* getWorkbench();                         // if you need to expose the workbench pointer

        private:
            DynamixelWorkbench  dxl;                // DynamixelWorkbench instance for managing servos
            const char          *log;               // Log string for debugging 
            bool                result = false;     // Result of operations
            uint16_t            model_number = 0;   // Model number of the servo being operated on
        

    };

#endif // SERVO_H
