#include "DynamixelController.h"
#include <vector>
#include "pitches.h"            // Include pitches for melody notes


DynamixelController::DynamixelController() {}
// Constructor initializes the DynamixelController
bool DynamixelController::begin(const char* device_name, uint32_t baudrate) {
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
        Serial.println("Succeeded to ping!");
        Serial.print("dxl_id : ");
        Serial.print(dxl_id);
        Serial.print(" model_number : ");
        Serial.println(model_number);
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
        Serial.print("Set joint mode for id: ");
        Serial.println(dxl_id);
    }
    return result;
}

// Initialize a servo with default settings
bool DynamixelController::initServo(uint8_t dxl_id) {
    result = jointMode(dxl_id);   // Set servo to joint mode
    if (!result) {
        Serial.print("Failed to initialize servo with ID: ");
        Serial.println(dxl_id);
    } else {
        Serial.print("Servo with ID: ");
        Serial.print(dxl_id);
        Serial.println(" initialized successfully.");
    }
    return result;
}

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

// Play a melody using OpenCR Buzzer
bool DynamixelController::playMelody() {
    
    int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};  // melody notes
    int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};                                     // note durations: 4 = quarter note, 8 = eighth note, etc.:

    // Check if the buzzer pin is defined
    if (BDPIN_BUZZER < 0) {
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