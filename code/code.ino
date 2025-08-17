

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

    bool success = true;

    // Initialize all components
    success &= con.begin();
    success &= mc.begin();
    success &= servo.begin( DXL_SERIAL,DXL_BAUD_RATE, DXL_PROTOCOL_VERSION);
    success &= hexapod.begin(&servo);
    success &= turret.begin(&servo);
    success &= axs1.begin(&servo, AXS1_SENSOR_ID);
    success &= gc.begin(&hexapod);
    success &= rc.begin(RC100_SERIAL,&mc,&hexapod,&turret,&gc);

    if (!success) {
        LOG_ERR("Failed to initialize components.");
    } else {
        LOG_INF("All components initialized successfully.");
        con.startShell();
    }
}

// Main loop function
void loop() {
    con.update();       // Update console state
    mc.update();        // Update microcontroller state
    hexapod.update();   // Update hexapod state
    turret.update();    // Update turret state
    axs1.update();      // Update AX-S1 sensor state
    gc.update();        // Update gait controller state
    rc.update();        // Update remote controller state
}
