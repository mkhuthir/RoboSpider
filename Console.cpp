#include "Console.h"
#include <stdarg.h>  // For va_list, va_start, va_end
#include <EEPROM.h>

// Static member initialization
DebugLevel  Console::debugLevel     = DEBUG_WRN;    // Default to WARN level
bool        Console::colorEnabled   = true;         // Enable colors by default
Stream*     Console::logStream      = &Serial;      // Default to Serial fo    println("Examples: 'lpu 2' moves leg 2 point up, 'sbu 72 200' plays note, 'mlon 3' turns on user LED 3, 'srpos 1 1024' moves servo 1 to position 1024"); logging

// Constructor for Console class
Console::Console(){
    shell = "$";                // Default shell prompt
    cursorPos = 0;              // Start cursor at position 0
    insertMode = true;          // Default to insert mode
    debugLevel = DEBUG_WRN;     // Default debug level
    colorEnabled = true;        // Default to color enabled
    logStream = &Serial;        // Default log stream is Serial
}

// Initialize the console with a baud rate and instances of components
bool Console::begin(Stream& stream,
                    unsigned long baud,
                    Servo* servo,
                    Hexapod* hexapod,
                    Turret* turret,
                    AXS1Sensor* sensor,
                    GaitController* gc,
                    Microcontroller* mc) {

    logStream = &stream;                // Set the log stream to the same stream

    if (logStream == &Serial) {
        Serial.begin(baud);
        while (!Serial);                // Wait for Serial if using USB
    }
    
    this->servo     = servo;            // Store the Servo instance
    this->hexapod   = hexapod;          // Store the hexapod instance
    this->turret    = turret;           // Store the turret instance
    this->sensor    = sensor;           // Store the AXS1Sensor instance
    this->gc        = gc;               // Store the GaitController instance
    this->mc        = mc;               // Store the Microcontroller instance

    commandHistory.resetToEnd();        // Reset command history to end
    PRINT("\033[2J\033[H");             // Clear the console screen and move cursor to home position
    PRINTLN("Type '?' for help.");      // Print help message
    PRINT(shell);                       // Print the shell prompt
    return true;
}

// Update the console, reading input and processing commands
void Console::update() {
    while (logStream->available()) {
        char c = logStream->read();            // Read a character from the console input

        if (c >= 32 && c <= 126) {      // Printable ASCII characters
            handlePrintableChar(c);
        } else {
            handleInputControl(c);      // Control characters and special sequences
        }
    }
}


void Console::saveSettingsToEEPROM() {
    EEPROM.write(EEPROM_ADDR_DEBUG_LEVEL, (uint8_t)debugLevel);
    EEPROM.write(EEPROM_ADDR_COLOR_ENABLED, colorEnabled ? 1 : 0);
}

void Console::loadSettingsFromEEPROM() {
    debugLevel = (DebugLevel)EEPROM.read(EEPROM_ADDR_DEBUG_LEVEL);
    colorEnabled = EEPROM.read(EEPROM_ADDR_COLOR_ENABLED) != 0;
}


// Static method to set debug level
void Console::setDebugLevel(DebugLevel level) {
    debugLevel = level;
    println("Debug level set to: " + String(level));
}

// Static method to enable/disable colors
void Console::setColorEnabled(bool enabled) {
    colorEnabled = enabled;
    println("Color output " + String(enabled ? "enabled" : "disabled"));
}

// Static method to print normal messages (always printed, no debug level filtering)
void Console::print(const String& message) {
    if (logStream == nullptr) {
        printError("Log stream is not initialized.");
        return;
    }
    
    // Print message directly without debug level check, timestamp, or color formatting
    logStream->print(message);
}

// Static method to print normal messages (always printed, no debug level filtering)
void Console::println(const String& message) {
    if (logStream == nullptr) {
        printError("Log stream is not initialized.");
        return;
    }
    
    // Print message directly without debug level check, timestamp, or color formatting
    logStream->println(message);
}

// Static method to print error messages
void Console::printError(const String& message) {
    printLog(DEBUG_ERR, LOG_HEADER_ERROR, COLOR_RED, message);
}

// Static method to print warning messages
void Console::printWarning(const String& message) {
    printLog(DEBUG_WRN, LOG_HEADER_WARNING, COLOR_YELLOW, message);
}

// Static method to print info messages
void Console::printInfo(const String& message) {
    printLog(DEBUG_INF, LOG_HEADER_INFO, COLOR_WHITE, message);
}

// Static method to print debug messages
void Console::printDebug(const String& message) {
    printLog(DEBUG_DBG, LOG_HEADER_DEBUG, COLOR_CYAN, message);
}

