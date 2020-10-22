#include "../../inc/BaseConfig.h"

#if ENABLED(RADIO_SUPPORT)

#include "radio.h"

RF24 radio(PINOUT_RADIO_CE, PINOUT_RADIO_CSN, 250000);

RadioPacket radioData;

////////
//INIT//
////////

void initRadio()
{
  bool initOK = false;
  if (!radio.begin())
  {
    Serial.println("ERROR: Radio was not initalized");
    delay(500);
    for (uint8_t i = 0; i < 5; i++)
    {
      if (radio.begin())
      {
        PRINT_DEBUG_LN("Success Radio");
        initOK = true;
        break;
      }
      delay(500);
    }
    if (!initOK)
    {
      PRINT_DEBUG_LN("Ultimate Fail!");
      RESET();
    }
  }
  else
  {
    PRINT_DEBUG_LN("Success Radio");
  }
  radio.setPALevel(RADIO_POWER_LEVEL);
  radio.setDataRate(RF24_1MBPS);

#if DISABLED(RADIO_SINGLE_RECIEVER)
  radio.setAutoAck(false);
#endif

#if DISABLED(RADIO_CRC_ENABLED)
  radio.disableCRC();
#else
  radio.setCRCLength(RF24_CRC_8);
#endif

#if ENABLED(RADIO_LOW_POWER_MODE)
  radio.powerDown();
#endif

  PRINT_DEBUG_LN("Setup Radio");
  PRINT_DEBUG_LN("Base: ");
  PRINT_DEBUG("Is Radio Plus Variant");
  PRINT_DEBUG(radio.isPVariant());
  PRINT_DEBUG_LN(".");
}

void radioWriteMode()
{
  radio.stopListening();
  PRINT_DEBUG("Open writing Pipe at adress ");
  PRINT_DEBUG_HEX(RADIO_SENDER_ADDRESS);
  PRINT_DEBUG_LN("");
  radio.openWritingPipe(RADIO_SENDER_ADDRESS);
}

void sendMessage()
{
#if ENABLED(RADIO_LOW_POWER_MODE)
  radio.powerUp();
  delay(15);
#endif

  radioWriteMode();
  bool writeOK = radio.writeFast(&radioData, sizeof(radioData));
  bool waitOK = radio.txStandBy();
  if (waitOK)
  {
    Serial.println("Wait OK");
  }
  if (writeOK) {
    Serial.println("Write OK");
  }
  if (!writeOK && !waitOK) {
    Serial.println("Fail");
  }

  radio.startListening();

#if ENABLED(RADIO_LOW_POWER_MODE)
  delay(15);
  radio.powerDown();
#endif
}

void setPropane(uint16_t propane)
{
  radioData.propane = propane;
}

void setMethane(uint16_t methane)
{
  radioData.methane = methane;
}

void setSmoke(uint16_t smoke)
{
  //radioData.smoke = smoke;
}

void setHydrogen(uint16_t hydrogen)
{
  radioData.hydrogen = hydrogen;
}

void setRain(uint8_t rain)
{
  radioData.rain = rain;
}

void setEarhtHumidity(uint8_t earthHumidity)
{
  radioData.earthHumidity = earthHumidity;
}

void setLightIntensity(uint8_t lightIntensity)
{
  radioData.light = lightIntensity;
}

void setAirHumidity(float airHumidity)
{
  radioData.airHumidity = airHumidity;
}

void setTemperature(int8_t temperature)
{
  radioData.temp = temperature;
}

void setAirPressure(float pressure)
{
  radioData.pressure = pressure;
}

void setAltitude(float altitude)
{
  radioData.altitude = altitude;
}

void setParticles(uint8_t p10, uint8_t p25)
{
  radioData.p10 = p10;
  radioData.p25 = p25;
}

void setWindSpeed(float speed)
{
  radioData.windSpeed = speed;
}

#endif //RADIO_SUPPORT