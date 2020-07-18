#pragma once

#include "../inc/BaseConfig.h"

class PMSensor
{
private:
    uint8_t readParticleByPin(int pin);
public:
    PMSensor() { };
    uint8_t get25();
    uint8_t get10();
};
