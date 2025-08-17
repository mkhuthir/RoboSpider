#include "Driver.h"
#include "Console.h"
#include "Log.h"

// Constructor for Servo
Driver::Driver(){}

//initialize the DynamixelWorkbench instance
bool Driver::begin(const char* device_name, uint32_t baudrate, float protocol_version) {
    if (!setPortHandler(device_name)) return false;
    if (!setBaudrate(baudrate)) return false;
    if (!setPacketHandler(protocol_version)) return false;
    LOG_INF("DynamixelWorkbench initialized successfully");
    return true;
}

// Set the port handler with the device name
bool Driver::setPortHandler(const char *device_name) {

    if (!dxl.setPortHandler(device_name, &log))
    {        
        LOG_ERR(log);
        return false;  
    }       
    LOG_INF("Port handler set to: " + String(device_name));
    return true;
}

// Set the baudrate for the port handler
bool Driver::setBaudrate(uint32_t baud_rate) {

    if (!dxl.setBaudrate(baud_rate, &log))
    {        
        LOG_ERR(log);
    }
    LOG_INF("Baudrate set to: "+ String(baud_rate));
    return true;
}

// Set the packet handler with the protocol version
bool Driver::setPacketHandler(float protocol_version) {

    if (!dxl.setPacketHandler(protocol_version, &log))
    {        
        LOG_ERR(log);
    }
    LOG_INF("Packet handler set with protocol version: " + String(protocol_version));
    return true;
}

// Get the protocol version
float Driver::getProtocolVersion(void) {
    float protocol_version = dxl.getProtocolVersion();
    if (protocol_version < 0)
    {        
        LOG_ERR("Failed to get protocol version!");
        return -1.0;
    }
    LOG_INF("Protocol Version: " + String(protocol_version));
    return protocol_version;
}

// Get the baudrate
uint32_t Driver::getBaudrate(void) {

    uint32_t baudrate = dxl.getBaudrate();

    if (baudrate == 0)
    {        
        LOG_ERR("Failed to get baudrate!");
        return 0;
    }
    return baudrate;
}

//-----------------------------------------------------------------------------

// Read a register from a servo with address and length
bool Driver::readRegister(uint8_t id, uint16_t address, uint16_t length, uint32_t *data) {

    if (!dxl.readRegister(id, address, length, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " address: " + String(address) + " length: " + String(length));
        return false;  
    }
    return true;
}

// Write a register to a servo with address and length
bool Driver::writeRegister(uint8_t id, uint16_t address, uint16_t length, uint8_t* data) {

    if (!dxl.writeRegister(id, address, length, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;
    }
    return true;
}

// Read a register from a servo with item name
bool Driver::readRegister(uint8_t id, const char *item_name, uint32_t *data) {

    if (!dxl.readRegister(id, item_name, (int32_t*)data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " item name: " + String(item_name));
        return false;  
    }

    return true;
}

// Write a register to a servo with item name
bool Driver::writeRegister(uint8_t id, const char *item_name, uint32_t data) {

    if (!dxl.writeRegister(id, item_name, (int32_t)data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " item name: " + String(item_name) + " data: " + String(data));
        return false;  
    }
    return true;
}

// Add a sync write handler with address and length
bool Driver::addSyncWriteHandler(uint16_t address, uint16_t length) {

    if (!dxl.addSyncWriteHandler(address, length, &log))
    {
        LOG_ERR(log);
        LOG_ERR("address: " + String(address) + " length: " + String(length));
        return false;  
    }
    return true;
}

// Add a sync write handler with ID and item name
bool Driver::addSyncWriteHandler(uint8_t id, const char *item_name) {

    if (!dxl.addSyncWriteHandler(id, item_name, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " item name: " + String(item_name));
        return false;  
    }
    return true;
}

// Sync write data for a specific index
bool Driver::syncWrite(uint8_t index, int32_t *data) {

    if (!dxl.syncWrite(index, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("index: " + String(index) + " data: " + String(*data));
        return false;  
    }
    return true;
}

// Sync write data for multiple IDs
bool Driver::syncWrite(uint8_t index, uint8_t *id, uint8_t id_num, int32_t *data, uint8_t data_num_for_each_id) {

    if (!dxl.syncWrite(index, id, id_num, data, data_num_for_each_id, &log))
    {
        LOG_ERR(log);
        LOG_ERR("index: " + String(index));
        return false;  
    }
    return true;
}

//-----------------------------------------------------------------------------

bool Driver::ping(uint8_t dxl_id) {

    if (!dxl.ping(dxl_id, &log))
    {
        LOG_ERR(log);
        return false;
    }
    return true;
}

// Get the model name of a servo
const char * Driver::getModelName(uint8_t id) {

    const char*  model_name = NULL;
    model_name = dxl.getModelName(id, &log);

    if (model_name == NULL)
    {   
        LOG_ERR(log);
        return NULL;
    }
    return model_name;
}

// end of DXLDriver.cpp
