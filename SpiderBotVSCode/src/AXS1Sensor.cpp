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
    if (servo == nullptr) {
        return false;               
    }

    this->servo = servo;
    id  =   sensor_id;

    setObstacleCompare(AXS1_OBSTACLE_DETECTED);     // Set default obstacle detection threshold
    setLightCompare(AXS1_LIGHT_DETECTED);           // Set default light detection threshold

    resetSoundDataMaxHold();
    resetSoundDetectedCount();
    

    return true;
}

// Update method to refresh sensor data
bool AXS1Sensor::update() {

    if (RemoconArrived()) {             // Check if remote control signal has arrived
        
        int remoconRX = getRemoconRX(); // Read remote control RX data
        
        if (remoconRX == -1) {
            LOG_ERR("Failed to read Remote Control RX data");
            return false;               
        }
        LOG_INF("Remote Control RX Data: " + String(remoconRX));
        playTone(40, 1);
    }

    if (ObstacleDetected()) {           // Check if obstacle is detected
        playTone(30, 1);
    }

    if (getSoundDetectedCount() == 3) {
        LOG_INF("Sound detected");
        playMelody(21);
        resetSoundDataMaxHold();
        resetSoundDetectedCount();
        //TODO: resetSoundDetectedTime()
    }

    return true;
}

// Ping method to check connectivity with the sensor
bool AXS1Sensor::ping() {
  return servo->ping(id);
}

// Get the model number of the sensor
int AXS1Sensor::getModelNumber() {
    
    uint32_t model_number_l = 0;
    uint32_t model_number_h = 0;

    if (!servo->readRegister(id, AXS1_Model_Number_L, 1, &model_number_l)) {
        LOG_ERR("Failed to read Model Number for ID: " + String(id));
        return -1;
    }

    if (!servo->readRegister(id, AXS1_Model_Number_H, 1, &model_number_h)) {
        LOG_ERR("Failed to read Model Number for ID: " + String(id));
        return -1;
    }

    return static_cast<int>((model_number_h << 8) | model_number_l);  // Combine high and low bytes to get full model number
}

// Get the firmware version of the sensor
int AXS1Sensor::getFirmwareVersion() {
    
    uint32_t version = 0;

    if (servo->readRegister(id, AXS1_Firmware_Version, 1, &version)) {
        return static_cast<int>(version);
    }
    LOG_ERR("Failed to read Firmware Version for ID: " + String(id));
    return -1;
}

// Get the ID of the sensor
int AXS1Sensor::getID() {
    uint32_t id_value = 0;  // Variable to hold the sensor ID

    if (servo->readRegister(id, AXS1_ID, 1, &id_value)) {
        return static_cast<int>(id_value);
    }
    LOG_ERR("Failed to read ID for ID: " + String(id));
    return -1;
}

// Get the baud rate of the sensor
int AXS1Sensor::getBaudRate() {
    uint32_t baud_rate = 0;  // Variable to hold the baud rate

    if (servo->readRegister(id, AXS1_Baud_Rate, 1, &baud_rate)) {
        return static_cast<int>(baud_rate);
    }
    LOG_ERR("Failed to read Baud Rate for ID: " + String(id));
    return -1;
}

// Get the return delay time of the sensor
int AXS1Sensor::getReturnDelayTime() {
    uint32_t return_delay = 0;  // Variable to hold the return delay time

    if (servo->readRegister(id, AXS1_Return_Delay_Time, 1, &return_delay)) {
        return static_cast<int>(return_delay);
    }
    LOG_ERR("Failed to read Return Delay Time for ID: " + String(id));
    return -1;
}

// Get the status return level of the sensor
int AXS1Sensor::getStatusReturnLevel() {
    uint32_t status_return = 0;  // Variable to hold the status return level

    if (servo->readRegister(id, AXS1_Status_Return_Level, 1, &status_return)) {
        return static_cast<int>(status_return);
    }
    LOG_ERR("Failed to read Status Return Level for ID: " + String(id));
    return -1;
}   

// Get the distance data from the IR left side sensor
int AXS1Sensor::getDistanceLeft() {

    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Left_Distance_Data, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read IR Left Distance Data for ID: " + String(id));
    return FALSE;
}

// Get the distance data from the IR center sensor
int AXS1Sensor::getDistanceCenter() {

    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Center_Distance_Data, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read IR Center Distance Data for ID: " + String(id));
    return FALSE;
}

// Get the distance data from the IR right side sensor
int AXS1Sensor::getDistanceRight() {

    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Right_Distance_Data, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read IR Right Distance Data for ID: " + String(id));
    return FALSE;
}

