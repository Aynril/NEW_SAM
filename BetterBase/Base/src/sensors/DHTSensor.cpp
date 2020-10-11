#include "../inc/BaseConfig.h"

#if ENABLED(DHT_SENSOR_INSTALLED)
#include "DHTSensor.h"
#include "../core/serial.h"

void DHTSensor::init()
{
    dht.setup(PINOUT_DHT22_INPUT);
}

float DHTSensor::getHumidity()
{
    float ret = dht.getHumidity();
    PRINT_DEBUG("DHT Humidity: ");
    PRINT_DEBUG_LN(ret);
    return ret;
}

float DHTSensor::getTemperature()
{
    float ret = dht.getTemperature();
    PRINT_DEBUG("DHT Temperature: ");
    PRINT_DEBUG_LN(ret);
    return ret;
}
#endif
