#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

    #include <Arduino.h>
    #include <RC100.h> // Include RC100 remote controller library
    
    #include "Config.h" // Include configuration header

    class Remotecontroller {
        public:
            Remotecontroller();
            void init(int serial_port); // Initialize the remote controller with a stream
            void update(); // Update the remote controller state

        private:
            RC100 rc; // Instance of the RC100 remote controller
    };

#endif // REMOTECONTROLLER_H