// Get the IR light left side sensor data
int AXS1Sensor::getIRLeft() {

    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Light_Left_Data, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read IR Left Data for ID: " + String(id));
    return FALSE;
}

// Get the IR light center sensor data
int AXS1Sensor::getIRCenter() {

    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Light_Center_Data, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read IR Center Data for ID: " + String(id));
    return FALSE;
}

// Get the IR light right side sensor data
int AXS1Sensor::getIRRight() {

    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Light_Right_Data, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read IR Right Data for ID: " + String(id));
    return FALSE;
}

// Set the obstacle detection comparison value
int AXS1Sensor::setObstacleCompare(uint8_t value) {

    if (servo->writeRegister(id, AXS1_IR_Obstacle_Detect_Compare, 1, &value)) {
        LOG_INF("Obstacle Compare set to: " + String(value) + " for ID: " + String(id));
        return value;
    }
    LOG_ERR("Failed to set Obstacle Compare for ID: " + String(id));
    return FALSE;
}

// Get the obstacle detection comparison value
int AXS1Sensor::getObstacleCompare() {
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_IR_Obstacle_Detect_Compare, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read Obstacle Compare Data for ID: " + String(id));
    return FALSE;
}

// Get the obstacle detected status
int AXS1Sensor::ObstacleDetected() {
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_IR_Obstacle_Detected, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read Obstacle Detected Data for ID: " + String(id));
    return FALSE;
}

// Set the light detection comparison value
int AXS1Sensor::setLightCompare(uint8_t value) {

    if (servo->writeRegister(id, AXS1_Light_Detect_Compare, 1, &value)) {
        LOG_INF("Light Compare set to: " + String(value) + " for ID: " + String(id));
        return value;
    }
    LOG_ERR("Failed to set Light Compare for ID: " + String(id));
    return FALSE;
}

// Get the light detection comparison value
int AXS1Sensor::getLightCompare() {
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Light_Detect_Compare,  1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read Light Compare Data for ID: " + String(id));
    return FALSE;
}

// Get the light detected status
int AXS1Sensor::LightDetected() {
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Light_Detected, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read Light Detected Data for ID: " + String(id));
    return FALSE;
}

// Get the sound data from the sensor
int AXS1Sensor::getSoundData() {
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Sound_Data, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read Sound Data for ID: " + String(id));
    return FALSE;
}

// Get the maximum hold sound data from the sensor
int AXS1Sensor::getSoundDataMaxHold() {
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Sound_Data_Max_Hold, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read Sound Data Max Hold for ID: " + String(id));
    return FALSE;
}

// Get the count of sound detected by the sensor
int AXS1Sensor::getSoundDetectedCount() {
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_Sound_Detected_Count, 1, &val))
        return static_cast<int>(val);
    LOG_ERR("Failed to read Sound Detected Count for ID: " + String(id));
    return FALSE;
}

// Get the time of sound detected by the sensor
int AXS1Sensor::getSoundDetectedTime() {
    uint32_t val_l = 0;
    uint32_t val_h = 0;
    if (servo->readRegister(id, AXS1_Sound_Detected_Time_L, 1, &val_l) &&
        servo->readRegister(id, AXS1_Sound_Detected_Time_H, 1, &val_h)) {
        return (static_cast<int>(val_h) << 8) | static_cast<int>(val_l);
    }
    LOG_ERR("Failed to read Sound Detected Time for ID: " + String(id));
    return FALSE;
}

// Reset the maximum hold sound data
bool AXS1Sensor::resetSoundDataMaxHold() {
    
    uint8_t value = 0;

    if (servo->writeRegister(id, AXS1_Sound_Data_Max_Hold, 1, &value)) {
        LOG_INF("Sound Data Max Hold reset for ID: " + String(id));
        return true;
    }
    LOG_ERR("Failed to reset Sound Data Max Hold for ID: " + String(id));
    return false;
}

// Reset the sound detected count
bool AXS1Sensor::resetSoundDetectedCount() {
    
    uint8_t value = 0;

    if (servo->writeRegister(id, AXS1_Sound_Detected_Count, 1, &value)) {
        LOG_INF("Sound Detected Count reset for ID: " + String(id));
        return true;
    }
    LOG_ERR("Failed to reset Sound Detected Count for ID: " + String(id));
    return false;
}

