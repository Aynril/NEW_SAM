/*
TMRh20 2014

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/** General Data Transfer Rate Test
 * This example demonstrates basic data transfer functionality with the 
 updated library. This example will display the transfer rates acheived using
 the slower form of high-speed transfer using blocking-writes.
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

void setup(void) {

  Serial.begin(115200);

  if(radio.begin()){
    
    Serial.println("Radio OK");
    
    } else {
      Serial.println("Radio fail");
    }
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS); //test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);       //cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();            //maybe this line is not needed, but i dunno
  
  radio.powerUp();
  
  radioData.propane = 123;
  radioData.methane = 2;
  radioData.rain = 3;
  radioData.earthHumidity = 1;
  radioData.light = 2;
  radioData.airHumidity = 3;
  radioData.temp = 4;
  radioData.pressure = 5;
  radioData.p25 = 6;
  radioData.p10 = 7;
}

void loop(void){
  radio.stopListening();
  radio.openWritingPipe(address);
  Serial.println(radio.write(&radioData, sizeof(radioData)));
  Serial.println("Send");

  delay(500);
}
