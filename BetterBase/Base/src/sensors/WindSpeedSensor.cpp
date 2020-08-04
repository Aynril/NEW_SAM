#include "../inc/BaseConfig.h"

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
#include "WindSpeedSensor.h"

Wind_Data_t *WindSpeedSensor::_windData;
void WindSpeedSensor::interruptHandler(void)
{
    handle(_windData);
    // PRINT_DEBUG("I");
    // PRINT_DEBUG_LN(_windData->count);
}

WindSpeedSensor::WindSpeedSensor(int pin, bool useInterrupts)
{
    noInterrupts();
    _pin = pin;
    interruptCapable = useInterrupts;
    speed = 0.0;

    windData.count = 0;
    windData.time = millis();

    if (interruptCapable)
    {
        interruptPin = digitalPinToInterrupt(_pin);
        attach_interrupt(interruptPin, &windData);
    }
    interrupts();
}

// void WindSpeedSensor::init(int pin, bool useInterrupts)
// {
//     initializeInner(pin, useInterrupts);
// }

// void WindSpeedSensor::init(int pin)
// {
//     initializeInner(pin, digitalPinToInterrupt(pin) > -1);
// }

void WindSpeedSensor::handle(Wind_Data_t *data)
{
    noInterrupts();
    data->count++;
    interrupts();
}

float WindSpeedSensor::getSpeed(speed_output_type oType = SPEED_MPS)
{
    readSpeed();
    if (oType == SPEED_MPS)
    {
        return speed;
    }
    else if (oType == SPEED_KMH)
    {
        return speed / 3.6;
    }
    else if (oType == SPEED_MIPS)
    {
        return speed * 0.6213709090909588;
    }
    else if (oType == SPEED_FPS)
    {
        return speed * 0.911344;
    }

    //return speed;
}

void WindSpeedSensor::readSpeed()
{
    noInterrupts();
    float deltaT = (millis() - windData.time) / 1000; // In Seconds
    if (deltaT >= 3)
    {
        PRINT_DEBUG("Counts: ");
        PRINT_DEBUG(windData.count);
        PRINT_DEBUG(" Time: ");
        PRINT_DEBUG(deltaT);
        // float ticksPerSecond = (float)windData.count / deltaT;
        // float speedoooooo = ((float)windData.count * (2 * PI) / TICKS_PER_ROTATION) / deltaT;

        // float r = WIND_SENSOR_RADIUS;
        float rotationsPerT = (windData.count / TICKS_PER_ROTATION);
        PRINT_DEBUG(" RPM: ");
        float rpm = (60 * rotationsPerT) / sq(deltaT);
        PRINT_DEBUG(rpm);
        // PRINT_DEBUG(" Angular Velocity: ");
        // float w = rpm * ((2* PI) / 60);
        // PRINT_DEBUG(w);

        // float angToLin = WIND_SENSOR_RADIUS * w;
        // speed = angToLin;
        speed = WIND_SENSOR_RADIUS * rpm * 0.10472;
        // speed = ticksPerSecond * 2.4;
        PRINT_DEBUG(" Speed: ");
        PRINT_DEBUG_LN(speed);
        windData.count = 0;
        windData.time = millis();
    }
    interrupts();
}

#endif