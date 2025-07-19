#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

    #include <Arduino.h>
    #include <RC100.h> // Include RC100 remote controller library
    #include "Turret.h" // Include Turret class for managing the sensor turret
    
    #include "Config.h" // Include configuration header

    class Remotecontroller {
        public:
            Remotecontroller();
            void begin(int serial_port, Turret* turret); // Initialize the remote controller with a stream
            void update(); // Update the remote controller state

        private:
            RC100 rc; // Instance of the RC100 remote controller
            Turret* turret; // Pointer to the turret instance
    };

#endif // REMOTECONTROLLER_H