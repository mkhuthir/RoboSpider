#ifndef GaitController_h
#define GaitController_h

    #include "Hexapod.h"

    enum GaitType {
        GAIT_IDLE,
        GAIT_WAVE,
        GAIT_RIPPLE,
        GAIT_TRIPOD
    };

    class GaitController {
        public:
            GaitController();                                           // Constructor
            bool            begin(Hexapod* hexapod);                    // Initialize with Hexapod
            bool            update();                                   // Update the gait controller

            bool            setGait(GaitType newGait);                  // Set the current gait type
            GaitType        getGait() const;                            // Get the current gait type

            void            setDirection(int dir);      // -180 to 180
            int             getDirection() const;

            void            setSpeed(int speed);        // 0 to 1023
            int             getSpeed() const;

            void            setStepSize(float size);    // step size, e.g. mm or degrees
            float           getStepSize() const;

            void            setRotationAngle(int rotation_angle);
            int             getRotationAngle();

            bool            printStatus();                              // Print current gait status to Serial
            bool            runConsoleCommands(const String& cmd, const String& args);  // Process console commands for gait control
            bool            printConsoleHelp();                         // Print gait-specific help information



        private:
            Hexapod*        hexapod;                                    // Pointer to the Hexapod instance
            GaitType        gaitType;                                   // Current gait type
            int             currentPhase;                               // Current phase of the gait
            int             currentStep;                                // Current step in the gait sequence

        // New parameters
            int             gaitDirection;                              // -180 to 180
            int             gaitSpeed;                                  // 0 to 1023
            float           gaitStepSize;                               // e.g. mm or degrees
            int             gaitRotationAngle;                          // -180 to 180

            bool            doWaveGait();                               // Perform the wave gait
            bool            doRippleGait();                             // Perform the ripple gait
            bool            doTripodGait();                             // Perform the tripod gait
            bool            doRotate();                                 // -180 to 180
    };

#endif // GaitController_h
