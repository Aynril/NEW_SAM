#pragma once

#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>

struct RadioPacket
{
  uint16_t propane;
  uint16_t methane;
  uint16_t smoke;
  uint16_t hydrogen;

  uint8_t rain;
  uint8_t earthHumidity;
  uint8_t light;

  float airHumidity;
  float temp;

  float pressure;
  float altitude;

  uint8_t p25;
  uint8_t p10;

  float windSpeed;
};

void initRadio();
void radioWriteMode();
void sendMessage();

void setPropane(uint16_t propane);

void setMethane(uint16_t methane);

void setSmoke(uint16_t smoke);

void setHydrogen(uint16_t hydrogen);

void setRain(uint8_t rain);

void setEarhtHumidity(uint8_t earthHumidity);

void setLightIntensity(uint8_t lightIntensity);


void setAirHumidity(float airHumidity);

void setTemperature(int8_t temperature);


void setAirPressure(float pressure);

void setAltitude(float altitude);


void setParticles(uint8_t p10, uint8_t p25);

void setWindSpeed(float speed);