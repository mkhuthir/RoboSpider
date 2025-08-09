#ifndef CONSOLE_H
#define CONSOLE_H

    #include <Arduino.h>
    
    #include "Turret.h"             // Include Turret class for managing the sensor turret
    #include "GaitController.h"     // Include GaitController for movement control
    #include "Hexapod.h"            // Include Hexapod class for managing the hexapod robot
    #include "Microcontroller.h"    // Include Microcontroller class for managing the microcontroller
    #include "CommandHistory.h"     // Include CommandHistory class for command history management
    #include "AXS1Sensor.h"         // Include AXS1Sensor class for sensor management

    // ANSI color codes for terminal output
    #define COLOR_RESET     "\033[0m"       // Reset color
    #define COLOR_RED       "\033[31m"      // Error messages
    #define COLOR_YELLOW    "\033[33m"      // Warning messages
    #define COLOR_WHITE     "\033[37m"      // Info messages
    #define COLOR_CYAN      "\033[36m"      // Debug messages
    #define COLOR_BOLD      "\033[1m"       // Bold text

    #define LOG_HEADER_ERROR   "[ERR]"      // Error log header
    #define LOG_HEADER_WARNING "[WRN]"      // Warning log header
    #define LOG_HEADER_INFO    "[INF]"      // Info log header
    #define LOG_HEADER_DEBUG   "[DBG]"      // Debug log header

    // Debug levels for logging system
    enum DebugLevel {
        DEBUG_NONE  = 0,     // No debug output
        DEBUG_ERROR = 1,     // Errors only
        DEBUG_WARN  = 2,     // Warnings and errors
        DEBUG_INFO  = 3,     // Info, warnings, and errors
        DEBUG_ALL   = 4      // All messages including debug
    };

    class Console {
    public:
        Console();                                              // Constructor with default stream
        bool begin( Stream&             stream,                 // Initialize the console with a stream
                    unsigned long       baud,                   // Baud rate for serial communication
                    
                    Servo*              servo   = nullptr,      // Pointer to Servo instance
                    Hexapod*            hexapod = nullptr,      // Pointer to Hexapod instance
                    Turret*             turret  = nullptr,      // Pointer to Turret instance
                    AXS1Sensor*         sensor  = nullptr,      // Pointer to AXS1Sensor instance
                    GaitController*     gc      = nullptr,      // Pointer to GaitController instance
                    Microcontroller*    mc      = nullptr       // Pointer to Microcontroller instance
        );

        void update();                                          // Call in loop()

        // Static methods for managing console state
        static void setDebugLevel(DebugLevel level);            // Set the global debug level
        static void setColorEnabled(bool enabled);              // Enable/disable color output

        // Static methods for printing messages
        static void print(const String& message);               // Normal print without debug level or color
        static void println(const String& message);             // Print with newline
        static void print(float value);                         // Print float value
        static void println(float value);                       // Print float value with newline
        static void print(int value);                           // Print int value  
        static void println(int value);                         // Print int value with newline
        static void printError(const String& message);          // Print error message (red)
        static void printWarning(const String& message);        // Print warning message (yellow)
        static void printInfo(const String& message);           // Print info message (green)
        static void printDebug(const String& message);          // Print debug message (cyan)

        // Global macros for printing messages
        #define PRINT(msg)      Console::print(msg)             // Normal print without debug level or color
        #define PRINTLN(msg)    Console::println(msg)           // Print with newline
        #define LOG_ERR(msg)    Console::printError(msg)        // Error logging with red color
        #define LOG_WRN(msg)    Console::printWarning(msg)      // Warning logging with yellow color
        #define LOG_INF(msg)    Console::printInfo(msg)         // Info logging with green color
        #define LOG_DBG(msg)    Console::printDebug(msg)        // Debug logging with cyan color

    private:
        
        String              inputBuffer     = "";               // Buffer to store the input command
        String              shell           = "$";              // Shell prompt string
        int                 cursorPos       = 0;                // Current cursor position in input buffer
        bool                insertMode      = true;             // Insert mode flag (true=insert, false=overwrite)
        CommandHistory      commandHistory;                     // Command history management

        // Static members for global logging
        static DebugLevel   debugLevel;                         // Current debug level
        static bool         colorEnabled;                       // Color output enabled flag
        static Stream*      logStream;                          // Stream for logging output
        
        // Pointers to instances of Hexapod, Turret, GaitController, and Microcontroller
        Servo*              servo;                              // Pointer to Servo instance for Dynamixel control
        Hexapod*            hexapod;                            // Pointer to hexapod instance
        Turret*             turret;                             // Pointer to turret instance  
        AXS1Sensor*         sensor;                             // Pointer to AXS1Sensor instance (can be nullptr)
        GaitController*     gc;                                 // Pointer to GaitController instance
        Microcontroller*    mc;                                 // Pointer to Microcontroller instance

        // Input processing methods
        void processInput(const String& input);                             // Process the input entered by the user
        void handleInputControl(char c);                                    // Handle all input control operations
        void handlePrintableChar(char c);                                   // Handle printable character input and display
        
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
        
        // Input helper methods
        void clearAndRedrawLine(const String& newContent);     // Clear line and redraw with new content
        void refreshLineFromCursor();                           // Refresh display from cursor position
        void resetInputState();                                 // Reset input state after command

        // Command execution methods - organized by category
        bool runConsoleCommands(const String& cmd, const String& args);     // System commands (help, cls, status)
        bool runLegCommand(const String& cmd, const String& args);          // Leg control commands

        // Helper methods for command processing
        void printAllHelp();                                       // Print comprehensive help information
        void printConsoleHelp();                                // Print console-specific help information
        
        // Private logging helper
        static void printLog(DebugLevel level, const String& prefix, const String& color, const String& message);
    };

#endif
