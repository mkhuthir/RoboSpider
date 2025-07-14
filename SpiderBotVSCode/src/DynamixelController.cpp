#include "DynamixelController.h"
#include <vector>
#include "pitches.h"            // Include pitches for melody notes


DynamixelController::DynamixelController() {}
// Constructor initializes the DynamixelController
bool DynamixelController::init(const char* device_name, uint32_t baudrate) {
   
    int leds[] = {LED_BUILTIN, BDPIN_LED_USER_1, BDPIN_LED_USER_2, BDPIN_LED_USER_3, BDPIN_LED_USER_4, BDPIN_LED_STATUS}; // User control LEDs
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        ucLEDOn(leds[i]);  // Turn on all user control LEDs
    }
    
    ucPlayMelody();                 // Play a melody using the buzzer
    
    Serial.println("Dynamixel Controller initialized successfully.");
    Serial.print("Battery Voltage: ");
    Serial.println(ucBatteryVoltage()); 
    
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        ucLEDOff(leds[i]); // Turn off all user control LEDs
    }    

    result = dxl_wb.init(device_name, baudrate, &log);
    if (!result)  // If initialization fails
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
    result = result && servoLEDOn(dxl_id);          // Turn on LED for the servo

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
bool DynamixelController::servoLEDOn(uint8_t dxl_id) {
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
bool DynamixelController::servoLEDOff(uint8_t dxl_id) {
    result = dxl_wb.ledOff(dxl_id, &log);
    if (!result)  // If turning off LED fails
    {
        Serial.println(log);
        Serial.print("Failed to turn off LED for id: ");
        Serial.println(dxl_id);
    }
    return result;
}

// Turn on a LED on OpenCR1.0 board
bool DynamixelController::ucLEDOn(uint8_t LED_id) {
    // OpenCR1.0 LED IDs are as follows:
    // LED_BUILTIN             13
    // BDPIN_LED_USER_1        22
    // BDPIN_LED_USER_2        23
    // BDPIN_LED_USER_3        24
    // BDPIN_LED_USER_4        25
    // BDPIN_LED_STATUS        36

    if (LED_id < LED_BUILTIN || LED_id > BDPIN_LED_STATUS) {    // Check if the LED ID is valid
        Serial.println("Invalid LED ID!");
        return false;                                           // Return false if LED ID is invalid
    }
    pinMode(LED_id, OUTPUT);    // Set the LED pin as output
    digitalWrite(LED_id, LOW);  // Turn on the user control LED
    return true;                // Return true as the operation is successful
}

// Turn off a LED on OpenCR1.0 board
bool DynamixelController::ucLEDOff(uint8_t LED_id) {
    // OpenCR1.0 LED IDs are as follows:
    // LED_BUILTIN             13
    // BDPIN_LED_USER_1        22
    // BDPIN_LED_USER_2        23
    // BDPIN_LED_USER_3        24
    // BDPIN_LED_USER_4        25
    // BDPIN_LED_STATUS        36

    if (LED_id < LED_BUILTIN || LED_id > BDPIN_LED_STATUS) {    // Check if the LED ID is valid
        Serial.println("Invalid LED ID!");                      
        return false;                                           // Return false if LED ID is invalid
    }
    pinMode(LED_id, OUTPUT);            // Set the LED pin as output
    digitalWrite(LED_id, HIGH);         // Turn off the user control LED
    return true;                        // Return true as the operation is successful
}

// Get the battery voltage from the ADC pin on OpenCR1.0 board
float DynamixelController::ucBatteryVoltage() {
    int adc_value = analogRead(BDPIN_BAT_PWR_ADC);          // Read the ADC value from the battery power pin
    return (map(adc_value, 0, 1023, 0, 330*57/10)/100.0);   // Convert ADC value to voltage (assuming 3300mV reference and 57:10 voltage divider)
}

// Play a melody using OpenCR1.0 Buzzer
bool DynamixelController::ucPlayMelody() {
    
    int melody[] = {NOTE_C4, NOTE_G3, NOTE_C4};             // melody notes
    int noteDurations[] = {4, 2, 4};                        // note durations: 4 = quarter note, 8 = eighth note, etc.:

    if (BDPIN_BUZZER < 0) {                                 // Check if the buzzer pin is defined
        Serial.println("Buzzer pin not defined!");
        return false;
    } else {
        for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
            int noteDuration = 1000 / noteDurations[thisNote];  // Calculate the note duration
            tone(BDPIN_BUZZER, melody[thisNote], noteDuration); // Play the note on the buzzer
            int pauseBetweenNotes = noteDuration * 1.30;        // Pause between notes
            delay(pauseBetweenNotes);                           // Wait for the note duration plus a pause
            noTone(BDPIN_BUZZER);                               // Stop the tone playing
        }
    }
    return true;
}

// Return the DynamixelWorkbench instance
DynamixelWorkbench* DynamixelController::getWorkbench() {
    return &dxl_wb;
}