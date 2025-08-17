#ifndef DRIVER_H
#define DRIVER_H

    #include <DynamixelWorkbench.h>

    class Driver {
        public:
            Driver();
//---------------------------------------------------------------------------------------------------------------------------------------------------            
            bool                begin(  const char* device_name,                            // initialize the controller with device name and baudrate
                                        uint32_t baudrate,
                                        float protocol_version);          
                                        
            bool                setPortHandler(const char *device_name);                    // set the port handler for the controller  
            bool                setBaudrate(uint32_t baud_rate);                            // set the baudrate for the controller
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

            DynamixelWorkbench* getWorkbench();                                             // if you need to expose the workbench pointer
//---------------------------------------------------------------------------------------------------------------------------------------------------

            bool                ping(uint8_t dxl_id);                                       // ping a servo to check if it is connected
            const char *        getModelName(uint8_t id);                                   // get the model name of a servo by its ID

//---------------------------------------------------------------------------------------------------------------------------------------------------
        private:
            const char*         log = NULL;         // Log string for debugging 
            DynamixelWorkbench  dxl;                // DynamixelWorkbench instance for managing servos
    };
#endif