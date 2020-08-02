#include "../inc/BaseConfig.h"

#if ENABLED(PARTICLE_SENSOR_INSTALLED)
#include "PMSensor.h"

uint8_t PMSensor::readParticleByPin(int pin)
{
    PRINT_DEBUG_LN("Pin is" + String(pin));
    return pulseIn(pin, HIGH, 1500000) / 1000 - 2;
}

uint8_t PMSensor::get10() {
    return readParticleByPin(PINOUT_NOVA_PM_SENSOR_PWM_10);
}

uint8_t PMSensor::get25() {
    return readParticleByPin(PINOUT_NOVA_PM_SENSOR_PWM_25);
}
#endif