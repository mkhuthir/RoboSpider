#ifndef MICROCONTROLLER_H
#define MICROCONTROLLER_H   

    #include <Arduino.h>          // Include Arduino library for basic functions

    class Microcontroller {
        public:
            Microcontroller();
            bool    begin();                                          // initialize the microcontroller
            void    update();                                         // update the microcontroller state
            bool    ledOn(uint8_t LED_id);                            // turn on the user control LED
            bool    ledOff(uint8_t LED_id);                           // turn off the user control LED
            bool    playMelody();                                     // play a melody using the servos
            float   batteryVoltage();                                 // get the battery voltage
            void    printStatus(Stream& stream);                      // print the current status to the given stream

        private:
            
    };

#endif // MICROCONTROLLER_H
