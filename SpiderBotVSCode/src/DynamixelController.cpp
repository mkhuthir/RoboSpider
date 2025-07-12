#include "DynamixelController.h"
#include <vector>

DynamixelController::DynamixelController() {}
// Constructor initializes the DynamixelController
bool DynamixelController::begin(const char* device_name, uint32_t baudrate) {
    result = dxl_wb.init(device_name, baudrate, &log);
    if (result == false)
    {
        Serial.println(log);
        Serial.println("Failed to initilize DynamixelWorkbench!");
    }
    else
    {
        Serial.print("Initilized DynamixelWorkbench at baudrate: ");
        Serial.print(baudrate);
        Serial.println(" bps.");
    }
}

bool DynamixelController::ping(uint8_t dxl_id) {
    result = dxl_wb.ping(dxl_id, &model_number, &log);
    if (!result)
    {
        Serial.println(log);
        Serial.println("Failed to ping!");
    }
    else
    {
        Serial.println("Succeeded to ping!");
        Serial.print("dxl_id : ");
        Serial.print(dxl_id);
        Serial.print(" model_number : ");
        Serial.println(model_number);
    }
    return result;
}

bool DynamixelController::jointMode(uint8_t dxl_id) {
    result = dxl_wb.jointMode(dxl_id, 0, 0, &log);
    if (!result)    // If joint mode setting fails
    {
        Serial.println(log);
        Serial.print("Failed to set joint mode for id: ");
        Serial.println(dxl_id);
    } else
    {
        Serial.print("Set joint mode for id: ");
        Serial.println(dxl_id);
    }
    return result;
}

bool DynamixelController::initializeServo(uint8_t dxl_id) {
    result = result && jointMode(dxl_id);   // Set servo to joint mode
    result = result && torqueOn(dxl_id);    // Turn on torque for the servo
    return result;
}


DynamixelWorkbench* DynamixelController::getWorkbench() {
    return &dxl_wb;
}