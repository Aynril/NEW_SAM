#include <Arduino.h>
#include "BaseCore.h"

#if ENABLED(RADIO_SUPPORT)

#include "features/radio/radio.h"

#endif

#include "sensors/SensorManager.h"

SensorManager manager;
#if ENABLED(LOW_POWER_FUNCTION_SUPPORTED)
LowPower lp(PINOUT_POWER_MOSFET_SWITCH, P_CHANNEL);
#endif

void setup()
{
    #if ANY(DEBUG_FEATURE, SERIAL_OUTPUT_SUPPORT)
    Serial.begin(115200);
    #endif

    #if ENABLED(RADIO_SUPPORT)
    initRadio();
    #endif

    #if ENABLED(LOW_POWER_FUNCTION_SUPPORTED)
    lp.init();
    lp.enable5V();
    delay(3000);
    #endif

    manager.initAll();
}

void loop()
{
    #if ENABLED(LOW_POWER_FUNCTION_SUPPORTED)
    PRINT_DEBUG_LN("5V on");
    lp.enable5V();
    delay(3000);
    #endif
    manager.sensorsOn();
    manager.saveAll();
    manager.sensorsOff();
    #if ENABLED(RADIO_SUPPORT)
    sendMessage();
    #endif
    #if ENABLED(LOW_POWER_FUNCTION_SUPPORTED)
    PRINT_DEBUG_LN("5V off");
    lp.disable5V();
    delay(5000);
    #endif
    delay(5000);
}
