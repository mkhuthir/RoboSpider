#include "Log.h"

// Static method to set the log stream
void log::setLogStream(Stream* stream) {
    logStream = stream;
}

// Static method to get the log stream
Stream* log::getLogStream() {
    return logStream;
}

// Static members initialization
Stream*     log::logStream       = &Serial;      // Default to Serial for logging
DebugLevel  log::debugLevel      = DEBUG_INF;    // Default debug level
bool        log::colorEnabled    = true;         // Default color enabled

// Static method to set debug level
void log::setDebugLevel(DebugLevel level) {
    debugLevel = level;
    println("Debug level set to: " + String(level));
}

// Static method to enable/disable colors
void log::setColorEnabled(bool enabled) {
    colorEnabled = enabled;
    println("Color output " + String(enabled ? "enabled" : "disabled"));
}

// Static method to print normal messages (always printed, no debug level filtering)
void log::print(const String& message) {
    if (logStream == nullptr) {
        printError("Log stream is not initialized.");
        return;
    }
    
    // Print message directly without debug level check, timestamp, or color formatting
    logStream->print(message);
}

// Static method to print normal messages (always printed, no debug level filtering)
void log::println(const String& message) {
    if (logStream == nullptr) {
        printError("Log stream is not initialized.");
        return;
    }
    
    // Print message directly without debug level check, timestamp, or color formatting
    logStream->println(message);
}

// Print float value
void log::print(float value) {
    if (logStream) {
        logStream->print(value);
    }
}

// Print float value with newline
void log::println(float value) {
    if (logStream) {
        logStream->println(value);
    }
}

// Print int value
void log::print(int value) {
    if (logStream) {
        logStream->print(value);
    }
}

// Print int value with newline
void log::println(int value) {
    if (logStream) {
        logStream->println(value);
    }
}

// Static method to print error messages
void log::printError(const String& message) {
    printLog(DEBUG_ERR, LOG_HEADER_ERROR, COLOR_RED, message);
}

// Static method to print warning messages
void log::printWarning(const String& message) {
    printLog(DEBUG_WRN, LOG_HEADER_WARNING, COLOR_YELLOW, message);
}

// Static method to print info messages
void log::printInfo(const String& message) {
    printLog(DEBUG_INF, LOG_HEADER_INFO, COLOR_WHITE, message);
}

// Static method to print debug messages
void log::printDebug(const String& message) {
    printLog(DEBUG_DBG, LOG_HEADER_DEBUG, COLOR_CYAN, message);
}

// Private helper method for formatted logging
void log::printLog(DebugLevel level, const String& prefix, const String& color, const String& message) {
    // Check if we should print this message based on debug level
    if (debugLevel < level || logStream == nullptr) {
        return;
    }

    // Build the formatted message
    String timestamp = String(millis());
    String formattedMessage = "";

    if (colorEnabled) {
        formattedMessage = color + COLOR_BOLD + prefix + COLOR_RESET + timestamp + "ms " + color + message + COLOR_RESET;
    } else {
        formattedMessage = prefix + timestamp + "ms " + message;
    }

    // Print the message
    logStream->println(formattedMessage);
}

// end of Logger.cpp