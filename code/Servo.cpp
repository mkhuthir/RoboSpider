#include "Servo.h"
#include "Driver.h"
#include "Console.h"
#include "Debug.h"


// Constructor for Servo
Servo::Servo(){}

//initialize the DynamixelWorkbench instance
bool Servo::begin(Driver* driver) {
    this->driver = driver;
    LOG_INF("Servo class initialized successfully");
    return true;
}

// Ping a servo to check if it is connected, ping is required before you can use a servo.
bool Servo::ping(uint8_t dxl_id) {
    return driver->ping(dxl_id);
}

// Get the model name of a servo
const char * Servo::getModelName(uint8_t id) {
    return driver->getModelName(id);
}

// ------------------ EPROM Area -----------------------------


// Get the model number of a servo
bool Servo::getModelNumber(uint8_t id, uint16_t* model_number) {
    if (!driver->readRegister(id, "Model_Number", (uint32_t*)model_number)) return false;
    return true;
}

// Get Firmware Version
bool Servo::getFirmwareVersion(uint8_t id, uint8_t* version) {
    if (!driver->readRegister(id, "Firmware_Version", (uint32_t*)version)) return false;
    return true;
}

// Get Baud Rate
bool Servo::getBaudRate(uint8_t id, uint8_t* baud_rate) {
    if (!driver->readRegister(id, "Baud_Rate", (uint32_t*)baud_rate)) return false;
    return true;
}

// Get Return Delay Time
bool Servo::getReturnDelayTime(uint8_t id, uint8_t* return_delay_time) {
    if (!driver->readRegister(id, "Return_Delay_Time", (uint32_t*)return_delay_time)) return false;
    return true;
}

// Get the angle limits for a servo
bool Servo::getAngleLimits(uint8_t id, uint16_t* CW_angle, uint16_t* CCW_angle) {
    if (!driver->readRegister(id, "CW_Angle_Limit", (uint32_t*)CW_angle)) return false;
    if (!driver->readRegister(id, "CCW_Angle_Limit", (uint32_t*)CCW_angle)) return false;
    return true;
}

// Set the angle limits for a servo
bool Servo::setAngleLimits(uint8_t id, uint16_t CW_angle, uint16_t CCW_angle) {
    if (!driver->writeRegister(id, "CW_Angle_Limit", (uint32_t)CW_angle)) return false;
    if (!driver->writeRegister(id, "CCW_Angle_Limit", (uint32_t)CCW_angle)) return false;
    return true;
}

// Get Temperature Limit
bool Servo::getTemperatureLimit(uint8_t id, uint8_t* max_temp) {
    if (!driver->readRegister(id, "Temperature_Limit", (uint32_t*)max_temp)) return false;
    return true;
}

// Get Voltage Limit
bool Servo::getVoltageLimit(uint8_t id, uint8_t* min_voltage, uint8_t* max_voltage) {
    if (!driver->readRegister(id, "Min_Voltage_Limit", (uint32_t*)min_voltage)) return false;
    if (!driver->readRegister(id, "Max_Voltage_Limit", (uint32_t*)max_voltage)) return false;
    return true;
}

// Get Max Torque
bool Servo::getMaxTorque(uint8_t id, uint16_t* max_torque) {
    if (!driver->readRegister(id, "Max_Torque", (uint32_t*)max_torque)) return false;
    return true;
}

// Get Status Return Level
bool Servo::getStatusReturnLevel(uint8_t id, uint8_t* level) {
    if (!driver->readRegister(id, "Status_Return_Level", (uint32_t*)level)) return false;
    return true;
}

    // Get Alarm LED
bool Servo::getAlarmLED(uint8_t id, uint8_t* alarm_led) {
    if (!driver->readRegister(id, "Alarm_LED", (uint32_t*)alarm_led)) return false;
    return true;
}

// Get Shutdown
bool Servo::getShutdown(uint8_t id, uint8_t* shutdown) {
    if (!driver->readRegister(id, "Shutdown", (uint32_t*)shutdown)) return false;
    return true;
}

// ------------------ RAM Area -----------------------------
// Check if the torque is enabled for a servo
bool Servo::isTorqueOn(uint8_t id) {
    uint32_t isTorqueOn = 0;
    if (!driver->readRegister(id, "Torque_Enable", &isTorqueOn)) return false;
    return isTorqueOn;
}

