#pragma once

#include "../inc/BaseConfig.h"
#include "SoftwareSerial.h"

class SDS011
{
private:
    uint8_t readParticleByPin(int pin);
    Stream &sds_data;
    void waitAndAnswer();
    void sendCMD(const byte cmd[19]);
    int process(int db1);

    int pwmPin10;
    int pwmPin25;

    bool serialMode = false;
    SoftwareSerial emptySerial = SoftwareSerial(0,0);
    int readSerial(float *p25, float *p10);

public:
    uint8_t get10();
    uint8_t get25();
    SDS011(Stream &serial) : sds_data(serial), pwmPin10(0), pwmPin25(0) {serialMode = true;};
    SDS011(int pin25, int pin10) : sds_data(emptySerial), pwmPin10(pin10), pwmPin25(pin25) {serialMode = false; };
    void init(Stream &serial)
    {
        sds_data = serial;
        serialMode = true;
    }
    void init(int pin25, int pin10)
    {
        pwmPin10 = pin10;
        pwmPin25 = pin25;
        serialMode = false;
    }

    void sleep();
    void wakeup();
    void printMode();
    void autoMode();
    void queryMode();
    int read(float *p25, float *p10);
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
