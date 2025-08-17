#include "Servo.h"
#include "Log.h"
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
bool Servo::readRegister(uint8_t id, const char *item_name, uint32_t *data) {

    if (!dxl.readRegister(id, item_name, (int32_t*)data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id) + " item name: " + String(item_name));
        return false;  
    }

    return true;
}

// Write a register to a servo with item name
bool Servo::writeRegister(uint8_t id, const char *item_name, uint32_t data) {
    
    if (!dxl.writeRegister(id, item_name, (int32_t)data, &log))
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
    
    if (!dxl.ping(dxl_id, &log))
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

// ------------------ EPROM Area -----------------------------


// Get the model number of a servo
bool Servo::getModelNumber(uint8_t id, uint16_t* model_number) {
    if (!readRegister(id, "Model_Number", (uint32_t*)model_number)) return false;
    return true;
}

// Get Firmware Version
bool Servo::getFirmwareVersion(uint8_t id, uint8_t* version) {
    if (!readRegister(id, "Firmware_Version", (uint32_t*)version)) return false;
    return true;
}

// Get Baud Rate
bool Servo::getBaudRate(uint8_t id, uint8_t* baud_rate) {
    if (!readRegister(id, "Baud_Rate", (uint32_t*)baud_rate)) return false;
    return true;
}

// Get Return Delay Time
bool Servo::getReturnDelayTime(uint8_t id, uint8_t* return_delay_time) {
    if (!readRegister(id, "Return_Delay_Time", (uint32_t*)return_delay_time)) return false;
    return true;
}

// Get the angle limits for a servo
bool Servo::getAngleLimits(uint8_t id, uint16_t* CW_angle, uint16_t* CCW_angle) {
    if (!readRegister(id, "CW_Angle_Limit", (uint32_t*)CW_angle)) return false;
    if (!readRegister(id, "CCW_Angle_Limit", (uint32_t*)CCW_angle)) return false;
    return true;
}

// Set the angle limits for a servo
bool Servo::setAngleLimits(uint8_t id, uint16_t CW_angle, uint16_t CCW_angle) {
    if (!writeRegister(id, "CW_Angle_Limit", (uint32_t)CW_angle)) return false;
    if (!writeRegister(id, "CCW_Angle_Limit", (uint32_t)CCW_angle)) return false;
    return true;
}

// Get Temperature Limit
bool Servo::getTemperatureLimit(uint8_t id, uint8_t* max_temp) {
    if (!readRegister(id, "Temperature_Limit", (uint32_t*)max_temp)) return false;
    return true;
}

// Get Voltage Limit
bool Servo::getVoltageLimit(uint8_t id, uint8_t* min_voltage, uint8_t* max_voltage) {
    if (!readRegister(id, "Voltage_Limit_Min", (uint32_t*)min_voltage)) return false;
    if (!readRegister(id, "Voltage_Limit_Max", (uint32_t*)max_voltage)) return false;
    return true;
}

// Get Max Torque
bool Servo::getMaxTorque(uint8_t id, uint16_t* max_torque) {
    if (!readRegister(id, "Max_Torque", (uint32_t*)max_torque)) return false;
    return true;
}

// Get Status Return Level
bool Servo::getStatusReturnLevel(uint8_t id, uint8_t* level) {
    if (!readRegister(id, "Status_Return_Level", (uint32_t*)level)) return false;
    return true;
}

    // Get Alarm LED
bool Servo::getAlarmLED(uint8_t id, uint8_t* alarm_led) {
    if (!readRegister(id, "Alarm_LED", (uint32_t*)alarm_led)) return false;
    return true;
}

// Get Shutdown
bool Servo::getShutdown(uint8_t id, uint8_t* shutdown) {
    if (!readRegister(id, "Shutdown", (uint32_t*)shutdown)) return false;
    return true;
}

// ------------------ RAM Area -----------------------------
// Check if the torque is enabled for a servo
bool Servo::isTorqueOn(uint8_t id) {
    uint32_t isTorqueOn = 0;
    if (!readRegister(id, "Torque_Enable", &isTorqueOn)) return false;
    return isTorqueOn;
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

// Check if the LED is enabled for a servo
bool Servo::isLedOn(uint8_t id) {
    uint32_t isLedOn = 0;
    if (!readRegister(id, "LED", &isLedOn)) return false;
    return isLedOn;
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

// Get the compliance margin of a servo
bool Servo::getComplianceMargin(uint8_t id, uint8_t* CW_margin, uint8_t* CCW_margin) {
    if (!readRegister(id, "CW_Compliance_Margin", (uint32_t*)CW_margin)) return false;
    if (!readRegister(id, "CCW_Compliance_Margin", (uint32_t*)CCW_margin)) return false;
    return true;
}

// Set the compliance margin of a servo
bool Servo::setComplianceMargin(uint8_t id, uint8_t CW_margin, uint8_t CCW_margin) {
    if (!writeRegister(id, "CW_Compliance_Margin", (uint32_t)CW_margin)) return false;
    if (!writeRegister(id, "CCW_Compliance_Margin", (uint32_t)CCW_margin)) return false;
    return true;
}

// Get the compliance slope of a servo
bool Servo::getComplianceSlope(uint8_t id, uint8_t* CW_slope, uint8_t* CCW_slope) {
    if (!readRegister(id, "CW_Compliance_Slope", (uint32_t*)CW_slope)) return false;
    if (!readRegister(id, "CCW_Compliance_Slope", (uint32_t*)CCW_slope)) return false;
    return true;
}

// Set the compliance slope of a servo
bool Servo::setComplianceSlope(uint8_t id, uint8_t CW_slope, uint8_t CCW_slope) {
    if (!writeRegister(id, "CW_Compliance_Slope", (uint32_t)CW_slope)) return false;
    if (!writeRegister(id, "CCW_Compliance_Slope", (uint32_t)CCW_slope)) return false;
    return true;
}

// Set the goal position of a servo in value
bool Servo::setGoalPosition(uint8_t dxl_id, int16_t position) {
    if (!writeRegister(dxl_id, "Goal_Position", (uint32_t)position)) return false;
    return true;
}

// Set the goal speed of a servo in int value
bool Servo::setGoalSpeed(uint8_t dxl_id, int16_t speed) {
    if (!writeRegister(dxl_id, "Moving_Speed", (uint32_t)speed)) return false;
    return true;
}

// Get the torque limit of a servo
bool Servo::getTorqueLimit(uint8_t id, uint16_t* torque_limit) {
    if (!readRegister(id, "Torque_Limit", (uint32_t*)torque_limit)) return false;
    return true;
}

// Set the torque limit of a servo
bool Servo::setTorqueLimit(uint8_t id, uint16_t torque_limit) {
    if (!writeRegister(id, "Torque_Limit", (uint32_t)torque_limit)) return false;
    return true;
}

// Get the present position data of a servo in value
bool Servo::getPresentPosition(uint8_t id, uint16_t* pos) {
    if (!readRegister(id, "Present_Position", (uint32_t*)pos)) return false;
    return true;
}

// Get the present velocity data of a servo in value
bool Servo::getPresentSpeed(uint8_t id, uint16_t* speed) {
    if (!readRegister(id, "Moving_Speed", (uint32_t*)speed)) return false;
    return true;
}

// Get the present load data of a servo in value
bool Servo::getPresentLoad(uint8_t id, uint16_t* load) {
    if (!readRegister(id, "Present_Load", (uint32_t*)load)) return false;
    return true;
}

// Get the present voltage data of a servo in value
bool Servo::getPresentVoltage(uint8_t id, uint8_t* voltage) {
    if (!readRegister(id, "Present_Voltage", (uint32_t*)voltage)) return false;
    return true;
}

// Get the present temperature data of a servo in value
bool Servo::getPresentTemperature(uint8_t id, uint8_t* temperature) {
    if (!readRegister(id, "Present_Temperature", (uint32_t*)temperature)) return false;
    return true;
}

// Check if a servo is currently moving
bool Servo::isMoving(uint8_t id) {
    uint32_t isMoving = 0;
    if (!readRegister(id, "Moving", &isMoving)) return false;
    return isMoving;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
// Initialize a servo with default settings
bool Servo::init(   uint8_t dxl_id, 
                    int32_t speed,
                    int32_t CW_angle, 
                    int32_t CCW_angle) {
    bool result =
        ping(dxl_id) &&
        setGoalSpeed(dxl_id, speed) &&
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

bool Servo::update(uint8_t id) {
    // Update the state of the servo
    return true;
}

// Process console commands for servo control
bool Servo::runConsoleCommands(const String& cmd, const String& args) {
    
    int id      = 1;                                // Default servo ID to 1
    int arg2    = 0;                                // Default argument2 to 0
    int arg3    = 0;                                // Default argument3 to 0

    if (args.length() > 0) {
        int spaceIndex1 = args.indexOf(' ');        
        if (spaceIndex1 != -1) {
            id = args.substring(0, spaceIndex1).toInt();
            int spaceIndex2 = args.indexOf(' ', spaceIndex1 + 1);
            if (spaceIndex2 == -1) {
                arg2 = args.substring(spaceIndex1 + 1).toInt();
            } else {
                arg2 = args.substring(spaceIndex1 + 1, spaceIndex2).toInt();
                arg3 = args.substring(spaceIndex2 + 1).toInt();
            }
        } else {
            id = args.toInt();
            if (id < 1 || id > 253)           // Validate servo ID
                id = 1;                       // Reset to default ID
        }
    }

    if (cmd == "ss") {
        if (arg2 > 0 && arg2 > id && arg2 < 253) {
            for (int i = id; i <= arg2; i++) {
                printStatus(i);
            }
        } else {
            printStatus(id);
        }
        return true;

    } else if (cmd == "sp") {
        if (arg2 > 0 && arg2 > id && arg2 < 253) {
            for (int i = id; i <= arg2; i++) {
                PRINTLN("Servo ID " + String(i) + " ping: " + String(ping((uint8_t)i) ? "SUCCESS" : "FAILED"));
            }
        } else {
            PRINTLN("Servo ID " + String(id) + " ping: " + String(ping((uint8_t)id) ? "SUCCESS" : "FAILED"));
        }
        return true;

    } else if (cmd == "sgal") {
        uint16_t CW_angle, CCW_angle;
        getAngleLimits((uint8_t)id, &CW_angle, &CCW_angle);
        PRINTLN("Servo ID " + String(id) + " angle limits: CW " + String(CW_angle) + " ~ CCW " + String(CCW_angle));
        return true;

    } else if (cmd == "sgp") {
        uint16_t presentPos;
        getPresentPosition((uint8_t)id, &presentPos);
        PRINTLN("Servo ID " + String(id) + " current position: " + String(presentPos));
        return true;
        
    } else if (cmd == "sgs") {
        uint16_t presentSpeed;
        getPresentSpeed((uint8_t)id, &presentSpeed);
        PRINTLN("Servo ID " + String(id) + " current speed: " + String(presentSpeed));
        return true;
        
    } else if (cmd == "sgl") {
        uint16_t presentLoad;
        getPresentLoad((uint8_t)id, &presentLoad);
        PRINTLN("Servo ID " + String(id) + " current load: " + String(presentLoad));
        return true;

    } else if (cmd == "sgv") {
        uint8_t presentVoltage;
        getPresentVoltage((uint8_t)id, &presentVoltage);
        PRINTLN("Servo ID " + String(id) + " current voltage: " + String(presentVoltage));
        return true;

    } else if (cmd == "sgt") {
        uint8_t presentTemperature;
        getPresentTemperature((uint8_t)id, &presentTemperature);
        PRINTLN("Servo ID " + String(id) + " current temperature: " + String(presentTemperature));
        return true;

    } else if (cmd == "sim") {
        PRINTLN("Servo ID " + String(id) + " is " + String(isMoving((uint8_t)id) ? "MOVING" : "NOT MOVING"));
        return true;

    } else if (cmd == "sit") {
        PRINTLN("Servo ID " + String(id) + " torque is " + String(isTorqueOn((uint8_t)id) ? "ENABLED" : "DISABLED"));
        return true;

    } else if (cmd == "sil") {
        PRINTLN("Servo ID " + String(id) + " LED is " + String(isLedOn((uint8_t)id) ? "ON" : "OFF"));
        return true;

    } else if (cmd == "ssp") {
        if (arg2 < 0 || arg2 > 1023)                // Validate position (typical range for AX is 0-1023)
            arg2 = 512;                             // Center position
        setGoalPosition((uint8_t)id, (int32_t)arg2);
        PRINTLN("Servo ID " + String(id) + " position set to " + String(arg2));
        return true;

    } else if (cmd == "sss") {
        if (arg2 < 0 || arg2 > 1023)                // Validate speed (typical range is 0-1023)
            arg2 = 100;                             // default speed
        setGoalSpeed((uint8_t)id, (int32_t)arg2);
        PRINTLN("Servo ID " + String(id) + " speed set to " + String(arg2));
        return true;

    } else if (cmd == "ssal") {
        if (arg2 < 0 || arg2 > 1023)                // Validate ANGLE (typical range is 0-1023)
            arg2 = 412;                             // default CW angle
        if (arg3 < 0 || arg3 > 1023)                // Validate ANGLE (typical range is 0-1023)
            arg3 = 612;                             // default CCW angle

        setAngleLimits((uint8_t)id, (int32_t)arg2, (int32_t)arg3);
        PRINTLN("Servo ID " + String(id) + " angle limits: CW " + String(arg2) + " ~ CCW " + String(arg3));
        return true;

    } else if (cmd == "ston") {
        
        bool result = torqueOn((uint8_t)id);
        PRINTLN("Servo ID " + String(id) + " torque " + String(result ? "ENABLED" : "FAILED"));
        return true;

    } else if (cmd == "stoff") {
        
        bool result = torqueOff((uint8_t)id);
        PRINTLN("Servo ID " + String(id) + " torque " + String(result ? "DISABLED" : "FAILED"));
        return true;
        
    } else if (cmd == "slon") {
        
        bool result = ledOn((uint8_t)id);
        PRINTLN("Servo ID " + String(id) + " LED " + String(result ? "ON" : "FAILED"));
        return true;

    } else if (cmd == "sloff") {
        
        bool result = ledOff((uint8_t)id);
        PRINTLN("Servo ID " + String(id) + " LED " + String(result ? "OFF" : "FAILED"));
        return true;

    } else if (cmd == "s?") {
        printConsoleHelp();
        return true;
    }
    
    return false;
}

// Print the status of a servo for debugging
bool Servo::printStatus(uint8_t id) {

    uint16_t model_number = 0;
    uint16_t position = 0;
    uint16_t speed = 0;
    uint16_t load = 0;
    uint8_t voltage = 0;
    uint8_t temperature = 0;
    uint16_t CW_angle = 0, CCW_angle = 0;

    getModelNumber(id, &model_number);
    getPresentPosition(id, &position);
    getAngleLimits(id, &CW_angle, &CCW_angle);
    getPresentSpeed(id, &speed);
    getPresentLoad(id, &load);
    getPresentVoltage(id, &voltage);
    getPresentTemperature(id, &temperature);

    PRINTLN("\nServo Status:");
    PRINTLN("Servo ID     : " + String(id));                         // Print the ID of the servo
    PRINTLN("Model Number : " + String(model_number));
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
    PRINTLN("  ss [id] [id]         - Show servo status for range of id's (default id=1)");
    PRINTLN("  sp [id] [id]         - Ping servo for range of id's (default id=1)");
    PRINTLN("");
    PRINTLN("  sgp [id]             - Get servo position (default id=1)");
    PRINTLN("  sgs [id]             - Get servo speed (default id=1)");
    PRINTLN("  sgal [id]            - Get servo angle limits (default id=1)");
    PRINTLN("  sgl [id]             - Get servo load (default id=1)");
    PRINTLN("  sgv [id]             - Get servo voltage (default id=1)");
    PRINTLN("  sgt [id]             - Get servo temperature (default id=1)");
    PRINTLN("");
    PRINTLN("  sim [id]             - Check is servo moving (default id=1)");
    PRINTLN("  sit [id]             - Check is servo torque enabled (default id=1)");
    PRINTLN("  sil [id]             - Check is servo LED enabled (default id=1)");
    PRINTLN("");
    PRINTLN("  ssp [id] [pos]       - Set servo position (default id=1, pos=512)");
    PRINTLN("  sss [id] [spd]       - Set servo speed (default id=1, spd=100)");
    PRINTLN("  ssal [id] [cw] [ccw] - Set servo angle limits (default id=1, cw=0, ccw=1023)");
    PRINTLN("");
    PRINTLN("  ston [id]            - Enable servo torque (default id=1)");
    PRINTLN("  stoff [id]           - Disable servo torque (default id=1)");
    PRINTLN("  slon [id]            - Turn on servo LED (default id=1)");
    PRINTLN("  sloff [id]           - Turn off servo LED (default id=1)");
    PRINTLN("");
    PRINTLN("  s?                   - Show this help message");
    PRINTLN("");
    return true;
}

// Return the DynamixelWorkbench instance
DynamixelWorkbench* Servo::getWorkbench()    {
    return &dxl;
}
// End of Servo.cpp