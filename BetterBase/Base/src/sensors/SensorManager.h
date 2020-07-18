#pragma once

#include "../inc/BaseConfig.h"

#include "AirPressureSensor.h"
#include "analog/AnalogSensor.h"
#include "analog/RainSensor.h"
#include "analog/EarthHumiditySensor.h"
#include "analog/LightSensor.h"
#include "DHTSensor.h"
#include "MQ2Sensor.h"
#include "PMSensor.h"

#if ENABLED(RADIO_SUPPORT)
#include "../features/radio/radio.h"
#endif

#if ENABLED(SD_SUPPORT)
#include "../features/sd/sdUtility.h"
#endif

class SensorManager
{
private:
    AirPressureSensor pressureSensor;
    RainSensor rainSensor;
    EarthHumiditySensor earhtHumiditySensor;
    LightSensor lightSensor;
    DHTSensor dhtSensor;
    MQ2Sensor mq2Sensor;
    PMSensor partilceSensor;
public:
    void saveAll();
};

void SensorManager::saveAll() {
    #if ENABLED(RADIO_SUPPORT)
    setAirPressure(pressureSensor.getPressure());
    setAirHumidity(dhtSensor.getHumidity());
    
    setPropane(mq2Sensor.getPropane());
    setMethane(mq2Sensor.getMethane());
    setSmoke(mq2Sensor.getSmoke());
    setHydrogen(mq2Sensor.getHydrogen());

    setRain(rainSensor.read());
    setEarhtHumidity(earhtHumiditySensor.read());
    setLightIntensity(lightSensor.read());

    setTemperature( (pressureSensor.getTemperature() + dhtSensor.getTemperature()) / 2 );

    setParticles(partilceSensor.get10(), partilceSensor.get25());

    #endif
}

