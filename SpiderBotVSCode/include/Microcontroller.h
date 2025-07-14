#ifndef MICROCONTROLLER_H
#define MICROCONTROLLER_H   

Class Microcontroller {
    public:
        Microcontroller();
        bool    init();                                             // initialize the microcontroller
        bool    LEDOn(uint8_t LED_id);                            // turn on the user control LED
        bool    LEDOff(uint8_t LED_id);                           // turn off the user control LED
        bool    PlayMelody();                                     // play a melody using the servos
        float   BatteryVoltage();                                 // get the battery voltage

    private:
        
};

#endif // MICROCONTROLLER_H
