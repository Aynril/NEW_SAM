#include "../inc/BaseConfig.h"
#if ENABLED(AIR_PRESSURE_SENSOR_INSTALLED)
#include "AirPressureSensor.h"

void AirPressureSensor::init()
{
    PRINT_DEBUG_LN("[AIR PRESSURE] Initializing...");
    if (!bmp.begin(0x76, 0x58))
    {
        PRINT_DEBUG_LN("[AIR PRESSURE] Could not find a valid BMP280 sensor!");
    }
    else
    {
        PRINT_DEBUG_LN("[AIR PRESSURE] Found a valid BMP280");
    }

    // Default settings from datasheet.
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode.
                    Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling
                    Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                    Adafruit_BMP280::FILTER_X16,      // Filtering.
                    Adafruit_BMP280::STANDBY_MS_500); // Standby time.
}

float AirPressureSensor::getPressure()
{
    return bmp.readPressure() / 100;
}

float AirPressureSensor::getTemperature()
{
    return bmp.readTemperature();
}

float AirPressureSensor::getAltitute()
{
    return bmp.readAltitude();
}
#endif