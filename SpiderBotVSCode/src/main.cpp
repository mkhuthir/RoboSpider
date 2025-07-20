

// include necessary libraries and headers
#include <Arduino.h>                    // Include Arduino core library

#include "Config.h"                     // Include configuration header

#include "Microcontroller.h"            // Include Microcontroller class for managing OpenCR1.0 board
#include "Servo.h"                      // Include Servo class for managing Dynamixel servos
#include "Hexapod.h"                    // Include Hexapod class for managing the hexapod robot
#include "Turret.h"                     // Include Turret class for managing the sensor turret
#include "AXS1Sensor.h"                 // Include AXS1Sensor class for managing the AX-S1 sensor
#include "GaitController.h"             // Include GaitController class for managing the gait of the hexapod
#include "Remotecontroller.h"           // Include RemoteController class for managing remote controller input


// Global variables and instances

Microcontroller     mc;
Servo               servo;  
Hexapod             hexapod;
Turret              turret; 
AXS1Sensor          axs1;   
GaitController      gc;     
Remotecontroller    rc;     

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
    axs1.begin(&servo, AXS1_SENSOR_ID);                     // Initialize the AX-S1 sensor
    gc.begin(&hexapod);                                     // Initialize the gait controller with the hexapod instance
    rc.begin(RC100_SERIAL,&hexapod,&turret,&gc);                     // Initialize the remote controller with the turret instance

    axs1.ping();                               // Ping the AX-S1 sensor to check if it's connected
    servo.printStatus(100);                      // Print the status of servo with ID 1 for debugging
 
}

// Loop function to handle remote controller input and control the robot
void loop() {
    mc.update(); // Update microcontroller state
    gc.update(); // Update gait controller state
    rc.update(); // Update remote controller state
}
