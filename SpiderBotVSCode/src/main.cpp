

#include <Arduino.h>                    // Include Arduino core library
#include "Config.h"                     // Include configuration header for serial ports and other settings     
#include "RC100.h"                      // Include Robotis RC100 remote controller library  
#include "DynamixelController.h"        // Include Dynamixel controller class for managing servos
#include "Hexapod.h"                    // Include Hexapod class for managing the hexapod robot
#include "Turret.h"                     // Include Turret class for managing the sensor turret
#include "AXS1Sensor.h"                 // Include AXS1Sensor class for managing the AX-S1 sensor
#include "GaitController.h"             // Include GaitController class for managing the gait of the hexapod


// Global variables and instances
DynamixelController dxl;                // Initialize DynamixelWorkbench with the specified serial port and baud rate
RC100               rcCtrl;             // RC100 remote controller instance
Hexapod*            hexapod;            // Pointer to Hexapod instance
Turret*             turret;             // Pointer to Turret instance
AXS1Sensor*         sensor;             // Pointer to AXS1Sensor instance
GaitController*     gaitController;     // Pointer to GaitController instance

// Setup function to initialize the robot components
void setup() {

    // Initialize Serial for debugging
    Serial.begin(DEBUG_BAUD_RATE);
    while (!Serial); // Wait for Serial to be ready
    Serial.println("SpiderBot Starting Setup...");

    // Initialize Robotis RC100 Remote Controller
    rcCtrl.begin(RC100_SERIAL);     
    Serial.println("Robotis RC100 Remote Controller initialized.");

    // Initialize Dynamixel Controller
    dxl.begin(DXL_SERIAL, DXL_BAUD_RATE); 
        

    //dxl.ping(1);                   // Ping Dynamixel with ID 1
    dxl.jointMode(1);                // Set Dynamixel to joint mode
    

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

  if (rcCtrl.available())
  {
    int RCRx = rcCtrl.readData();
 
    if (RCRx == 0){
        Serial.println("No button is pressed.");
    
    } else if (RCRx & RC100_BTN_U) {
        Serial.println("U");
                
    } else if (RCRx & RC100_BTN_D) {
        Serial.println("D");


    } else if (RCRx & RC100_BTN_L) {
        Serial.println("L");

    } else if (RCRx & RC100_BTN_R) {
        Serial.println("R");
        
    } else if (RCRx & RC100_BTN_1) {
        Serial.println("1");

    } else if (RCRx & RC100_BTN_2) {
        Serial.println("2");

    } else if (RCRx & RC100_BTN_3) {
        Serial.println("3");

    } else if (RCRx & RC100_BTN_4) {
        Serial.println("4");

    } else if (RCRx & RC100_BTN_5) {
        Serial.println("5");

    } else if (RCRx & RC100_BTN_6) {
        Serial.println("6");
       
    }
  }

}
