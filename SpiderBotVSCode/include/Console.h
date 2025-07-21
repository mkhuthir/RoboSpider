#ifndef CONSOLE_H
#define CONSOLE_H

    #include <Arduino.h>
    #include "Config.h"  // Include configuration header

    class Console {
    public:
        Console(Stream& stream = DEBUG_SERIAL);
        void begin(unsigned long baud = DEBUG_BAUD_RATE);
        void update();  // Call in loop()

    private:
        Stream&     con;
        String      inputBuffer;

        void processCommand(const String& command);
    };

#endif
