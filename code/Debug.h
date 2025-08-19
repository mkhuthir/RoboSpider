#ifndef DEBUG_H
#define DEBUG_H

    #include <Arduino.h>

    // ANSI color codes for terminal output
    #define COLOR_RESET     "\033[0m"       // Reset color
    #define COLOR_RED       "\033[31m"      // Error messages
    #define COLOR_YELLOW    "\033[33m"      // Warning messages
    #define COLOR_WHITE     "\033[37m"      // Info messages
    #define COLOR_CYAN      "\033[36m"      // Debug messages
    #define COLOR_BOLD      "\033[1m"       // Bold text

    #define LOG_HEADER_ERROR   "[ERR] "      // Error log header
    #define LOG_HEADER_WARNING "[WRN] "      // Warning log header
    #define LOG_HEADER_INFO    "[INF] "      // Info log header
    #define LOG_HEADER_DEBUG   "[DBG] "      // Debug log header

    // Global macros for printing messages
    #define PRINT(msg)      log::print(msg)             // Normal print without debug level or color
    #define PRINTLN(msg)    log::println(msg)           // Print with newline

    #define LOG_ERR(msg)    log::printError(msg)        // Error logging with red color
    #define LOG_WRN(msg)    log::printWarning(msg)      // Warning logging with yellow color
    #define LOG_INF(msg)    log::printInfo(msg)         // Info logging with green color
    #define LOG_DBG(msg)    log::printDebug(msg)        // Debug logging with cyan color

    // Debug levels for logging system
    enum DebugLevel {
        DEBUG_NON = 0,     // No debug output
        DEBUG_ERR = 1,     // Errors only
        DEBUG_WRN = 2,     // Warnings and Errors
        DEBUG_INF = 3,     // Info, Warnings, and Errors
        DEBUG_DBG = 4      // Debug, Info, Warnings, and Errors
    };

    class log {
        public:
            // Static methods for managing console state
            static void setDebugLevel(DebugLevel level);            // Set the global debug level
            static void setColorEnabled(bool enabled);              // Enable/disable color output
            static DebugLevel getDebugLevel() { return debugLevel; }
            static bool getColorEnabled() { return colorEnabled; }

            // Static methods for managing log stream
            static void setLogStream(Stream* stream);               // Set the log stream
            static Stream* getLogStream();                          // Get the log stream

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

        private:
            // Static members for global logging
            static Stream*      logStream;                          // Stream for logging output
            static DebugLevel   debugLevel;                         // Current debug level
            static bool         colorEnabled;                       // Color output enabled flag

            // Private logging helper
            static void printLog(DebugLevel level, const String& prefix, const String& color, const String& message);

    };

#endif
// DEBUG_H