// Reset the sound detected time
bool AXS1Sensor::resetSoundDetectedTime() {
    //FIXME: reset not working.
    uint8_t value = 1;

    if (servo->writeRegister(id, AXS1_Sound_Detected_Time_L, 1, &value) &&
        servo->writeRegister(id, AXS1_Sound_Detected_Time_H, 1, &value)) {
        LOG_INF("Sound Detected Time reset for ID: " + String(id));
        return true;
    }
    LOG_ERR("Failed to reset Sound Detected Time for ID: " + String(id));
    return false;
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

    LOG_INF("Buzzer set for ID: " + String(id) + " - Note: " + String(note) + ", Duration: " + String(duration));
    return true;  // Return true to indicate success
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

    LOG_INF("Playing melody: " + String(melody));
    return true;  // Return true to indicate success
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

    LOG_INF("Started tone for ID: " + String(id) + " - Note: " + String(note));
    return true;  // Return true to indicate success
}

// Stop the continuous tone on the sensor's buzzer
bool AXS1Sensor::stopTone() {

    uint8_t duration = 0;  // value to stop continuous tone

    if (!servo->writeRegister(id, AXS1_Buzzer_Data_1, 1, &duration)) {
        LOG_ERR("Failed to stop tone for ID: " + String(id));
        return false;
    }

    LOG_INF("Stopped tone for ID: " + String(id));
    return true;  // Return true to indicate success
}

// Check if the remote control data has arrived
bool AXS1Sensor::RemoconArrived() {         //TODO: validation using remote to be done.
    uint32_t val = 0;
    if (servo->readRegister(id, AXS1_IR_Remocon_Arrived, 1, &val)) {
        return (val != 0);  // Return true if data is received
    }
    LOG_ERR("Failed to read Remote Control Data for ID: " + String(id));
    return false;  // Return false if reading fails
}

// Get the remote control RX data
int AXS1Sensor::getRemoconRX() {
    uint32_t val_l = 0;
    uint32_t val_h = 0;
    if (servo->readRegister(id, AXS1_Remocon_RX_Data_0, 1, &val_l) &&
        servo->readRegister(id, AXS1_Remocon_RX_Data_1, 1, &val_h)) {
        return static_cast<int>((val_h << 8) | val_l);  // Combine high and low bytes to get full RX data
    }
    LOG_ERR("Failed to read Remote Control RX Data for ID: " + String(id));
    return -1;  // Return -1 if reading fails
}
 

// Get the remote control TX data
int AXS1Sensor::getRemoconTX() {
    uint32_t val_l = 0;
    uint32_t val_h = 0;
    if (servo->readRegister(id, AXS1_Remocon_TX_Data_0, 1, &val_l) &&
        servo->readRegister(id, AXS1_Remocon_TX_Data_1, 1, &val_h)) {
        return static_cast<int>((val_h << 8) | val_l);  // Combine high and low bytes to get full TX data
    }
    LOG_ERR("Failed to read Remote Control TX Data for ID: " + String(id));
    return -1;  // Return -1 if reading fails
}

// Set the remote control TX data
bool AXS1Sensor::setRemoconTX(uint16_t value) {
    
    if (value > 65535) {  // Check if value is within valid range
        LOG_ERR("Invalid Remote Control TX value: " + String(value));
        return false;
    }

    uint8_t value_l = value & 0xFF;        
    uint8_t value_h = (value >> 8) & 0xFF;

    if (servo->writeRegister(id, AXS1_Remocon_TX_Data_0, 1, &value_l) &&
        servo->writeRegister(id, AXS1_Remocon_TX_Data_1, 1, &value_h)) {
        LOG_INF("Remote Control TX value set to: " + String(value) +     " for ID: " + String(id));
        return true;
    }
    LOG_ERR("Failed to set Remote Control TX value for ID: " + String(id));
    return false;
}

