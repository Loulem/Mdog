#ifndef SERVO_CHANNEL_MAP_H
#define SERVO_CHANNEL_MAP_H

#include <Arduino.h>
#include "LegKinematics.h"

namespace ServoChannelMap {
    // Servo channel assignments for each leg and joint
    // Hip servos: channels 4-7
    // Knee servos: channels 8-11
    // Shoulder servos: channels 12-15
    
    static constexpr uint8_t HIP_BASE_CHANNEL = 4;
    static constexpr uint8_t KNEE_BASE_CHANNEL = 8;
    static constexpr uint8_t SHOULDER_BASE_CHANNEL = 12;
    
    inline uint8_t getHipChannel(LegId leg) {
        return HIP_BASE_CHANNEL + static_cast<uint8_t>(leg);
    }
    
    inline uint8_t getKneeChannel(LegId leg) {
        return KNEE_BASE_CHANNEL + static_cast<uint8_t>(leg);
    }
    
    inline uint8_t getShoulderChannel(LegId leg) {
        return SHOULDER_BASE_CHANNEL + static_cast<uint8_t>(leg);
    }
}

#endif // SERVO_CHANNEL_MAP_H
