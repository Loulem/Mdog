#ifndef DOG_MASTER_H
#define DOG_MASTER_H

#include <Arduino.h>
#include "RFController.h"

// DogMaster orchestrates input -> gait.
// Owns RFController and will own gait/kinematics in future steps.
class DogMaster {
public:
    // Initialize RF and gait tables. Also init leg objects.
    void begin(uint8_t rf_channel = 115);

    bool update();

private:
    RFController rf_controller_;
    uint8_t rf_channel_;
};

#endif // DOG_MASTER_H
