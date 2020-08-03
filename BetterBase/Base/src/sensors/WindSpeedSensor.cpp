#include "../inc/BaseConfig.h"

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
#include "WindSpeedSensor.h"

Wind_Data_t *WindSpeedSensor::_windData;
void WindSpeedSensor::interruptHandler(void)
{
    handle(_windData);
    Serial.print("I");
    Serial.println(_windData->count);
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

float WindSpeedSensor::getSpeed(speed_output_type oType = SPEED_KMH)
{
    readSpeed();
    if (oType == SPEED_KMH)
    {
        return speed;
    }
    else if (oType == SPEED_MPS)
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
    float timeBetweenMeasurements = (millis() - windData.time) / 1000;
    if (timeBetweenMeasurements >= 5)
    {
        PRINT_DEBUG("Counts: ");
        PRINT_DEBUG(windData.count);
        PRINT_DEBUG(" Time: ");
        PRINT_DEBUG(timeBetweenMeasurements);
        speed = (float)windData.count / timeBetweenMeasurements * 2.4;
        PRINT_DEBUG(" Speed: ");
        PRINT_DEBUG_LN(speed);
        windData.count = 0;
        windData.time = millis();
    }
    interrupts();
}

#endif