// Private helper method for formatted logging
void Console::printLog(DebugLevel level, const String& prefix, const String& color, const String& message) {
    // Check if we should print this message based on debug level
    if (debugLevel < level || logStream == nullptr) {
        return;
    }

    // Build the formatted message
    String timestamp = String(millis());
    String formattedMessage = "";

    if (colorEnabled) {
        formattedMessage = color + COLOR_BOLD + prefix + COLOR_RESET + 
                          " [" + timestamp + "ms] " + 
                          color + message + COLOR_RESET;
    } else {
        formattedMessage = prefix + " [" + timestamp + "ms] " + message;
    }

    // Print the message
    logStream->println(formattedMessage);
}

// Process the command entered by the user
void Console::processInput(const String& input) {
    // Trim whitespace and convert to lowercase for consistent handling
    String cmd = input;
    cmd.trim();
    cmd.toLowerCase();
    
    // Skip empty commands
    if (cmd.length() == 0) {
        return;
    }

    commandHistory.addCommand(input);       // Add original command to history
    LOG_DBG("Received: " + input);          // Log the received command for debugging
        
    // Parse command and arguments
    String mainCmd;
    String args;
    int spaceIndex = cmd.indexOf(' ');
    if (spaceIndex != -1) {
        mainCmd = cmd.substring(0, spaceIndex);
        args = cmd.substring(spaceIndex + 1);
        args.trim();
    } else {
        mainCmd = cmd;
    }

    // Execute command using organized command handlers
    if (!runConsoleCommands(mainCmd, args) &&
        !turret->runConsoleCommands(mainCmd, args) &&
        !gc->runConsoleCommands(mainCmd, args) &&
        !hexapod->runConsoleCommands(mainCmd, args) &&
        !runLegCommand(mainCmd, args) &&
        !sensor->runConsoleCommands(mainCmd, args) &&
        !servo->runConsoleCommands(mainCmd, args) &&
        !mc->runConsoleCommands(mainCmd, args)) 
        {
        
        // Unknown command
        LOG_ERR("Unknown command: " + input);
        PRINTLN("Type '?' for help.");
    }
}

// System commands (help, cls, status)
bool Console::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "?" || cmd == "h") {
        printConsoleHelp();
        return true;
    
    } else if (cmd == "??") {
        printAllHelp();
        return true;

    } else if (cmd == "cls" || cmd == "clear") {
        PRINT("\033[2J\033[H");                     // ANSI escape code to clear screen and move cursor to home
        PRINT(shell);
        return true;

    } else if (cmd == "debug") {
        // Set debug level: debug 0-4
        if (args.length() > 0) {
            int level = args.toInt();
            if (level >= DEBUG_NON && level <= DEBUG_DBG) {
                setDebugLevel((DebugLevel)level);
                printInfo("Debug level set to: " + String(level));
            } else {
                printError("Invalid debug level. Use 0-4 (0=NON, 1=ERR, 2=WRN, 3=INF, 4=DBG)");
            }

        } else {
            println("Current debug level: " + String(debugLevel));
        }
        return true;

    } else if (cmd == "color") {
        // Toggle color output: color on/off
        if (args == "on" || args == "1" || args == "true") {
            setColorEnabled(true);
        } else if (args == "off" || args == "0" || args == "false") {
            setColorEnabled(false);
        } else {
            println("Current color setting: " + String(colorEnabled ? "enabled" : "disabled"));
        }
        return true;

    } else if (cmd == "test") {
        // Test all log levels
        println("This is a normal message");
        printError("This is an error message");
        printWarning("This is a warning message");
        printInfo("This is an info message");
        printDebug("This is a debug message");
        println("Current debug level: " + String(debugLevel));
        println("Color output is " + String(colorEnabled ? "enabled" : "disabled"));
        return true;

    } 
    return false;
}

// Leg control commands (operates on leg 0 by default, could be extended for specific legs)
bool Console::runLegCommand(const String& cmd, const String& args) {
    int legIndex = 0; // Default to leg 0
    
    // Parse leg number from arguments if provided
    if (args.length() > 0) {
        int parsedLeg = args.toInt();
        if (parsedLeg >= 0 && parsedLeg < 6) { // Assuming 6 legs indexed from 0 to 5
            legIndex = parsedLeg;
        }
    }
    
    // Call the specific leg's runConsoleCommands method
    return hexapod->legs[legIndex].runConsoleCommands(cmd, args, legIndex);
}

//Print comprehensive help information
void Console::printAllHelp() {
    
    println("SpiderBot Console - Available Commands:");
    println("");
    
    // Print help for each component
    this->printConsoleHelp();
    mc->printConsoleHelp();
    servo->printConsoleHelp();
    hexapod->printConsoleHelp();
    hexapod->legs[0].printConsoleHelp();
    gc->printConsoleHelp();
    turret->printConsoleHelp();
    sensor->printConsoleHelp();


    // Show examples for common commands
    println("Examples: 'lpu 2' moves leg 2 point up, 'sbu 72 200' plays note, 'mlon 3' turns on user LED 3");
}

