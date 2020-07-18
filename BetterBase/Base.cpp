/////////////
//LIBRARIES//
/////////////
//#include <config.h>
//#include <Adafruit_BMP280.h>
//#include <MQ2.h>
//#include <DHT.h>
//#include <Arduino.h>
//#include <Sleep_n0m1.h>

//////////////
//ATTRIBUTES//
//////////////

DHT dht(inputDHT22, DHT22);
Adafruit_BMP280 bmp;
MQ2 gasSensor(inputMQ2);
Sleep sleep;

#define sleepTime 300000

unsigned long startTime = 0;
int lpg, methane, smoke;
int earthState, rainState, lightIntensity, airHumidity, temperature;
float pressure = 1023;
float p10Out, p25Out;
unsigned long counts;
unsigned long previousMillis;
bool altSend = false;
bool SDinitialized = false;

void bmpInit()
{
  if (!bmp.begin(0x76, 0x58))
  {
#ifdef DEBUG_FEATURE
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
#ifdef DEBUG_FEATURE
  Serial.println("Init begin");
#endif
  gasSensor.calibrate();
  dht.begin();
#ifdef RADIO_SUPPORT
  initRadio();
#endif
#ifdef SD_SUPPORT
  initSDCard();
#endif
  pinMode(P25, INPUT);
  pinMode(P10, INPUT);
  bmpInit();
#ifdef DEBUG_FEATURE
  Serial.println("Init done");
#endif
}

void setup()
{
  Serial.begin(9600);
#ifdef DEBUG_FEATURE
  Serial.println("Hello, you're a cool person! You built your own SAM! We hope you'll have fun with it!");
  Serial.println("First we're gonna initialise");
#endif
  sensorsInit();
#ifdef DEBUG_FEATURE
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
#ifdef DEBUG_FEATURE
  Serial.println("Gas Sensor done reading");
#endif
}

void readRain()
{
  rainState = analogRead(inputRainSensor);
#ifdef DEBUG_FEATURE
  Serial.println("Rain Sensor done reading");
#endif
}

void readEarthHumidity()
{
  earthState = 1023 - analogRead(inputEarthSensor);
#ifdef DEBUG_FEATURE
  Serial.println("Earth Humidity Sensor done reading");
#endif
}

void readLightIntensity()
{
  lightIntensity = 1023 - analogRead(inputLightSensor);
#ifdef DEBUG_FEATURE
  Serial.println("Light Sensor done reading");
#endif
}

void readAirHumidity()
{
  delay(2000);
  airHumidity = dht.readHumidity();
#ifdef DEBUG_FEATURE
  Serial.println("Air Humidity Sensor done reading");
#endif
}

void readTemperature()
{
  delay(2000);
  temperature = dht.readTemperature();
#ifdef DEBUG_FEATURE
  Serial.println("Temperature Sensor done reading");
#endif
}

void readPressure()
{
  pressure = bmp.readPressure();
#ifdef DEBUG_FEATURE
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
#ifdef DEBUG_FEATURE
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
#ifdef DEBUG_FEATURE
    Serial.println("Could not read " + String(pin));
#endif
  }
}

void readParticles()
{
  readParticleByPin(P25);
  readParticleByPin(P10);
#ifdef DEBUG_FEATURE
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
#ifdef DEBUG_FEATURE
  Serial.println(msg);
#endif
#ifdef SD_SUPPORT
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

#if defined(DEBUG_FEATURE) || defined(SD_SUPPORT)
  printDebugAndSave();
#endif

#ifdef RADIO_SUPPORT
  saveDataInRadioStorage();
  sendMessage();
#endif

  //triggerSleep();
  delay(2 * 1000);
}
