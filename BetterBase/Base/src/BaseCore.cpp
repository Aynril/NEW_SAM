#include <Arduino.h>
#include "BaseCore.h"

#if ENABLED(RADIO_SUPPORT)

#include "features/radio/radio.h"

#endif

#include "sensors/SensorManager.h"

void setup()
{
#if ANY(DEBUG_FEATURE, SERIAL_OUTPUT_SUPPORT)
    Serial.begin(115200);
#endif

#if ENABLED(RADIO_SUPPORT)
    initRadio();
#endif

    PRINT_DEBUG_LN("Hi i'm a tree!");
}

SensorManager manager;

void loop()
{
    manager.saveAll();
#if ENABLED(RADIO_SUPPORT)
    sendMessage();
#endif
}
