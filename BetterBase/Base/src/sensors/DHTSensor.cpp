#include "../inc/BaseConfig.h"

#if ENABLED(DHT_SENSOR_INSTALLED)
#include "DHTSensor.h"

DHTSensor::DHTSensor()
{
    dht = new DHT(PINOUT_DHT22_INPUT, DHT22);
}

void DHTSensor::init()
{
    dht->begin();
}

float DHTSensor::getHumidity()
{
    return dht->readHumidity();
}

float DHTSensor::getTemperature()
{
    return dht->readTemperature();
}
#endif
