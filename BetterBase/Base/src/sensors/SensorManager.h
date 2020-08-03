#pragma once

#include "../inc/BaseConfig.h"

#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
#include "AirPressureSensor.h"
#endif
#include "analog/AnalogSensor.h"
#include "analog/RainSensor.h"
#include "analog/EarthHumiditySensor.h"
#include "analog/LightSensor.h"
#if ENABLED(DHT_SENSOR_INSTALLED)
#include "DHTSensor.h"
#endif
#if ENABLED(GAS_SENSOR_INSTALLED)
#include "MQ2Sensor.h"
#endif
#if ENABLED(PARTICLE_SENSOR_INSTALLED)
#include "PMSensor.h"
#endif

#if ENABLED(RADIO_SUPPORT)
#include "../features/radio/radio.h"
#endif

#if ENABLED(SD_SUPPORT)
#include "../features/sd/sdUtility.h"
#endif

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
#include "WindSpeedSensor.h"
#endif

#if ENABLED(RTC_SUPPORT)
#include "../features/rtc.h"
#endif

class SensorManager
{
private:
#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    AirPressureSensor pressureSensor;
#endif
    RainSensor rainSensor;
    EarthHumiditySensor earhtHumiditySensor;
    LightSensor lightSensor;
#if ENABLED(DHT_SENSOR_INSTALLED)
    DHTSensor dhtSensor;
#endif
#if ENABLED(GAS_SENSOR_INSTALLED)
    MQ2Sensor mq2Sensor;
#endif
#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    PMSensor partilceSensor;
#endif
#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    WindSpeedSensor *windSpeedSensor = new WindSpeedSensor(PINOUT_WIND_SPEED_SENSOR_INTERRUPT, true);
#endif

#if ENABLED(GAS_SENSOR_INSTALLED)
    uint16_t propane;
    uint16_t methane;
    uint16_t smoke;
    uint16_t hydrogen;
#endif

    uint8_t rain;
    uint8_t earthHumidity;
    uint8_t light;

#if ENABLED(DHT_SENSOR_INSTALLED)
    float airHumidity;
    float temp;
#endif

#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    float pressure;
#endif

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    uint8_t p25;
    uint8_t p10;
#endif

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    float speed;
#endif

public:
    void initAll();
    void saveAll();
};

void SensorManager::initAll()
{
#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    // windSpeedSensor.init(PINOUT_WIND_SPEED_SENSOR_INTERRUPT, true);
#endif

#if ENABLED(RTC_SUPPORT)
    initRTC();
#endif

}

void SensorManager::saveAll()
{
#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    pressure = pressureSensor.getPressure();
#endif
#if ENABLED(DHT_SENSOR_INSTALLED)
    airHumidity = dhtSensor.getHumidity();
#endif

#if ENABLED(GAS_SENSOR_INSTALLED)
    propane = (mq2Sensor.getPropane());
    methane = (mq2Sensor.getMethane());
    smoke = (mq2Sensor.getSmoke());
    hydrogen = (mq2Sensor.getHydrogen());
#endif

    rain = (rainSensor.read());
    earthHumidity = (earhtHumiditySensor.read());
    light = (lightSensor.read());

#if BOTH(AIR_PRESSURE_SENSOR_INSTALLED, DHT_SENSOR_INSTALLED)
    temp = ((pressureSensor.getTemperature() + dhtSensor.getTemperature()) / 2);
#elif ENABLED(DHT_SENSOR_INSTALLED)
    temp = dhtSensor.getTemperature();
#elif ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    temp = pressureSensor.getTemperature();
#endif

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    p10 = partilceSensor.get10();
    p25 = partilceSensor.get25();
#endif

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    speed = windSpeedSensor->getSpeed(SPEED_KMH);
#endif

#if ENABLED(RADIO_SUPPORT)

#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    setAirPressure(pressure);
#endif
#if ENABLED(DHT_SENSOR_INSTALLED)
    setAirHumidity(airHumidity);
#endif

#if ENABLED(GAS_SENSOR_INSTALLED)
    setPropane(propane);
    setMethane(methane);
    setSmoke(smoke);
    setHydrogen(hydrogen);
#endif

    setRain(rain);
    setEarhtHumidity(earthHumidity);
    setLightIntensity(light);

#if ANY(AIR_PRESSURE_SENSOR_INSTALLED, DHT_SENSOR_INSTALLED)
    setTemperature(temp);
#endif

    setParticles(p10, p25);

#endif

#if ENABLED(SERIAL_OUTPUT_SUPPORT)
#if ENABLED(RTC_SUPPORT)
    Serial.print(getTimeNow());
    Serial.print(F(","));
#endif
#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    Serial.print((double)pressure);
    Serial.print(F(","));
#endif
#if ENABLED(DHT_SENSOR_INSTALLED)
    Serial.print((double)airHumidity);
    Serial.print(F(","));
#endif

#if ENABLED(GAS_SENSOR_INSTALLED)
    Serial.print(propane);
    Serial.print(F(","));
    Serial.print(methane);
    Serial.print(F(","));
    Serial.print(smoke);
    Serial.print(F(","));
    Serial.print(hydrogen);
    Serial.print(F(","));
#endif

    // Serial.print(rain);
    // Serial.print(F(","));
    // Serial.print(earthHumidity);
    // Serial.print(F(","));
    // Serial.print(light);
    // Serial.print(F(","));

#if ANY(AIR_PRESSURE_SENSOR_INSTALLED, DHT_SENSOR_INSTALLED)
    Serial.print((double)temp);
    Serial.print(F(","));
#endif
#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    Serial.print(p10);
    Serial.print(F(","));
    Serial.println(p25);
    Serial.print(F(","));
#endif
#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    Serial.println(speed);
#endif

#endif
}
