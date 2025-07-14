#include "DynamixelController.h"


DynamixelController::DynamixelController() {}
// Constructor initializes the DynamixelController
bool DynamixelController::init(const char* device_name, uint32_t baudrate) {
    result = dxl_wb.init(device_name, baudrate, &log);
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

// Ping a servo to check if it is connected
bool DynamixelController::ping(uint8_t dxl_id) {
    result = dxl_wb.ping(dxl_id, &model_number, &log);
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
bool DynamixelController::jointMode(uint8_t dxl_id) {
    result = dxl_wb.jointMode(dxl_id, 0, 0, &log);
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
bool DynamixelController::initServo(uint8_t dxl_id) {
    
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
bool DynamixelController::goalPosition(uint8_t dxl_id, int32_t position) {
    result = dxl_wb.goalPosition(dxl_id, position, &log);
    if (!result)  // If setting goal position fails
    {
        Serial.println(log);
        Serial.print("Failed to set goal position for id: ");
        Serial.println(dxl_id);
    }
    return result;
}

// Set the goal velocity of a servo
bool DynamixelController::goalVelocity(uint8_t dxl_id, int32_t velocity) {
    result = dxl_wb.goalVelocity(dxl_id, velocity, &log);
    if (!result)  // If setting goal velocity fails 
    {
        Serial.println(log);
        Serial.print("Failed to set goal velocity for id: ");
        Serial.println(dxl_id);
    }
    return result;
}
// Turn on the LED of a servo
bool DynamixelController::LEDOn(uint8_t dxl_id) {
    result = dxl_wb.ledOn(dxl_id, &log);
    if (!result)  // If turning on LED fails
    {
        Serial.println(log);
        Serial.print("Failed to turn on LED for id: ");
        Serial.println(dxl_id);
    }
    return result;
}   

// Turn off the LED of a servo
bool DynamixelController::LEDOff(uint8_t dxl_id) {
    result = dxl_wb.ledOff(dxl_id, &log);
    if (!result)  // If turning off LED fails
    {
        Serial.println(log);
        Serial.print("Failed to turn off LED for id: ");
        Serial.println(dxl_id);
    }
    return result;
}



// Return the DynamixelWorkbench instance
DynamixelWorkbench* DynamixelController::getWorkbench() {
    return &dxl_wb;
}