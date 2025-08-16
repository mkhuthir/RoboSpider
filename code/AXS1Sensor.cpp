#include "Servo.h"                  // Include Servo class for managing Dynamixel servos
#include "AXS1Sensor.h"
#include "Console.h"        // Add this include for logging macros


// Constructor for AXS1Sensor class
AXS1Sensor::AXS1Sensor(){
    servo   = nullptr;              // Initialize servo pointer to null
    id      = AXS1_SENSOR_ID;       // Initialize sensor ID to AXS1_SENSOR_ID
}

// Begin method to initialize the sensor with a Servo instance and sensor ID
bool AXS1Sensor::begin(Servo* servo, uint8_t sensor_id){
    
    if (servo == nullptr) return false;
    this->servo = servo;
    this->id    = sensor_id;

    if (!setObstacleCompare(AXS1_OBSTACLE_DETECTED)) return false;  // Set default obstacle detection threshold
    if (!setLightCompare(AXS1_LIGHT_DETECTED)) return false;        // Set default light detection threshold
    if (!resetSoundDataMaxHold()) return false;                     // Reset sound data max hold
    if (!resetSoundDetectedCount()) return false;                   // Reset sound detected count
    if (!resetSoundDetectedTime()) return false;                    // Reset sound detected time

    LOG_INF("AXS1Sensor initialized successfully. (ID: " + String(id) + ")");
    return true;
}

// Update method to refresh sensor data
bool AXS1Sensor::update() {

    return true;
}

// Ping method to check connectivity with the sensor
bool AXS1Sensor::ping() {
  return servo->ping(this->id);
}

// Get the ID of the sensor
bool AXS1Sensor::getID(uint8_t* sensorID) {
    *sensorID = this->id;
    return true;
}

// Get the model number of the sensor
bool AXS1Sensor::getModelNumber(uint16_t* model_number) {
    if (!servo->readRegister(this->id, AXS1_Model_Number_L, 2, (uint32_t*)model_number)) {
        LOG_ERR("Failed to read Model Number for ID: " + String(this->id));
        return false;
    }
    return true;
}

