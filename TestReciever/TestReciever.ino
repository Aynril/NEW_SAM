/*

Demonstrates sending data packets of different length.  The receiver will check to see what size of
packet was received and act accordingly.

Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND

*/

#include <SPI.h>
#include "RF24.h"

/*************  USER Configuration *****************************/
                                          // Hardware configuration
RF24 radio(7,8);                        // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

/***************************************************************/
const uint64_t address = 0x4209696;

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

void setup()
{
    Serial.begin(115200);

    if (!radio.begin())
    {
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
    }
  bool radioOK = radio.begin();
  if (!radioOK)
  {
    Serial.println("ERROR DURING RADIO INIT");
  }
  radio.setPALevel(RF24_PA_MIN); //tmp => cus of power reasons, should be changed to high later to strenghen signal
  radio.setDataRate(RF24_1MBPS); //test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);       //cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();            //maybe this line is not needed, but i dunno
    radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop()
{
   if (radio.available())
  {
    Serial.println(F("Got message"));
    radio.read(&radioData, sizeof(radioData));
  }
}
