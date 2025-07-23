#include "Console.h"

// Constructor for Console class
Console::Console(Stream& stream) : con(stream), inputBuffer(""), shell("$") {}

// Initialize the console with a baud rate and instances of Hexapod, Turret, GaitController, and Microcontroller
void Console::begin(unsigned long baud, Hexapod* hexapod, Turret* turret, GaitController* gc, Microcontroller* mc) {
    
    if (&con == &Serial) {
        Serial.begin(baud);
        #ifdef DEBUG
            while (!Serial);  // Wait for Serial if using USB
            con.println("Ready. Type a command or help.");
            con.print(shell);
        #endif // DEBUG
    } 
    
    this->hexapod   = hexapod;      // Store the hexapod instance
    this->turret    = turret;       // Store the turret instance
    this->gc        = gc;           // Store the GaitController instance
    this->mc        = mc;           // Store the Microcontroller instance
}

// Update the console, reading input and processing commands
void Console::update() {
    while (con.available()) {
        
        char c = con.read();                                    // Read a character from the console input

        if (c == '\x1B') {                                      // Escape character (start of arrow key sequence)
            while (!con.available());                           // Wait for next two bytes: '[' and the code
            char c1 = con.read();

            if (c1 == '[' && con.available()) {
                char c2 = con.read();
                switch (c2) {                                   // Arrow keys: A=up, B=down, C=right, D=left
                    case 'A':                                   // ESC[A Up arrow - Command history: previous
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

                    case 'B':                                   // ESC[B Down arrow - Command history: next
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

                    case 'C':                                   // ESC[C Right arrow - Move cursor right if not at end
                        if (cursorPos < (int)inputBuffer.length()) {
                            con.print("\033[C");
                            cursorPos++;
                        }
                    break;

                    case 'D':                                   // ESC[D Left arrow - Move cursor left if not at start
                        if (cursorPos > 0) {
                            con.print("\033[D");
                            cursorPos--;
                        }
                    break;

                    case '1':                                   // Home key sequence: ESC[1~
                        if (con.available()) {
                            char c3 = con.read();
                            if (c3 == '~') {
                                // Move cursor to beginning of line
                                while (cursorPos > 0) {
                                    con.print("\033[D");            // Move cursor left
                                    cursorPos--;
                                }
                            }
                        }
                    break;

                    case '2':                                   // Insert key sequence: ESC[2~
                        if (con.available()) {
                            char c3 = con.read();
                            if (c3 == '~') {
                                insertMode = !insertMode;           // Toggle insert/overwrite mode
                                // Visual feedback could be added here if desired
                            }
                        }
                    break;

                    case '3':                                   // Delete key sequence: ESC[3~
                        if (con.available()) {
                            char c3 = con.read();
                            if (c3 == '~') {
                                
                                if (cursorPos < (int)inputBuffer.length()) {                // Delete character at cursor position
                                    int savedPos = cursorPos;                               // Save current cursor position
                                    inputBuffer.remove(cursorPos);                          // Remove character from buffer
                                    con.print("\033[K");                                    // Clear line from cursor to end
                                    String remaining = inputBuffer.substring(cursorPos);    // Print remaining text after cursor
                                    con.print(remaining);
                                    for (int i = remaining.length(); i > 0; i--) {
                                        con.print("\033[D");                                // Move cursor back to original position
                                    }
                                }
                            }
                        }
                    break;

                    case '4':                                               // End key sequence: ESC[4~
                        if (con.available()) {
                            char c3 = con.read();
                            if (c3 == '~') {
                                while (cursorPos < (int)inputBuffer.length()) { // Move cursor to end of line
                                    con.print("\033[C");                        // Move cursor right
                                    cursorPos++;
                                }
                            }
                        }
                    break;
                }
            }
        } else if (c == '\b') {                                 // Backspace
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1);   // Remove last character from input buffer
                con.print("\b \b");                             // Move cursor back, print space to erase character, move cursor back again
            }
        } else if (c == '\x0C') {                               // Ctrl-L (ASCII 12)
            con.print("\033[2J\033[H");                         // ANSI escape code to clear screen and move cursor to home
            inputBuffer = "";                                   // Clear input buffer
            con.print(shell);                                   // Print shell prompt

        } else {
            con.write(c);                                       // Echo to Serial for debugging
            if (c == '\n' || c == '\r') {                       // Newline or carriage return
                if (inputBuffer.length() > 0) {                 // If there is input in the buffer
                    con.print("\n\r");                          // Print newline and carriage return to move to the next line
                    processCommand(inputBuffer);                // Process the command entered
                    inputBuffer = "";
                    cursorPos = 0;                              // Reset cursor position
                    commandHistory.resetToEnd();                // Reset command history cursor to end
                    con.print(shell);                           // Print shell prompt again
                } else {
                    con.print("\n\r"+shell);                    // If no input, just print the shell prompt again
                }

            } else {
                inputBuffer += c;                               // Add character to input buffer
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

    } else {
        con.println("[Error] Unknown command: " + command);
        con.println("Type 'help', 'h' or '?' for a list of commands.");

    }
}


