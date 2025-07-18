#include "Servo.h"
#include "DynamixelWorkbench.h" // Include DynamixelWorkbench library for servo control
#include "Config.h"             // Include configuration header for constants and settings

// Constructor for Servo
Servo::Servo(){
    result          = false;    // Initialize result to false
    model_number    = 0;        // Initialize model number to 0
}

//initialize the DynamixelWorkbench instance
bool Servo::begin(const char* device_name, uint32_t baudrate) {
    result = dxl.init(device_name, baudrate, &log);
    if (!result)  // If initialization fails
    {
        Serial.println(log);
        Serial.println("Failed to initilize DynamixelWorkbench!");
    }
    else
    {
        #ifdef DEBUG
            Serial.print("Initilized DynamixelWorkbench at baudrate: ");
            Serial.print(baudrate);
            Serial.println(" bps.");
        #endif // DEBUG
    }
    setPacketHandler(PROTOCOL_VERSION); // Set the packet handler with the protocol version
    return result;
}

// Set the port handler with the device name
bool Servo::setPortHandler(const char *device_name) {
    result = dxl.setPortHandler(device_name, &log);
    if (!result)  // If setting port handler fails
    {        
        Serial.println(log);
        Serial.println("Failed to set port handler!");
    }
    else
    {        
        #ifdef DEBUG
            Serial.print("Port handler set to: ");
            Serial.println(device_name);
        #endif // DEBUG
    }
    return result;
}   

// Set the baudrate for the port handler
bool Servo::setBaudrate(uint32_t baud_rate) {     
    result = dxl.setBaudrate(baud_rate, &log);
    if (!result)  // If setting baudrate fails
    {        
        Serial.println(log);
        Serial.println("Failed to set baudrate!");
    }
    else
    {        
        #ifdef DEBUG
            Serial.print("Baudrate set to: ");
            Serial.println(baud_rate);
        #endif // DEBUG
    }
    return result;
}   

// Set the packet handler with the protocol version
bool Servo::setPacketHandler(float protocol_version) {
    result = dxl.setPacketHandler(protocol_version, &log);
    if (!result)  // If setting packet handler fails
    {        
        Serial.println(log);
        Serial.println("Failed to set packet handler!");
    }
    else
    {        
        #ifdef DEBUG
            Serial.print("Packet handler set with protocol version: ");
            Serial.println(protocol_version);
        #endif // DEBUG
    }
    return result;
}

// Get the protocol version
float Servo::getProtocolVersion(void) {
    float protocol_version = dxl.getProtocolVersion();
    if (protocol_version < 0)  // If getting protocol version fails
    {        
        Serial.println("Failed to get protocol version!");
        return -1.0;  // Return -1.0 to indicate failure
    }
    #ifdef DEBUG
        Serial.print("Protocol Version: ");
        Serial.println(protocol_version);
    #endif // DEBUG
    return protocol_version;  // Return the protocol version
}

// Get the baudrate
uint32_t Servo::getBaudrate(void) {
    uint32_t baudrate = dxl.getBaudrate();
    if (baudrate == 0)  // If getting baudrate fails
    {        
        Serial.println("Failed to get baudrate!");
        return 0;  // Return 0 to indicate failure
    }
    #ifdef DEBUG
        Serial.print("Baudrate: ");
        Serial.println(baudrate);
    #endif // DEBUG
    return baudrate;  // Return the baudrate
}

// Get the model name of a servo
const char * Servo::getModelName(uint8_t id) {
    model_name = dxl.getModelName(id, &log);
    if (model_name == NULL)  // If getting model name fails
    {   
        Serial.println(log);
        Serial.print("Failed to get model name for id: ");
        Serial.println(id);     
        return NULL;  // Return NULL to indicate failure
    }
    #ifdef DEBUG
        Serial.print("Model Name for id ");
        Serial.print(id);
        Serial.print(": ");
        Serial.println(model_name); 
    #endif // DEBUG
    return model_name;  // Return the model name
}

// Get the model number of a servo
uint16_t Servo::getModelNumber(uint8_t id) {
    model_number = dxl.getModelNumber(id, &log);
    if (model_number == 0)  // If getting model number fails
    {
        Serial.println(log);
        Serial.print("Failed to get model number for id: ");
        Serial.println(id); 
        return 0;  // Return 0 to indicate failure
    }
    #ifdef DEBUG
        Serial.print("Model Number for id ");
        Serial.print(id);
        Serial.print(": ");
        Serial.println(model_number);
    #endif // DEBUG
    return model_number;  // Return the model number
}

