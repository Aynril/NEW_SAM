#pragma once

#include "../inc/BaseConfig.h"
#include <DHT.h>

class DHTSensor
{
private:
    DHT *dht;
public:
    DHTSensor();
    float getTemperature();
    float getHumidity();

    void init();
};