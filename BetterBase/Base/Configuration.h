#pragma once


//#define DEBUG_DEVELOPMENT_MODE

//If you have no SD card included, please comment out the line below!

//#define SD_SUPPORT

//If you have no radio included, please comment out the line below!

#define RADIO_SUPPORT

#if ENABLED(RADIO_SUPPORT)
    #define RADIO_SENDER_ADDRESS 0x4209696
    #define RADIO_POWER_LEVEL 3
    // #define RADIO_LOW_POWER_MODE
    // #define RADIO_CRC_ENABLED
    // #define RADIO_SINGLE_RECIEVER
#endif

#define RTC_SUPPORT

#if ENABLED(RTC_SUPPORT)
    #define RTC_TYPE_D3231
    //#define RTC_TYPE_D1207
#endif

#define SERIAL_OUTPUT_SUPPORT

// If you want to see what SAM has to say

#define DEBUG_FEATURE


///////////
//SENSORS//
///////////

#define AIR_PRESSURE_SENSOR_INSTALLED

#define DHT_SENSOR_INSTALLED

// #define GAS_SENSOR_INSTALLED

//#define PARTICLE_SENSOR_INSTALLED

#define WIND_SPEED_SENSOR_INSTALLED