// Turn on the torque for a servo
bool Servo::torqueOn(uint8_t id) {
    if (!driver->writeRegister(id, "Torque_Enable", 1)) return false;
    return true;
}

// Turn off the torque for a servo
bool Servo::torqueOff(uint8_t id) {
    if (!driver->writeRegister(id, "Torque_Enable", 0)) return false;
    return true;
}

// Check if the LED is enabled for a servo
bool Servo::isLedOn(uint8_t id) {
    uint32_t isLedOn = 0;
    if (!driver->readRegister(id, "LED", &isLedOn)) return false;
    return isLedOn;
}

// Turn on the LED of a servo
bool Servo::ledOn(uint8_t dxl_id) {
    if (!driver->writeRegister(dxl_id, "LED", 1)) return false;
    return true;
}

// Turn off the LED of a servo
bool Servo::ledOff(uint8_t dxl_id) {
    if (!driver->writeRegister(dxl_id, "LED", 0)) return false;
    return true;
}

// Get the compliance margin of a servo
bool Servo::getComplianceMargin(uint8_t id, uint8_t* CW_margin, uint8_t* CCW_margin) {
    if (!driver->readRegister(id, "CW_Compliance_Margin", (uint32_t*)CW_margin)) return false;
    if (!driver->readRegister(id, "CCW_Compliance_Margin", (uint32_t*)CCW_margin)) return false;
    return true;
}

// Set the compliance margin of a servo
bool Servo::setComplianceMargin(uint8_t id, uint8_t CW_margin, uint8_t CCW_margin) {
    if (!driver->writeRegister(id, "CW_Compliance_Margin", (uint32_t)CW_margin)) return false;
    if (!driver->writeRegister(id, "CCW_Compliance_Margin", (uint32_t)CCW_margin)) return false;
    return true;
}

// Get the compliance slope of a servo
bool Servo::getComplianceSlope(uint8_t id, uint8_t* CW_slope, uint8_t* CCW_slope) {
    if (!driver->readRegister(id, "CW_Compliance_Slope", (uint32_t*)CW_slope)) return false;
    if (!driver->readRegister(id, "CCW_Compliance_Slope", (uint32_t*)CCW_slope)) return false;
    return true;
}

// Set the compliance slope of a servo
bool Servo::setComplianceSlope(uint8_t id, uint8_t CW_slope, uint8_t CCW_slope) {
    if (!driver->writeRegister(id, "CW_Compliance_Slope", (uint32_t)CW_slope)) return false;
    if (!driver->writeRegister(id, "CCW_Compliance_Slope", (uint32_t)CCW_slope)) return false;
    return true;
}

// Set the goal position of a servo in value
bool Servo::setGoalPosition(uint8_t dxl_id, int16_t position) {
    if (!driver->writeRegister(dxl_id, "Goal_Position", (uint32_t)position)) return false;
    return true;
}

// Set the goal speed of a servo in int value
bool Servo::setGoalSpeed(uint8_t dxl_id, int16_t speed) {
    if (!driver->writeRegister(dxl_id, "Moving_Speed", (uint32_t)speed)) return false;
    return true;
}

// Get the torque limit of a servo
bool Servo::getTorqueLimit(uint8_t id, uint16_t* torque_limit) {
    if (!driver->readRegister(id, "Torque_Limit", (uint32_t*)torque_limit)) return false;
    return true;
}

// Set the torque limit of a servo
bool Servo::setTorqueLimit(uint8_t id, uint16_t torque_limit) {
    if (!driver->writeRegister(id, "Torque_Limit", (uint32_t)torque_limit)) return false;
    return true;
}

// Get the present position data of a servo in value
bool Servo::getPresentPosition(uint8_t id, uint16_t* pos) {
    if (!driver->readRegister(id, "Present_Position", (uint32_t*)pos)) return false;
    return true;
}

// Get the present velocity data of a servo in value
bool Servo::getPresentSpeed(uint8_t id, uint16_t* speed) {
    if (!driver->readRegister(id, "Moving_Speed", (uint32_t*)speed)) return false;
    return true;
}

// Get the present load data of a servo in value
bool Servo::getPresentLoad(uint8_t id, uint16_t* load) {
    if (!driver->readRegister(id, "Present_Load", (uint32_t*)load)) return false;
    return true;
}

