#pragma once

#include "../inc/BaseConfig.h"

class WindSpeedSensor
{
private:
    int _pin;
    bool interruptCapable;
    float speed;
    volatile unsigned long lastMeasurement;
    volatile unsigned long lastPulseTime;
    int interruptPin;

    void initializeInner(int pin, bool useInterrupts);
    void interruptHandler();

public:
    void init(int pin);
    void init(int pin, bool useInterrupts);
    float readSpeed();
    float getSpeed();
    long getPulseTime();
};
