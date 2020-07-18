#include "AnalogSensor.h"

class EarthHumiditySensor : public AnalogSensor
{
private:
    /* data */
public:
    EarthHumiditySensor(): AnalogSensor(PINOUT_RAIN_SENSOR_INPUT, false) {}
};