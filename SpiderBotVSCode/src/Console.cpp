#include "Console.h"

Console::Console(Stream& stream) : con(stream), inputBuffer(""), shell("$") {}

void Console::begin(unsigned long baud) {
    
    if (&con == &Serial) {
        Serial.begin(baud);
        #ifdef DEBUG
            while (!Serial);  // Wait for Serial if using USB
        #endif // DEBUG
        con.println("Ready. Type command or help.");
        con.print("\n"+shell);
    }
}

void Console::update() {
    while (con.available()) {
        char c = con.read();
        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
                inputBuffer = "";
            } else {
                con.print("\n"+shell);
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

    if (command == "walk") {
        con.println("[Action] Start walking");
        
    } else if (command == "stop") {
        con.println("[Action] Stop walking");
        
    } else if (command == "status") {
        con.println("[Action] Report system status...");
        
    } else {
        con.println("[Error] Unknown command: " + command);
        con.println("Type 'help' or '?' for a list of commands.");
    }
    con.print("\n" + shell);
}
