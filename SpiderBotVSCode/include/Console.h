#ifndef CONSOLE_H
#define CONSOLE_H

    #include <Arduino.h>
    #include "Config.h"  // Include configuration header

    #include "Turret.h"             // Include Turret class for managing the sensor turret
    #include "GaitController.h"     // Include GaitController for movement control
    #include "Hexapod.h"            // Include Hexapod class for managing the hexapod robot
    #include "Microcontroller.h"    // Include Microcontroller class for managing the microcontroller

    class Console {
    public:
        Console(Stream& stream = DEBUG_SERIAL);
        void begin( unsigned long       baud = DEBUG_BAUD_RATE, 
                    Hexapod*            hexapod = nullptr, 
                    Turret*             turret = nullptr, 
                    GaitController*     gc = nullptr, 
                    Microcontroller*    mc = nullptr);
                    
        void update();  // Call in loop()

    private:
        Stream&             con;                // Reference to the stream for console output
        String              inputBuffer;        // Buffer for input commands
        String              shell;              // Shell prompt string
        Hexapod*            hexapod = nullptr;  // Pointer to Hexapod instance
        Turret*             turret = nullptr;   // Pointer to Turret instance
        GaitController*     gc = nullptr;       // Pointer to GaitController instance
        Microcontroller*    mc = nullptr;       // Pointer to Microcontroller instance

        void processCommand(const String& command);
    };

#endif
