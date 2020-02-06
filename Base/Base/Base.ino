/////////////
//LIBRARIES//
/////////////

#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_BMP280.h>
#include <MQ2.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <Sleep_n0m1.h>

//////////////
//ATTRIBUTES//
//////////////

#define inputMQ2 A0
#define inputRainSensor A1
#define inputLightSensor A2
#define inputEarthSensor A3
#define inputDHT22 6
#define SDPin 8
#define P25 4
#define P10 5
#define LOG_PERIOD 60000

DHT dht(inputDHT22, DHT22);
Adafruit_BMP280 bmp;
MQ2 gasSensor(inputMQ2);
Sleep sleep;

unsigned long startTime = 0;
int lpg, co, smoke;
int earthState, rainState, lightIntensity, airHumidity, temperature;
float pressure = 1023;
float p10Out, p25Out;
int radiation = 0;
unsigned long counts;
unsigned long previousMillis;
unsigned long sleepTime;
bool altSend = false;
bool SDinitialized = false;

////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////IMPORTANT/////
////////////////////////////////////////////////////////////////////////

//If you have no SD card included, please comment out the line below!

#define sdBuiltIn true

//If you have no radio included, please comment out the line below!

//#define radioBuiltIn true


/////////
//RADIO//
/////////

#ifdef radioBuiltIn

RF24 radio (9, 10);
const uint64_t address = 0xF0F0F0F0F0;

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

////////
//INIT//
////////

void initRadio() {
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);//tmp => cus of power reasons, should be changed to high later to strengthen signal
  radio.setDataRate(RF24_1MBPS);//test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);//cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();//maybe this line is not needed, but i dunno

  //radio.maskIRQ(1, 1, 0);//only interrupt in reviecing data
  radioListenMode();
  //attachInterrupt(0, receivedMessage, FALLING);//0 for pin 2 and 1 for pin 3 => these are the only hardware interrupt pins => there are ways to use other pins, but they only work with CHANGE and are a little more difficult

  Serial.println("Setup");
  Serial.print("Base: ");
  Serial.print("Radio is ");
  Serial.print(radio.isPVariant());
  Serial.println(".");
}
/*
  void receivedMessage(){
  if(radio.available()) {
   Serial.println("Got message");
   radio.read(&_radioData, sizeof(_radioData));
  } else{
   Serial.println("ERROR: Interrupt triggered, even thought there is no message out there.");
  }
  }
*/
void radioListenMode() {
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void radioWriteMode() {
  radio.stopListening();
  radio.openWritingPipe(address);
}

void sendMessage() { //parameter is the number writen on the tower
  radioWriteMode();
  radio.write(&_radioData, sizeof(_radioData));
  radioListenMode();
}

#endif

#ifdef sdBuiltIn

File myFile;

void initSDCard() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(SDPin)) {
    Serial.println("initialization failed!");
  }
  else {
    Serial.println("initialization done.");
    SDinitialized = true;
    myFile = SD.open("data.txt", FILE_WRITE);
    if (!myFile) {
      Serial.println("error opening test.txt");
      SDinitialized = false;
    }
    myFile.close();
  }
}

#endif

void bmpInit() {
  if (!bmp.begin(0x76, 0x58)) {
    Serial.print(("Could not find a valid BMP280 sensor!"));
  }
  // Default settings from datasheet.
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode.
                  Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16,      // Filtering.
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time.
}

void sensorsInit() {
  Serial.println("Init begin");
  gasSensor.begin();
  dht.begin();
  if (radioBuiltIn) {
    initRadio();
  }
  if(sdBuiltIn){
    sdInit();
  }
  pinMode(P25, INPUT);
  pinMode(P10, INPUT);
  bmpInit();
  sleepTime = 300000;
  Serial.println("Init done");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, you're a cool person! You built your own SAM! We hope you'll have fun with it!");
  Serial.println("First we're gonna initialise");
  sensorsInit();
  Serial.println("That was it! let's start reading from the sensors and saving the data (or sending it to your mirror)");
}

/////////////////////
//READING FUNCTIONS//
/////////////////////

