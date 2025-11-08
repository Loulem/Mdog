#ifndef CONTROLLER_COMMAND_H
#define CONTROLLER_COMMAND_H

#include <Arduino.h>

struct ControllerCommand {
    float angle;
    float power;
    bool valid;

    void print() const {
        if (valid) {
            Serial.print("CMD(angle=");
            Serial.print(angle);
            Serial.print(", power=");
            Serial.print(power);
            Serial.println(")");
        } else {
            Serial.println("CMD(invalid)");
        }
    }
};

#endif // CONTROLLER_COMMAND_H
