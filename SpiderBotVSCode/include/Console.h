#ifndef CONSOLE_H
#define CONSOLE_H

    #include <Arduino.h>
    #include "Config.h"  // Include configuration header

    #include "Turret.h"             // Include Turret class for managing the sensor turret
    #include "GaitController.h"     // Include GaitController for movement control
    #include "Hexapod.h"            // Include Hexapod class for managing the hexapod robot
    #include "Microcontroller.h"    // Include Microcontroller class for managing the microcontroller
    #include "CommandHistory.h"     // Include CommandHistory class for command history management

    class Console {
    public:
        Console(Stream& stream = DEBUG_SERIAL);     // Constructor with default stream
        void begin( unsigned long       baud = DEBUG_BAUD_RATE, 
                    Hexapod*            hexapod = nullptr, 
                    Turret*             turret = nullptr, 
                    GaitController*     gc = nullptr, 
                    Microcontroller*    mc = nullptr);

        void update();                              // Call in loop()

    private:
        Stream&             con = DEBUG_SERIAL;     // Reference to the stream for console output
        
        String              inputBuffer = "";       // Buffer for input commands
        CommandHistory      commandHistory;         // Command history manager
        int                 cursorPos = 0;          // Current cursor position in input buffer
        String              shell = "\n\r$";        // Shell prompt string

        Hexapod*            hexapod = nullptr;      // Pointer to Hexapod instance
        Turret*             turret = nullptr;       // Pointer to Turret instance
        GaitController*     gc = nullptr;           // Pointer to GaitController instance
        Microcontroller*    mc = nullptr;           // Pointer to Microcontroller instance

        void processCommand(const String& command);
    };

#endif
