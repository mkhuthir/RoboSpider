#include "AXS1Sensor.h"
#include "Servo.h"                     // Include Servo class for managing Dynamixel servos
#include "Config.h"

AXS1Sensor::AXS1Sensor(){
    servo   = nullptr;  // Initialize servo pointer to null
    id      = 0;        // Initialize sensor ID to 0
}

bool AXS1Sensor::begin(Servo* servo, uint8_t sensor_id){
    this->servo = servo;
    id  =   sensor_id;
}

bool AXS1Sensor::ping() {
  return servo->ping(id);
}

bool AXS1Sensor::setBuzzer(uint8_t note, uint8_t duration) {
    bool ok = servo->itemWrite(id, "Buzzer_Index", note);
    ok &= servo->itemWrite(id, "Buzzer_Time", duration);
    return ok;
}

int AXS1Sensor::getIRLeft() {
    int32_t val = 0;
    if (servo->itemRead(id, "IR_Left", &val))
        return static_cast<int>(val);
    return FALSE;
}

int AXS1Sensor::getIRCenter() {
    int32_t val = 0;
    if (servo->itemRead(id, "IR_Center", &val))
        return static_cast<int>(val);
    return FALSE;
}

int AXS1Sensor::getIRRight() {
    int32_t val = 0;
    if (servo->itemRead(id, "IR_Right", &val))
        return static_cast<int>(val);
    return FALSE;
}

int AXS1Sensor::getLightLeft() {
    int32_t val = 0;
    if (servo->itemRead(id, "Light_Left", &val))
        return static_cast<int>(val);
    return FALSE;
}

int AXS1Sensor::getLightCenter() {
    int32_t val = 0;
    if (servo->itemRead(id, "Light_Center", &val))
        return static_cast<int>(val);
    return FALSE;
}

int AXS1Sensor::getLightRight() {
    int32_t val = 0;
    if (servo->itemRead(id, "Light_Right", &val))
        return static_cast<int>(val);
    return FALSE;
}

int AXS1Sensor::getSoundLevel() {
    int32_t val = 0;
    if (servo->itemRead(id, "Sound_Level", &val))
        return static_cast<int>(val);
    return FALSE;
}

int AXS1Sensor::getSoundCount() {
    int32_t val = 0;
    if (servo->itemRead(id, "Sound_Count", &val))
        return static_cast<int>(val);
    return FALSE;
}

float AXS1Sensor::getVoltage() {
    int32_t val = 0;
    if (servo->itemRead(id, "Present_Voltage", &val))
        return static_cast<float>(val) / 10.0f;
    return -1.0f;
}

float AXS1Sensor::getTemperature() {
    int32_t val = 0;
    if (servo->itemRead(id, "Present_Temperature", &val))
        return static_cast<float>(val);
    return -1.0f;
}