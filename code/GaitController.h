#ifndef GaitController_h
#define GaitController_h

    #include "Hexapod.h"

    enum GaitType {
        GAIT_IDLE,
        GAIT_WAVE,
        GAIT_RIPPLE,
        GAIT_TRIPOD,
        GAIT_ROTATE
    };

    enum RotateDirection {
        ROTATE_CW,
        ROTATE_CCW
    };

    class GaitController {
        public:
            GaitController();                                           // Constructor
            bool            begin(Hexapod* hexapod);                    // Initialize with Hexapod
            bool            update();                                   // Update the gait controller

            bool            setGaitType(GaitType newGait);              // Set the current gait type
            GaitType        getGaitType() const;                        // Get the current gait type
            void            setWalkDirection(int8_t w_dir);               // -180 to 180
            int8_t          getWalkDirection() const;
            void            setRotateDirection(RotateDirection r_dir); // -180 to 180
            RotateDirection getRotateDirection() const;
            bool            setGaitSpeed(uint16_t speed);                   // 0 to 1023
            uint16_t        getGaitSpeed() const;
            void            setGaitStepSize(uint16_t step_size);                 // 0 to 1023
            uint16_t        getGaitStepSize() const;

            bool            printStatus();                              // Print current gait status to Serial
            bool            runConsoleCommands(const String& cmd, const String& args);  // Process console commands for gait control
            bool            printConsoleHelp();                         // Print gait-specific help information



        private:
            Hexapod*        hexapod;                                    // Pointer to the Hexapod instance

            uint8_t         currentPhase;                               // Current phase of the gait
            uint8_t         currentStep;                                // Current step in the gait sequence
            
            GaitType        gaitType;                                   // Current gait type
            int8_t          gaitWalkDirection;                          // -180 to 180
            RotateDirection gaitRotateDirection;                        // Clockwise or counter-clockwise
            uint16_t        gaitSpeed;                                  // 0 to 1023
            uint16_t        gaitStepSize;                               // 0 to 1023

            bool            doWaveGait();                               // Perform the wave gait
            bool            doRippleGait();                             // Perform the ripple gait
            bool            doTripodGait();                             // Perform the tripod gait
            bool            doRotateGait();                             // Perform the rotate gait
    };

#endif // GaitController_h
