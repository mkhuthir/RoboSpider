

// include necessary libraries and headers
#include <Arduino.h>                    // Include Arduino core library

#include "Config.h"                     // Include configuration header

#include "Console.h"                    // Include Console class for managing serial console
#include "Microcontroller.h"            // Include Microcontroller class for managing OpenCR1.0 board
#include "Servo.h"                      // Include Servo class for managing Dynamixel servos
#include "Hexapod.h"                    // Include Hexapod class for managing the hexapod robot
#include "Turret.h"                     // Include Turret class for managing the sensor turret
#include "AXS1Sensor.h"                 // Include AXS1Sensor class for managing the AX-S1 sensor
#include "GaitController.h"             // Include GaitController class for managing the gait of the hexapod
#include "Remotecontroller.h"           // Include RemoteController class for managing remote controller input


// Global variables and instances

Console             con;       
Microcontroller     mc;
Servo               servo;  
Hexapod             hexapod;
Turret              turret; 
AXS1Sensor          axs1;   
GaitController      gc;     
Remotecontroller    rc;     

bool                errorFlag = false;  // Global error flag to indicate if an error has occurred

// Setup function to initialize the robot components
void setup() {

    errorFlag = !con.begin(DEBUG_BAUD_RATE, &hexapod, &turret, &gc, &mc);       // Initialize the console for debugging
    if (!errorFlag) { errorFlag = !mc.begin(); }                                // Initialize the microcontroller if no error has occurred
    if (!errorFlag) { errorFlag = !servo.begin(DXL_SERIAL, DXL_BAUD_RATE); }    // Initialize Dynamixel controller with specified serial port and baud rate
    if (!errorFlag) { errorFlag = !hexapod.begin(&servo); }                     // Initialize the hexapod
    if (!errorFlag) { errorFlag = !turret.begin(&servo); }                      // Initialize the turret
    if (!errorFlag) { errorFlag = !axs1.begin(&servo, AXS1_SENSOR_ID); }        // Initialize the AX-S1 sensor
    if (!errorFlag) { errorFlag = !gc.begin(&hexapod); }                        // Initialize the gait controller with the hexapod instance
    if (!errorFlag) { errorFlag = !rc.begin(RC100_SERIAL,&hexapod,&turret,&gc,&mc); } // Initialize the remote controller with the turret instance

    if (errorFlag) {
        Serial.println("Initialization failed!");                               // Print error message if initialization fails
        while (1);                                                              // Stop execution
    } else {
        Serial.println("Initialization successful!");                          // Print success message if initialization is successful
    }
}

// Loop function to handle remote controller input and control the robot
void loop() {
    con.update();   // Update console state
    mc.update();    // Update microcontroller state
    gc.update();    // Update gait controller state
    rc.update();    // Update remote controller state
}
