#include "Console.h"

Console::Console(Stream& stream) : con(stream), inputBuffer("") {}

void Console::begin(unsigned long baud) {
    con.begin(baud);
    #ifdef DEBUG
        while (!con);  // Wait for the console to be ready
    #endif // DEBUG
    con.println("[Console] Ready. Type commands.");
}

void Console::update() {
    while (con.available()) {
        char c = con.read();
        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
                inputBuffer = "";
            }
        } else {
            inputBuffer += c;
        }
    }
}

void Console::processCommand(const String& command) {
    con.print("[Console] Received: ");
    con.println(command);

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
}
