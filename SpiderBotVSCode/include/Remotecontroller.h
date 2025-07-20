#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

    #include <RC100.h>              // Include RC100 remote controller library
    #include "Turret.h"             // Include Turret class for managing the sensor turret
    #include "GaitController.h"     // Include GaitController for movement control
    
    class Remotecontroller {
        public:
            Remotecontroller();
            void begin(int serial_port, Turret* turret, GaitController* gc); // Initialize the remote controller with a stream
            void update(); // Update the remote controller state

        private:
            RC100           rc;       // Instance of the RC100 remote controller
            Turret*         turret;   // Pointer to the turret instance
            GaitController* gc;       // Pointer to the GaitController instance
    };

#endif // REMOTECONTROLLER_H