// Print console-specific help information
void Console::printConsoleHelp() {
    println("Console Commands:");
    println("  ? / h            - Show this help message");
    println("  ??               - Show all available commands");
    println("  s?               - Show servo commands");
    println("  h?               - Show hexapod commands");
    println("  l?               - Show leg commands");
    println("  a?               - Show AX-S1 Sensor commands");
    println("  t?               - Show turret commands");
    println("  g?               - Show gait controller commands");
    println("  cls / clear      - Clear the terminal screen");
    println("  debug [0-4]      - Set debug level (0=NONE, 1=ERROR, 2=WARN, 3=INFO, 4=ALL)");
    println("  color [on/off]   - Enable/disable color output");
    println("  test             - Test all log message types");
    println("");
}

// Handle printable character input and display
void Console::handlePrintableChar(char c) {
    if (insertMode) {
        if (cursorPos < (int)inputBuffer.length()) {            // Insert mode: insert character at cursor position
            String before = inputBuffer.substring(0, cursorPos);    // Insert character in the middle of the string
            String after = inputBuffer.substring(cursorPos);
            inputBuffer = before + c + after;
            print("\033[K");                                // Clear from cursor to end and reprint
            String remaining = inputBuffer.substring(cursorPos);
            print(remaining);
            for (int i = remaining.length() - 1; i > 0; i--) {  // Move cursor back to position after inserted character
                print("\033[D");
            }
        } else {
            inputBuffer += c;                                   // At end of string, just append and echo
            logStream->write(c);
        }
    } else {
        if (cursorPos < (int)inputBuffer.length()) {            // Overwrite mode: replace character at cursor position
            inputBuffer.setCharAt(cursorPos, c);                // Replace character at cursor position
            logStream->write(c);                                       // Echo the character
        } else {
            inputBuffer += c;                                   // At end of string, append new character
            logStream->write(c);                                       // Echo the character
        }
    }
    cursorPos++;
}

// Handle all input control operations (escape sequences, backspace, newlines, etc.)
void Console::handleInputControl(char c) {
    switch (c) {
        case '\x1B':                                            // Escape character (start of escape sequence)
            handleEscapeSequence();
            break;
            
        case '\b':                                              // Backspace
        case '\x7F':                                            // DEL character (some terminals)
            handleBackspace();
            break;
            
        case '\x0C':                                            // Ctrl-L (clear screen)
            handleClearScreen();
            break;
            
        case '\n':                                              // Newline
        case '\r':                                              // Carriage return
            handleNewline();
            break;
            
        case '\t':                                              // Tab character (ignore for now)
            // Could be extended for command completion
            break;
            
        default:
            // Ignore other control characters
            break;
    }
}

// Helper method to handle escape sequences with timeout protection
void Console::handleEscapeSequence() {
    // Wait for next character with timeout (100ms)
    unsigned long timeout = millis() + 100;
    while (!logStream->available() && millis() < timeout);

    if (!logStream->available()) return;                               // Timeout - ignore incomplete sequence

    char c1 = logStream->read();
    if (c1 == '[') {
        handleCSISequence();                                    // Control Sequence Introducer
    } else if (c1 == 'O') {
        handleSSSequence();                                     // Single Shift sequence (function keys)
    }
    // Ignore other escape sequences
}

// Handle CSI (Control Sequence Introducer) sequences like ESC[A, ESC[2~, etc.
void Console::handleCSISequence() {
    unsigned long timeout = millis() + 100;
    while (!logStream->available() && millis() < timeout);

    if (!logStream->available()) return;                               // Timeout

    char c2 = logStream->read();
    switch (c2) {
        case 'A':                                               // Up arrow - Command history: previous
            handleArrowUp();
            break;
            
        case 'B':                                               // Down arrow - Command history: next
            handleArrowDown();
            break;
            
        case 'C':                                               // Right arrow - Move cursor right
            handleArrowRight();
            break;
            
        case 'D':                                               // Left arrow - Move cursor left
            handleArrowLeft();
            break;
            
        case 'H':                                               // Home key
            handleHome();
            break;
            
        case 'F':                                               // End key
            handleEnd();
            break;
            
        case '1':                                               // Function keys F1-F4 (ESC[1~)
        case '2':                                               // Insert key (ESC[2~)
        case '3':                                               // Delete key (ESC[3~)
        case '4':                                               // End key alternative (ESC[4~)
        case '5':                                               // Page Up (ESC[5~)
        case '6':                                               // Page Down (ESC[6~)
            handleExtendedKey(c2);
            break;
            
        default:
            // Ignore unknown sequences
            break;
    }
}

