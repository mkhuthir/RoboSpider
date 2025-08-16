

// include necessary libraries and headers
#include <Arduino.h>                    // Include Arduino core library

#include "main.h"                    // Include configuration header for constants and settings

#include "Console.h"                    // Include Console class for managing serial console
#include "Microcontroller.h"            // Include Microcontroller class for managing OpenCR1.0 board
#include "Servo.h"                      // Include Servo class for managing Dynamixel servos
#include "Hexapod.h"                    // Include Hexapod class for managing the hexapod robot
#include "Turret.h"                     // Include Turret class for managing the sensor turret
#include "AXS1Sensor.h"                 // Include AXS1Sensor class for managing the AX-S1 sensor
#include "GaitController.h"             // Include GaitController class for managing the gait of the hexapod
#include "Remotecontroller.h"           // Include RemoteController class for managing remote controller input


// Global variables and instances

Microcontroller     mc;                         // Microcontroller instance
Servo               servo;                      // Servo instance
Hexapod             hexapod;                    // Hexapod instance
Turret              turret;                     // Turret instance
AXS1Sensor          axs1;                       // AX-S1 Sensor instance
GaitController      gc;                         // Gait Controller instance
Remotecontroller    rc;                         // Remote Controller instance

// Initialize console with all necessary components
Console             con(    DEBUG_SERIAL,       // Initialize console with debug serial stream
                            DEBUG_BAUD_RATE,    // Set baud rate for console communication
                            &mc,                // Pass the Microcontroller instance
                            &servo,             // Pass the Servo instance for Dynamixel control
                            &hexapod,           // Pass the Hexapod instance
                            &turret,            // Pass the Turret instance
                            &axs1,              // Pass the AXS1Sensor instance
                            &gc,                // Pass the GaitController instance
                            &rc                 // Pass the RemoteController instance
                        );  

// Setup function to initialize the robot components
void setup() {

    mc.begin();
    servo.begin( DXL_SERIAL,DXL_BAUD_RATE, DXL_PROTOCOL_VERSION);            
    hexapod.begin(&servo);
    turret.begin(&servo);
    axs1.begin(&servo, AXS1_SENSOR_ID);
    gc.begin(&hexapod);
    rc.begin(RC100_SERIAL,&hexapod,&turret,&gc,&mc);

    con.begin();
}

// Loop function to handle remote controller input and control the robot
void loop() {
    con.update();   // Update console state
    mc.update();    // Update microcontroller state
    axs1.update();  // Update AX-S1 sensor state
    turret.update(); // Update turret state
    gc.update();    // Update gait controller state
    rc.update();    // Update remote controller state
}
