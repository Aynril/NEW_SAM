#pragma once

#include "../inc/BaseConfig.h"
// #if ENABLED(DEBUG_FEATURE)
// #define DHT_DEBUG
// #endif
#include <DHT.h>

class DHTSensor
{
private:
    DHT dht;
public:
    float getTemperature();
    float getHumidity();

    void init();
};