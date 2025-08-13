#include "Servo.h"
#include "Console.h"        // Add this include for logging macros


// Constructor for Servo
Servo::Servo(){}

//initialize the DynamixelWorkbench instance
bool Servo::begin(const char* device_name, uint32_t baudrate, float protocol_version) {
    if (!setPortHandler(device_name)) return false;
    if (!setBaudrate(baudrate)) return false;
    if (!setPacketHandler(protocol_version)) return false;
    LOG_INF("DynamixelWorkbench initialized successfully");
    return true;
}

// Set the port handler with the device name
bool Servo::setPortHandler(const char *device_name) {
    
    if (!dxl.setPortHandler(device_name, &log))
    {        
        LOG_ERR(log);
        return false;  
    }       
    LOG_INF("Port handler set to: " + String(device_name));
    return true;
}

// Set the baudrate for the port handler
bool Servo::setBaudrate(uint32_t baud_rate) {     
    
    if (!dxl.setBaudrate(baud_rate, &log))
    {        
        LOG_ERR(log);
    }
    LOG_INF("Baudrate set to: "+ String(baud_rate));
    return true;
}

// Set the packet handler with the protocol version
bool Servo::setPacketHandler(float protocol_version) {
    
    if (!dxl.setPacketHandler(protocol_version, &log))
    {        
        LOG_ERR(log);
    }
    LOG_INF("Packet handler set with protocol version: " + String(protocol_version));
    return true;
}

