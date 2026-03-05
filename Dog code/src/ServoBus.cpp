#include "ServoBus.h"

ServoBus::ServoBus() : driver_count_(0), pwm_freq(50) {
    for (uint8_t i = 0; i < MAX_DRIVERS; i++) {
        drivers_[i] = nullptr;
        oscillator_freq_[i] = 0;
        i2c_addresses_[i] = 0;
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
    i2c_addresses_[driver_count_] = i2c_address;
    driver_count_++;
}

void ServoBus::begin(uint16_t _pwm_freq) {
    pwm_freq = _pwm_freq;
    // Init each driver individually (reset, set oscillator, set PWM freq)
    for (uint8_t i = 0; i < driver_count_; i++) {
        if (drivers_[i] != nullptr) {
            drivers_[i]->begin();
            drivers_[i]->setOscillatorFrequency(oscillator_freq_[i]);
            drivers_[i]->setPWMFreq(pwm_freq);
        }
    }
    // Switch to I2C Fast Mode (400kHz)
    Wire.setClock(400000);
    // Enable All-Call on each driver so they respond to 0x70
    for (uint8_t i = 0; i < driver_count_; i++) {
        enableAllCall(i2c_addresses_[i]);
    }
}

void ServoBus::enableAllCall(uint8_t i2c_addr) {
    // Read MODE1 register
    Wire.beginTransmission(i2c_addr);
    Wire.write(0x00); // MODE1 register
    Wire.endTransmission(false);
    Wire.requestFrom(i2c_addr, (uint8_t)1);
    uint8_t mode1 = Wire.read();
    // Set ALLCALL bit (bit 0)
    Wire.beginTransmission(i2c_addr);
    Wire.write(0x00);
    Wire.write(mode1 | 0x01);
    Wire.endTransmission();
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
    // Convert microseconds to ticks: ticks = us * freq * 4096 / 1000000
    uint16_t ticks = (uint32_t)us * pwm_freq * 4096UL / 1000000UL;
    // Write directly to broadcast address 0x70
    Wire.beginTransmission(BROADCAST_ADDR);
    Wire.write(0x06 + 4 * channel); // LEDn_ON_L register
    Wire.write(0);                  // ON_L  = 0
    Wire.write(0);                  // ON_H  = 0
    Wire.write(ticks & 0xFF);       // OFF_L
    Wire.write(ticks >> 8);         // OFF_H
    Wire.endTransmission();
}
