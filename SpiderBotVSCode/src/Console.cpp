#include "Console.h"

// Constructor for Console class
Console::Console(){
    shell = "$";
    cursorPos = 0;
    insertMode = true;
}

// Initialize the console with a baud rate and instances of Hexapod, Turret, GaitController, and Microcontroller
bool Console::begin(Stream& stream,
                    unsigned long baud,
                    Servo* servo,
                    Hexapod* hexapod,
                    Turret* turret,
                    AXS1Sensor* sensor,
                    GaitController* gc,
                    Microcontroller* mc) {

    con = stream;                                       // Set the console stream

    if (&con == &Serial) {
        Serial.begin(baud);
        while (!Serial);                                // Wait for Serial if using USB
    }

    if (servo == nullptr || hexapod == nullptr || turret == nullptr || sensor == nullptr || gc == nullptr || mc == nullptr) {
        con.println("[Error] One or more components are not initialized.");
        return false;  // Return false if any component is not initialized
    } else {
        this->servo     = servo;        // Store the Servo instance
        this->hexapod   = hexapod;      // Store the hexapod instance
        this->turret    = turret;       // Store the turret instance
        this->sensor    = sensor;       // Store the AXS1Sensor instance (can be nullptr)
        this->gc        = gc;           // Store the GaitController instance
        this->mc        = mc;           // Store the Microcontroller instance
    }

    con.print("\033[2J\033[H");         // Clear the console screen and move cursor to home position
    con.println(shell);                 // Print initial message
    commandHistory.resetToEnd();        // Reset command history to end

    return true;
}

// Update the console, reading input and processing commands
void Console::update() {
    while (con.available()) {
        char c = con.read();                                        // Read a character from the console input

        if (c >= 32 && c <= 126) {                                  // Printable ASCII characters
            handlePrintableChar(c);
        } else {
            handleInputControl(c);                                  // Control characters and special sequences
        }
    }
}

// Process the command entered by the user
void Console::processCommand(const String& command) {
    // Trim whitespace and convert to lowercase for consistent handling
    String cmd = command;
    cmd.trim();
    cmd.toLowerCase();
    
    // Skip empty commands
    if (cmd.length() == 0) {
        return;
    }

    commandHistory.addCommand(command);                     // Add original command to history
    
    #ifdef DEBUG
        con.print("[Console] Received: ");
        con.println(cmd);
    #endif // DEBUG
    
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
    if (!executeSystemCommand(mainCmd, args) &&
        !executeTurretCommand(mainCmd, args) &&
        !executeGaitCommand(mainCmd, args) &&
        !executeHexapodCommand(mainCmd, args) &&
        !executeLegCommand(mainCmd, args) &&
        !executeSensorCommand(mainCmd, args)) {
        
        // Unknown command
        con.println("[Error] Unknown command: " + command);
        con.println("Type 'help', 'h' or '?' for a list of commands.");
    }
}

// System commands (help, cls, status)
bool Console::executeSystemCommand(const String& cmd, const String& args) {
    if (cmd == "help" || cmd == "h" || cmd == "?") {
        printHelp();
        return true;
    } else if (cmd == "cls" || cmd == "clear") {
        con.print("\033[2J\033[H");                     // ANSI escape code to clear screen and move cursor to home
        return true;
    } else if (cmd == "status") {
        printSystemStatus();
        return true;
    }
    return false;
}

// Turret control commands
bool Console::executeTurretCommand(const String& cmd, const String& args) {
    if (cmd == "tu") {
        turret->moveUp();
        con.println("[Info] Turret moving up");
        return true;
    } else if (cmd == "td") {
        turret->moveDown();
        con.println("[Info] Turret moving down");
        return true;
    } else if (cmd == "tl") {
        turret->moveLeft();
        con.println("[Info] Turret moving left");
        return true;
    } else if (cmd == "tr") {
        turret->moveRight();
        con.println("[Info] Turret moving right");
        return true;
    } else if (cmd == "th") {
        turret->moveHome();
        con.println("[Info] Turret moving to home position");
        return true;
    }
    return false;
}

