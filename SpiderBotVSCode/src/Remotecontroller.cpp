#include Remotecontroller.h // Include RemoteController class for managing remote control input

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
}