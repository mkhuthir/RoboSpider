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

bool DynamixelController::jointmode(uint8_t dxl_id) {
    result = dxl_wb.jointMode(dxl_id, 0, 0, &log);
    if (!result)    // If joint mode setting fails
    {
        Serial.println(log);
        Serial.println("Failed to set joint mode!");
    }
    return result;
}

bool DynamixelController::initializeServo(uint8_t dxl_id) {
    result = result && jointmode(dxl_id);   // Set servo to joint mode
    result = result && torqueOn(dxl_id);    // Turn on torque for the servo
    return result;
}

bool DynamixelController::torqueOn(uint8_t dxl_id) {
    return dxl_wb.torqueOn(dxl_id);
}

bool DynamixelController::torqueOff(uint8_t dxl_id) {
    return dxl_wb.torqueOff(dxl_id);
}

bool DynamixelController::setGoalPosition(uint8_t dxl_id, uint32_t position) {
    return dxl_wb.itemWrite(dxl_id, "Goal_Position", position);
}

bool DynamixelController::setGoalVelocity(uint8_t dxl_id, uint32_t velocity) {
    return dxl_wb.itemWrite(dxl_id, "Profile_Velocity", velocity);
}

bool DynamixelController::readPresentPosition(uint8_t dxl_id, uint32_t &position) {
    int32_t pos = 0;
    bool result = dxl_wb.itemRead(dxl_id, "Present_Position", &pos);
    position = static_cast<uint32_t>(pos);
    return result;
}

bool DynamixelController::syncWritePosition(const std::vector<uint8_t>& ids, const std::vector<uint32_t>& positions) {
    if (ids.size() != positions.size() || ids.empty()) return false;
    std::vector<int32_t> pos32(positions.begin(), positions.end());
    const ControlItem* goal_position_item = dxl_wb.getItemInfo(ids[0], "Goal_Position");
    uint16_t goal_position_address = goal_position_item->address;
    return dxl_wb.syncWrite(goal_position_address, const_cast<uint8_t*>(ids.data()), ids.size(), pos32.data(), 1);
}

bool DynamixelController::bulkReadPositions(const std::vector<uint8_t>& ids, std::vector<uint32_t>& positions) {
    positions.clear();
    for (size_t i = 0; i < ids.size(); ++i) {
        int32_t pos = 0;
        if (!dxl_wb.itemRead(ids[i], "Present_Position", &pos)) {
            positions.push_back(0);
            return false;
        }
        positions.push_back(static_cast<uint32_t>(pos));
    }
    return true;
}

DynamixelWorkbench* DynamixelController::getWorkbench() {
    return &dxl_wb;
}