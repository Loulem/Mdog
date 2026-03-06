#ifndef DOG_MASTER_H
#define DOG_MASTER_H

#include <Arduino.h>
#include "RFController.h"
#include "ServoBus.h"
#include "LegKinematics.h"
#include "CrawlingTrajectory.h"
#include "LegState.h"
#include "ServoChannelMap.h"

class DogMaster {
public:
    DogMaster(uint8_t _rf_channel=115);
    bool begin();
    void setStepInterval(uint16_t interval_ms);
    bool update();

private:
    RFController rfController;
    ServoBus servoBus;
    LegKinematics kinematics;
    CrawlingTrajectory gaitPath;
    const uint8_t rf_channel;
    
    AllLegsPosition current_positions;
    AllLegsJoints current_joints;
    ControllerCommand lastCommand{0.0f, 0.0f, false};
    unsigned long lastRFReceiveTime = 0;
    static constexpr unsigned long RF_TIMEOUT_MS = 1000; // failsafe: stop after 1s without RF
    
    void updateLegPositions();
    void computeJointAngles();
    void applyJointAngles();
    static constexpr bool IGNORE_RF = false;
    
};

#endif // DOG_MASTER_H
