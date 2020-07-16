#include "../../inc/BaseConfig.h"

#if ENABLED(RADIO_SUPPORT)

#include "radio.h"

RF24 radio(9, 10);

RadioPacket radioData;

////////
//INIT//
////////

void initRadio()
{
  radio.begin();
  radio.setPALevel(RADIO_POWER_LEVEL);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(false);       //cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();            //maybe this line is not needed, but i dunno
  radio.powerDown();

#if ENABLED(DEBUG_FEATURE)
  Serial.println(F("Setup"));
  Serial.print("Base: ");
  Serial.print("Radio is ");
  Serial.print(radio.isPVariant());
  Serial.println(".");
#endif
}

void radioWriteMode()
{
  radio.stopListening();
  radio.openWritingPipe((const uint8_t*) RADIO_SENDER_ADDRESS);
}

void sendMessage()
{
  #if ENABLED(RADIO_LOW_POWER_MODE)
  radio.powerUp();
  delay(15);
  #endif

  radioWriteMode();
  radio.write(&radioData, sizeof(radioData));

  #if ENABLED(RADIO_LOW_POWER_MODE)
  delay(15);
  radio.powerDown();
  #endif
}

void setLPG(uint8_t lpg) {
  radioData.lpg = lpg;
}

void setMethane(uint8_t methane) {
  radioData.methane = methane;
}

void setRain(uint8_t rain) {
  radioData.rain = rain;
}

void setEarhtHumidity(uint8_t earthHumidity) {
  radioData.earthHumidity = earthHumidity;
}

void setLightIntensity(uint8_t lightIntensity) {
  radioData.light = lightIntensity;
}

void setAirHumidity(uint8_t airHumidity) {
  radioData.airHumidity = airHumidity;
}

void setTemperature(int8_t temperature) {
  radioData.temp = temperature;
}

void setAirPressure(float pressure) {
  radioData.pressure = pressure;
}

void setParticles(uint8_t p10, uint8_t p25) {
  radioData.p10 = p10;
  radioData.p25 = p25;
}

#endif //RADIO_SUPPORT