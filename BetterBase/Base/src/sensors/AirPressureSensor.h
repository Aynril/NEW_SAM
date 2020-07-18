#pragma once

#include "../templates/Singleton.h"
#include "../inc/BaseConfig.h"
#include <Adafruit_BMP280.h>

class AirPressureSensor
{
private:
    Adafruit_BMP280 bmp;
public:
    AirPressureSensor() { };

    void init();
    float getTemperature();
    float getPressure();
    float getAltitute();
};