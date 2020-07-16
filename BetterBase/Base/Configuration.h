#pragma once

#define inputMQ2 A0
#define inputRainSensor A1
#define inputLightSensor A2
#define inputEarthSensor A3
#define inputDHT22 6
#define SDPin 8
#define P25 4
#define P10 5

//If you have no SD card included, please comment out the line below!

#define SD_SUPPORT

//If you have no radio included, please comment out the line below!

#define RADIO_SUPPORT

#if ENABLED(RADIO_SUPPORT)
    #define RADIO_SENDER_ADDRESS 0x000000
    #define RADIO_POWER_LEVEL 3
    // #define RADIO_LOW_POWER_MODE
#endif

// If you want to see what SAM has to say

#define DEBUG_FEATURE