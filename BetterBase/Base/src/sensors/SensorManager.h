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
#include <SoftwareSerial.h>
#endif

#if ENABLED(RADIO_SUPPORT)
#include "../features/radio/radio.h"
#endif

#if ENABLED(SD_SUPPORT)
#include "../features/sd/SDHelper.h"
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
    SoftwareSerial particleSerial = SoftwareSerial(PINOUT_NOVA_PM_SENSOR_RX, PINOUT_NOVA_PM_SENSOR_TX);
    SDS011 partilceSensor = SDS011(particleSerial);
#endif
#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    WindSpeedSensor windSpeedSensor = WindSpeedSensor(PINOUT_WIND_SPEED_SENSOR_INTERRUPT, true);
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
#endif

#if ANY(AIR_PRESSURE_SENSOR_INSTALLED, DHT_SENSOR_INSTALLED)
    float temp;
#endif

#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    float pressure;
    float height;
#endif

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    float p25;
    float p10;
#endif

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    float speed;
#endif

public:
    void initAll();
    void saveAll();
    void sensorsOn();
    void sensorsOff();
};

void SensorManager::initAll()
{
    PRINT_DEBUG_LN("[SENSOR MANAGER] Initializing...");
#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    //windSpeedSensor.init(PINOUT_WIND_SPEED_SENSOR_INTERRUPT, true);
#endif

#if ENABLED(RTC_SUPPORT)
    initRTC();
#endif

#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    PRINT_DEBUG_LN("Initializing Pressure Sensor");
    pressureSensor.init();
#endif

#if ENABLED(DHT_SENSOR_INSTALLED)
    PRINT_DEBUG_LN("Initializing DHT Sensor");
    dhtSensor.init();
#endif

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    PRINT_DEBUG_LN("Initializing PM Sensor & Serial");
    particleSerial.begin(9600);
    partilceSensor.autoMode();
#endif
}

void SensorManager::sensorsOn()
{

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    partilceSensor.wakeup();
#endif
    delay(5000);
}

void SensorManager::sensorsOff()
{
#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    partilceSensor.sleep();
#endif
    delay(2000);
    //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
}

void SensorManager::saveAll()
{
#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    PRINT_DEBUG_LN("Reading Pressure Sensor");
    pressure = pressureSensor.getPressure();
    height = pressureSensor.getAltitute();
#endif
#if ENABLED(DHT_SENSOR_INSTALLED)
    PRINT_DEBUG_LN("Reading DHT Sensor");
    airHumidity = dhtSensor.getHumidity();
#endif

#if ENABLED(GAS_SENSOR_INSTALLED)
    PRINT_DEBUG_LN("Reading MQ2 Sensor");
    propane = (mq2Sensor.getPropane());
    methane = (mq2Sensor.getMethane());
    smoke = (mq2Sensor.getSmoke());
    hydrogen = (mq2Sensor.getHydrogen());
#endif

    PRINT_DEBUG_LN("Reading Analog Sensors");
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
    PRINT_DEBUG_LN("Reading PM Sensor");
    partilceSensor.read(&p25, &p10);
#endif

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    speed = windSpeedSensor.getSpeed(SPEED_KMH);
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

#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    setAltitude(height);
#endif

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    setParticles(p10, p25);
#endif

#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    setWindSpeed(speed);
#endif

#endif

#if ANY(SD_SUPPORT, SERIAL_OUTPUT_SUPPORT)
    String out = "";
#if ENABLED(RTC_SUPPORT)
    out += getTimeNow();
    out += F(",");
#endif
#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
    out += (double)pressure;
    out += F(",");
    out += (double)height;
    out += F(",");
#endif
#if ENABLED(DHT_SENSOR_INSTALLED)
    out += (double)airHumidity;
    out += F(",");
#endif

#if ENABLED(GAS_SENSOR_INSTALLED)
    out += propane;
    out += F(",");
    out += methane;
    out += F(",");
    out += smoke;
    out += F(",");
    out += hydrogen;
    out += F(",");
#endif

    out += rain;
    out += F(",");
    out += earthHumidity;
    out += F(",");
    out += light;
    out += F(",");

#if ANY(AIR_PRESSURE_SENSOR_INSTALLED, DHT_SENSOR_INSTALLED)
    out += (double)temp;
    out += F(",");
#endif
#if ENABLED(PARTICLE_SENSOR_INSTALLED)
    out += p10;
    out += F(",");
    out += p25;
    out += F(",");
#endif
#if ENABLED(WIND_SPEED_SENSOR_INSTALLED)
    out += speed;
#endif
#if ENABLED(SERIAL_OUTPUT_SUPPORT)
    Serial.println(out.c_str());
#endif

#if ENABLED(SD_SUPPORT)
    saveData(out);
#endif

#endif
}