// Handle SS (Single Shift) sequences like ESC-O for function keys
void Console::handleSSSequence() {
    unsigned long timeout = millis() + 100;
    while (!logStream->available() && millis() < timeout);

    if (!logStream->available()) return;                               // Timeout

    char c2 = logStream->read();
    switch (c2) {
        case 'H':                                               // Home key (alternative)
            handleHome();
            break;
            
        case 'F':                                               // End key (alternative)
            handleEnd();
            break;
            
        case 'P':                                               // F1 key
        case 'Q':                                               // F2 key
        case 'R':                                               // F3 key
        case 'S':                                               // F4 key
            // Function keys - could be used for shortcuts in the future
            break;
            
        default:
            // Ignore unknown sequences
            break;
    }
}

// Handle extended keys that end with '~'
void Console::handleExtendedKey(char keyCode) {
    unsigned long timeout = millis() + 100;
    while (!logStream->available() && millis() < timeout);

    if (!logStream->available()) return;                               // Timeout

    char c3 = logStream->read();
    if (c3 == '~') {
        switch (keyCode) {
            case '2':                                           // Insert key
                handleInsertKey();
                break;
                
            case '3':                                           // Delete key
                handleDeleteKey();
                break;
                
            case '1':                                           // Home key (alternative)
            case '4':                                           // End key (alternative)
            case '5':                                           // Page Up
            case '6':                                           // Page Down
                // Could implement page up/down for command history scrolling
                break;
        }
    }
}

// Arrow key handlers
void Console::handleArrowUp() {
    String prevCommand = commandHistory.getPrevious();
    if (prevCommand.length() > 0) {
        clearAndRedrawLine(prevCommand);
    }
}

void Console::handleArrowDown() {
    String nextCommand = commandHistory.getNext();
    clearAndRedrawLine(nextCommand);
}

void Console::handleArrowRight() {
    if (cursorPos < (int)inputBuffer.length()) {
        print("\033[C");
        cursorPos++;
    }
}

void Console::handleArrowLeft() {
    if (cursorPos > 0) {
        print("\033[D");
        cursorPos--;
    }
}

// Navigation key handlers
void Console::handleHome() {
    while (cursorPos > 0) {
        print("\033[D");
        cursorPos--;
    }
}

void Console::handleEnd() {
    while (cursorPos < (int)inputBuffer.length()) {
        print("\033[C");
        cursorPos++;
    }
}

// Special key handlers
void Console::handleInsertKey() {
    insertMode = !insertMode;
    // Change cursor type based on mode
    print(insertMode ? "\033[4 q" : "\033[2 q");           // Underline for insert, block for overwrite
}

void Console::handleDeleteKey() {
    if (cursorPos < (int)inputBuffer.length()) {
        inputBuffer.remove(cursorPos, 1);
        refreshLineFromCursor();
    }
}

void Console::handleBackspace() {
    if (cursorPos > 0) {
        cursorPos--;
        inputBuffer.remove(cursorPos, 1);
        print("\b");
        refreshLineFromCursor();
    }
}

void Console::handleClearScreen() {
    print("\033[2J\033[H");                                 // Clear screen and move cursor to home
    inputBuffer = "";
    cursorPos = 0;
    print(shell);
}

void Console::handleNewline() {
    if (inputBuffer.length() > 0) {
        print("\n\r");
        processInput(inputBuffer);
        resetInputState();
        print(shell);
    } else {
        print("\n\r");
        print(shell);
    }
}

// Clear current input line and redraw with new content
void Console::clearAndRedrawLine(const String& newContent) {
    // Move cursor to beginning of input
    while (cursorPos > 0) {
        print("\b");
        cursorPos--;
    }
    
    // Clear the entire line content
    print("\033[K");                                        // Clear from cursor to end of line
    
    // Set new content and redraw
    inputBuffer = newContent;
    print(inputBuffer);
    cursorPos = inputBuffer.length();
}

// Refresh line after cursor position changes
void Console::refreshLineFromCursor() {
    print("\033[K");                                        // Clear from cursor to end
    String remaining = inputBuffer.substring(cursorPos);
    print(remaining);

    // Move cursor back to correct position
    for (int i = 0; i < (int)remaining.length(); i++) {
        print("\033[D");
    }
}

// Helper to reset input state after command processing
void Console::resetInputState() {
    inputBuffer = "";
    cursorPos = 0;
    commandHistory.resetToEnd();
    insertMode = true;
    print("\033[4 q");                                      // Reset to insert mode cursor
}

// Print float value
void Console::print(float value) {
    if (logStream) {
        logStream->print(value);
    }
}

// Print float value with newline
void Console::println(float value) {
    if (logStream) {
        logStream->println(value);
    }
}

// Print int value
void Console::print(int value) {
    if (logStream) {
        logStream->print(value);
    }
}

// Print int value with newline
void Console::println(int value) {
    if (logStream) {
        logStream->println(value);
    }
}