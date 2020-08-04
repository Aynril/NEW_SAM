#include "../inc/BaseConfig.h"

#if ENABLED(DHT_SENSOR_INSTALLED)
#include "DHTSensor.h"
#include "../core/serial.h"

DHTSensor::DHTSensor()
{
    dht = new DHT(PINOUT_DHT22_INPUT, DHT11);
}

void DHTSensor::init()
{
    dht->begin();
}

float DHTSensor::getHumidity()
{
    float ret = dht->readHumidity();
    PRINT_DEBUG("DHT Humidity: ");
    PRINT_DEBUG_LN(ret);
    return ret;
}

float DHTSensor::getTemperature()
{
    float ret = dht->readTemperature();
    PRINT_DEBUG("DHT Temperature: ");
    PRINT_DEBUG_LN(ret);
    return ret;
}
#endif
