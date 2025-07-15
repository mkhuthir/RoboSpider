

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

Microcontroller     mc;                 // Initialize Microcontroller instance for OpenCR1.0 board
Servo               servo;              // Initialize Servo instance for Dynamixel servos
Remotecontroller    rc;                 // Initialize RemoteController instance for handling remote controller input
Hexapod*            hexapod;            // Pointer to Hexapod instance
Turret*             turret;             // Pointer to Turret instance
AXS1Sensor*         axs1;               // Pointer to AXS1Sensor instance
GaitController*     gc;                 // Pointer to GaitController instance

// Setup function to initialize the robot components
void setup() {

    
    Serial.begin(DEBUG_BAUD_RATE);          // Initialize Serial for debugging
    #ifdef DEBUG                            // if DEBUG enabled
        while (!Serial);                    // wait for serial to be ready
    #endif // DEBUG

    mc.init();                              // Initialize the microcontroller (OpenCR1.0 board)
    servo.init(DXL_SERIAL, DXL_BAUD_RATE);  // Initialize Dynamixel controller with specified serial port and baud rate
    rc.begin(RC100_SERIAL);                 // Initialize RC100 remote controller with specified serial port

    // Create Hexapod instance
    //hexapod = new Hexapod(&dxl);                                    // Create Hexapod instance with Dynamixel controller
    //hexapod->initialize();                                          // Initialize all legs
    //hexapod->setGaitType(0);                                        // Set default gait
    //hexapod->setGaitSpeed(0.5);                                     // Set default speed
    //hexapod->setServoSpeed(0.5);                                    // Set default servo speed
    //Serial.println("Hexapod initialized.");
    
    // Initialize Sensor Turret
    //turret = new Turret(TURRET_PAN_ID, TURRET_TILT_ID, &dxl);       // Create Turret instance with Dynamixel controller
    //turret->initialize();                                           // Initialize turret servos
    //Serial.println("Sensor Turret initialized.");

    // Initialize AX-S1 Sensor
    //sensor = new AXS1Sensor(dxl.getWorkbench(), AX_S1_SENSOR_ID);   // Create AX-S1 sensor instance with Dynamixel controller
    
    // Initialize Gait Controller
    //gaitController = new GaitController(hexapod);                   // Create GaitController instance with Hexapod
    //Serial.println("Gait Controller initialized.");

}

// Loop function to handle remote controller input and control the robot
void loop() {
    rc.update(); // Update remote controller state
}
