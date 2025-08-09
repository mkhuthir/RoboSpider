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

// Write an item to a servo
bool Servo::itemWrite(uint8_t id, const char *item_name, int32_t data) {
    
    if (!dxl.itemWrite(id, item_name, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("item: " + String(item_name)+ " id: " + String(id));
        return false;  
    }
    return true;
    
}

// Read an item from a servo
bool Servo::itemRead(uint8_t id, const char *item_name, int32_t *data) {
    
    if (!dxl.itemRead(id, item_name, data, &log))
    {        
        LOG_ERR(log);
        LOG_ERR("item: " + String(item_name)+ " id: " + String(id) );
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

// Set the normal direction for a servo
bool Servo::setNormalDirection(uint8_t id) {
    
    if (!dxl.setNormalDirection(id, &log))
    {        
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;

}

// Set the reverse direction for a servo
bool Servo::setReverseDirection(uint8_t id) {
    
    if (!dxl.setReverseDirection(id, &log))
    {     
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;

}

// Set a servo to joint mode
bool Servo::jointMode(uint8_t dxl_id) {
    
    if (!dxl.jointMode(dxl_id, 0, 0, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;  
    }
    return true;

}


// Set the goal position of a servo in value
bool Servo::goalPosition(uint8_t dxl_id, int32_t position) {

    if (!dxl.goalPosition(dxl_id, position, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;  
    }
    return true;
    
}

// Set the goal position of a servo in radians
bool Servo::goalPosition(uint8_t dxl_id, float radian) {
    
    if (!dxl.goalPosition(dxl_id, radian, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;
    }
    return true;

}

// Set the goal velocity of a servo in int value
bool Servo::goalVelocity(uint8_t dxl_id, int32_t velocity) {
    
    if (!dxl.goalVelocity(dxl_id, velocity, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;
    }
    return true;
    
}

// Set the goal velocity of a servo in float
bool Servo::goalVelocity(uint8_t dxl_id, float velocity) {
    
    if (!dxl.goalVelocity(dxl_id, velocity, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(dxl_id));
        return false;  
    }
    return true;
    
}

// Get the present position data of a servo in value
bool Servo::getPresentPosition(uint8_t id, int32_t* data) {
    
    if (!dxl.getPresentPositionData(id, data, &log))
    {        
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;

}

// Get the present position of a servo in radians
bool Servo::getRadian(uint8_t id, float* radian) {
    
    if (!dxl.getRadian(id, radian, &log))
    {        
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;
    
}

// Get the present velocity data of a servo in value
bool Servo::getPresentSpeed(uint8_t id, int32_t* data) {
    
    if (!dxl.getPresentVelocityData(id, data, &log))
    {
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;
}

// Get the present velocity of a servo in float
bool Servo::getVelocity(uint8_t id, float* velocity) {
    
    if (!dxl.getVelocity(id, velocity, &log))
    {        
        LOG_ERR(log);
        LOG_ERR("id: " + String(id));
        return false;  
    }
    return true;
}

// Initialize a servo with default settings
bool Servo::init(uint8_t dxl_id, int32_t velocity) {
    bool result =
        ping(dxl_id) &&
        jointMode(dxl_id) &&
        torqueOn(dxl_id) &&
        goalVelocity(dxl_id, velocity) &&
        ledOn(dxl_id);

    if (!result) {
        LOG_ERR("Failed to initialize servo with ID: " + String(dxl_id));
    } else {
        LOG_INF("Servo with ID: " + String(dxl_id) + " initialized successfully.");
    }
    return result;
}

// Check if a servo is currently moving
bool Servo::isMoving(uint8_t id) {
    int32_t isMoving = 0;
    if (!readRegister(id, "Moving", &isMoving)) {
        LOG_ERR("Failed to read moving status for servo ID: " + String(id));
        return false;
    }
    return isMoving;
}

// Print the status of a servo for debugging
bool Servo::printStatus(uint8_t id) {

    PRINTLN("\nServo Status:");
    PRINTLN("Servo ID         : " + String(id));                         // Print the ID of the servo
    PRINTLN("Model Number     : " + String(getModelNumber(id)));
    PRINTLN("Model Name       : " + String(getModelName(id)));

    int32_t position = 0;
    getPresentPosition(id, &position);
    PRINTLN("Present Position : " + String(position));

    float radian = 0.0f;
    getRadian(id, &radian);
    PRINTLN("Radian           : " + String(radian));

    int32_t velocity = 0;
    getPresentSpeed(id, &velocity);
    PRINTLN("Present Velocity : " + String(velocity));

    float velocity_f = 0.0f;
    getVelocity(id, &velocity_f);
    PRINTLN("Velocity (float) : " + String(velocity_f));

    int32_t torque_status = 0;
    readRegister(id, "Torque_Enable", &torque_status);
    PRINTLN("Torque Enabled   : " + String(torque_status ? "YES" : "NO"));

    int32_t led_status = 0;
    readRegister(id, "LED", &led_status);
    PRINTLN("LED Status       : " + String(led_status ? "ON" : "OFF"));

    int32_t moving = 0;
    readRegister(id, "Moving", &moving);
    PRINTLN("Moving           : " + String(moving ? "YES" : "NO"));
    return true;                                                    // Return true to indicate successful status print

}

// Process console commands for servo control
bool Servo::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "ss") {
        // Servo status: ss [id] - show status of specific servo (default ID 1)
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
        // Servo ping: sp [id] - ping specific servo (default ID 1)
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

    } else if (cmd == "ston") {
        // Servo torque on: ston [id] - enable torque for specific servo (default ID 1)
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
        // Servo torque off: stoff [id] - disable torque for specific servo (default ID 1)
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
        // Servo LED on: slon [id] - turn on LED for specific servo (default ID 1)
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
        // Servo LED off: sloff [id] - turn off LED for specific servo (default ID 1)
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

    } else if (cmd == "ssp") {
        // Servo position: ssp [id] [position] - set goal position for servo
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
        
        bool result = goalPosition((uint8_t)servoId, (int32_t)position);
        PRINTLN("Servo ID " + String(servoId) + " position set to " + String(position) + ": " + String(result ? "SUCCESS" : "FAILED"));
        return true;

    } else if (cmd == "ssv") {
        // Servo velocity: ssv [id] [velocity] - set goal velocity for servo
        int servoId = 1; // Default to servo ID 1
        int velocity = 100; // Default velocity
        
        if (args.length() > 0) {
            int spaceIndex = args.indexOf(' ');
            if (spaceIndex != -1) {
                servoId = args.substring(0, spaceIndex).toInt();
                velocity = args.substring(spaceIndex + 1).toInt();
            } else {
                servoId = args.toInt();
            }
            
            // Validate servo ID
            if (servoId < 1 || servoId > 253) {
                servoId = 1;
            }
            
            // Validate velocity (typical range for AX-12A is 0-1023)
            if (velocity < 0 || velocity > 1023) {
                velocity = 100;
            }
        }
        
        bool result = goalVelocity((uint8_t)servoId, (int32_t)velocity);
        PRINTLN("Servo ID " + String(servoId) + " velocity set to " + String(velocity) + ": " + String(result ? "SUCCESS" : "FAILED"));
        return true;

    } else if (cmd == "sgp") {
        // Get servo current position: sgp [id] - read current position of servo
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }
        
        int32_t currentPos;
        bool result = getPresentPosition((uint8_t)servoId, &currentPos);
        if (result) {
            PRINTLN("Servo ID " + String(servoId) + " current position: " + String(currentPos));
        } else {
            LOG_ERR("Failed to read position from servo ID " + String(servoId));
        }
        return true;
    } else if (cmd == "sgv") {
        // Get servo current velocity: sgv [id] - read current velocity of servo
        int servoId = 1; // Default to servo ID 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 1 && parsedId <= 253) {
                servoId = parsedId;
            }
        }

        int32_t currentVel;
        bool result = getPresentSpeed((uint8_t)servoId, &currentVel);
        if (result) {
            PRINTLN("Servo ID " + String(servoId) + " current velocity: " + String(currentVel));
        } else {
            LOG_ERR("Failed to read velocity from servo ID " + String(servoId));
        }
        return true;
        
    } else if (cmd == "s?") {
        // Show help for servo commands
        printConsoleHelp();
        return true;
    }
    
    return false;
}

// Print servo-specific help information
void Servo::printConsoleHelp() {
    PRINTLN("Servo Commands:");
    PRINTLN("  ss [id]         - Show servo status (default ID=1)");
    PRINTLN("  sp [id]         - Ping servo (default ID=1)");
    PRINTLN("  ston [id]       - Enable servo torque (default ID=1)");
    PRINTLN("  stoff [id]      - Disable servo torque (default ID=1)");
    PRINTLN("  slon [id]       - Turn on servo LED (default ID=1)");
    PRINTLN("  sloff [id]      - Turn off servo LED (default ID=1)");
    PRINTLN("  ssp [id] [pos]  - Set servo position (default ID=1, pos=512)");
    PRINTLN("  ssv [id] [vel]  - Set servo velocity (default ID=1, vel=100)");
    PRINTLN("  sgp [id]        - Get current servo position (default ID=1)");
    PRINTLN("  sgv [id]        - Get current servo velocity (default ID=1)");
    PRINTLN("  s?              - Show this help message");
    PRINTLN("");
}

// Return the DynamixelWorkbench instance
DynamixelWorkbench* Servo::getWorkbench() {
    return &dxl;
}