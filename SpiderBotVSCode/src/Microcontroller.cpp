#include "Microcontroller.h"
#include "pitches.h"            // Include pitches for melody notes
#include <Arduino.h>            // Include Arduino core library
#include "Config.h"             // Include configuration header


// Constructor for Microcontroller
Microcontroller::Microcontroller() {}

// Initialize the microcontroller
bool Microcontroller::begin() {
    // Initialization code for the microcontroller
    int leds[] = {LED_BUILTIN, BDPIN_LED_USER_1, BDPIN_LED_USER_2, BDPIN_LED_USER_3, BDPIN_LED_USER_4, BDPIN_LED_STATUS}; // User control LEDs
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        ledOn(leds[i]);  // Turn on all user control LEDs
    }

    playMelody();                 // Play a melody using the buzzer
    #ifdef DEBUG
        Serial.println("Dynamixel Controller initialized successfully.");
        Serial.print("Dynamixel Controller Battery Voltage: ");
        Serial.println(batteryVoltage());
    #endif // DEBUG
    
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        ledOff(leds[i]); // Turn off all user control LEDs
    }
    return true;  // Return true to indicate successful initialization
}

// Update the microcontroller state
void Microcontroller::update() {
    // Update code for the microcontroller
    // This can include reading sensors, updating states, etc.
    // Currently, no specific update logic is implemented.
}

// Turn on a LED on OpenCR1.0 board
bool Microcontroller::ledOn(uint8_t LED_id) {
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
bool Microcontroller::ledOff(uint8_t LED_id) {
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
float Microcontroller::batteryVoltage() {
    int adc_value = analogRead(BDPIN_BAT_PWR_ADC);          // Read the ADC value from the battery power pin
    return (map(adc_value, 0, 1023, 0, 330*57/10)/100.0);   // Convert ADC value to voltage (assuming 3300mV reference and 57:10 voltage divider)
}

// Play a melody using OpenCR1.0 Buzzer
bool Microcontroller::playMelody() {

    int melody[] = {NOTE_C7, NOTE_G3, NOTE_C6};             // melody notes
    int noteDurations[] = {8, 4, 8};                        // note durations: 4 = quarter note, 8 = eighth note, etc.:

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