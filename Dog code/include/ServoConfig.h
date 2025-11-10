#ifndef SERVO_CONFIG_H
#define SERVO_CONFIG_H

#include <Arduino.h>

namespace ServoConfig {
    static constexpr uint8_t CHANNELS = 16;
    
    static constexpr int neutral_us[CHANNELS] = {
        1500, 1500, 1500, 1500, 1500, 1500, 
        1490, 1430, 1460, 1590, 1440, 1560, 
        1550, 1500, 1490, 1533
    };
    
    static constexpr int min_us[CHANNELS] = {
        500, 500, 500, 500, 500, 500, 500, 500,
        500, 500, 500, 500, 500, 500, 500, 500
    };
    
    static constexpr int max_us[CHANNELS] = {
        2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500,
        2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500
    };
    
    static constexpr int min_angle[CHANNELS] = {
        -90, -90, -90, -90, -90, -90, -90, -90,
        -90, -90, -90, -90, -90, -90, -90, -90
    };
    
    static constexpr int max_angle[CHANNELS] = {
        90, 90, 90, 90, 90, 90, 90, 90,
        90, 90, 90, 90, 90, 90, 90, 90
    };
}

#endif // SERVO_CONFIG_H
