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

#define sleepTime 300000

DHT dht(inputDHT22, DHT22);
Adafruit_BMP280 bmp;
MQ2 gasSensor(inputMQ2);
Sleep sleep;

unsigned long startTime = 0;
int lpg, methane, smoke;
int earthState, rainState, lightIntensity, airHumidity, temperature;
float pressure = 1023;
float p10Out, p25Out;
unsigned long counts;
unsigned long previousMillis;
bool altSend = false;
bool SDinitialized = false;

////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////IMPORTANT/////
////////////////////////////////////////////////////////////////////////

//If you have no SD card included, please comment out the line below!

#define sdBuiltIn

//If you have no radio included, please comment out the line below!

//#define radioBuiltIn

// If you want to see what SAM has to say

#define debug

/////////
//RADIO//
/////////

#ifdef radioBuiltIn

RF24 radio(9, 10);
const uint64_t address = 0xF0F0F0F0F0;

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

#ifdef debug
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

#ifdef sdBuiltIn

File logFile;

void initSDCard()
{
#ifdef debug
  Serial.print("Initializing SD card...");
#endif

  if (!SD.begin(SDPin))
  {
#ifdef debug
    Serial.println("initialization failed!");
#endif
  }
  else
  {
#ifdef debug
    Serial.println("initialization done.");
#endif
    SDinitialized = true;
    logFile = SD.open("data.csv", FILE_WRITE);
    if (!logFile)
    {
#ifdef debug
      Serial.println("error opening data.csv");
#endif
      SDinitialized = false;
    }
    logFile.close();
  }
}

void saveData(String msg)
{
  if (SDinitialized)
  {
    logFile = SD.open("data.csv", FILE_WRITE);
    if (logFile)
    {
#ifdef debug
      Serial.println("writing to file");
#endif
      logFile.println(msg);
      logFile.close();
#ifdef debug
      Serial.println("writing done");
#endif
    }
    else
    {
#ifdef debug
      Serial.println("error opening file");
#endif
    }
  }
}

#endif

void bmpInit()
{
  if (!bmp.begin(0x76, 0x58))
  {
#ifdef debug
    Serial.print(("Could not find a valid BMP280 sensor!"));
#endif
  }
  // Default settings from datasheet.
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode.
                  Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16,      // Filtering.
                  Adafruit_BMP280::STANDBY_MS_500); // Standby time.
}

void sensorsInit()
{
#ifdef debug
  Serial.println("Init begin");
#endif
  gasSensor.calibrate();
  dht.begin();
#ifdef radioBuiltIn
  initRadio();
#endif
#ifdef sdBuiltIn
  initSDCard();
#endif
  pinMode(P25, INPUT);
  pinMode(P10, INPUT);
  bmpInit();
#ifdef debug
  Serial.println("Init done");
#endif
}

void setup()
{
  Serial.begin(9600);
#ifdef debug
  Serial.println("Hello, you're a cool person! You built your own SAM! We hope you'll have fun with it!");
  Serial.println("First we're gonna initialise");
#endif
  sensorsInit();
#ifdef debug
  Serial.println("That was it! let's start reading from the sensors and saving the data (or sending it to your mirror)");
#endif
}

/////////////////////
//READING FUNCTIONS//
/////////////////////

void readGas()
{
  lpg = gasSensor.readLPG();
  methane = gasSensor.readMethane();
  smoke = gasSensor.readSmoke();
#ifdef debug
  Serial.println("Gas Sensor done reading");
#endif
}

void readRain()
{
  rainState = analogRead(inputRainSensor);
#ifdef debug
  Serial.println("Rain Sensor done reading");
#endif
}

void readEarthHumidity()
{
  earthState = 1023 - analogRead(inputEarthSensor);
#ifdef debug
  Serial.println("Earth Humidity Sensor done reading");
#endif
}

void readLightIntensity()
{
  lightIntensity = 1023 - analogRead(inputLightSensor);
#ifdef debug
  Serial.println("Light Sensor done reading");
#endif
}

void readAirHumidity()
{
  delay(2000);
  airHumidity = dht.readHumidity();
#ifdef debug
  Serial.println("Air Humidity Sensor done reading");
#endif
}

void readTemperature()
{
  delay(2000);
  temperature = dht.readTemperature();
#ifdef debug
  Serial.println("Temperature Sensor done reading");
#endif
}

void readPressure()
{
  pressure = bmp.readPressure();
#ifdef debug
  Serial.println("Pressure Sensor done reading");
#endif
}

void readParticleByPin(int pin)
{
  if (digitalRead(pin))
  {
    startTime = micros();
    while (digitalRead(10))
    {
      delayMicroseconds(1);
    }
#ifdef debug
    Serial.println(pin);
#endif
    if (pin == P25)
    {
      p25Out = micros() - startTime;
    }
    else
    {
      p10Out = micros() - startTime;
    }
  }
  else
  {
#ifdef debug
    Serial.println("Could not read " + String(pin));
#endif
  }
}

void readParticles()
{
  readParticleByPin(P25);
  readParticleByPin(P10);
#ifdef debug
  Serial.println("Particle Sensor done reading");
#endif
}

void readValuesFromSensors()
{
  readRain();
  readLightIntensity();
  readGas();
  readEarthHumidity();
  readAirHumidity();
  readTemperature();
  readPressure();
  readParticles();
}

void printDebugAndSave()
{
  String msg;
  msg += String(lpg);
  msg += ", ";
  msg += String(methane);
  msg += ", ";
  msg += String(smoke);
  msg += ", ";
  msg += String(rainState);
  msg += ", ";
  msg += String(earthState);
  msg += ", ";
  msg += String(lightIntensity);
  msg += ", ";
  msg += String(airHumidity);
  msg += ", ";
  msg += String(temperature);
  msg += ", ";
  msg += String(pressure);
  msg += ", ";
  msg += String(p25Out);
  msg += ", ";
  msg += String(p10Out);
#ifdef debug
  Serial.println(msg);
#endif
#ifdef sdBuiltIn
  saveData(msg);
#endif
}

void triggerSleep()
{
  sleep.pwrDownMode();
  sleep.sleepDelay(sleepTime);
}

void loop()
{
  readValuesFromSensors();

#if defined(debug) || defined(sdBuiltIn)
  printDebugAndSave();
#endif

#ifdef radioBuiltIn
  saveDataInRadioStorage();
  sendMessage();
#endif

  //triggerSleep();
  delay(2 * 1000);
}
