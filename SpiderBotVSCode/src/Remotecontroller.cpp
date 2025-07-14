
#include "Remotecontroller.h" // Include RemoteController class for managing remote control input


// Constructor for RemoteController
RemoteController::RemoteController() {}

void RemoteController::begin() {
    rc.begin(RC100_SERIAL); // Initialize the remote controller with the specified serial port
}

void RemoteController::update() {
    if (rc.available()) {
        updateButtonStates(); // Update the states of buttons when new data is available
    }
}

void RemoteController::updateButtonStates() {
    // Update the states of buttons based on the remote controller input
    rc.update();
    // Here you can add logic to handle button presses, releases, etc.

 if (rc.available())
  {
    int RCRx = rc.readData();
 
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