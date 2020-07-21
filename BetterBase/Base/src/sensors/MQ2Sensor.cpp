#include "../inc/BaseConfig.h"

#if ENABLED(GAS_SENSOR_INSTALLED)
#include "MQ2Sensor.h"

MQ2Sensor::MQ2Sensor()
{
    gasSensor = new MQ2(PINOUT_MQ2_INPUT);
}

void MQ2Sensor::init()
{
    gasSensor->calibrate();
}

uint16_t MQ2Sensor::getPropane()
{
    return gasSensor->readLPG();
}

uint16_t MQ2Sensor::getMethane()
{
    return gasSensor->readMethane();
}

uint16_t MQ2Sensor::getSmoke()
{
    return gasSensor->readSmoke();
}

uint16_t MQ2Sensor::getHydrogen()
{
    return gasSensor->readHydrogen();
}
#endif
