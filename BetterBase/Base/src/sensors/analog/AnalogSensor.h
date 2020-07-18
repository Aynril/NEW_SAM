#pragma once

#include "../../inc/BaseConfig.h"

class AnalogSensor
{
private:
    uint8_t pinInternal;
    bool invertedInternal;
public:
    AnalogSensor(uint8_t pin): pinInternal(pin) {}
    AnalogSensor(uint8_t pin, bool inverted): pinInternal(pin), invertedInternal(inverted) {}

    int read();
};

int AnalogSensor::read() {
    if (invertedInternal)
    {
        return 1023 - analogRead(pinInternal);
    } else
    {
        return analogRead(pinInternal);
    }
}