// Get the model info of a servo
const ModelInfo* Servo::getModelInfo(uint8_t id) {
    model_info = dxl.getModelInfo(id, &log);
    if (model_info == NULL)  // If getting model info fails
    {
        Serial.println(log);
        Serial.print("Failed to get model info for id: ");
        Serial.println(id);
    }
    #ifdef DEBUG
        Serial.print("Model Info for id ");
        Serial.print(id);
        Serial.print(": ");
    #endif // DEBUG
    return model_info;  // Return the model info
}

// Ping a servo to check if it is connected
bool Servo::ping(uint8_t dxl_id) {
    result = dxl.ping(dxl_id, &model_number, &log);
    if (!result)  // If ping fails
    {
        Serial.println(log);
        Serial.println("Failed to ping!");
    }
    else
    {
        #ifdef DEBUG
            Serial.println("Succeeded to ping!");
            Serial.print("dxl_id : ");
            Serial.print(dxl_id);
            Serial.print(" model_number : ");
            Serial.println(model_number);
        #endif // DEBUG
    }
    return result;
}

// Turn on the torque for a servo
bool Servo::torqueOn(uint8_t id) {
    result = dxl.torqueOn(id, &log);
    if (!result)  // If turning on torque fails
    {
        Serial.println(log);
        Serial.print("Failed to turn on torque for id: ");
        Serial.println(id);
    }
    else
    {
        #ifdef DEBUG
            Serial.print("Torque turned on for id: ");
            Serial.println(id);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Turn off the torque for a servo
bool Servo::torqueOff(uint8_t id) {
    result = dxl.torqueOff(id, &log);
    if (!result)  // If turning off torque fails
    {
        Serial.println(log);
        Serial.print("Failed to turn off torque for id: ");
        Serial.println(id);
    }
    else
    {
        #ifdef DEBUG
            Serial.print("Torque turned off for id: ");
            Serial.println(id);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Write an item to a servo
bool Servo::itemWrite(uint8_t id, const char *item_name, int32_t data) {
    result = dxl.itemWrite(id, item_name, data, &log);
    if (!result)  // If item write fails
    {
        Serial.println(log);
        Serial.print("Failed to write item: ");
        Serial.print(item_name);
        Serial.print(" for id: ");
        Serial.println(id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Item: ");
            Serial.print(item_name);
            Serial.print(" written for id: ");
            Serial.println(id);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Read an item from a servo
bool Servo::itemRead(uint8_t id, const char *item_name, int32_t *data) {
    result = dxl.itemRead(id, item_name, data, &log);
    if (!result)  // If item read fails
    {        
        Serial.println(log);
        Serial.print("Failed to read item: ");
        Serial.print(item_name);
        Serial.print(" for id: ");
        Serial.println(id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Item: ");
            Serial.print(item_name);
            Serial.print(" read for id: ");
            Serial.print(id);
            Serial.print(" with data: ");
            Serial.println(*data);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Turn on the LED of a servo
bool Servo::ledOn(uint8_t dxl_id) {
    result = dxl.ledOn(dxl_id, &log);
    if (!result)  // If turning on LED fails
    {
        Serial.println(log);
        Serial.print("Failed to turn on LED for id: ");
        Serial.println(dxl_id);
    }
    return result;
}   

// Turn off the LED of a servo
bool Servo::ledOff(uint8_t dxl_id) {
    result = dxl.ledOff(dxl_id, &log);
    if (!result)  // If turning off LED fails
    {
        Serial.println(log);
        Serial.print("Failed to turn off LED for id: ");
        Serial.println(dxl_id);
    }
    return result;
}

// Set a servo to joint mode
bool Servo::jointMode(uint8_t dxl_id) {
    result = dxl.jointMode(dxl_id, 0, 0, &log);
    if (!result)    // If joint mode setting fails
    {
        Serial.println(log);
        Serial.print("Failed to set joint mode for id: ");
        Serial.println(dxl_id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Set joint mode for id: ");
            Serial.println(dxl_id);
        #endif // DEBUG
    }
    return result;
}


// Set the goal position of a servo in value
bool Servo::goalPosition(uint8_t dxl_id, int32_t position) {
    result = dxl.goalPosition(dxl_id, position, &log);
    if (!result)  // If setting goal position fails
    {
        Serial.println(log);
        Serial.print("Failed to set goal position for id: ");
        Serial.println(dxl_id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Set goal position for id: ");
            Serial.print(dxl_id);
            Serial.print(" to position: ");
            Serial.println(position);
        #endif // DEBUG
    }
    return result;
}

// Set the goal position of a servo in radians
bool Servo::goalPosition(uint8_t dxl_id, float radian) {
    result = dxl.goalPosition(dxl_id, radian, &log);
    if (!result)  // If setting goal position fails
    {
        Serial.println(log);
        Serial.print("Failed to set goal position for id: ");
        Serial.println(dxl_id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Set goal position for id: ");
            Serial.print(dxl_id);
            Serial.print(" to radian: ");
            Serial.println(radian);
        #endif // DEBUG
    }
    return result;
}

// Set the goal velocity of a servo in int value
bool Servo::goalVelocity(uint8_t dxl_id, int32_t velocity) {
    result = dxl.goalVelocity(dxl_id, velocity, &log);
    if (!result)  // If setting goal velocity fails 
    {
        Serial.println(log);
        Serial.print("Failed to set goal velocity for id: ");
        Serial.println(dxl_id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Set goal velocity for id: ");
            Serial.print(dxl_id);
            Serial.print(" to velocity: ");
            Serial.println(velocity);
        #endif // DEBUG
    }
    return result;
}

// Set the goal velocity of a servo in float
bool Servo::goalVelocity(uint8_t dxl_id, float velocity) {
    result = dxl.goalVelocity(dxl_id, velocity, &log);
    if (!result)  // If setting goal velocity fails
    {
        Serial.println(log);
        Serial.print("Failed to set goal velocity for id: ");
        Serial.println(dxl_id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Set goal velocity for id: ");
            Serial.print(dxl_id);
            Serial.print(" to velocity: ");
            Serial.println(velocity);
        #endif // DEBUG
    }
    return result;
}

// Get the present position data of a servo in value
bool Servo::getPresentPositionData(uint8_t id, int32_t* data) {
    result = dxl.getPresentPositionData(id, data, &log);
    if (!result)  // If getting present position data fails
    {        
        Serial.println(log);
        Serial.print("Failed to get present position data for id: ");
        Serial.println(id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Present position data for id: ");
            Serial.print(id);
            Serial.print(" is: ");
            Serial.println(*data);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Get the present position of a servo in radians
bool Servo::getRadian(uint8_t id, float* radian) {
    result = dxl.getRadian(id, radian, &log);
    if (!result)  // If getting radian fails
    {        
        Serial.println(log);
        Serial.print("Failed to get radian for id: ");
        Serial.println(id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Radian for id: ");
            Serial.print(id);
            Serial.print(" is: ");
            Serial.println(*radian);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Get the present velocity data of a servo in value
bool Servo::getPresentVelocityData(uint8_t id, int32_t* data) {
    result = dxl.getPresentVelocityData(id, data, &log);
    if (!result)  // If getting present velocity data fails
    {
        Serial.println(log);
        Serial.print("Failed to get present velocity data for id: ");
        Serial.println(id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Present velocity data for id: ");
            Serial.print(id);
            Serial.print(" is: ");
            Serial.println(*data);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Get the present velocity of a servo in float
bool Servo::getVelocity(uint8_t id, float* velocity) {
    result = dxl.getVelocity(id, velocity, &log);
    if (!result)  // If getting velocity fails
    {        Serial.println(log);
        Serial.print("Failed to get velocity for id: ");
        Serial.println(id);
    } else
    {
        #ifdef DEBUG
            Serial.print("Velocity for id: ");
            Serial.print(id);
            Serial.print(" is: ");
            Serial.println(*velocity);
        #endif // DEBUG
    }
    return result;  // Return the result of the operation
}

// Initialize a servo with default settings
bool Servo::init(uint8_t dxl_id, int32_t position, int32_t velocity) {
    result = ping(dxl_id);                              // Ping the servo to check if it is connected
    result = result && jointMode(dxl_id);               // Set servo to joint mode
    result = result && goalVelocity(dxl_id, velocity);  // Set goal velocity to default
    result = result && goalPosition(dxl_id, position);  // Set goal position to 512
    result = result && ledOn(dxl_id);                   // Turn on LED for the servo

    if (!result) {
        Serial.print("Failed to initialize servo with ID: ");
        Serial.println(dxl_id);
    } else {
        #ifdef DEBUG
            Serial.print("Servo with ID: ");
            Serial.print(dxl_id);
            Serial.println(" initialized successfully.");
        #endif // DEBUG
    }
    return result;
}

// Return the DynamixelWorkbench instance
DynamixelWorkbench* Servo::getWorkbench() {
    return &dxl;
}