#ifndef RF_CONTROLLER_H
#define RF_CONTROLLER_H

#include <Arduino.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "ControllerCommand.h"

// RFController manages nRF24L01 radio and parses incoming commands.
// No side effects; owns its RF24 instance.
class RFController {
public:
    RFController(uint8_t ce_pin = 7, uint8_t csn_pin = 8);
    bool begin(uint8_t channel = 115, const char* address = "00001");
    ControllerCommand retrieveRadioCommand();

private:
    RF24 rfDevices;
    char data_receive_buffer[32];
    uint8_t channel;
    ControllerCommand extractCommandFromData(const char* payload, uint8_t len);
};

#endif // RF_CONTROLLER_H
