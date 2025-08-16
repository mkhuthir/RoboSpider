#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

    #include <RC100.h>                      // Include RC100 remote controller library
    #include "Microcontroller.h"            // Include Microcontroller class for managing the microcontroller
    #include "Hexapod.h"                    // Include Hexapod class for managing the hexapod robot
    #include "Turret.h"                     // Include Turret class for managing the sensor turret
    #include "GaitController.h"             // Include GaitController for movement control
    
    class Remotecontroller {
        public:
            Remotecontroller();             // Constructor
            bool begin(int serial_port, Microcontroller* mc, Hexapod* hexapod, Turret* turret, GaitController* gc);                
            bool update();                  // Update the remote controller state

        private:
            RC100               rc;         // Instance of the RC100 remote controller
            Hexapod*            hexapod;    // Pointer to the Hexapod instance
            Turret*             turret;     // Pointer to the turret instance
            GaitController*     gc;         // Pointer to the GaitController instance
            Microcontroller*    mc;         // Pointer to the Microcontroller instance
    };

#endif // REMOTECONTROLLER_H