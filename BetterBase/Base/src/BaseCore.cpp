#include <Arduino.h>
#include "BaseCore.h"

#include "sensors/SensorManager.h"

SensorManager manager;
#if ENABLED(LOW_POWER_MOSFET)
LowPower lp(PINOUT_POWER_MOSFET_SWITCH, P_CHANNEL);
#elif ENABLED(LOW_POWER_MODE)
LowPower lp(-1, NO_MOSFET);
#endif

void setup()
{
    digitalWrite(PINOUT_HARDWARE_RESET_PIN, HIGH);
    pinMode(PINOUT_HARDWARE_RESET_PIN, OUTPUT);
#if ANY(DEBUG_FEATURE, SERIAL_OUTPUT_SUPPORT)
    Serial.begin(115200);
#endif
#if ENABLED(RADIO_SUPPORT)
    delay(3000); // To stabelize the voltages
    initRadio();
#endif
#if ENABLED(SD_SUPPORT)
    initSDCard();
#endif

#if ENABLED(LOW_POWER_MODE)
    lp.init();
#if ENABLED(LOW_POWER_MOSFET)
    lp.enable5V();
#endif
    delay(3000);
#endif

    manager.initAll();
}

void loop()
{
#if ENABLED(LOW_POWER_MODE)
    //lp.powerupFunctions();
    delay(2000);
    manager.sensorsOn();
#endif

    manager.saveAll();
#if ENABLED(LOW_POWER_MODE)
    manager.sensorsOff();
#endif
#if ENABLED(RADIO_SUPPORT)
    sendMessage();
#endif
#if ENABLED(LOW_POWER_MODE)
    //lp.shutdownFuntions();
    delay(5000);
#endif
#if DISABLED(DEBUG_DEVELOPMENT_MODE)
    delay(30000);
#else
    delay(300);
#endif
    //delay(5000);
}
