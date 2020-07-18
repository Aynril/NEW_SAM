#pragma once

#include "../inc/BaseConfig.h"
#include <MQ2.h>

class MQ2Sensor
{
private:
    MQ2 *gasSensor;
public:
    MQ2Sensor();
    uint16_t getPropane();
    uint16_t getMethane();
    uint16_t getSmoke();
    uint16_t getHydrogen();

    void init();
};