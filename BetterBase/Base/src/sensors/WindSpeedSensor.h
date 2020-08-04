#pragma once

#include "../inc/BaseConfig.h"

typedef enum
{
    SPEED_KMH = 0,
    SPEED_MPS,
    SPEED_MIPS,
    SPEED_FPS
} speed_output_type;

typedef struct
{
    unsigned long time;
    int count;
} Wind_Data_t;

#define TICKS_PER_ROTATION 2
#define WIND_SENSOR_RADIUS 90 /* mm */ / 10 /*(cm)*/

class WindSpeedSensor
{
private:
    int _pin;
    bool interruptCapable;
    float speed;
    Wind_Data_t windData;
    int interruptPin;

    // void initializeInner(int pin, bool useInterrupts);
    void readSpeed();

public:
    static Wind_Data_t *_windData;

private:
    static void attach_interrupt(uint8_t pin, Wind_Data_t *data)
    {
        _windData = data;
        attachInterrupt(pin, interruptHandler, RISING);
    }
    // const unsigned long windTimeReset = 10 * 1000;

public:
    // void init(int pin);
    // void init(int pin, bool useInterrupts);
    float getSpeed(speed_output_type oType);
    static void handle(Wind_Data_t *data);
    static void interruptHandler(void);
    WindSpeedSensor(int pin, bool useInterrupts);
};