// Gait control commands
bool Console::executeGaitCommand(const String& cmd, const String& args) {
    if (cmd == "gw") {
        gc->setGait(GAIT_WAVE);
        con.println("[Info] Gait set to WAVE");
        return true;
    } else if (cmd == "gr") {
        gc->setGait(GAIT_RIPPLE);
        con.println("[Info] Gait set to RIPPLE");
        return true;
    } else if (cmd == "gt") {
        gc->setGait(GAIT_TRIPOD);
        con.println("[Info] Gait set to TRIPOD");
        return true;
    } else if (cmd == "gi") {
        gc->setGait(GAIT_IDLE);
        con.println("[Info] Gait set to IDLE");
        return true;
    }
    return false;
}

// Hexapod control commands
bool Console::executeHexapodCommand(const String& cmd, const String& args) {
    if (cmd == "hsu") {
        hexapod->moveStandUp();
        con.println("[Info] Hexapod standing up");
        return true;
    } else if (cmd == "hsd") {
        hexapod->moveStandDown();
        con.println("[Info] Hexapod standing down");
        return true;
    }
    return false;
}

// Leg control commands (operates on leg 0 by default, could be extended for specific legs)
bool Console::executeLegCommand(const String& cmd, const String& args) {
    int legIndex = 0; // Default to leg 0
    
    // Parse leg number from arguments if provided
    if (args.length() > 0) {
        int parsedLeg = args.toInt();
        if (parsedLeg >= 0 && parsedLeg < 6) { // Assuming 6 legs (0-5)
            legIndex = parsedLeg;
        }
    }
    
    if (cmd == "lpu") {
        hexapod->legs[legIndex].movePointUp();
        con.println("[Info] Leg " + String(legIndex) + " point moving up");
        return true;
    } else if (cmd == "lpd") {
        hexapod->legs[legIndex].movePointDown();
        con.println("[Info] Leg " + String(legIndex) + " point moving down");
        return true;
    } else if (cmd == "lpo") {
        hexapod->legs[legIndex].movePointOut();
        con.println("[Info] Leg " + String(legIndex) + " point moving out");
        return true;
    } else if (cmd == "lsu") {
        hexapod->legs[legIndex].moveStandUp();
        con.println("[Info] Leg " + String(legIndex) + " standing up");
        return true;
    } else if (cmd == "lsd") {
        hexapod->legs[legIndex].moveStandDown();
        con.println("[Info] Leg " + String(legIndex) + " standing down");
        return true;
    }
    return false;
}

// AXS1 Sensor commands
bool Console::executeSensorCommand(const String& cmd, const String& args) {
    // Check if sensor is available
    if (sensor == nullptr) {
        con.println("[Error] AXS1 Sensor not initialized");
        return false;
    }
    
    if (cmd == "sp") {
        bool result = sensor->ping();
        con.println("[Info] Sensor ping: " + String(result ? "SUCCESS" : "FAILED"));
        return true;
    } else if (cmd == "sir") {
        con.println("[Info] IR Sensors - Left: " + String(sensor->getIRLeft()) + 
                   ", Center: " + String(sensor->getIRCenter()) + 
                   ", Right: " + String(sensor->getIRRight()));
        return true;
    } else if (cmd == "sli") {
        con.println("[Info] Light Sensors - Left: " + String(sensor->getLightLeft()) + 
                   ", Center: " + String(sensor->getLightCenter()) + 
                   ", Right: " + String(sensor->getLightRight()));
        return true;
    } else if (cmd == "sso") {
        con.println("[Info] Sound - Level: " + String(sensor->getSoundLevel()) + 
                   ", Count: " + String(sensor->getSoundCount()));
        return true;
    } else if (cmd == "sve") {
        con.println("[Info] Voltage: " + String(sensor->getVoltage(), 2) + "V");
        return true;
    } else if (cmd == "ste") {
        con.println("[Info] Temperature: " + String(sensor->getTemperature(), 1) + "°C");
        return true;
    } else if (cmd == "sbu") {
        // Parse buzzer arguments: note and duration
        int note = 60;      // Default note (middle C)
        int duration = 100; // Default duration (100ms)
        
        if (args.length() > 0) {
            int spaceIndex = args.indexOf(' ');
            if (spaceIndex != -1) {
                note = args.substring(0, spaceIndex).toInt();
                duration = args.substring(spaceIndex + 1).toInt();
            } else {
                note = args.toInt();
            }
        }
        
        // Validate ranges
        if (note < 32 || note > 127) note = 60;
        if (duration < 1 || duration > 255) duration = 100;
        
        bool result = sensor->setBuzzer(note, duration);
        con.println("[Info] Buzzer " + String(result ? "activated" : "failed") + 
                   " - Note: " + String(note) + ", Duration: " + String(duration) + "ms");
        return true;
    } else if (cmd == "sup") {
        bool result = sensor->update();
        con.println("[Info] Sensor update: " + String(result ? "SUCCESS" : "FAILED"));
        return true;
    }
    return false;
}

