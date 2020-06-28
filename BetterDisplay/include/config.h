//#define SD_SUPPORT
#define I2C_LCD_SUPPORT
#define NRF24_SUPPORT

//#define INTERRUPT_ENABLED

#define usualDelay 5000 //default delay of side change

#ifdef NRF24_SUPPORT

#define CE D4
#define CSN D8

#endif