#include "Microcontroller.h"
#include "pitches.h"
#include "Console.h"
#include "Debug.h"



// Constructor for Microcontroller
Microcontroller::Microcontroller() {}

// Initialize the microcontroller
bool Microcontroller::begin() {

    // Initialization code for the microcontroller
    int leds[] = {LED_BUILTIN, BDPIN_LED_USER_1, BDPIN_LED_USER_2, BDPIN_LED_USER_3, BDPIN_LED_USER_4, BDPIN_LED_STATUS}; // User control LEDs
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        ledOn(leds[i]);  // Turn on all user control LEDs
    }

    // Check Battery connection and voltage
    if (!checkBattery()) {
        LOG_ERR("Battery check failed!");    // Print error message if battery check fails
        return false;                               // Return false if battery check fails
    }
    LOG_INF("Microcontroller Battery Voltage: " + String(getBatteryVoltage()));    
    LOG_INF("Microcontroller initialized successfully.");

    // Turn off all user control LEDs after initialization
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        ledOff(leds[i]);
    }
    return true;         // Return true as the initialization is successful
}

// Update the microcontroller state
bool Microcontroller::update() {
    // TODO: Add any periodic update logic here if needed
    return true;
}

// OpenCR1.0 LED IDs are as follows:
// LED_BUILTIN             13
// BDPIN_LED_USER_1        22
// BDPIN_LED_USER_2        23
// BDPIN_LED_USER_3        24
// BDPIN_LED_USER_4        25
// BDPIN_LED_STATUS        36

// Turn on a LED on OpenCR1.0 board
bool Microcontroller::ledOn(uint8_t LED_id) {

    if (LED_id < LED_BUILTIN || LED_id > BDPIN_LED_STATUS) {    // Check if the LED ID is valid
        LOG_ERR("Invalid LED ID!");
        return false;                                           // Return false if LED ID is invalid
    }
    pinMode(LED_id, OUTPUT);         // Set the LED pin as output
    if (LED_id == LED_BUILTIN) {
        digitalWrite(LED_id, HIGH);  // Turn on the built-in LED
    } else {
        digitalWrite(LED_id, LOW);   // Turn on the user control LED
    }
    return true;                // Return true as the operation is successful
}

// Turn off a LED on OpenCR1.0 board
bool Microcontroller::ledOff(uint8_t LED_id) {

    if (LED_id < LED_BUILTIN || LED_id > BDPIN_LED_STATUS) {    // Check if the LED ID is valid
        LOG_ERR("Invalid LED ID!");
        return false;                                           // Return false if LED ID is invalid
    }
    pinMode(LED_id, OUTPUT);            // Set the LED pin as output
    if (LED_id == LED_BUILTIN) {
        digitalWrite(LED_id, LOW);      // Turn off the built-in LED
    } else {
        digitalWrite(LED_id, HIGH);     // Turn off the user control LED
    }
    return true;                        // Return true as the operation is successful
}

