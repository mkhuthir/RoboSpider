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
                    Hexapod* hexapod,
                    Turret* turret,
                    GaitController* gc,
                    Microcontroller* mc) {

    con = stream;                                       // Set the console stream

    if (&con == &Serial) {
        Serial.begin(baud);
        while (!Serial);                                // Wait for Serial if using USB
    }

    if (hexapod == nullptr || turret == nullptr || gc == nullptr || mc == nullptr) {
        con.println("[Error] One or more components are not initialized.");
        return false;  // Return false if any component is not initialized
    } else {
        this->hexapod   = hexapod;      // Store the hexapod instance
        this->turret    = turret;       // Store the turret instance
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

    commandHistory.addCommand(command);                     // Add command to history
    
    #ifdef DEBUG
        con.print("[Console] Received: ");
        con.println(command);
    #endif // DEBUG
    
    if (command == "help" || command == "h" || command == "?") {
        con.println("Available commands:");
        con.println("  help/h/? - Show this help message");
        con.println("  cls      - Clear the terminal screen");
        con.println("  status   - Show current status of the system");
        con.println("  tu       - Turret up");
        con.println("  td       - Turret down");
        con.println("  tl       - Turret left");
        con.println("  tr       - Turret right");
        con.println("  th       - Turret home");
        con.println("  gw       - wave gait");
        con.println("  gr       - ripple gait");
        con.println("  gt       - tripod gait");
        con.println("  gi       - idle gait");
        con.println("  hsu      - hexapod standup");
        con.println("  hsd      - hexapod standdown");
        con.println("  lpu      - leg up");
        con.println("  lpd      - leg down");
        con.println("  lpo      - leg out");
        con.println("  lsu      - leg standup");
        con.println("  lsd      - leg standdown");

    } else if (command == "cls") {                      
        con.print("\033[2J\033[H");                     // ANSI escape code to clear screen and move cursor to home

    } else if (command == "status") {
        mc->printStatus(con);
        hexapod->printStatus(con);
        turret->printStatus(con);
        gc->printStatus(con);

    } else if (command == "tu") {
        turret->moveUp();

    } else if (command == "td") {
        turret->moveDown();

    } else if (command == "tl") {
        turret->moveLeft();

    } else if (command == "tr") {
        turret->moveRight();

    } else if (command == "th") {
        turret->moveHome();

    } else if (command == "gw") {
        gc->setGait(GAIT_WAVE);

    } else if (command == "gr") {
        gc->setGait(GAIT_RIPPLE);

    } else if (command == "gt") {
        gc->setGait(GAIT_TRIPOD);

    } else if (command == "gi") {
        gc->setGait(GAIT_IDLE);

    } else if (command == "hsu") {
        hexapod->moveStandUp();

    } else if (command == "hsd") {
        hexapod->moveStandDown();

    } else if (command == "lpu") {
        hexapod->legs[0].movePointUp();

    } else if (command == "lpd") {
        hexapod->legs[0].movePointDown();

    } else if (command == "lpo") {
        hexapod->legs[0].movePointOut();

    } else if (command == "lsu") {
        hexapod->legs[0].moveStandUp();

    } else if (command == "lsd") {
        hexapod->legs[0].moveStandDown();

    } else {
        con.println("[Error] Unknown command: " + command);
        con.println("Type 'help', 'h' or '?' for a list of commands.");

    }
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