// Print the status of the sensor
void AXS1Sensor::printStatus() {
    PRINTLN("AXS1 Sensor Status:");
    PRINTLN("AXS1 Sensor ID      : " + String(getID()));
    PRINTLN("Model Number        : " + String(getModelNumber()));
    PRINTLN("Firmware Version    : " + String(getFirmwareVersion()));
    PRINTLN("Baud Rate           : " + String(getBaudRate()));
    PRINTLN("Return Delay Time   : " + String(getReturnDelayTime()));
    PRINTLN("Status Return Level : " + String(getStatusReturnLevel()));
    PRINTLN("Obstacle Compare    : " + String(getObstacleCompare()));
    PRINTLN("Light Compare       : " + String(getLightCompare()));
    PRINTLN("Distance            : L = " + String(getDistanceLeft()) + " C = " + String(getDistanceCenter()) + " R = " + String(getDistanceRight()));
    PRINTLN("IR                  : L = " + String(getIRLeft()) + " C = " + String(getIRCenter()) + " R = " + String(getIRRight()));
    uint8_t od = ObstacleDetected();
    PRINTLN("Obstacle Detected   : L = " + 
            String(((od&1)==1)? "On " : "Off") + " C = " + 
            String(((od&2)==2)? "On " : "Off") + " R = " + 
            String(((od&4)==4)? "On " : "Off"));
    uint8_t ld = LightDetected();
    PRINTLN("Light Detected      : L = " + 
            String(((ld&1)==1)? "On " : "Off") + " C = " + 
            String(((ld&2)==2)? "On " : "Off") + " R = " + 
            String(((ld&4)==4)? "On " : "Off"));
    PRINTLN("Sound Data          : " + String(getSoundData()));
    PRINTLN("Sound Data Max Hold : " + String(getSoundDataMaxHold()));
    PRINTLN("Sound Detected Count: " + String(getSoundDetectedCount()));
    PRINTLN("Sound Detected Time : " + String(getSoundDetectedTime()));
    PRINTLN("Remocon RX          : " + String(getRemoconRX()));
    PRINTLN("Remocon TX          : " + String(getRemoconTX()));
    PRINTLN("Remocon Arrived     : " + String(RemoconArrived() ? "Yes" : "No"));
    
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
        int compareValue = getObstacleCompare();
        if (compareValue == -1) {
            LOG_ERR("Failed to get Obstacle Compare value");
        } else {
            PRINTLN("Obstacle Compare value: " + String(compareValue));
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
        int compareValue = getLightCompare();
        if (compareValue == -1) {
            LOG_ERR("Failed to get Light Compare value");
        } else {
            PRINTLN("Light Compare value: " + String(compareValue));
        }
        return true;

    } else if (cmd == "aod") {
        uint8_t od = ObstacleDetected();
        PRINTLN("Obstacle Detected - Left: " + String((od & 1) == 1 ? "On" : "Off") + 
                   ", Center: " + String((od & 2) == 2 ? "On" : "Off") +
                   ", Right: " + String((od & 4) == 4 ? "On" : "Off"));
        return true;

    } else if (cmd == "ald") {
        uint8_t ld = LightDetected();
        PRINTLN("Light Detected - Left: " + String((ld & 1) == 1 ? "On" : "Off") + 
                   ", Center: " + String((ld & 2) == 2 ? "On" : "Off") +
                   ", Right: " + String((ld & 4) == 4 ? "On" : "Off"));
        return true;

    } else if (cmd == "agsd") {

        int soundData = getSoundData();

        if (soundData == -1) {
            LOG_ERR("Failed to read Sound Detected data");
        } else {
            PRINTLN("Sound Detected Data: " + String(soundData));
        }
        return true;

    } else if (cmd == "agsdmh") {
        
        int soundDataMaxHold = getSoundDataMaxHold();

        if (soundDataMaxHold == -1) {
            LOG_ERR("Failed to read Sound Data Max Hold data");
        } else {
            PRINTLN("Sound Data Max Hold Data: " + String(soundDataMaxHold));
        }
        return true;

    } else if (cmd == "agsdc") {

        int soundDetectedCount = getSoundDetectedCount();

        if (soundDetectedCount == -1) {
            LOG_ERR("Failed to read Sound Detected Count data");
        } else {
            PRINTLN("Sound Detected Count Data: " + String(soundDetectedCount));
        }
        return true;

    } else if (cmd == "agsdt") {

        int soundDetectedTime = getSoundDetectedTime();

        if (soundDetectedTime == -1) {
            LOG_ERR("Failed to read Sound Detected Time data");
        } else {
            PRINTLN("Sound Detected Time Data: " + String(soundDetectedTime));
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
        int remoconRX = getRemoconRX();
        if (remoconRX == -1) {
            LOG_ERR("Failed to read Remote Control RX data");
        } else {
            PRINTLN("Remote Control RX Data: " + String(remoconRX));
        }
        return true;

    } else if (cmd == "agtx") {
        int remoconTX = getRemoconTX();
        if (remoconTX == -1) {
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
void AXS1Sensor::printConsoleHelp() {
    PRINTLN("Sensor Commands (AXS1):");
    PRINTLN("  ap               - Ping sensor (connectivity test)");
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
    PRINTLN("  as               - Print Sensor status");
    PRINTLN("  a?               - Show this help message");
    PRINTLN("");
}