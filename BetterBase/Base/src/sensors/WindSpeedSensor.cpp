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
    PRINT_DEBUG_LN("[WIND SPEED] Initializing...");
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
    Serial.print("I");
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
        return speed * 3.6;
    }
    else if (oType == SPEED_MIPS)
    {
        return speed * 0.6213709090909588;
    }
    else if (oType == SPEED_FPS)
    {
        return speed * 0.911344;
    }

    return speed;
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
        speed = (WIND_SENSOR_RADIUS * PI * windData.count) / deltaT;
        PRINT_DEBUG(" Speed: ");
        PRINT_DEBUG_LN(speed);
        windData.count = 0;
        windData.time = millis();
    }
    interrupts();
}

#endif