#include "Console.h"

Console::Console(Stream& stream) : con(stream), inputBuffer(""), shell("\n\r$") {}

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

void Console::update() {
    while (con.available()) {
        char c = con.read();
        con.write(c);  // Echo to Serial for debugging

        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                con.print("\n\r");
                processCommand(inputBuffer);
                inputBuffer = "";
            } else {
                con.print(shell);
            }
        } else {
            inputBuffer += c;
        }
    }
}

void Console::processCommand(const String& command) {
    #ifdef DEBUG
        con.print("[Console] Received: ");
        con.println(command);
    #endif // DEBUG

    if (command == "status") {
        mc->printStatus(con);
        hexapod->printStatus(con);
        turret->printStatus(con);
        gc->printStatus(con);

    } else if (command == "help" || command == "h" || command == "?") {
        con.println("Available commands:");
        con.println("  status    - Show current status of the system");
        con.print(  "  help/h/?  - Show this help message");
    } else {
        con.println("[Error] Unknown command: " + command);
        con.print("Type 'help' or '?' for a list of commands.");
    }
    con.print(shell);
}
