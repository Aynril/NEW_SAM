#include <Arduino.h>
#include "BaseCore.h"

#if ENABLED(RADIO_SUPPORT)

#include "features/radio/radio.h"

#endif

#include "sensors/SensorManager.h"

void setup() {
#if ENABLED(DEBUG_FEATURE)
    Serial.begin(115200);
#endif

    PRINT_DEBUG_LN("Hi i'm a tree!");

}

SensorManager manager;

void loop() {
    manager.saveAll();
    #if ENABLED(RADIO_SUPPORT)
    sendMessage();
    #endif
}