// Play a melody using OpenCR1.0 Buzzer
bool Microcontroller::playMelody() {

    int melody[] = {NOTE_C7, NOTE_G3, NOTE_C6};             // melody notes
    int noteDurations[] = {8, 4, 8};                        // note durations: 4 = quarter note, 8 = eighth note, etc.:

    if (BDPIN_BUZZER < 0) {                                 // Check if the buzzer pin is defined
        LOG_ERR("Buzzer pin not defined!");
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

// Get the battery voltage from the ADC pin on OpenCR1.0 board
float Microcontroller::getBatteryVoltage() {
    int adc_value = analogRead(BDPIN_BAT_PWR_ADC);          // Read the ADC value from the battery power pin
    return (map(adc_value, 0, 1023, 0, 330*57/10)/100.0);   // Convert ADC value to voltage (assuming 3300mV reference and 57:10 voltage divider)
}

// Check the battery status by reading the battery voltage
bool Microcontroller::checkBattery() {
    float voltage = getBatteryVoltage();                    // Get the battery voltage
    if (voltage < 6.0) {                                    // Check if the voltage is below the threshold
        LOG_WRN("Battery voltage is low!");                // Print warning message
    }
    return voltage >= 6.0;                                  // Return true if battery voltage is sufficient
}   

// Get the uptime of the microcontroller
unsigned long Microcontroller::getUpTime() {
    return millis();
}

void Microcontroller::resetMicrocontroller() {
    NVIC_SystemReset();
}

// Print the current status of the microcontroller to the given stream
bool Microcontroller::printStatus() {
    PRINTLN("Microcontroller Status:\n\r");
    PRINTLN("Microcontroller Uptime: " + String(getUpTime()) + " ms");
    PRINTLN("Battery Voltage       : " + String(getBatteryVoltage())+" V");
    PRINTLN("Battery Status        : " + String(checkBattery() ? "PASSED" : "FAILED"));
    PRINT(String("LEDS -> Built-in:") + ((digitalRead(LED_BUILTIN) == HIGH) ? "On " : "Off"));
    PRINT(String(" | User1:") + ((digitalRead(BDPIN_LED_USER_1) == LOW) ? "On " : "Off"));
    PRINT(String(" | User2:") + ((digitalRead(BDPIN_LED_USER_2) == LOW) ? "On " : "Off"));
    PRINT(String(" | User3:") + ((digitalRead(BDPIN_LED_USER_3) == LOW) ? "On " : "Off"));
    PRINT(String(" | User4:") + ((digitalRead(BDPIN_LED_USER_4) == LOW) ? "On " : "Off"));
    PRINTLN(String(" | Status:") + ((digitalRead(BDPIN_LED_STATUS) == LOW) ? "On " : "Off"));
    return true;

}

// Process console commands for microcontroller control
bool Microcontroller::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "ms") {
        if (!printStatus()) {
            LOG_ERR("Failed to print status");
        }
        return true;

    } else if (cmd == "mbv") {
        PRINTLN("Battery Voltage: " + String(getBatteryVoltage(), 4) + "V");
        return true;
        
    } else if (cmd == "mbc") {
        PRINTLN("Battery check: " + String(checkBattery()? "PASSED" : "FAILED"));
        return true;
        
    } else if (cmd == "mpm") {
        PRINTLN("Play melody: " + String(playMelody() ? "SUCCESS" : "FAILED"));
        return true;
        
    } else if (cmd == "mlon") {
        // Turn on LED: mlon [led_id] (1-4 for user LEDs, 0 for built-in, 5 for status)
        int ledId = 1; // Default to user LED 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 0 && parsedId <= 5) {
                ledId = parsedId;
            }
        }
        
        uint8_t actualLedPin;
        String ledName;
        switch (ledId) {
            case 0:  actualLedPin = LED_BUILTIN;      ledName = "Built-in"; break;
            case 1:  actualLedPin = BDPIN_LED_USER_1; ledName = "User1";    break;
            case 2:  actualLedPin = BDPIN_LED_USER_2; ledName = "User2";    break;
            case 3:  actualLedPin = BDPIN_LED_USER_3; ledName = "User3";    break;
            case 4:  actualLedPin = BDPIN_LED_USER_4; ledName = "User4";    break;
            case 5:  actualLedPin = BDPIN_LED_STATUS; ledName = "Status";   break;
            default: actualLedPin = BDPIN_LED_USER_1; ledName = "User1";    break;
        }
        
        PRINTLN(ledName + " LED turned " + String(ledOn(actualLedPin) ? "ON" : "FAILED"));
        return true;
        
    } else if (cmd == "mloff") {
        // Turn off LED: mloff [led_id] (1-4 for user LEDs, 0 for built-in, 5 for status)
        int ledId = 1; // Default to user LED 1
        if (args.length() > 0) {
            int parsedId = args.toInt();
            if (parsedId >= 0 && parsedId <= 5) {
                ledId = parsedId;
            }
        }
        
        uint8_t actualLedPin;
        String ledName;
        switch (ledId) {
            case 0:  actualLedPin = LED_BUILTIN;      ledName = "Built-in"; break;
            case 1:  actualLedPin = BDPIN_LED_USER_1; ledName = "User1";    break;
            case 2:  actualLedPin = BDPIN_LED_USER_2; ledName = "User2";    break;
            case 3:  actualLedPin = BDPIN_LED_USER_3; ledName = "User3";    break;
            case 4:  actualLedPin = BDPIN_LED_USER_4; ledName = "User4";    break;
            case 5:  actualLedPin = BDPIN_LED_STATUS; ledName = "Status";   break;
            default: actualLedPin = BDPIN_LED_USER_1; ledName = "User1";    break;
        }
        
        PRINTLN(ledName + " LED turned " + String(ledOff(actualLedPin) ? "OFF" : "FAILED"));
        return true;

    } else if (cmd == "mu") {
        PRINTLN("Microcontroller Uptime: " + String(getUpTime()) + " ms");
        return true;

    } else if (cmd == "mr") {
        resetMicrocontroller();
        return true;


            
    } else if (cmd == "m?") {
        printConsoleHelp();
        return true;
            
    }   
    return false;
}

// Print microcontroller-specific help information
bool Microcontroller::printConsoleHelp() {
    PRINTLN("Microcontroller Commands:\n\r");
    PRINTLN("  ms               - Show microcontroller status");
    PRINTLN("");
    PRINTLN("  mbv              - Read battery voltage");
    PRINTLN("  mbc              - Check battery status");
    PRINTLN("");
    PRINTLN("  mpm              - Play melody");
    PRINTLN("  mlon [0-5]       - Turn on LED (0=builtin, 1-4=user, 5=status)");
    PRINTLN("  mloff [0-5]      - Turn off LED (0=builtin, 1-4=user, 5=status)");
    PRINTLN("");
    PRINTLN("  mu               - Show microcontroller uptime in milliseconds");
    PRINTLN("  mr               - Reset the microcontroller");
    PRINTLN("");
    PRINTLN("  m?               - Show this help");
    PRINTLN("");
    return true;
}