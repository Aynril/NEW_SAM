#include <inc/BaseConfig.h>
#include <features/radio/radio.h>

#ifdef RADIO_SUPPORT

RF24 radio(9, 10);
const uint64_t address = 0xF0F0F0F0F0;

RadioPacket radioData;

////////
//INIT//
////////

void initRadio()
{
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS); //test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);       //cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();            //maybe this line is not needed, but i dunno
  radio.powerDown();

#ifdef DEBUG_FEATURE
  Serial.println("Setup");
  Serial.print("Base: ");
  Serial.print("Radio is ");
  Serial.print(radio.isPVariant());
  Serial.println(".");
#endif
}

void radioWriteMode()
{
  radio.stopListening();
  radio.openWritingPipe(address);
}

void sendMessage()
{ //parameter is the number writen on the tower
  radio.powerUp();
  delay(15);
  radioWriteMode();
  radio.write(&radioData, sizeof(radioData));
  delay(15);
  radio.powerDown();
}

void saveDataInRadioStorage()
{
  radioData.lpg = lpg;
  radioData.methane = methane;
  radioData.rain = rainState;
  radioData.earthHumidity = earthState;
  radioData.light = lightIntensity;
  radioData.airHumidity = airHumidity;
  radioData.temp = temperature;
  radioData.pressure = pressure;
  radioData.p25 = p25Out;
  radioData.p10 = p10Out;
}

#endif