// Print comprehensive help information
void Console::printHelp() {
    con.println("SpiderBot Console - Available Commands:");
    con.println("");
    con.println("System Commands:");
    con.println("  help/h/?         - Show this help message");
    con.println("  cls/clear        - Clear the terminal screen");
    con.println("  status           - Show current status of all systems");
    con.println("");
    con.println("Turret Commands:");
    con.println("  tu               - Move turret up");
    con.println("  td               - Move turret down");
    con.println("  tl               - Move turret left");
    con.println("  tr               - Move turret right");
    con.println("  th               - Move turret to home position");
    con.println("");
    con.println("Gait Commands:");
    con.println("  gw               - Set wave gait");
    con.println("  gr               - Set ripple gait");
    con.println("  gt               - Set tripod gait");
    con.println("  gi               - Set idle gait");
    con.println("");
    con.println("Hexapod Commands:");
    con.println("  hsu              - Hexapod stand up");
    con.println("  hsd              - Hexapod stand down");
    con.println("");
    con.println("Leg Commands (add leg number 0-5 as argument, default=0):");
    con.println("  lpu [n]          - Move leg point up");
    con.println("  lpd [n]          - Move leg point down");
    con.println("  lpo [n]          - Move leg point out");
    con.println("  lsu [n]          - Move leg to stand up position");
    con.println("  lsd [n]          - Move leg to stand down position");
    con.println("");
    con.println("Sensor Commands (AXS1):");
    con.println("  sp               - Ping sensor (connectivity test)");
    con.println("  sir              - Read IR sensors (left, center, right)");
    con.println("  sli              - Read light sensors (left, center, right)");
    con.println("  sso              - Read sound level and count");
    con.println("  sve              - Read voltage");
    con.println("  ste              - Read temperature");
    con.println("  sbu [note] [dur] - Play buzzer (note 32-127, duration 1-255ms)");
    con.println("  sup              - Update sensor readings");
    con.println("");
    con.println("Examples: 'lpu 2' moves leg 2 point up, 'sbu 72 200' plays note");
}

