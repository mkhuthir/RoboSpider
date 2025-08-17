#ifndef MICROCONTROLLER_H
#define MICROCONTROLLER_H   

    #include <Arduino.h>          // Include Arduino library for basic functions

    class Microcontroller {
        
        public:
            Microcontroller();
            bool            begin();                    // initialize the microcontroller
            bool            update();                   // update the microcontroller state
            
            bool            ledOn(uint8_t LED_id);      // turn on the user control LED
            bool            ledOff(uint8_t LED_id);     // turn off the user control LED
            bool            playMelody();               // play a melody using the servos
            
            void            resetMicrocontroller();     // reset the microcontroller
            bool            checkBattery();             // check the battery status
            float           getBatteryVoltage();        // get the battery voltage
            unsigned long   getUpTime();                // get the uptime of the microcontroller in milliseconds

            bool            printStatus();              // print the current status to the given stream
            bool            runConsoleCommands(const String& cmd, const String& args);  // Process console commands for microcontroller control
            bool            printConsoleHelp();                // Print microcontroller-specific help information

        private:
            
    };

#endif // MICROCONTROLLER_H
