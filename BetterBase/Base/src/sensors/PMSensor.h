#pragma once

#include "../inc/BaseConfig.h"

class SDS011
{
private:
    uint8_t readParticleByPin(int pin);
    Stream &sds_data;
    void waitAndAnswer();
    void sendCMD(const byte cmd[19]);
    int process(int db1);

    int pwmPin25;
    int pwmPin10;

    bool serialMode = false;

public:
    uint8_t get25();
    uint8_t get10();
    SDS011() {};
    void init(Stream &serial)
    {
        sds_data = serial;
        serialMode = true;
    }
    void init(int pin25, int pin10) {
        pwmPin10 = pin10;
        pwmPin25 = pin25;
        serialMode = false;
    }
    int read(float *p25, float *p10);
    void sleep();
    void wakeup();
    void printMode();
    void autoMode();
    const byte *constructCMD(
        byte command,
        byte data2,
        byte data3);
    const byte *constructCMD(
        byte command,
        byte data2,
        byte data3,
        int deviceID);
};