// Print system status information
void Console::printSystemStatus() {
    con.println("=== SpiderBot System Status ===");
    if (mc) mc->printStatus(con);
    if (hexapod) hexapod->printStatus(con);
    if (turret) turret->printStatus(con);
    if (gc) gc->printStatus(con);
    if (sensor) {
        con.println("AXS1 Sensor Status:");
        con.println("  Voltage: " + String(sensor->getVoltage(), 2) + "V");
        con.println("  Temperature: " + String(sensor->getTemperature(), 1) + "°C");
        con.println("  IR (L/C/R): " + String(sensor->getIRLeft()) + "/" + 
                   String(sensor->getIRCenter()) + "/" + String(sensor->getIRRight()));
        con.println("  Light (L/C/R): " + String(sensor->getLightLeft()) + "/" + 
                   String(sensor->getLightCenter()) + "/" + String(sensor->getLightRight()));
        con.println("  Sound Level: " + String(sensor->getSoundLevel()));
    }
    con.println("==============================");
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

// Handle printable character input and display
void Console::handlePrintableChar(char c) {
    if (insertMode) {
        if (cursorPos < (int)inputBuffer.length()) {            // Insert mode: insert character at cursor position
            String before = inputBuffer.substring(0, cursorPos);    // Insert character in the middle of the string
            String after = inputBuffer.substring(cursorPos);
            inputBuffer = before + c + after;
            con.print("\033[K");                                // Clear from cursor to end and reprint
            String remaining = inputBuffer.substring(cursorPos);
            con.print(remaining);
            for (int i = remaining.length() - 1; i > 0; i--) {  // Move cursor back to position after inserted character
                con.print("\033[D");
            }
        } else {
            inputBuffer += c;                                   // At end of string, just append and echo
            con.write(c);
        }
    } else {
        if (cursorPos < (int)inputBuffer.length()) {            // Overwrite mode: replace character at cursor position
            inputBuffer.setCharAt(cursorPos, c);                // Replace character at cursor position
            con.write(c);                                       // Echo the character
        } else {
            inputBuffer += c;                                   // At end of string, append new character
            con.write(c);                                       // Echo the character
        }
    }
    cursorPos++;
}

// Helper method to handle escape sequences with timeout protection
void Console::handleEscapeSequence() {
    // Wait for next character with timeout (100ms)
    unsigned long timeout = millis() + 100;
    while (!con.available() && millis() < timeout);
    
    if (!con.available()) return;                               // Timeout - ignore incomplete sequence
    
    char c1 = con.read();
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
    while (!con.available() && millis() < timeout);
    
    if (!con.available()) return;                               // Timeout
    
    char c2 = con.read();
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
    while (!con.available() && millis() < timeout);
    
    if (!con.available()) return;                               // Timeout
    
    char c2 = con.read();
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
    while (!con.available() && millis() < timeout);
    
    if (!con.available()) return;                               // Timeout
    
    char c3 = con.read();
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

// Clear current input line and redraw with new content
void Console::clearAndRedrawLine(const String& newContent) {
    // Move cursor to beginning of input
    while (cursorPos > 0) {
        con.print("\b");
        cursorPos--;
    }
    
    // Clear the entire line content
    con.print("\033[K");                                        // Clear from cursor to end of line
    
    // Set new content and redraw
    inputBuffer = newContent;
    con.print(inputBuffer);
    cursorPos = inputBuffer.length();
}

// Refresh line after cursor position changes
void Console::refreshLineFromCursor() {
    con.print("\033[K");                                        // Clear from cursor to end
    String remaining = inputBuffer.substring(cursorPos);
    con.print(remaining);
    
    // Move cursor back to correct position
    for (int i = 0; i < (int)remaining.length(); i++) {
        con.print("\033[D");
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
        con.print("\033[C");
        cursorPos++;
    }
}

void Console::handleArrowLeft() {
    if (cursorPos > 0) {
        con.print("\033[D");
        cursorPos--;
    }
}

// Navigation key handlers
void Console::handleHome() {
    while (cursorPos > 0) {
        con.print("\033[D");
        cursorPos--;
    }
}

void Console::handleEnd() {
    while (cursorPos < (int)inputBuffer.length()) {
        con.print("\033[C");
        cursorPos++;
    }
}

// Special key handlers
void Console::handleInsertKey() {
    insertMode = !insertMode;
    // Change cursor type based on mode
    con.print(insertMode ? "\033[4 q" : "\033[2 q");           // Underline for insert, block for overwrite
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
        con.print("\b");
        refreshLineFromCursor();
    }
}

void Console::handleClearScreen() {
    con.print("\033[2J\033[H");                                 // Clear screen and move cursor to home
    inputBuffer = "";
    cursorPos = 0;
    con.print(shell);
}

void Console::handleNewline() {
    if (inputBuffer.length() > 0) {
        con.print("\n\r");
        processCommand(inputBuffer);
        resetInputState();
        con.print(shell);
    } else {
        con.print("\n\r");
        con.print(shell);
    }
}

// Helper to reset input state after command processing
void Console::resetInputState() {
    inputBuffer = "";
    cursorPos = 0;
    commandHistory.resetToEnd();
    insertMode = true;
    con.print("\033[4 q");                                      // Reset to insert mode cursor
}