#include "../inc/BaseConfig.h"
#include "AirPressureSensor.h"

void AirPressureSensor::init()
{
    if (!bmp.begin(0x76, 0x58))
    {
        PRINT_DEBUG_LN("Could not find a valid BMP280 sensor!");
    }
    // Default settings from datasheet.
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode.
                    Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling
                    Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                    Adafruit_BMP280::FILTER_X16,      // Filtering.
                    Adafruit_BMP280::STANDBY_MS_500); // Standby time.
}

float AirPressureSensor::getPressure() {
    return bmp.readPressure();
}

float AirPressureSensor::getTemperature() {
    return bmp.readTemperature();
}

float AirPressureSensor::getAltitute() {
    return bmp.readAltitude();
}