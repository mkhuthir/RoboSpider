#include "Console.h"

// Constructor for Console class
Console::Console(Stream& stream) : con(stream), inputBuffer(""), shell("\n\r$") {}

// Initialize the console with a baud rate and instances of Hexapod, Turret, GaitController, and Microcontroller
void Console::begin(unsigned long baud, Hexapod* hexapod, Turret* turret, GaitController* gc, Microcontroller* mc) {
    
    if (&con == &Serial) {
        Serial.begin(baud);
        #ifdef DEBUG
            while (!Serial);  // Wait for Serial if using USB
        #endif // DEBUG
        con.println("Ready. Type a command or help.");
        con.print(shell);
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

        if (c == '\b' || c == 127) {                            // Backspace or DEL
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1);   // Remove last character from input buffer
                con.print("\b \b");                             // Move cursor back, print space to erase character, move cursor back again
            }

        } else if (c == '\x1B') {                               // Escape character (start of arrow key sequence)
            while (!con.available());                           // Wait for next two bytes: '[' and the code
            char c1 = con.read();

            if (c1 == '[' && con.available()) {
                char c2 = con.read();
                switch (c2) {                                   // Arrow keys: A=up, B=down, C=right, D=left
                    case 'A':                                   // Up arrow - Command history: previous
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

                    case 'B':                                   // Down arrow - Command history: next
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

                    case 'C':                                   // Right arrow - Move cursor right if not at end
                    if (cursorPos < (int)inputBuffer.length()) {
                        con.print("\033[C");
                        cursorPos++;
                    }
                    break;

                    case 'D':                                   // Left arrow - Move cursor left if not at start
                    if (cursorPos > 0) {
                        con.print("\033[D");
                        cursorPos--;
                    }
                    break;
                }
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
                } else {
                    con.print(shell);                           // If no input, just print the shell prompt again
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
        con.print("Type 'help', 'h' or '?' for a list of commands.");

    }
    con.print(shell);
}


