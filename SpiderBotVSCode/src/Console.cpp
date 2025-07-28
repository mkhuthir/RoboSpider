#include "Console.h"

// Constructor for Console class
Console::Console(Stream& stream) : con(stream), inputBuffer(""), shell("$") {}

// Initialize the console with a baud rate and instances of Hexapod, Turret, GaitController, and Microcontroller
void Console::begin(unsigned long baud, Hexapod* hexapod, Turret* turret, GaitController* gc, Microcontroller* mc) {
    
    if (&con == &Serial) {
        Serial.begin(baud);
        while (!Serial);                                    // Wait for Serial if using USB
    }
    
    con.println("Ready. Type '?' for commands.");    // Print initial message
    con.print(shell);                                   // Print shell prompt
    con.print("\033[4 q");                              // Set initial cursor to underline (insert mode)

   
    
    this->hexapod   = hexapod;      // Store the hexapod instance
    this->turret    = turret;       // Store the turret instance
    this->gc        = gc;           // Store the GaitController instance
    this->mc        = mc;           // Store the Microcontroller instance
}

// Update the console, reading input and processing commands
void Console::update() {
    while (con.available()) {
        
        char c = con.read();                                        // Read a character from the console input

        if (c == '\x1B') {                                          // Escape character (start of arrow key sequence)
            while (!con.available());                               // Wait for next two bytes: '[' and the code
            char c1 = con.read();

            if (c1 == '[' && con.available()) {
                char c2 = con.read();
                switch (c2) {                                       // Arrow keys: A=up, B=down, C=right, D=left
                    case 'A':                                       // ESC[A Up arrow - Command history: previous
                        {
                            String prevCommand = commandHistory.getPrevious();
                            if (prevCommand.length() > 0) {
                                while (inputBuffer.length() > 0) {  // Clear current line
                                    con.print("\b \b");
                                    inputBuffer.remove(inputBuffer.length() - 1);
                                }
                                inputBuffer = prevCommand;
                                con.print(inputBuffer);
                                cursorPos = inputBuffer.length();
                            }
                        }
                    break;

                    case 'B':                                       // ESC[B Down arrow - Command history: next
                        {
                            while (inputBuffer.length() > 0) {      // Clear current line
                                con.print("\b \b");
                                inputBuffer.remove(inputBuffer.length() - 1);
                            }
                            
                            String nextCommand = commandHistory.getNext();
                            inputBuffer = nextCommand;
                            con.print(inputBuffer);
                            cursorPos = inputBuffer.length();
                        }
                    break;

                    case 'C':                                       // ESC[C Right arrow - Move cursor right if not at end
                        if (cursorPos < (int)inputBuffer.length()) {
                            con.print("\033[C");
                            cursorPos++;
                        }
                    break;

                    case 'D':                                       // ESC[D Left arrow - Move cursor left if not at start
                        if (cursorPos > 0) {
                            con.print("\033[D");
                            cursorPos--;
                        }
                    break;

                    case 'H':                                   // Home key sequence: ESC[H
                        while (cursorPos > 0) {                 // Move cursor to beginning of line
                            con.print("\033[D");                // Move cursor left
                            cursorPos--;
                        }
                    break;

                    case 'F':                                               // End key sequence: ESC[F
                        while (cursorPos < (int)inputBuffer.length()) {     // Move cursor to end of line
                            con.print("\033[C");                            // Move cursor right
                            cursorPos++;
                        }
                    break;

                    case '2':                                                               // Insert key sequence: ESC[2~
                        if (con.available()) {
                            char c3 = con.read();
                            if (c3 == '~') {
                                insertMode = !insertMode;                                   // Toggle insert/overwrite mode
                                                                                            // Change cursor type based on mode             
                                con.print(insertMode ? "\033[4 q" : "\033[2 q");            // Underline for insert, block for overwrite
                            }
                        }
                    break;

                    case '3':                                                               // Delete key sequence: ESC[3~
                        if (con.available()) {
                            char c3 = con.read();
                            if (c3 == '~') {
                                
                                if (cursorPos < (int)inputBuffer.length()) {                // Delete character at cursor position
                                    inputBuffer.remove(cursorPos, 1);                       // Remove one character at cursor position
                                    con.print("\033[K");                                    // Clear line from cursor to end
                                    String remaining = inputBuffer.substring(cursorPos);    // Print remaining text after cursor
                                    con.print(remaining);
                                    for (int i = 0; i < (int)remaining.length(); i++) {     // Move cursor back to original position
                                        con.print("\033[D");                                // Move cursor left
                                    }
                                }
                            }
                        }
                    break;
                }
            }
        } else if (c == '\b') {                                         // Backspace
            if (cursorPos > 0) {
                cursorPos--;
                inputBuffer.remove(cursorPos, 1);                       // Remove character before cursor
                con.print("\b");                                        // Move cursor back
                con.print("\033[K");                                    // Clear line from cursor to end
                String remaining = inputBuffer.substring(cursorPos);    // Print remaining text after cursor
                con.print(remaining);
                
                for (int i = 0; i < (int)remaining.length(); i++) {     // Move cursor back to correct position
                    con.print("\033[D");
                }
            }
        } else if (c == '\x0C') {                               // Ctrl-L (ASCII 12)
            con.print("\033[2J\033[H");                         // ANSI escape code to clear screen and move cursor to home
            inputBuffer = "";                                   // Clear input buffer
            con.print(shell);                                   // Print shell prompt

        } else {
            if (c == '\n' || c == '\r') {                       // Newline or carriage return
                if (inputBuffer.length() > 0) {                 // If there is input in the buffer
                    con.print("\n\r");                          // Print newline and carriage return to move to the next line
                    processCommand(inputBuffer);                // Process the command entered
                    inputBuffer = "";
                    cursorPos = 0;                              // Reset cursor position
                    commandHistory.resetToEnd();                // Reset command history cursor to end
                    con.print("\033[4 q");                      // Reset cursor to insert mode
                    con.print(shell);                           // Print shell prompt again
                } else {
                    con.print("\n\r"+shell);                    // If no input, just print the shell prompt again
                    inputBuffer = "";                           // Clear input buffer
                    cursorPos = 0;                              // Reset cursor position
                    commandHistory.resetToEnd();                // Reset command history cursor to end
                }

            } else {
                if (insertMode) {
                    if (cursorPos < (int)inputBuffer.length()) {                // Insert mode: insert character at cursor position
                        String before = inputBuffer.substring(0, cursorPos);    // Insert character in the middle of the string
                        String after = inputBuffer.substring(cursorPos);
                        inputBuffer = before + c + after;
                        con.print("\033[K");                                    // Clear from cursor to end and reprint
                        String remaining = inputBuffer.substring(cursorPos);
                        con.print(remaining);
                        for (int i = remaining.length() - 1; i > 0; i--) {      // Move cursor back to position after inserted character
                            con.print("\033[D");
                        }
                    } else {
                        inputBuffer += c;                                       // At end of string, just append and echo
                        con.write(c);
                    }
                } else {
                    if (cursorPos < (int)inputBuffer.length()) {                // Overwrite mode: replace character at cursor position
                        inputBuffer.setCharAt(cursorPos, c);                    // Replace character at cursor position
                        con.write(c);                                           // Echo the character
                    } else {
                        inputBuffer += c;                                       // At end of string, append new character
                        con.write(c);                                           // Echo the character
                    }
                }
                cursorPos++;
            }
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
        con.println("  help/h/?  - Show this help message");
        con.println("  cls       - Clear the terminal screen");
        con.println("  status    - Show current status of the system");
        con.println("  t-up      - Turret up");
        con.println("  t-down    - Turret down");
        con.println("  t-left    - Turret left");
        con.println("  t-right   - Turret right");
        con.println("  t-home    - Turret home position");
        con.println("  g-wave    - Start wave gait");
        con.println("  g-ripple  - Start ripple gait");
        con.println("  g-tripod  - Start tripod gait");
        con.println("  g-idle    - Start idle gait");
        con.println("  h-up      - hexapod up");
        con.println("  h-down    - hexapod down");
        con.println("  l-up      - leg up");
        con.println("  l-down    - leg down");
        con.println("  l-out     - leg out");

    } else if (command == "cls") {                      
        con.print("\033[2J\033[H");                     // ANSI escape code to clear screen and move cursor to home

    } else if (command == "status") {
        mc->printStatus(con);
        hexapod->printStatus(con);
        turret->printStatus(con);
        gc->printStatus(con);

    } else if (command == "t-up") {
        turret->moveUp();

    } else if (command == "t-down") {
        turret->moveDown();

    } else if (command == "t-left") {
        turret->moveLeft();

    } else if (command == "t-right") {
        turret->moveRight();
        
    } else if (command == "t-home") {
        turret->moveHome();

    } else if (command == "g-wave") {
        gc->setGait(GAIT_WAVE);

    } else if (command == "g-ripple") {
        gc->setGait(GAIT_RIPPLE);

    } else if (command == "g-tripod") {
        gc->setGait(GAIT_TRIPOD);

    } else if (command == "g-idle") {
        gc->setGait(GAIT_IDLE);

    } else if (command == "h-up") {
        hexapod->moveUp();

    } else if (command == "h-down") {
        hexapod->moveDown();

    } else if (command == "l-up") {
        hexapod->legs[0].moveUp();

    } else if (command == "l-down") {
        hexapod->legs[0].moveDown();

    } else if (command == "l-out") {
        hexapod->legs[0].moveOut();

    } else {
        con.println("[Error] Unknown command: " + command);
        con.println("Type 'help', 'h' or '?' for a list of commands.");

    }
}


