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
        bool begin( unsigned long       baud = DEBUG_BAUD_RATE, 
                    Hexapod*            hexapod = nullptr, 
                    Turret*             turret = nullptr, 
                    GaitController*     gc = nullptr, 
                    Microcontroller*    mc = nullptr);

        void update();                              // Call in loop()

    private:
        Stream&             con;                                // Reference to the stream (Serial, etc.)
        String              inputBuffer;                        // Buffer to store the input command
        String              shell;                              // Shell prompt string
        int                 cursorPos = 0;                      // Current cursor position in input buffer
        bool                insertMode = true;                  // Insert mode flag (true=insert, false=overwrite)
        CommandHistory      commandHistory;                     // Command history management
        
        // Component instances
        Hexapod*            hexapod;                            // Pointer to hexapod instance
        Turret*             turret;                             // Pointer to turret instance  
        GaitController*     gc;                                 // Pointer to GaitController instance
        Microcontroller*    mc;                                 // Pointer to Microcontroller instance

        void processCommand(const String& command);
    };

#endif
