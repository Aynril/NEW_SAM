#include "../inc/BaseConfig.h"

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
#include "WindSpeedSensor.h"

void WindSpeedSensor::initializeInner(int pin, bool useInterrupts)
{
    noInterrupts();
    _pin = pin;
    interruptCapable = useInterrupts;

    lastMeasurement = millis();
    lastPulseTime = 0;

    if (interruptCapable)
    {
        interruptPin = digitalPinToInterrupt(_pin);
        attachInterrupt(interruptPin, interruptHandler, RISING);
    }
    interrupts();
}

void WindSpeedSensor::init(int pin, bool useInterrupts)
{
    initializeInner(pin, useInterrupts);
}

void WindSpeedSensor::init(int pin)
{
    initializeInner(pin, digitalPinToInterrupt(pin) > -1);
}

void WindSpeedSensor::interruptHandler() {
    noInterrupts();
    unsigned long now = millis();
    lastPulseTime = now - lastMeasurement;
    interrupts();
}

long WindSpeedSensor::getPulseTime()
{
    noInterrupts();
    unsigned long c = lastPulseTime;
    interrupts();
    return c;
}

float WindSpeedSensor::getSpeed()
{
}

float WindSpeedSensor::readSpeed()
{
    noInterrupts();
    lastPulseTime = pulseIn(_pin, LOW, 3000000);
    interrupts();
}

#endif