// Get the protocol version
float Servo::getProtocolVersion(void) {
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
uint32_t Servo::getBaudrate(void) {

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
bool Servo::readRegister(uint8_t id, uint16_t address, uint16_t length, uint32_t *data) {
    
    if (!dxl.readRegister(id, address, length, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " address: " + String(address) + " length: " + String(length));
        return false;  
    }
    return true;
}

// Write a register to a servo with address and length
bool Servo::writeRegister(uint8_t id, uint16_t address, uint16_t length, uint8_t* data) {
    
    if (!dxl.writeRegister(id, address, length, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;
    }
    return true;
}

// Read a register from a servo with item name
bool Servo::readRegister(uint8_t id, const char *item_name, int32_t *data) {
    
    if (!dxl.readRegister(id, item_name, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " item name: " + String(item_name));
        return false;  
    }

    return true;
}

// Write a register to a servo with item name
bool Servo::writeRegister(uint8_t id, const char *item_name, int32_t data) {
    
    if (!dxl.writeRegister(id, item_name, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " item name: " + String(item_name) + " data: " + String(data));
        return false;  
    }
    return true;
}

// Add a sync write handler with address and length
bool Servo::addSyncWriteHandler(uint16_t address, uint16_t length) {
    
    if (!dxl.addSyncWriteHandler(address, length, &log))
    {
        LOG_ERR(log);
        LOG_ERR("address: " + String(address) + " length: " + String(length));
        return false;  
    }
    return true;
}

// Add a sync write handler with ID and item name
bool Servo::addSyncWriteHandler(uint8_t id, const char *item_name) {
    
    if (!dxl.addSyncWriteHandler(id, item_name, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " item name: " + String(item_name));
        return false;  
    }
    return true;
}

// Sync write data for a specific index
bool Servo::syncWrite(uint8_t index, int32_t *data) {
    
    if (!dxl.syncWrite(index, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("index: " + String(index) + " data: " + String(*data));
        return false;  
    }
    return true;
}

// Sync write data for multiple IDs
bool Servo::syncWrite(uint8_t index, uint8_t *id, uint8_t id_num, int32_t *data, uint8_t data_num_for_each_id) {
    
    if (!dxl.syncWrite(index, id, id_num, data, data_num_for_each_id, &log))
    {
        LOG_ERR(log);
        LOG_ERR("index: " + String(index));
        return false;  
    }
    return true;
}

//------------------------------------------------------------------------
// Ping a servo to check if it is connected, ping is required before you can use a servo.
bool Servo::ping(uint8_t dxl_id) {

    uint16_t model_number = 0;
    if (!dxl.ping(dxl_id, &model_number, &log))
    {
        LOG_ERR(log);
        return false;
    }
    return true;
}

// Get the model name of a servo
const char * Servo::getModelName(uint8_t id) {

    const char*  model_name = NULL;
    model_name = dxl.getModelName(id, &log);

    if (model_name == NULL)
    {   
        LOG_ERR(log);
        return NULL;
    }
    return model_name;
}

// Get the model number of a servo
uint16_t Servo::getModelNumber(uint8_t id) {
    uint16_t model_number = 0;
    model_number = dxl.getModelNumber(id, &log);
    if (model_number == 0)
    {
        LOG_ERR(log);
        return 0;
    }
    return model_number;
}

// Get the present position data of a servo in value
bool Servo::getPosition(uint8_t id, int32_t* pos) {
    if (!readRegister(id, "Present_Position", pos)) return false;  
    return true;
}

// Get the present velocity data of a servo in value
bool Servo::getSpeed(uint8_t id, int32_t* speed) {
    if (!readRegister(id, "Moving_Speed", speed)) return false;
    return true;
}

// Get the present load data of a servo in value
bool Servo::getLoad(uint8_t id, int32_t* load) {
    if (!readRegister(id, "Present_Load", load)) return false;
    return true;
}

// Get the present voltage data of a servo in value
bool Servo::getVoltage(uint8_t id, int32_t* voltage) {
    if (!readRegister(id, "Present_Voltage", voltage)) return false;
    return true;
}

// Get the present temperature data of a servo in value
bool Servo::getTemperature(uint8_t id, int32_t* temperature) {
    if (!readRegister(id, "Present_Temperature", temperature)) return false;
    return true;
}

// Get the angle limits for a servo
bool Servo::getAngleLimits(uint8_t id, int32_t* CW_angle, int32_t* CCW_angle) {
    if (!readRegister(id, "CW_Angle_Limit", CW_angle)) return false;
    if (!readRegister(id, "CCW_Angle_Limit", CCW_angle)) return false;
    return true;
}

// Set the goal position of a servo in value
bool Servo::setPosition(uint8_t dxl_id, int32_t position) {
    if (!writeRegister(dxl_id, "Goal_Position", position)) return false;
    return true;
}

// Set the goal speed of a servo in int value
bool Servo::setSpeed(uint8_t dxl_id, int32_t speed) {
    if (!writeRegister(dxl_id, "Moving_Speed", speed)) return false;
    return true;
}

// Set the angle limits for a servo
bool Servo::setAngleLimits(uint8_t id, int32_t CW_angle, int32_t CCW_angle) {
    if (!writeRegister(id, "CW_Angle_Limit", CW_angle)) return false;
    if (!writeRegister(id, "CCW_Angle_Limit", CCW_angle)) return false;
    return true;
}
        
// Check if a servo is currently moving
bool Servo::isMoving(uint8_t id) {
    int32_t isMoving = 0;
    if (!readRegister(id, "Moving", &isMoving)) return false;
    return isMoving;
}

// Check if the torque is enabled for a servo
bool Servo::isTorqueOn(uint8_t id) {
    int32_t isTorqueOn = 0;
    if (!readRegister(id, "Torque_Enable", &isTorqueOn)) return false;
    return isTorqueOn;
}

// Check if the LED is enabled for a servo
bool Servo::isLedOn(uint8_t id) {
    int32_t isLedOn = 0;
    if (!readRegister(id, "LED", &isLedOn)) return false;
    return isLedOn;
}

// Turn on the torque for a servo
bool Servo::torqueOn(uint8_t id) {
    if (!writeRegister(id, "Torque_Enable", 1)) return false;
    return true;
}

// Turn off the torque for a servo
bool Servo::torqueOff(uint8_t id) {
    if (!writeRegister(id, "Torque_Enable", 0)) return false;
    return true;
}

// Turn on the LED of a servo
bool Servo::ledOn(uint8_t dxl_id) {
    if (!writeRegister(dxl_id, "LED", 1)) return false;
    return true;
}

// Turn off the LED of a servo
bool Servo::ledOff(uint8_t dxl_id) {
    if (!writeRegister(dxl_id, "LED", 0)) return false;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// Initialize a servo with default settings
bool Servo::init(   uint8_t dxl_id, 
                    int32_t speed,
                    int32_t CW_angle, 
                    int32_t CCW_angle) {
    bool result =
        ping(dxl_id) &&
        setSpeed(dxl_id, speed) &&
        setAngleLimits(dxl_id, CW_angle, CCW_angle) &&
        torqueOn(dxl_id) &&
        ledOn(dxl_id);

    if (!result) {
        LOG_ERR("Failed to initialize servo with ID: " + String(dxl_id));
    } else {
        LOG_INF("Servo with ID: " + String(dxl_id) + " initialized successfully.");
    }
    return result;
}

// Process console commands for servo control
bool Servo::runConsoleCommands(const String& cmd, const String& args) {
    
    int servoId = 1;                                // Default to servo ID 1
    int arg2    = 0;                                // Default to 0

    if (args.length() > 0) {
        int spaceIndex = args.indexOf(' ');
        if (spaceIndex != -1) {
            servoId = args.substring(0, spaceIndex).toInt();
            arg2    = args.substring(spaceIndex + 1).toInt();
        } else {
            servoId = args.toInt();
        }
        
        if (servoId < 1 || servoId > 253)           // Validate servo ID
            servoId = 1;                            // Reset to default ID
    }

    if (cmd == "ss") {
        printStatus(servoId);
        return true;

    } else if (cmd == "sp") {
        PRINTLN("Servo ID " + String(servoId) + " ping: " + String(ping((uint8_t)servoId) ? "SUCCESS" : "FAILED"));
        return true;

    } else if (cmd == "sgp") {
        int32_t currentPos;
        getPosition((uint8_t)servoId, &currentPos);
        PRINTLN("Servo ID " + String(servoId) + " current position: " + String(currentPos));
        return true;
        
    } else if (cmd == "sgs") {
        int32_t currentSpeed;
        getSpeed((uint8_t)servoId, &currentSpeed);
        PRINTLN("Servo ID " + String(servoId) + " current speed: " + String(currentSpeed));
        return true;
        
    } else if (cmd == "sgl") {
        int32_t currentLoad;
        getLoad((uint8_t)servoId, &currentLoad);
        PRINTLN("Servo ID " + String(servoId) + " current load: " + String(currentLoad));
        return true;

    } else if (cmd == "sgv") {
        int32_t currentVoltage;
        getVoltage((uint8_t)servoId, &currentVoltage);
        PRINTLN("Servo ID " + String(servoId) + " current voltage: " + String(currentVoltage));
        return true;

    } else if (cmd == "sgt") {
        int32_t currentTemperature;
        getTemperature((uint8_t)servoId, &currentTemperature);
        PRINTLN("Servo ID " + String(servoId) + " current temperature: " + String(currentTemperature));
        return true;

    } else if (cmd == "sim") {
        PRINTLN("Servo ID " + String(servoId) + " is " + String(isMoving((uint8_t)servoId) ? "MOVING" : "NOT MOVING"));
        return true;

    } else if (cmd == "sit") {
        PRINTLN("Servo ID " + String(servoId) + " torque is " + String(isTorqueOn((uint8_t)servoId) ? "ENABLED" : "DISABLED"));
        return true;

    } else if (cmd == "sil") {
        PRINTLN("Servo ID " + String(servoId) + " LED is " + String(isLedOn((uint8_t)servoId) ? "ON" : "OFF"));
        return true;

    } else if (cmd == "ssp") {
        if (arg2 < 0 || arg2 > 1023)                // Validate position (typical range for AX-12A is 0-1023)
            arg2 = 512;                             // Center position
        setPosition((uint8_t)servoId, (int32_t)arg2);
        PRINTLN("Servo ID " + String(servoId) + " position set to " + String(arg2));
        return true;

    } else if (cmd == "sss") {
        if (arg2 < 0 || arg2 > 1023)                // Validate speed (typical range is 0-1023)
            arg2 = 100;                             // default speed
        setSpeed((uint8_t)servoId, (int32_t)arg2);
        PRINTLN("Servo ID " + String(servoId) + " speed set to " + String(arg2));
        return true;
        
    } else if (cmd == "ston") {
        
        bool result = torqueOn((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " torque " + String(result ? "ENABLED" : "FAILED"));
        return true;

    } else if (cmd == "stoff") {
        
        bool result = torqueOff((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " torque " + String(result ? "DISABLED" : "FAILED"));
        return true;
        
    } else if (cmd == "slon") {
        
        bool result = ledOn((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " LED " + String(result ? "ON" : "FAILED"));
        return true;

    } else if (cmd == "sloff") {
        
        bool result = ledOff((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " LED " + String(result ? "OFF" : "FAILED"));
        return true;

    } else if (cmd == "s?") {
        printConsoleHelp();
        return true;
    }
    
    return false;
}

// Print the status of a servo for debugging
bool Servo::printStatus(uint8_t id) {
    
    int32_t position = 0;
    int32_t speed = 0;
    int32_t load = 0;
    int32_t voltage = 0;
    int32_t temperature = 0;
    int32_t CW_angle = 0, CCW_angle = 0;

    getPosition(id, &position);
    getAngleLimits(id, &CW_angle, &CCW_angle);
    getSpeed(id, &speed);
    getLoad(id, &load);
    getVoltage(id, &voltage);
    getTemperature(id, &temperature);

    PRINTLN("\nServo Status:");
    PRINTLN("Servo ID     : " + String(id));                         // Print the ID of the servo
    PRINTLN("Model Number : " + String(getModelNumber(id)));
    PRINTLN("Model Name   : " + String(getModelName(id)));
    PRINTLN("Position     : " + String(position));
    PRINTLN("Angle Limits : CW " + String(CW_angle) + " ~ CCW " + String(CCW_angle));
    PRINTLN("Speed        : " + String(speed));
    PRINTLN("Load         : " + String(load));
    PRINTLN("Voltage      : " + String(voltage));
    PRINTLN("Temperature  : " + String(temperature));
    PRINTLN("Torque       : " + String(isTorqueOn(id) ? "Enabled" : "Disabled"));
    PRINTLN("Moving       : " + String(isMoving(id) ? "YES" : "NO"));
    PRINTLN("LED          : " + String(isLedOn(id) ? "ON" : "OFF"));
    return true;                                                    // Return true to indicate successful status print
}

// Print servo-specific help information
bool Servo::printConsoleHelp() {
    PRINTLN("Servo Commands:");
    PRINTLN("  ss [id]         - Show servo status (default id=1)");
    PRINTLN("  sp [id]         - Ping servo (default id=1)");
    PRINTLN("");
    PRINTLN("  sgp [id]        - Get servo position (default id=1)");
    PRINTLN("  sgs [id]        - Get servo speed (default id=1)");
    PRINTLN("  sgl [id]        - Get servo load (default id=1)");
    PRINTLN("  sgv [id]        - Get servo voltage (default id=1)");
    PRINTLN("  sgt [id]        - Get servo temperature (default id=1)");
    PRINTLN("");
    PRINTLN("  sim [id]        - Check is servo moving (default id=1)");
    PRINTLN("  sit [id]        - Check is servo torque enabled (default id=1)");
    PRINTLN("  sil [id]        - Check is servo LED enabled (default id=1)");
    PRINTLN("");
    PRINTLN("  ssp [id] [pos]  - Set servo position (default id=1, pos=512)");
    PRINTLN("  sss [id] [spd]  - Set servo speed (default id=1, spd=100)");
    PRINTLN("");
    PRINTLN("  ston [id]       - Enable servo torque (default id=1)");
    PRINTLN("  stoff [id]      - Disable servo torque (default id=1)");
    PRINTLN("  slon [id]       - Turn on servo LED (default id=1)");
    PRINTLN("  sloff [id]      - Turn off servo LED (default id=1)");
    PRINTLN("");
    PRINTLN("  s?              - Show this help message");
    PRINTLN("");
    return true;
}

// Return the DynamixelWorkbench instance
DynamixelWorkbench* Servo::getWorkbench()    {
    return &dxl;
}

// End of Servo.cpp