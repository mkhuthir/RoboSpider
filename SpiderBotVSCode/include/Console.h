#ifndef CONSOLE_H
#define CONSOLE_H

    #include <Arduino.h>
    
    #include "Turret.h"             // Include Turret class for managing the sensor turret
    #include "GaitController.h"     // Include GaitController for movement control
    #include "Hexapod.h"            // Include Hexapod class for managing the hexapod robot
    #include "Microcontroller.h"    // Include Microcontroller class for managing the microcontroller
    #include "CommandHistory.h"     // Include CommandHistory class for command history management
 
    class Console {
    public:
        Console();                                              // Constructor with default stream
        bool begin( Stream&             stream,                 // Initialize the console with a stream
                    unsigned long       baud,                   // Baud rate for serial communication 
                    Hexapod*            hexapod = nullptr,      // Pointer to Hexapod instance
                    Turret*             turret  = nullptr,      // Pointer to Turret instance
                    GaitController*     gc      = nullptr,      // Pointer to GaitController instance
                    Microcontroller*    mc      = nullptr       // Pointer to Microcontroller instance
        );

        void update();                                          // Call in loop()

    private:
        Stream&             con             = Serial;           // Reference to the stream (Serial, etc.)

        String              inputBuffer     = "";               // Buffer to store the input command
        String              shell           = "$";              // Shell prompt string
        int                 cursorPos       = 0;                // Current cursor position in input buffer
        bool                insertMode      = true;             // Insert mode flag (true=insert, false=overwrite)
        CommandHistory      commandHistory;                     // Command history management
        
        // Pointers to instances of Hexapod, Turret, GaitController, and Microcontroller        
        Hexapod*            hexapod;                            // Pointer to hexapod instance
        Turret*             turret;                             // Pointer to turret instance  
        GaitController*     gc;                                 // Pointer to GaitController instance
        Microcontroller*    mc;                                 // Pointer to Microcontroller instance

        void processCommand(const String& command);
        void handleInputControl(char c);                        // Handle all input control operations
        void handlePrintableChar(char c);                       // Handle printable character input and display
        
        // Command execution methods - organized by category
        bool executeSystemCommand(const String& cmd, const String& args);   // System commands (help, cls, status)
        bool executeTurretCommand(const String& cmd, const String& args);   // Turret control commands
        bool executeGaitCommand(const String& cmd, const String& args);     // Gait control commands
        bool executeHexapodCommand(const String& cmd, const String& args);  // Hexapod control commands
        bool executeLegCommand(const String& cmd, const String& args);      // Leg control commands
        
        // Helper methods for command processing
        void printHelp();                                       // Print comprehensive help information
        void printSystemStatus();                               // Print system status information
        
        // Escape sequence handlers
        void handleEscapeSequence();                            // Main escape sequence handler
        void handleCSISequence();                               // Control Sequence Introducer (ESC[)
        void handleSSSequence();                                // Single Shift sequence (ESC-O)
        void handleExtendedKey(char keyCode);                   // Extended keys ending with '~'
        
        // Arrow key handlers
        void handleArrowUp();                                   // Up arrow - command history previous
        void handleArrowDown();                                 // Down arrow - command history next
        void handleArrowRight();                                // Right arrow - move cursor right
        void handleArrowLeft();                                 // Left arrow - move cursor left
        
        // Navigation key handlers
        void handleHome();                                      // Home key - move to beginning
        void handleEnd();                                       // End key - move to end
        
        // Edit key handlers
        void handleInsertKey();                                 // Insert key - toggle insert/overwrite
        void handleDeleteKey();                                 // Delete key - delete character at cursor
        void handleBackspace();                                 // Backspace - delete character before cursor
        
        // Control key handlers
        void handleClearScreen();                               // Ctrl-L - clear screen
        void handleNewline();                                   // Enter - process command
        
        // Helper methods
        void clearAndRedrawLine(const String& newContent);     // Clear line and redraw with new content
        void refreshLineFromCursor();                           // Refresh display from cursor position
        void resetInputState();                                 // Reset input state after command
    };

#endif
