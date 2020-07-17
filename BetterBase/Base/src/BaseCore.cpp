//#pragma once

#include <Arduino.h>
#include "BaseCore.h"

#if ENABLED(RADIO_SUPPORT)
#include "features/radio/radio.h"
#endif

void setup()
{
#if ENABLED(DEBUG_FEATURE)
    Serial.begin(115200);
#endif
}

void loop()
{
}
