#pragma once

#ifndef __RADIO_H__
#define __RADIO_H__
#endif // !__RADIO_H__

#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>

extern unsigned long startTime;
extern int lpg, methane, smoke;
extern int earthState, rainState, lightIntensity, airHumidity, temperature;
extern float pressure;
extern float p10Out, p25Out;
extern unsigned long counts;
extern unsigned long previousMillis;
extern bool altSend;
extern bool SDinitialized;

struct RadioPacket
{
  uint8_t lpg;
  uint8_t methane;
  uint8_t smoke;
  uint8_t rain;
  uint8_t earthHumidity;
  uint8_t light;
  uint8_t airHumidity;
  int8_t temp;
  float pressure;
  uint8_t p25;
  uint8_t p10;
};

void initRadio();
void radioWriteMode();
void saveDataInRadioStorage();
void sendMessage();