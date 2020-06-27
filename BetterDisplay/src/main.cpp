#include <Arduino.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SD_CS 8

#define IRQ 2
#define CE 9
#define CSN 10

File configFile;

#define FILE_NAME "tips.csv"

bool sdFound = false;

unsigned short displayPage = 0;

unsigned long long startTime = 0;

RF24 radio(CE, CSN);
const uint64_t address = 0xF0F0F0F0F0;

#define usualDelay 5000 //default delay of side change

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

void receivedMessage()
{
  if (radio.available())
  {
    Serial.println("Got message");
    radio.read(&radioData, sizeof(radioData));
    //Serial.println(returnDataString());
  }
  else
  {
    Serial.println("ERROR: Interrupt triggered, even thought there is no message out there.");
  }
}

void radioListenMode()
{
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void initRadio()
{
  radio.begin();
  radio.setPALevel(RF24_PA_MIN); //tmp => cus of power reasons, should be changed to high later to strenghen signal
  radio.setDataRate(RF24_1MBPS); //test RF24_250KBPS later maybe, but i dunno (only works with + variants), this method return a boolean that indicates the success of the setFunktion
  radio.setAutoAck(false);       //cus we want >1 devices to listen to this message... think about it a bit, it will make sense
  radio.disableCRC();            //maybe this line is not needed, but i dunno

  radio.maskIRQ(1, 1, 0); //only interrupt in reviecing data
  radioListenMode();
  attachInterrupt(digitalPinToInterrupt(IRQ), receivedMessage, FALLING); //0 for pin 2 and 1 for pin 3 => these are the only hardware interrupt pins => there are ways to use other pins, but they only work with CHANGE and are a little more difficult

  Serial.println("Setup");
  Serial.print("Mirror: ");
  Serial.print("Radio is ");
  Serial.print(radio.isPVariant());
  Serial.println(".");
}

void initSD()
{
  if (!SD.begin(SD_CS))
  {
    Serial.println("No SD found");
    sdFound = false;
  }
  else
  {
    Serial.println("SD found");
    sdFound = true;
  }

  if (SD.exists(FILE_NAME))
  {
    Serial.println("File found");
  }
  else
  {
    Serial.println("File not found");
    if (sdFound)
      configFile = SD.open(FILE_NAME, FILE_WRITE);
    configFile.close();
  }
}

void sdSave(String toSave)
{
  if (SD.exists(FILE_NAME) && sdFound)
  {
    configFile = SD.open(FILE_NAME, FILE_WRITE);
    configFile.println(toSave);
    configFile.close();

    Serial.println("saved Data");
  }
  else if (!sdFound)
  {
    Serial.println("no SD card found");
  }
  else
  {
    Serial.println("File does not exist");
  }
}

float values[] = {
    17,   //temp in °C
    37,   //hum in %
    530,  //earthHum in x/1023
    267,  //rain in x/1023
    1003, //pressure in HPa
    200,  //lightIntensity in x/1023
    20,   //pm25 in ppm
    17,   //pm10 in ppm
    0,    //lpg in ppm
    0,    //co in ppm
    0,    //smoke in ppm
    5,    //windspeed in m/s
    2     //winddirection; 0 for North, 1 for NorthEast... 7 for NorthWest
};

const char *const tip[20] PROGMEM = {
    "Heat up living space",                                              //[0] TempTipBelow15nr1
    "Dress up properly for cold weather",                                //[1] TempTipBelow15nr2
    "Heat living space but remember to shock-ventilate and close doors", //[2] TempTip16to20
    "Optimal temperature for living space",                              //[3] TempTipOver20

    "Build plant humidifier to increase wellbeing",    //[4] HumidityTip0to40nr1
    "Plant plants that thrive in these circumstances", //[5] HumidityTip0to40nr2
    "Plant plants that thrive in these circumstances", //[6] HumidityTip40To50
    "Optimal for both people and plants",              //[7] HumidityTip50To60
    "",                                                //[8] HumidityTipOver60

    "Water plants out of water butt",               //[9] RainTip0
    "Get your laundry inside",                      //[10] RainTipOver0nr1
    "Close windows and doors of living area",       //[11] RainTipOver0nr2
    "How about using rain as water source?",        //[12] RainTipOver0nr3
    "Houseplants might get damaged if put outside", //[13] RainTipOver1000nr1
    "Put up water butt",                            //[14] RainTipOver1000nr2
    "Get your laundry inside fast",                 //[15] RainTipOver1000nr3
    "Close windows and doors of living area",       //[16] RainTipOver1000nr4

    "Get outside to collect some vitamin D", //[17] LightTipNr1
    "How about installing solar panels?",    //[18] LightTipNr2

    "Try to wear a cap if you leave the house!", //[19] WindspeedTipOver15kmH

};

const char *const sensorName[] PROGMEM = {
    "Temperature",
    "Humidity",
    "Earth Humidity",
    "Rain",
    "Pressure",
    "Light Intensity",
    "PM Sensor",
    "Gas Sensor",
    "Wind"};

LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned short site = 0;

void printSensorName(String text)
{
  lcd.clear();
  //printTime();
  lcd.setCursor(0, 0);
  lcd.print(text.c_str());
}
void printValue(String prefixIn, float value, String unitIn, int lineIndex, bool hasTip)
{
  const char *prefix = prefixIn.c_str();
  const char *unit = unitIn.c_str();
  lcd.setCursor(1, lineIndex);
  if (prefix != "")
  {
    lcd.print(prefix);
  }
  lcd.print(value);
  lcd.print(unit);
  if (!hasTip)
  {
    delay(usualDelay);
  }
}
void printTip(String sin, int lineIndex)
{
  const char *s = sin.c_str();
  if (strlen(s) <= 20)
  {
    lcd.setCursor(0, lineIndex);
    lcd.print(s);
    return;
  }
  for (int i = 0; i < strlen(s) - 19; i++)
  {

    String toPrint = "";

    for (int c = 0; c < 20; c++)
    {
      toPrint += s[i + c];
    }

    lcd.setCursor(0, lineIndex);
    lcd.print(toPrint.c_str());
    if (i == 0)
      delay(600);
    delay(600);
  }
  delay(usualDelay / 4);
}

String tempTip()
{
  if (values[0] < 15)
  {
    if (random(1))
    {
      return tip[0];
    }
    else
    {
      return tip[1];
    }
  }
  else if (values[0] < 20)
  { //16 - 20
    return tip[2];
  }
  else
  { //>20
    return tip[3];
  }
}

String humidityTip()
{
  if (values[1] < 40)
  { //-40
    if (random(1))
    {
      return tip[4];
    }
    else
    {
      return tip[5];
    }
  }
  else if (values[1] < 50)
  { //40-50
    return tip[6];
  }
  else if (values[1] < 60)
  { //50 - 60
    return tip[7];
  }
  else
  { //>60
    return tip[8];
  }
}

String rainTip()
{
  if (values[3] < 15)
  {
    return tip[9];
  }
  else if (values[3] < 900)
  {
    if (random(1))
    {
      return tip[10];
    }
    else
    {
      if (random(1))
      {
        return tip[11];
      }
      else
      {
        return tip[12];
      }
    }
  }
  else
  {
    if (random(1))
    {
      if (random(1))
      {
        return tip[13];
      }
      else
      {
        return tip[14];
      }
    }
    else
    {
      if (random(1))
      {
        return tip[15];
      }
      else
      {
        return tip[16];
      }
    }
  }
}

String lightTip()
{
  if (random(1))
  {
    return tip[17];
  }
  else
  {
    return tip[18];
  }
}

void siteInit()
{
  lcd.clear();
  printTip("Connecting to SAM", 0);
  for (int z = 0; z < 3; z++)
  {
    printTip(".    ", 1);
    delay(500);
    printTip("..   ", 1);
    delay(500);
    printTip("...  ", 1);
    delay(500);
    printTip(".... ", 1);
    delay(500);
    printTip(".....", 1);
    delay(500);
    printTip(".... ", 1);
    delay(500);
    printTip("...  ", 1);
    delay(500);
    printTip("..   ", 1);
    delay(500);
    printTip(".    ", 1);
    delay(500);
  }
}
void site1()
{ //Temperature
  printSensorName(sensorName[0]);
  printValue("", values[0], " C", 1, true);
  printValue("", values[0] * 5 / 9 + 32, " F", 2, true);
  printTip(tempTip().c_str(), 3);
}
void site2()
{ //Humidity
  printSensorName(sensorName[1]);
  printValue("", values[1], "%", 1, true);
  printTip(humidityTip().c_str(), 3);
}
void site3()
{ //Earth Hum
  printSensorName(sensorName[2]);
  printValue("", map(values[2], 0, 1023, 0, 100), "%", 1, false);
}
void site4()
{ //Rain
  printSensorName(sensorName[3]);
  printValue("", map(values[3], 0, 1023, 0, 100), "%", 1, true);
  printTip(rainTip().c_str(), 2);
}
void site5()
{ //Pressure
  printSensorName(sensorName[4]);
  printValue("", values[4], "HPa", 1, false);
}
void site6()
{ //Light Intensity
  printSensorName(sensorName[5]);
  printValue("", map(values[5], 0, 1023, 0, 10), "%", 1, true);
}
void site7()
{ //PM
  printSensorName(sensorName[6]);
  printValue("Size 2.5nm:", values[6], "ppm", 1, true);
  printValue("Size 10nm:", values[7], "ppm", 2, false);
}
void site8()
{ //Gas
  printSensorName(sensorName[7]);
  printValue("LPG: ", values[8], "ppm", 1, true);
  printValue("CO: ", values[9], "ppm", 2, true);
  printValue("Smoke: ", values[10], "ppm", 3, false);
}
void site9()
{ //wind
  printSensorName(sensorName[8]);
  printValue("Speed: ", values[11], "m/s", 2, false);
  String printing = "Direction: ";
  Serial.println(int(values[12]));
  switch (int(values[12]))
  {
  case 0:
    printing += "N";
  case 1:
    printing += "NE";
  case 2:
    printing += "E";
  case 3:
    printing += "SE";
  case 4:
    printing += "S";
  case 5:
    printing += "SW";
  case 6:
    printing += "W";
  case 7:
    printing += "NW";
  default:
    printing += "N/A";
  }
  lcd.setCursor(1, 3);
  lcd.print(printing.c_str());
}
void siteFailure()
{
  lcd.clear();
  printTip("Something went wrong", 0);
  printTip("Our pros are trying", 1);
  printTip("to fix this problem", 2);
  printTip("Props to MAI!", 3);
}

void displayChange()
{
  site++;
  if (site > 9)
  {
    site = 1;
  }
  switch (site)
  {
  case 1:
    site1();
    return;
  case 2:
    site2();
    return;
  case 3:
    site3();
    return;
  case 4:
    site4();
    return;
  case 5:
    site5();
    return;
  case 6:
    site6();
    return;
  case 7:
    site7();
    return;
  case 8:
    site8();
    return;
  case 9:
    site9();
    return;
  default:
    siteFailure();
    return;
  }
}

void setup2()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  siteInit();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Init begin");
  initRadio();
  initSD();
  startTime = millis();
  setup2();
  Serial.println("Init done");
}

void loop()
{
  receivedMessage();
  displayChange();
  delay(10);
}
