

// include necessary libraries and headers
#include <Arduino.h>                    // Include Arduino core library

#include "Config.h"                     // Include configuration header
#include "Microcontroller.h"            // Include Microcontroller class for managing OpenCR1.0 board
#include "Servo.h"                      // Include Servo class for managing Dynamixel servos
#include "Remotecontroller.h"           // Include RemoteController class for managing remote controller input
#include "Hexapod.h"                    // Include Hexapod class for managing the hexapod robot
#include "Turret.h"                     // Include Turret class for managing the sensor turret
#include "AXS1Sensor.h"                 // Include AXS1Sensor class for managing the AX-S1 sensor
#include "GaitController.h"             // Include GaitController class for managing the gait of the hexapod


// Global variables and instances

Microcontroller     mc;
Servo               servo;  
Remotecontroller    rc;     
Hexapod             hexapod;
Turret              turret; 
AXS1Sensor          axs1;   
GaitController      gc;     

// Setup function to initialize the robot components
void setup() {

    
    Serial.begin(DEBUG_BAUD_RATE);          // Initialize Serial for debugging
    #ifdef DEBUG                            // if DEBUG enabled
       while (!Serial); 
       Serial.println("Debug serial ready"); // Wait for the debug serial to be ready
    #endif // DEBUG

    mc.begin();                                             // Initialize the microcontroller (OpenCR1.0 board)
    servo.begin(DXL_SERIAL, DXL_BAUD_RATE);                 // Initialize Dynamixel controller with specified serial port and baud rate
    hexapod.begin(&servo);                                  // Initialize the hexapod
    turret.begin(TURRET_PAN_ID, TURRET_TILT_ID, &servo);    // Initialize the turret
    rc.begin(RC100_SERIAL,&turret);                         // Initialize the remote controller with the turret instance
}

// Loop function to handle remote controller input and control the robot
void loop() {
    rc.update(); // Update remote controller state
}