void printGas() {
  lpg = gasSensor.readLPG();
  co = gasSensor.readCO();
  smoke = gasSensor.readSmoke();
  Serial.println("Gas Sensor done reading");
}

void printRain() {
  rainState = analogRead(inputRainSensor);
  Serial.println("Rain Sensor done reading");
}

void printEarthHumidity() {
  earthState = 1023 - analogRead(inputEarthSensor);
  Serial.println("Earth Humidity Sensor done reading");
}

void printLightIntensity() {
  lightIntensity = 1023 - analogRead(inputLightSensor);
  Serial.println("Light Sensor done reading");
}

void printAirHumidity() {
  delay(2000);
  airHumidity = dht.readHumidity();
  Serial.println("Air Humidity Sensor done reading");
}

void printTemperature() {
  delay(2000);
  temperature = dht.readTemperature();
  Serial.println("Temperature Sensor done reading");
}

void printPressure() {
  pressure = bmp.readPressure();
  Serial.println("Pressure Sensor done reading");
}

void readParticle(int pin) {
  if (digitalRead(pin)) {
    startTime = micros();
    while (digitalRead(10)) {
      delayMicroseconds(1);
    }
    Serial.println(pin);
    if (pin == P25) {
      p25Out = micros() - startTime;
    }
    /*
    else {
      p10Out = micros() - startTime;
    }
    */
  }
  else {
    Serial.println("Could not read " + String(pin));
  }
}

void printParticle() {
  readParticle(P25);
  //readParticle(P10);
  Serial.println("Particle Sensor done reading");
}

/*
  void printRadiation() {
  counts = 0;
  unsigned long measurement = millis();
  while (true) {
    if (millis() - measurement > 60000) break;
  }
  Serial.println(radiation);
  }

  void impulse() {
  detachInterrupt(3);
  counts++;
  while (digitalRead(3) == 0) {}
  attachInterrupt(3, impulse, FALLING);
  }
*/


void readValuesFromSensors() {
  printRain();
  printLightIntensity();
  printGas();
  printEarthHumidity();
  printAirHumidity();
  printTemperature();
  printPressure();
  printParticle();
  //printRadiation();
  saveDataInRadioStorage();
}

void saveDataInRadioStorage() {
  _radioData.lpg = lpg;
  _radioData.co = co;
  _radioData.rain = rainState;
  _radioData.earthHumidity = earthState;
  _radioData.light = lightIntensity;
  _radioData.airHumidity = airHumidity;
  _radioData.temp = temperature;
  _radioData.pressure = pressure;
  _radioData.p25 = p25Out;
  _radioData.p10 = p10Out;
  _radioData.radiation = radiation;
}

void saveData(String msg){
  if(SDinitialized){
    myFile = SD.open("data.txt", FILE_WRITE);
    if(myFile){
      Serial.println("writing to file");
      myFile.println(msg);
      myFile.close();
      Serial.println("writing done");
    }
    else{
      Serial.println("error opening file");
    }
  }
}

/*
  void setAlternatingPlainValues() {
  if (altSend) {
    lpg = 1023;
    co = 1023;
    smoke = 1023;
    rainState = 1023;
    earthState = 1023;
    lightIntensity = 1023;
    airHumidity = 1023;
    temperature = 1023;
    pressure = 1023;
    p25Out = 1023;
    p10Out = 1023;
    radiation = 1023;
  }
  else {
    lpg = 0;
    co = 0;
    smoke = 0;
    rainState = 0;
    earthState = 0;
    lightIntensity = 0;
    airHumidity = 0;
    temperature = 0;
    pressure = 0;
    p25Out = 0;
    p10Out = 0;
    radiation = 0;
  }
  saveDataInRadioStorage();
  altSend = !altSend;
  }
*/


void printRadioDataStringAndSaveData() {
  String msg;
  msg = "Radio: ";
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

  Serial.println(msg);
  saveData(msg);
}

void triggerSleep() {
  sleep.pwrDownMode();
  sleep.sleepDelay(sleepTime);
}

void loop() {
  readValuesFromSensors();
  saveDataInRadioStorage();
  printRadioDataStringAndSaveData();
  sendMessage();
  triggerSleep();
  delay(2 * 1000);
}
