#include <RF24.h>

int lpg, methane, smoke;
int earthHumidity, rain, light, airHumidity, temperature;
int winddirection = 2;
float pressure = 1023;
float p10, p25 = 0;
float windspeed = 5;
bool radioOK = true;
bool firstBlood = true;

#ifdef ESP8266
#define IRQ 10
#endif
#ifdef __AVR__
#define IRQ 2
#endif

RF24 radio(CE, CSN);
const uint64_t address PROGMEM = 0x4209696;

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

  uint8_t p25;
  uint8_t p10;
};
RadioPacket radioData;
void saveRadio()
{
  lpg = radioData.propane;
  methane = radioData.methane;
  smoke = radioData.smoke;
  rain = radioData.rain;
  earthHumidity = radioData.earthHumidity;
  light = radioData.light;
  airHumidity = radioData.airHumidity;
  temperature = radioData.temp;
  pressure = radioData.pressure;
  p25 = radioData.p25;
  p10 = radioData.p10;
  firstBlood = true;
}

#ifdef ESP8266
ICACHE_RAM_ATTR void receivedMessage()
{
  if (radio.available())
  {
    Serial.println(F("Got message"));
    radio.read(&radioData, sizeof(radioData));
    saveRadio();
  }
  else
  {
#ifdef INTERRUPT_ENABLED
    Serial.println(F("ERROR: Interrupt triggered, even thought there is no message out there."));
#endif
  }
}
#endif
#ifdef __AVR__
void receivedMessage()
{
  if (radio.available())
  {
    Serial.println(F("Got message"));
    radio.read(&radioData, sizeof(radioData));
    saveRadio();
  }
  else
  {
#ifdef INTERRUPT_ENABLED
    Serial.println(F("ERROR: Interrupt triggered, even thought there is no message out there."));
#endif
  }
}
#endif

void radioListenMode()
{
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void initRadio()
{
  radioOK =false;
  firstBlood = false;
  radioOK = radio.begin();
  if (!radioOK)
  {
    Serial.println("ERROR DURING RADIO INIT");
  }
  radio.setPALevel(RF24_PA_MIN); //tmp => cus of power reasons, should be changed to high later to strenghen signal
  radio.setDataRate(RF24_1MBPS); //test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);       //cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();            //maybe this line is not needed, but i dunno

  radioListenMode();
#ifdef INTERRUPT_ENABLED
  radio.maskIRQ(1, 1, 0);                                                //only interrupt in reviecing data
  attachInterrupt(digitalPinToInterrupt(IRQ), receivedMessage, FALLING); //0 for pin 2 and 1 for pin 3 => these are the only hardware interrupt pins => there are ways to use other pins, but they only work with CHANGE and are a little more difficult
#endif
  Serial.println(F("Setup"));
  Serial.print(F("Mirror: "));
  Serial.print(F("Radio is "));
  Serial.print(radio.isPVariant());
  Serial.println(F("."));
}