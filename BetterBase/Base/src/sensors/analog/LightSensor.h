#include "AnalogSensor.h"

class LightSensor : public AnalogSensor
{
private:
    /* data */
public:
    LightSensor(): AnalogSensor(PINOUT_LIGHT_SENSOR_INPUT, true) {}
};