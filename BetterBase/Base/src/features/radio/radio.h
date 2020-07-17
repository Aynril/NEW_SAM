#pragma once

#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>

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
void sendMessage();

void setLPG(uint8_t lpg);

void setMethane(uint8_t methane);

void setRain(uint8_t rain);

void setEarhtHumidity(uint8_t earthHumidity);

void setLightIntensity(uint8_t lightIntensity);

void setAirHumidity(uint8_t airHumidity);

void setTemperature(int8_t temperature);

void setAirPressure(float pressure);

void setParticles(uint8_t p10, uint8_t p25);