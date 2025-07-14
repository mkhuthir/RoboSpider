#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

    #include <Arduino.h>
    #include <RC100.h> // Include RC100 remote controller library

    class RemoteController {
        public:
            RemoteController();
            void begin(); // Initialize the remote controller with a stream
            void update(); // Update the remote controller state

        private:
            RC100 rc; // Instance of the RC100 remote controller
            void updateButtonStates(); // Update the states of buttons
    };

#endif // REMOTECONTROLLER_H