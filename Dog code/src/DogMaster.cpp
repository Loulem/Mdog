#include "DogMaster.h"

DogMaster::DogMaster(uint8_t _rf_channel)
    : rf_channel(_rf_channel) {}

bool DogMaster::begin() {
    // Setup RF controller
    bool success = rfController.begin(rf_channel);
    if(!success){
        return false;
    }
    
    // Setup servo bus with two drivers in mirror mode
    servoBus.addDriver(0x40, 26250000);
    servoBus.addDriver(0x41, 25360000);
    servoBus.begin(300);
    
    // Generate walking trajectory
    gaitPath.generateTrajectory();
    
    return true;
}

bool DogMaster::update() { 
    ControllerCommand cmd;
    if (!IGNORE_RF) {
        // 1. Poll for RF input
        cmd = rfController.retrieveRadioCommand();
        if (!cmd.valid) {
            return false;
        }
    }

    else {
        // Use default forward walking command
        cmd.angle = 0.0f;
        cmd.power = 50.0f;
        cmd.valid = true;
    }
    
    
    // 2. Advance gait step based on command
    int direction = (cmd.power > 10) ? 1 : 0;
    bool stepped = gaitPath.advanceStep(direction, millis());
    
    if (stepped) {
        // 3. Get new leg positions from trajectory
        updateLegPositions();
        
        // 4. Compute joint angles from positions
        computeJointAngles();
        
        // 5. Apply joint angles to servos
        applyJointAngles();
    }
    
    return true;
}

void DogMaster::updateLegPositions() {
    for (uint8_t i = 0; i < 4; i++) {
        current_positions[i] = gaitPath.getLegPosition(i);
    }
}

void DogMaster::computeJointAngles() {
    for (uint8_t i = 0; i < 4; i++) {
        current_joints[i] = kinematics.calculateJointAngles((LegId)i, current_positions[i]);
    }
}

void DogMaster::applyJointAngles() {
    for (uint8_t i = 0; i < 4; i++) {
        LegId leg = (LegId)i;
        LegJoints& joints = current_joints[i];
        
        // Apply hip angle
        servoBus.setServoAngle(ServoChannelMap::getHipChannel(leg), joints.hipAngle);
        
        // Apply shoulder angle
        servoBus.setServoAngle(ServoChannelMap::getShoulderChannel(leg), joints.shoulderAngle);
        
        // Apply knee angle
        servoBus.setServoAngle(ServoChannelMap::getKneeChannel(leg), joints.kneeAngle);
    }
}
