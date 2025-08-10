#include "Servo.h"
#include "Console.h"        // Add this include for logging macros


// Constructor for Servo
Servo::Servo(){}

//initialize the DynamixelWorkbench instance
bool Servo::begin(const char* device_name, uint32_t baudrate) {
    
    if (!dxl.init(device_name, baudrate, &log)) {
        LOG_ERR(log);
        return false;  
    } else {
        LOG_INF("DynamixelWorkbench initialized successfully at baudrate: "+ String(baudrate)+ " bps");
    }

    if (!setPacketHandler(PROTOCOL_VERSION))
        return false;  
    
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
// Ping a servo to check if it is connected
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

    if (!dxl.getPresentPositionData(id, pos, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;
}

// Get the present velocity data of a servo in value
bool Servo::getSpeed(uint8_t id, int32_t* speed) {

    if (!dxl.getPresentVelocityData(id, speed, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;
}

// Get the present load data of a servo in value
bool Servo::getLoad(uint8_t id, int32_t* load) {
    if (!readRegister(id, "Present_Load", load))
        return false;
    return true;
}

// Get the present voltage data of a servo in value
bool Servo::getVoltage(uint8_t id, int32_t* voltage) {
    if (!readRegister(id, "Present_Voltage", voltage))
        return false;
    return true;
}

// Get the present temperature data of a servo in value
bool Servo::getTemperature(uint8_t id, int32_t* temperature) {
    if (!readRegister(id, "Present_Temperature", temperature))
        return false;
    return true;
}

// Set a servo to joint mode
bool Servo::setJointMode(uint8_t dxl_id) {
    
    if (!dxl.jointMode(dxl_id, 0, 0, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;  
    }
    return true;
}

// Set the goal position of a servo in value
bool Servo::setPosition(uint8_t dxl_id, int32_t position) {

    if (!dxl.goalPosition(dxl_id, position, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;  
    }
    return true;
    
}

// Set the goal speed of a servo in int value
bool Servo::setSpeed(uint8_t dxl_id, int32_t speed) {

    if (!dxl.goalVelocity(dxl_id, speed, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;
    }
    return true;
}

// Check if a servo is currently moving
bool Servo::isMoving(uint8_t id) {
    int32_t isMoving = 0;
    if (!readRegister(id, "Moving", &isMoving))
        return false;
    return isMoving;
}

// Turn on the torque for a servo
bool Servo::torqueOn(uint8_t id) {
    
    if (!dxl.torqueOn(id, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;
}

// Turn off the torque for a servo
bool Servo::torqueOff(uint8_t id) {
    
    if (!dxl.torqueOff(id, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;
}

// Turn on the LED of a servo
bool Servo::ledOn(uint8_t dxl_id) {
    
    if (!dxl.ledOn(dxl_id, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;  
    }
    return true;
}

// Turn off the LED of a servo
bool Servo::ledOff(uint8_t dxl_id) {
    
    if (!dxl.ledOff(dxl_id, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;  
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
// Initialize a servo with default settings
bool Servo::init(uint8_t dxl_id, int32_t speed) {
    bool result =
        ping(dxl_id) &&
        setJointMode(dxl_id) &&
        torqueOn(dxl_id) &&
        setSpeed(dxl_id, speed) &&
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
    if (cmd == "ss") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) { // Valid Dynamixel ID range
                servoId = parsedId;
            }
        }
              
        if (!printStatus(servoId)) {
            LOG_ERR("Failed to get status for servo ID " + String(servoId));
            return false;  // Return false if status retrieval fails
        }
        return true;

    } else if (cmd == "sp") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }
        
        bool result = ping((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " ping: " + String(result ? "SUCCESS" : "FAILED"));
        return true;

    } else if (cmd == "sgp") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }
        
        int32_t currentPos;
        bool result = getPosition((uint8_t)servoId, &currentPos);
        if (result) {
            PRINTLN("Servo ID " + String(servoId) + " current position: " + String(currentPos));
        } else {
            LOG_ERR("Failed to read position from servo ID " + String(servoId));
        }
        return true;
        
    } else if (cmd == "sgs") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }

        int32_t currentSpeed;
        bool result = getSpeed((uint8_t)servoId, &currentSpeed);
        if (result) {
            PRINTLN("Servo ID " + String(servoId) + " current speed: " + String(currentSpeed));
        } else {
            LOG_ERR("Failed to read speed from servo ID " + String(servoId));
        }
        return true;
        
    } else if (cmd == "sgl") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }

        int32_t currentLoad;
        bool result = getLoad((uint8_t)servoId, &currentLoad);
        if (result) {
            PRINTLN("Servo ID " + String(servoId) + " current load: " + String(currentLoad));
        } else {
            LOG_ERR("Failed to read load from servo ID " + String(servoId));
        }
        return true;

    } else if (cmd == "sgv") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }

        int32_t currentVoltage;
        bool result = getVoltage((uint8_t)servoId, &currentVoltage);
        if (result) {
            PRINTLN("Servo ID " + String(servoId) + " current voltage: " + String(currentVoltage));
        } else {
            LOG_ERR("Failed to read voltage from servo ID " + String(servoId));
        }
        return true;

    } else if (cmd == "sgt") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }

        int32_t currentTemperature;
        bool result = getTemperature((uint8_t)servoId, &currentTemperature);
        if (result) {
            PRINTLN("Servo ID " + String(servoId) + " current temperature: " + String(currentTemperature));
        } else {
            LOG_ERR("Failed to read temperature from servo ID " + String(servoId));
        }
        return true;

    } else if (cmd == "ssp") {
        int servoId = 1; // Default to servo ID 1
        int position = 2048; // Default to center position
        
        if (args.length() > 0) {
            int spaceIndex = args.indexOf(' ');
            if (spaceIndex != -1) {
                servoId = args.substring(0, spaceIndex).toInt();
                position = args.substring(spaceIndex + 1).toInt();
            } else {
                servoId = args.toInt();
            }
            
            // Validate servo ID
            if (servoId < 1 || servoId > 253) {
                servoId = 1;
            }

            // Validate position (typical range for AX-12A is 0-1023)
            if (position < 0 || position > 1023) {
                position = 512; // Center position
            }
        }
        
        bool result = setPosition((uint8_t)servoId, (int32_t)position);
        PRINTLN("Servo ID " + String(servoId) + " position set to " + String(position) + ": " + String(result ? "SUCCESS" : "FAILED"));
        return true;

    } else if (cmd == "sss") {
        int servoId = 1; // Default to servo ID 1
        int speed = 100; // Default speed

        if (args.length() > 0) {
            int spaceIndex = args.indexOf(' ');
            if (spaceIndex != -1) {
                servoId = args.substring(0, spaceIndex).toInt();
                speed = args.substring(spaceIndex + 1).toInt();
            } else {
                servoId = args.toInt();
            }
            
            // Validate servo ID
            if (servoId < 1 || servoId > 253) {
                servoId = 1;
            }

            // Validate speed (typical range for AX-12A is 0-1023)
            if (speed < 0 || speed > 1023) {
                speed = 100;
            }
        }

        bool result = setSpeed((uint8_t)servoId, (int32_t)speed);
        PRINTLN("Servo ID " + String(servoId) + " speed set to " + String(speed) + ": " + String(result ? "SUCCESS" : "FAILED"));
        return true;
        
    } else if (cmd == "ston") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }
        
        bool result = torqueOn((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " torque " + String(result ? "ENABLED" : "FAILED"));
        return true;

    } else if (cmd == "stoff") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }
        
        bool result = torqueOff((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " torque " + String(result ? "DISABLED" : "FAILED"));
        return true;
        
    } else if (cmd == "slon") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }
        
        bool result = ledOn((uint8_t)servoId);
        PRINTLN("Servo ID " + String(servoId) + " LED " + String(result ? "ON" : "FAILED"));
        return true;

    } else if (cmd == "sloff") {
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }
        
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
    int32_t torque_status = 0;
    int32_t moving = 0;
    int32_t led_status = 0;
    int32_t load = 0;
    int32_t voltage = 0;
    int32_t temperature = 0;

    getPosition(id, &position);
    getSpeed(id, &speed);
    getLoad(id, &load);
    getVoltage(id, &voltage);
    getTemperature(id, &temperature);

    readRegister(id, "Torque_Enable", &torque_status);
    readRegister(id, "Moving", &moving);
    readRegister(id, "LED", &led_status);


    PRINTLN("\nServo Status:");
    PRINTLN("Servo ID     : " + String(id));                         // Print the ID of the servo
    PRINTLN("Model Number : " + String(getModelNumber(id)));
    PRINTLN("Model Name   : " + String(getModelName(id)));
    PRINTLN("Position     : " + String(position));
    PRINTLN("Speed        : " + String(speed));
    PRINTLN("Load         : " + String(load));
    PRINTLN("Voltage      : " + String(voltage));
    PRINTLN("Temperature  : " + String(temperature));
    PRINTLN("Torque       : " + String(torque_status ? "Enabled" : "Disabled"));
    PRINTLN("Moving       : " + String(moving ? "YES" : "NO"));
    PRINTLN("LED          : " + String(led_status ? "ON" : "OFF"));


    return true;                                                    // Return true to indicate successful status print
}

// Print servo-specific help information
void Servo::printConsoleHelp() {
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
}

// Return the DynamixelWorkbench instance
DynamixelWorkbench* Servo::getWorkbench()    {
    return &dxl;
}

// End of Servo.cpp