// Get the present voltage data of a servo in value
bool Servo::getPresentVoltage(uint8_t id, uint8_t* voltage) {
    if (!driver->readRegister(id, "Present_Voltage", (uint32_t*)voltage)) return false;
    return true;
}

// Get the present temperature data of a servo in value
bool Servo::getPresentTemperature(uint8_t id, uint8_t* temperature) {
    if (!driver->readRegister(id, "Present_Temperature", (uint32_t*)temperature)) return false;
    return true;
}

// Check if a servo is currently moving
bool Servo::isMoving(uint8_t id) {
    uint32_t isMoving = 0;
    if (!driver->readRegister(id, "Moving", &isMoving)) return false;
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
    uint8_t firmware_version = 0;
    uint8_t baud_rate = 0;
    uint8_t return_delay_time = 0;
    uint16_t CW_angle = 0, CCW_angle = 0;
    uint8_t max_temperature = 0;
    uint8_t min_voltage = 0, max_voltage = 0;
    uint16_t max_torque = 0;
    uint8_t status_return_level = 0;
    uint8_t alarm_LED = 0;
    uint8_t shutdown = 0;

    getModelNumber(id, &model_number);
    getFirmwareVersion(id, &firmware_version);
    getBaudRate(id, &baud_rate);
    getReturnDelayTime(id, &return_delay_time);
    getAngleLimits(id, &CW_angle, &CCW_angle);
    getTemperatureLimit(id, &max_temperature);
    getVoltageLimit(id, &min_voltage, &max_voltage);
    getTorqueLimit(id, &max_torque);
    getStatusReturnLevel(id, &status_return_level);
    getAlarmLED(id, &alarm_LED);
    getShutdown(id, &shutdown);

    uint8_t CW_margin = 0, CCW_margin = 0;
    uint8_t CW_slope = 0, CCW_slope = 0;
    uint16_t torque_limit = 0;
    uint16_t position = 0;
    uint16_t speed = 0;
    uint16_t load = 0;
    uint8_t voltage = 0;
    uint8_t temperature = 0;

    getComplianceMargin(id, &CW_margin, &CCW_margin);
    getComplianceSlope(id, &CW_slope, &CCW_slope);
    getTorqueLimit(id, &torque_limit);
    getPresentPosition(id, &position);
    getPresentSpeed(id, &speed);
    getPresentLoad(id, &load);
    getPresentVoltage(id, &voltage);
    getPresentTemperature(id, &temperature);

    PRINTLN("\nServo Status:");
    PRINTLN("Servo ID            : " + String(id));
    PRINTLN("Model Number        : " + String(model_number));
    PRINTLN("Model Name          : " + String(getModelName(id)));
    PRINTLN("Firmware Ver        : " + String(firmware_version));
    PRINTLN("Baud Rate           : " + String(baud_rate));
    PRINTLN("Return Delay        : " + String(return_delay_time));
    PRINTLN("Angle Limits        : CW " + String(CW_angle) + " ~ CCW " + String(CCW_angle));
    PRINTLN("Max Temperature     : " + String(max_temperature));
    PRINTLN("Voltage Limits      : " + String(min_voltage) + " ~ " + String(max_voltage) + " V");
    PRINTLN("Max Torque          : " + String(max_torque));
    PRINTLN("Status Level        : " + String(status_return_level));
    PRINTLN("Alarm LED Status    : " + String(alarm_LED));
    PRINTLN("Shutdown Status     : " + String(shutdown));
    PRINTLN("Torque              : " + String(isTorqueOn(id) ? "Enabled" : "Disabled"));
    PRINTLN("LED                 : " + String(isLedOn(id) ? "ON" : "OFF"));
    PRINTLN("Compliance Margin   : CW " + String(CW_margin) + " ~ CCW " + String(CCW_margin));
    PRINTLN("Compliance Slope    : CW " + String(CW_slope) + " ~ CCW " + String(CCW_slope));
    PRINTLN("Torque Limit        : " + String(torque_limit));
    PRINTLN("Present Position    : " + String(position));
    PRINTLN("Present Speed       : " + String(speed));
    PRINTLN("Present Load        : " + String(load));
    PRINTLN("Present Voltage     : " + String(voltage));
    PRINTLN("Present Temperature : " + String(temperature));
    PRINTLN("Moving              : " + String(isMoving(id) ? "YES" : "NO"));
    return true;
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


// End of Servo.cpp