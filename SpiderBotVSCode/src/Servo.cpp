#include "Servo.h"


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

// Initialize a servo with default settings
bool Servo::init(uint8_t dxl_id) {
    
    result = ping(dxl_id);                          // Ping the servo to
    result = result && jointMode(dxl_id);           // Set servo to joint mode
    result = result && goalVelocity(dxl_id,-100);   // Set goal velocity to 0
    result = result && goalPosition(dxl_id, 512);   // Set goal position to 512
    result = result && LEDOn(dxl_id);          // Turn on LED for the servo

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

// Set the goal position of a servo
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

// Set the goal velocity of a servo
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

// Turn on the LED of a servo
bool Servo::LEDOn(uint8_t dxl_id) {
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
bool Servo::LEDOff(uint8_t dxl_id) {
    result = dxl.ledOff(dxl_id, &log);
    if (!result)  // If turning off LED fails
    {
        Serial.println(log);
        Serial.print("Failed to turn off LED for id: ");
        Serial.println(dxl_id);
    }
    return result;
}

// Return the DynamixelWorkbench instance
DynamixelWorkbench* Servo::getWorkbench() {
    return &dxl;
}