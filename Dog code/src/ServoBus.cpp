#include "ServoBus.h"

ServoBus::ServoBus(uint16_t _pwm_freq) : driver_count_(0), pwm_freq_(_pwm_freq) {
    for (uint8_t i = 0; i < MAX_DRIVERS; i++) {
        drivers_[i] = nullptr;
        oscillator_freq_[i] = 0;
    }
    for (uint8_t i = 0; i < CHANNELS; i++) {
        servo_neutral_us[i] = ServoConfig::neutral_us[i];
        servo_min_us_[i] = ServoConfig::min_us[i];
        servo_max_us_[i] = ServoConfig::max_us[i];
        servoMinAngle_[i] = ServoConfig::min_angle[i];
        servoMaxAngle_[i] = ServoConfig::max_angle[i];
    }
}

void ServoBus::addDriver(uint8_t i2c_address, uint32_t oscillator_freq) {
    if (driver_count_ >= MAX_DRIVERS) return;
    drivers_[driver_count_] = new Adafruit_PWMServoDriver(i2c_address);
    oscillator_freq_[driver_count_] = oscillator_freq;
    driver_count_++;
}

void ServoBus::begin(uint16_t pwm_freq) {
    pwm_freq_ = pwm_freq;
    for (uint8_t i = 0; i < driver_count_; i++) {
        if (drivers_[i] != nullptr) {
            drivers_[i]->begin();
            drivers_[i]->setOscillatorFrequency(oscillator_freq_[i]);
            drivers_[i]->setPWMFreq(pwm_freq);
        }
    }
}

void ServoBus::setServoAngle(uint8_t channel, float angle_deg) {
    if (channel >= CHANNELS) return;
    int us = angleToMicroseconds(channel, angle_deg);
    writeMicroseconds(channel, us);
}

void ServoBus::setAllToNeutral() {
    for (uint8_t i = 4; i < CHANNELS; i++) {
        writeMicroseconds(i, servo_neutral_us[i]);
    }
}

int ServoBus::angleToMicroseconds(uint8_t channel, float angle) const {
    float restrictedAngle = constrain(angle, servoMinAngle_[channel], servoMaxAngle_[channel]);
    float angleToUsScale = (servo_max_us_[channel] - servo_min_us_[channel]) / (float)(servoMaxAngle_[channel] - servoMinAngle_[channel]);
    int us = (int)(angleToUsScale * restrictedAngle) + servo_neutral_us[channel];
    return constrain(us, servo_min_us_[channel], servo_max_us_[channel]);
}

void ServoBus::writeMicroseconds(uint8_t channel, int us) {
    for (uint8_t i = 0; i < driver_count_; i++) {
        if (drivers_[i] != nullptr) {
            drivers_[i]->writeMicroseconds(channel, us);
        }
    }
}
