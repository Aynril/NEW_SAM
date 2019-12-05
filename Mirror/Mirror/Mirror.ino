#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

extern float values[];

unsigned short displayPage = 0;

unsigned long startTime = 0;

RF24 radio (9, 10);
PROGMEM const uint64_t address = 0xF0F0F0F0F0;  
 
struct RadioPacket {
  uint8_t lpg;
  uint8_t co;
  uint8_t smoke;
  uint8_t rain;
  uint8_t earthHumidity;
  uint8_t light;
  uint8_t airHumidity;
  uint8_t temp;
  float pressure;
  uint8_t p25;
  uint8_t p10;
  uint8_t radiation;
};
 
RadioPacket _radioData;
 
void initRadio(){
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);//tmp => cus of power reasons, should be changed to high later to strenghen signal
  radio.setDataRate(RF24_1MBPS);//test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);//cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();//maybe this line is not needed, but i dunno
 
  radio.maskIRQ(1, 1, 0);//only interrupt in reviecing data
  radioListenMode();
  //attachInterrupt(0, receivedMessage, FALLING);//0 for pin 2 and 1 for pin 3 => these are the only hardware interrupt pins => there are ways to use other pins, but they only work with CHANGE and are a little more difficult
 
  /*Serial.println("Setup");
  Serial.print("Mirror: ");
  Serial.print("Radio is ");
  Serial.print(radio.isPVariant());
  Serial.println(".");*/
}
 
void receivedMessage(){
  if(radio.available()) {
    //Serial.println("Got message");
    radio.read(&_radioData, sizeof(_radioData));
    Serial.println(returnDataString());

  } /*else{
    Serial.println("ERROR: Interrupt triggered, even thought there is no message out there.");
  }
  */
}
 
void radioListenMode(){
  radio.openReadingPipe(0, address);
  radio.startListening();
}
 /*
void radioWriteMode(){
  radio.stopListening();
  radio.openWritingPipe(address);
}
 
void sendMessage(){//parameter is the number writen on the tower
  radioWriteMode();
  radio.write(&_radioData, sizeof(_radioData));
  radioListenMode();
}
*/

void setup() {
  Serial.begin(115200);
  Serial.println("Init begin");
  initRadio();
  startTime = millis();
  setup2();
  Serial.println("Init done");
}

String returnDataString() {
  String msg;
  msg = "";
  msg += String(_radioData.lpg);
  msg += ", ";
  msg += String(_radioData.co);
  msg += ", ";
  msg += String(_radioData.smoke);
  msg += ", ";
  msg += String(_radioData.rain);
  msg += ", ";
  msg += String(_radioData.earthHumidity);
  msg += ", ";
  msg += String(_radioData.light);
  msg += ", ";
  msg += String(_radioData.airHumidity);
  msg += ", ";
  msg += String(_radioData.temp);
  msg += ", ";
  msg += String(_radioData.pressure);
  msg += ", ";
  msg += String(_radioData.p25);
  msg += ", ";
  msg += String(_radioData.p10);
  msg += ", ";
  msg += String(_radioData.radiation);

  return msg;
}

void loop() {
  receivedMessage();
  displayChange();
  delay(10);
}
