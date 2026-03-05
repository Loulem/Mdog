#ifndef SERVO_BUS_H
#define SERVO_BUS_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoConfig.h"

class ServoBus {
public:
    ServoBus();
    
    void addDriver(uint8_t i2c_address, uint32_t oscillator_freq);
    void begin(uint16_t pwm_freq);
    
    void setServoAngle(uint8_t channel, float angle_deg);
    void setAllToNeutral();

private:
    static constexpr uint8_t MAX_DRIVERS = 2;
    static constexpr uint8_t CHANNELS = 16;
    static constexpr uint8_t BROADCAST_ADDR = 0x70;
    
    Adafruit_PWMServoDriver* drivers_[MAX_DRIVERS];
    uint32_t oscillator_freq_[MAX_DRIVERS];
    uint8_t i2c_addresses_[MAX_DRIVERS];
    uint8_t driver_count_;
    uint16_t pwm_freq;
    
    int servo_neutral_us[CHANNELS];
    int servo_min_us_[CHANNELS];
    int servo_max_us_[CHANNELS];
    int servoMinAngle_[CHANNELS];
    int servoMaxAngle_[CHANNELS];
    
    int angleToMicroseconds(uint8_t channel, float angle) const;
    void writeMicroseconds(uint8_t channel, int us);
    void enableAllCall(uint8_t i2c_addr);
};

#endif // SERVO_BUS_H
