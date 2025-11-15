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
    DogMaster(uint8_t _rf_channel = 115);
    bool begin();
    bool update();

private:
    RFController rfController;
    ServoBus servoBus;
    LegKinematics kinematics;
    CrawlingTrajectory gaitPath;
    const uint8_t rf_channel;
    
    AllLegsPosition current_positions;
    AllLegsJoints current_joints;
    
    void updateLegPositions();
    void computeJointAngles();
    void applyJointAngles();
    static constexpr bool IGNORE_RF = true;
    
};

#endif // DOG_MASTER_H
