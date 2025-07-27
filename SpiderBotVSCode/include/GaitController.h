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
            void            begin(Hexapod* hexapod);                     // Initialize with Hexapod
            void            setGait(GaitType newGait);                  // Set the current gait type
            GaitType        getGait() const;                            // Get the current gait type
            void            setStepInterval(unsigned long interval);    // Set the step interval
            unsigned long   getStepInterval() const;                    // Get the step interval
            void            update();                                   // Update the gait controller
            void            printStatus(Stream& stream);                // Print current gait status to Serial

        private:
            Hexapod*        hexapod;                                    // Pointer to the Hexapod instance
            GaitType        gaitType;                                   // Current gait type
            int             currentPhase;                               // Current phase of the gait
            int             currentStep;                                // Current step in the gait sequence

            void            doWaveGait();                               // Perform the wave gait
            void            doRippleGait();                             // Perform the ripple gait
            void            doTripodGait();                             // Perform the tripod gait
    };

#endif // GaitController_h
