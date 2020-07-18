#include "AnalogSensor.h"

class RainSensor : public AnalogSensor
{
private:
    /* data */
public:
    RainSensor(): AnalogSensor(PINOUT_RAIN_SENSOR_INPUT, true) {}
};