// Get the firmware version of the sensor
bool AXS1Sensor::getFirmwareVersion(uint8_t* version) {
    if (!servo->readRegister(id, AXS1_Firmware_Version, 1, (uint32_t*)version)) {
        LOG_ERR("Failed to read Firmware Version for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the baud rate of the sensor
bool AXS1Sensor::getBaudRate(uint8_t* baud_rate) {
    if (!servo->readRegister(id, AXS1_Baud_Rate, 1, (uint32_t*)baud_rate)) {
        LOG_ERR("Failed to read Baud Rate for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the return delay time of the sensor
bool AXS1Sensor::getReturnDelayTime(uint8_t* return_delay) {
    if (!servo->readRegister(id, AXS1_Return_Delay_Time, 1, (uint32_t*)return_delay)) {
        LOG_ERR("Failed to read Return Delay Time for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the status return level of the sensor
bool AXS1Sensor::getStatusReturnLevel(uint8_t* status_return) {
    if (!servo->readRegister(id, AXS1_Status_Return_Level, 1, (uint32_t*)status_return)) {
        LOG_ERR("Failed to read Status Return Level for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the distance data from the IR left side sensor
int AXS1Sensor::getDistanceLeft() {

    uint32_t val = 0;
    if (!servo->readRegister(id, AXS1_Left_Distance_Data, 1, &val)) {
        LOG_ERR("Failed to read IR Left Distance Data for ID: " + String(id));
        return false;
    }
    return static_cast<int>(val);
}

// Get the distance data from the IR center sensor
int AXS1Sensor::getDistanceCenter() {

    uint32_t val = 0;
    if (!servo->readRegister(id, AXS1_Center_Distance_Data, 1, &val)) {
        LOG_ERR("Failed to read IR Center Distance Data for ID: " + String(id));
        return false;
    }
    return static_cast<int>(val);
}

// Get the distance data from the IR right side sensor
int AXS1Sensor::getDistanceRight() {

    uint32_t val = 0;
    if (!servo->readRegister(id, AXS1_Right_Distance_Data, 1, &val)) {
        LOG_ERR("Failed to read IR Right Distance Data for ID: " + String(id));
        return false;
    }
    return static_cast<int>(val);
}

// Get the IR light left side sensor data
int AXS1Sensor::getIRLeft() {

    uint32_t val = 0;
    if (!servo->readRegister(id, AXS1_Light_Left_Data, 1, &val)) {
        LOG_ERR("Failed to read IR Left Data for ID: " + String(id));
        return false;
    }
    return static_cast<int>(val);
}

// Get the IR light center sensor data
int AXS1Sensor::getIRCenter() {

    uint32_t val = 0;
    if (!servo->readRegister(id, AXS1_Light_Center_Data, 1, &val)) {
        LOG_ERR("Failed to read IR Center Data for ID: " + String(id));
        return false;
    }
    return static_cast<int>(val);
}

// Get the IR light right side sensor data
int AXS1Sensor::getIRRight() {

    uint32_t val = 0;
    if (!servo->readRegister(id, AXS1_Light_Right_Data, 1, &val)) {
        LOG_ERR("Failed to read IR Right Data for ID: " + String(id));
        return false;
    }
    return static_cast<int>(val);
}

// Set the obstacle detection comparison value
bool AXS1Sensor::setObstacleCompare(uint8_t value) {

    if (!servo->writeRegister(id, AXS1_IR_Obstacle_Detect_Compare, 1, &value)) {
        LOG_ERR("Failed to set Obstacle Compare for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the obstacle detection comparison value
bool AXS1Sensor::getObstacleCompare(uint8_t* value) {
    if (!servo->readRegister(id, AXS1_IR_Obstacle_Detect_Compare, 1, (uint32_t*)value)) {
        LOG_ERR("Failed to read Obstacle Compare Data for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the obstacle detected status
bool AXS1Sensor::ObstacleDetected(uint8_t* value) {
    if (!servo->readRegister(this->id, AXS1_IR_Obstacle_Detected, 1, (uint32_t*)value)) {
        LOG_ERR("Failed to read Obstacle Detected Data for ID: " + String(this->id));
        return false;
    }
    return true;
}

// Set the light detection comparison value
bool AXS1Sensor::setLightCompare(uint8_t value) {
    if (!servo->writeRegister(id, AXS1_Light_Detect_Compare, 1, &value)) {
        LOG_ERR("Failed to set Light Compare for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the light detection comparison value
bool AXS1Sensor::getLightCompare(uint8_t* value) {
    if (!servo->readRegister(id, AXS1_Light_Detect_Compare, 1, (uint32_t*)value)) {
        LOG_ERR("Failed to read Light Compare Data for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the light detected status
bool AXS1Sensor::LightDetected(uint8_t* value) {
    if (!servo->readRegister(id, AXS1_Light_Detected, 1, (uint32_t*)value)) {
        LOG_ERR("Failed to read Light Detected Data for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the sound data from the sensor
bool AXS1Sensor::getSoundData(uint8_t* value) {
    if (!servo->readRegister(id, AXS1_Sound_Data, 1, (uint32_t*)value)) {
        LOG_ERR("Failed to read Sound Data for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the maximum hold sound data from the sensor
bool AXS1Sensor::getSoundDataMaxHold(uint8_t* value) {
    if (!servo->readRegister(id, AXS1_Sound_Data_Max_Hold, 1, (uint32_t*)value)) {
        LOG_ERR("Failed to read Sound Data Max Hold for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the count of sound detected by the sensor
bool AXS1Sensor::getSoundDetectedCount(uint8_t* value) {
    if (!servo->readRegister(id, AXS1_Sound_Detected_Count, 1, (uint32_t*)value)){
        LOG_ERR("Failed to read Sound Detected Count for ID: " + String(id));
        return false;
    }
    return true;
}

// Get the time of sound detected by the sensor
bool AXS1Sensor::getSoundDetectedTime(uint8_t* value) {
    if (!servo->readRegister(id, AXS1_Sound_Detected_Time_L, 2, (uint32_t*)value)) {
        LOG_ERR("Failed to read Sound Detected Time for ID: " + String(id));
        return false;
    }
    return true;
}

// Reset the maximum hold sound data
bool AXS1Sensor::resetSoundDataMaxHold() {
    uint8_t value = 0;
    if (!servo->writeRegister(id, AXS1_Sound_Data_Max_Hold, 1, &value)) {
        LOG_ERR("Failed to reset Sound Data Max Hold for ID: " + String(id));
        return false;       
    }
    return true;
}

// Reset the sound detected count
bool AXS1Sensor::resetSoundDetectedCount() {
    
    uint8_t value = 0;
    if (!servo->writeRegister(id, AXS1_Sound_Detected_Count, 1, &value)) {
        LOG_ERR("Failed to reset Sound Detected Count for ID: " + String(id));
        return false;
    }
    return true;
}

// Reset the sound detected time
bool AXS1Sensor::resetSoundDetectedTime() {
    uint8_t txData[2] = {0, 0};
    if (!servo->writeRegister(id, AXS1_Sound_Detected_Time_L, 2, txData)){
        LOG_ERR("Failed to reset Sound Detected Time for ID: " + String(id));
        return false;
    }
    return true;
}

// Play a tone on the sensor's buzzer
bool AXS1Sensor::playTone(uint8_t note, uint8_t duration) {
    if (note > 51 || duration > 50) {
        LOG_ERR("Invalid note or duration: " + String(note) + ", " + String(duration));
        return false;
    }

    if (!servo->writeRegister(id, AXS1_Buzzer_Data_1, 1, &duration)) {
        LOG_ERR("Failed to set Buzzer Duration for ID: " + String(id));
        return false;
    }

    if (!servo->writeRegister(id, AXS1_Buzzer_Data_0, 1, &note)) {
        LOG_ERR("Failed to set Buzzer Note for ID: " + String(id));
        return false;
    }
    return true;
}

// Play a melody on the sensor's buzzer
bool AXS1Sensor::playMelody(uint8_t melody) {
    uint8_t duration = 255;  // Set duration to 255 for melody
    if (melody > 26) {
        LOG_ERR("Invalid melody value: " + String(melody));
        return false;
    }
    if (!servo->writeRegister(id, AXS1_Buzzer_Data_1, 1, &duration)) {
        LOG_ERR("Failed to set Buzzer Duration for ID: " + String(id));
        return false;
    }

    if (!servo->writeRegister(id, AXS1_Buzzer_Data_0, 1, &melody)) {
        LOG_ERR("Failed to set Buzzer Note for ID: " + String(id));
        return false;
    }
    return true;
}

// Start a continuous tone on the sensor's buzzer
bool AXS1Sensor::startTone(uint8_t note) {
    uint8_t duration = 254;  // value to start continuous tone
    if (note > 51) {
        LOG_ERR("Invalid note value: " + String(note));
        return false;
    }
    if (!servo->writeRegister(id, AXS1_Buzzer_Data_1, 1, &duration)) { 
        LOG_ERR("Failed to set Buzzer Duration for ID: " + String(id));
        return false;
    }

    if (!servo->writeRegister(id, AXS1_Buzzer_Data_0, 1, &note)) {
        LOG_ERR("Failed to start tone for ID: " + String(id));
        return false;
    }
    return true;
}

// Stop the continuous tone on the sensor's buzzer
bool AXS1Sensor::stopTone() {
    uint8_t duration = 0;  // value to stop continuous tone
    if (!servo->writeRegister(id, AXS1_Buzzer_Data_1, 1, &duration)) {
        LOG_ERR("Failed to stop tone for ID: " + String(id));
        return false;
    }
    return true;
}

// Check if the remote control data has arrived
bool AXS1Sensor::RemoconArrived() {         //TODO: validation using remote to be done.
    uint32_t value = 0;  
    if (!servo->readRegister(id, AXS1_IR_Remocon_Arrived, 1, &value)) {
        LOG_ERR("Failed to read Remote Control Data for ID: " + String(id));
        return false;
    }
    return (value != 0);  // Return true if data has arrived (value is not zero)
}

// Get the remote control RX data
bool AXS1Sensor::getRemoconRX(uint16_t* value) {
    if (!servo->readRegister(id, AXS1_Remocon_RX_Data_0, 2, (uint32_t*)value)) {
        LOG_ERR("Failed to read Remote Control RX Data for ID: " + String(id));
        return false;
    }
    return true;
}
 

// Get the remote control TX data
bool AXS1Sensor::getRemoconTX(uint16_t* value) {
    if (!servo->readRegister(id, AXS1_Remocon_TX_Data_0, 2, (uint32_t*)value)){
        LOG_ERR("Failed to read Remote Control TX Data for ID: " + String(id));
        return false;
    }
    return true;
}

// Set the remote control TX data
bool AXS1Sensor::setRemoconTX(uint16_t value) {
    if (value > 65535) {  // Check if value is within valid range
        LOG_ERR("Invalid Remote Control TX value: " + String(value));
        return false;
    }
    uint8_t txData[2];
    txData[0] = value & 0xFF;         // Low byte
    txData[1] = (value >> 8) & 0xFF;  // High byte
    if (!servo->writeRegister(id, AXS1_Remocon_TX_Data_0, 2, txData)) {
        LOG_ERR("Failed to set Remote Control TX value for ID: " + String(id));
        return false;
    }
    return true;
}

// Print the status of the sensor
bool AXS1Sensor::printStatus() {
    uint8_t     servoID = 0;
    uint16_t    model_number = 0;       //FIXME: number is getting reset by another function!!
    uint8_t     firmware_version = 0;
    uint8_t     baud_rate = 0;
    uint8_t     return_delay = 0;
    uint8_t     status_return = 0;
    
    uint8_t     obstacle_compare = 0;
    uint8_t     light_compare = 0;
    uint8_t     od = 0;
    uint8_t     ld = 0;
    
    uint8_t     sound_data = 0;
    uint8_t     sound_data_max_hold = 0;
    uint8_t     sound_detected_count = 0;
    uint8_t     sound_detected_time = 0;

    uint16_t    remocon_rx = 0;
    uint16_t    remocon_tx = 0;


    if (!getID(&servoID)) return false;
    if (!getFirmwareVersion(&firmware_version)) return false;
    if (!getBaudRate(&baud_rate)) return false;
    if (!getReturnDelayTime(&return_delay)) return false;
    if (!getStatusReturnLevel(&status_return)) return false;

    if (!getObstacleCompare(&obstacle_compare)) return false;
    if (!getLightCompare(&light_compare)) return false;
    if (!ObstacleDetected(&od)) return false;
    if (!LightDetected(&ld)) return false;

    if (!getSoundData(&sound_data)) return false;
    if (!getSoundDataMaxHold(&sound_data_max_hold)) return false;
    if (!getSoundDetectedCount(&sound_detected_count)) return false;
    if (!getSoundDetectedTime(&sound_detected_time)) return false;

    if (!getRemoconRX(&remocon_rx)) return false;
    if (!getRemoconTX(&remocon_tx)) return false;
    
    // FIXME: should be moved to first section
    if (!getModelNumber(&model_number)) return false; //FIXME: number is getting reset by another function!!
    LOG_DBG("Model Number: " + String(model_number)); //FIXME:

    PRINTLN("AXS1 Sensor Status:");
    PRINTLN("AXS1 Sensor ID      : " + String(servoID));
    PRINTLN("Model Number        : " + String(model_number));
    PRINTLN("Firmware Version    : " + String(firmware_version));
    PRINTLN("Baud Rate           : " + String(2000000 / (baud_rate+1)) + " bps"); 
    PRINTLN("Return Delay Time   : " + String(return_delay * 2) + " us");
    PRINTLN("Status Return Level : " + String(status_return));

    PRINTLN("Distance            : L = " + String(getDistanceLeft()) + 
                                 " C = " + String(getDistanceCenter()) + 
                                 " R = " + String(getDistanceRight()));

    PRINTLN("IR                  : L = " + String(getIRLeft()) + 
                                 " C = " + String(getIRCenter()) + 
                                 " R = " + String(getIRRight()));

    PRINTLN("Obstacle Compare    : " + String(obstacle_compare));
    PRINTLN("Light Compare       : " + String(light_compare));

    PRINTLN("Obstacle Detected   : L = " + 
            String(((od&1)==1)? "On " : "Off") + " C = " + 
            String(((od&2)==2)? "On " : "Off") + " R = " + 
            String(((od&4)==4)? "On " : "Off"));

    PRINTLN("Light Detected      : L = " + 
            String(((ld&1)==1)? "On " : "Off") + " C = " + 
            String(((ld&2)==2)? "On " : "Off") + " R = " + 
            String(((ld&4)==4)? "On " : "Off"));

    PRINTLN("Sound Data          : " + String(sound_data));
    PRINTLN("Sound Data Max Hold : " + String(sound_data_max_hold));
    PRINTLN("Sound Detected Count: " + String(sound_detected_count));
    PRINTLN("Sound Detected Time : " + String(sound_detected_time));

    PRINTLN("Remocon RX          : " + String(remocon_rx));
    PRINTLN("Remocon TX          : " + String(remocon_tx));
    PRINTLN("Remocon Arrived     : " + String(RemoconArrived() ? "Yes" : "No"));
    return true;
}

// Process console commands for sensor control
bool AXS1Sensor::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "ap") {
        bool result = ping();
        PRINTLN("Sensor ping: " + String(result ? "SUCCESS" : "FAILED"));
        return true;

    } else if (cmd == "ad") {
        PRINTLN("Distance - Left: " + String(getDistanceLeft()) + 
                   ", Center: " + String(getDistanceCenter()) + 
                   ", Right: " + String(getDistanceRight()));
        return true;

    } else if (cmd == "air") {
        PRINTLN("IR light - Left: " + String(getIRLeft()) + 
                   ", Center: " + String(getIRCenter()) + 
                   ", Right: " + String(getIRRight()));
        return true;

    } else if (cmd == "asoc") {
        uint8_t value = 0;
        if (args.length() > 0) {
            value = args.toInt();
        }
        if (!setObstacleCompare(value)) {
            LOG_ERR("Failed to set Obstacle Compare value to " + String(value));
        } else {
            PRINTLN("Obstacle Compare value set to " + String(value));
        }
        return true;

    } else if (cmd == "agoc") {
        uint8_t obstacle_compare = 0;
        if (!getObstacleCompare(&obstacle_compare)) {
            LOG_ERR("Failed to get Obstacle Compare value");
        } else {
            PRINTLN("Obstacle Compare value: " + String(obstacle_compare));
        }
        return true;

    } else if (cmd == "aslc") {
        uint8_t value = 0;
        if (args.length() > 0) {
            value = args.toInt();
        }
        if (!setLightCompare(value)) {
            LOG_ERR("Failed to set Light Compare value to " + String(value));
        } else {
            PRINTLN("Light Compare value set to " + String(value));
        }
        return true;

    } else if (cmd == "aglc") {
        uint8_t compareValue = 0;
        if (!getLightCompare(&compareValue)) {
            LOG_ERR("Failed to get Light Compare value");
        } else {
            PRINTLN("Light Compare value: " + String(compareValue));
        }
        return true;

    } else if (cmd == "aod") {
        uint8_t od = 0;
        if (!ObstacleDetected(&od)) {
            LOG_ERR("Failed to read Obstacle Detected data");
        } else {
            PRINTLN("Obstacle Detected - Left: " + String((od & 1) == 1 ? "On" : "Off") + 
                    ", Center: " + String((od & 2) == 2 ? "On" : "Off") +
                    ", Right: " + String((od & 4) == 4 ? "On" : "Off"));
        }
        return true;

    } else if (cmd == "ald") {
        uint8_t ld = 0;
        if (!LightDetected(&ld)) {
            LOG_ERR("Failed to read Light Detected data");
        } else {
            PRINTLN("Light Detected - Left: " + String((ld & 1) == 1 ? "On" : "Off") + 
                    ", Center: " + String((ld & 2) == 2 ? "On" : "Off") +
                    ", Right: " + String((ld & 4) == 4 ? "On" : "Off"));
        }
        return true;

    } else if (cmd == "agsd") {
        uint8_t sound_data = 0;
        if (!getSoundData(&sound_data)) {
            LOG_ERR("Failed to read Sound Detected data");
        } else {
            PRINTLN("Sound Detected Data: " + String(sound_data));
        }
        return true;

    } else if (cmd == "agsdmh") {
        uint8_t sound_data_max_hold = 0;
        if (!getSoundDataMaxHold(&sound_data_max_hold)) {
            LOG_ERR("Failed to read Sound Data Max Hold data");
        } else {
            PRINTLN("Sound Data Max Hold Data: " + String(sound_data_max_hold));
        }
        return true;

    } else if (cmd == "agsdc") {
        uint8_t sound_detected_count = 0;
        if (!getSoundDetectedCount(&sound_detected_count)) {
            LOG_ERR("Failed to read Sound Detected Count data");
        } else {
            PRINTLN("Sound Detected Count Data: " + String(sound_detected_count));
        }
        return true;

    } else if (cmd == "agsdt") {
        uint8_t sound_detected_time = 0;
        if (!getSoundDetectedTime(&sound_detected_time)) {
            LOG_ERR("Failed to read Sound Detected Time data");
        } else {
            PRINTLN("Sound Detected Time Data: " + String(sound_detected_time));
        }
        return true;

    } else if (cmd == "arsdmh") {
        if (!resetSoundDataMaxHold()) {
            LOG_ERR("Failed to reset Sound Data Max Hold");
        } else {
            PRINTLN("Sound Data Max Hold reset successfully");
        }
        return true;

    } else if (cmd == "arsdc") {
        if (!resetSoundDetectedCount()) {
            LOG_ERR("Failed to reset Sound Detected Count");
        } else {
            PRINTLN("Sound Detected Count reset successfully");
        }
        return true;

    } else if (cmd == "arsdt") {
        if (!resetSoundDetectedTime()) {
            LOG_ERR("Failed to reset Sound Detected Time");
        } else {
            PRINTLN("Sound Detected Time reset successfully");
        }
        return true;

    } else if (cmd == "apt") {
        uint8_t note = 0;
        uint8_t duration = 0;
        
        if (args.length() > 0) {
            int spaceIndex = args.indexOf(' ');
            if (spaceIndex != -1) {
                note = args.substring(0, spaceIndex).toInt();
                duration = args.substring(spaceIndex + 1).toInt();
            } else {
                note = args.toInt();
            }
        }

        if (!playTone(note, duration)) {
            LOG_ERR("Failed to play tone " + String(note) + " for duration " + String(duration));
        } else {
            PRINTLN("Playing tone " + String(note) + " for duration " + String(duration));
        }
        return true;

    } else if (cmd == "apm") {

        uint8_t note = 0;

        if (args.length() > 0) {
            note = args.toInt();
        }
        
        if (!playMelody(note)) {
            LOG_ERR("Failed to play melody " + String(note));
        } else {
            PRINTLN("Playing melody " + String(note));
        }
        return true;
    
    } else if (cmd == "apn") {
        
        uint8_t melody = 0;

        if (args.length() > 0) {
            if (args == "stop") {
                if (!stopTone()) {
                    LOG_ERR("Failed to stop continuous tone.");
                } else {
                    PRINTLN("Continuous tone stopped successfully.");
                }
                return true;
            }

            melody = args.toInt();
        }

        if (!startTone(melody)) {
            LOG_ERR("Failed to play melody " + String(melody));
        } else {
            PRINTLN("Playing melody " + String(melody));
        }
        return true;

    } else if (cmd == "agrx") {
        uint16_t remoconRX = 0;
        if (!getRemoconRX(&remoconRX)) {
            LOG_ERR("Failed to read Remote Control RX data");
        } else {
            PRINTLN("Remote Control RX Data: " + String(remoconRX));
        }
        return true;

    } else if (cmd == "agtx") {
        uint16_t remoconTX = 0;
        if (!getRemoconTX(&remoconTX)) {
            LOG_ERR("Failed to read Remote Control TX data");
        } else {
            PRINTLN("Remote Control TX Data: " + String(remoconTX));
        }
        return true;

    } else if (cmd == "arxa") {
        bool arrived = RemoconArrived();
        PRINTLN("Remote Control Data Arrived: " + String(arrived ? "Yes" : "No"));
        return true;    

    } else if (cmd == "astx") {
        uint16_t value = 0;
        if (args.length() > 0) {
            value = args.toInt();
        }
        if (!setRemoconTX(value)) {
            LOG_ERR("Failed to set Remote Control TX value to " + String(value));
        } else {
            PRINTLN("Remote Control TX value set to " + String(value));
        }
        return true;

    } else if (cmd == "as") {
              
        printStatus();
        return true;
        
    } else if (cmd == "a?") {
        printConsoleHelp();  // Print help information for sensor commands
        return true;
    
    }
    return false;
}

// Print sensor-specific help information
bool AXS1Sensor::printConsoleHelp() {
    PRINTLN("Sensor Commands (AXS1):");
    PRINTLN("  ap               - Ping sensor (connectivity test)");
    PRINTLN("  as               - Print Sensor status");
    PRINTLN("");
    PRINTLN("  ad               - Read Distance sensors (left, center, right)");
    PRINTLN("  air              - Read IR light sensors (left, center, right)");
    PRINTLN("  asoc [value]     - Set Obstacle Detection Compare value (0-255)");
    PRINTLN("  agoc             - Read Obstacle Detection Compare value");
    PRINTLN("  aslc [value]     - Set Light Detection Compare value (0-255)");
    PRINTLN("  aglc             - Read Light Detection Compare value");
    PRINTLN("  aod              - Read Obstacle Detection (left, center, right)");
    PRINTLN("  ald              - Read Light Detection (left, center, right)");
    PRINTLN("");
    PRINTLN("  agsd             - Read Sound Data");
    PRINTLN("  agsdmh           - Read Sound Data Max Hold");
    PRINTLN("  agsdc            - Read Sound Detected Count");
    PRINTLN("  agsdt            - Read Sound Detected Time");
    PRINTLN("  arsdmh           - Reset Sound Data Max Hold");
    PRINTLN("  arsdc            - Reset Sound Detected Count");
    PRINTLN("  arsdt            - Reset Sound Detected Time");
    PRINTLN("");
    PRINTLN("  apt [note] [dur] - Play Tone (note 0-51, duration 0-50");
    PRINTLN("  apm [melody]     - Play Melody (0-26)");
    PRINTLN("  apn [note]       - Start continuous tone (note 0-51)");
    PRINTLN("  apn stop         - Stop continuous tone");
    PRINTLN("");
    PRINTLN("  agrx             - Read Remote Control RX data");
    PRINTLN("  agtx             - Read Remote Control TX data");
    PRINTLN("  arxa             - Check if Remote Control data has arrived");
    PRINTLN("  astx [value]     - Set Remote Control TX data (0-65535)");
    PRINTLN("");
    PRINTLN("  a?               - Show this help message");
    PRINTLN("");
    return true;
}