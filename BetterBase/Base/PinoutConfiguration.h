#pragma once

#define inputMQ2 A0

#define inputRainSensor A1

#define inputLightSensor A2

#define inputEarthSensor A3

#define inputDHT22 6

#define P25 4
#define P10 5

#if ENABLED(SD_SUPPORT)
#define SDPin 8
#endif

#if ENABLED(RADIO_SUPPORT)
#define PINOUT_RADIO_CE 9
#define PINOUT_RADIO_CSN 10
#endif