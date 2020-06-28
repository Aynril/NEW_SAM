#include <Arduino.h>
#include <config.h>

#ifdef I2C_LCD_SUPPORT
#include <lcd.h>
#endif

#ifdef ESP8266
extern "C"
{
#include "user_interface.h"
}

#ifdef NRF24_SUPPORT
os_timer_t RadioTimer; // Verwaltungsstruktur des Timers
#endif

#ifdef I2C_LCD_SUPPORT
os_timer_t LCDTimer;
#endif

#endif

unsigned short displayPage = 0;


#if defined(SD_SUPPORT) && defined(ARDUINO)
#include <SD.h>

#define SD_CS 8
File configFile;

const char *const FILE_NAME PROGMEM = "tips.csv";

bool sdFound = false;
#endif

#ifdef NRF24_SUPPORT
#include <nrf24.h>

#endif

#if defined(SD_SUPPORT) && defined(ARDUINO)
void initSD()
{
  if (!SD.begin(SD_CS))
  {
    while (1)
    {
      delay(500);
      Serial.println(F("SD not found"));
    }
    sdFound = false;
  }
  else
  {
    Serial.println(F("SD found"));
    sdFound = true;
  }

  if (SD.exists((char *)FILE_NAME))
  {
    Serial.println(F("File found"));
  }
  else
  {
    while (1)
    {
      delay(500);
      Serial.println(F("File not found"));
    }
  }
}
#endif

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

String tempTip()
{
  if (temperature < 15)
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
  else if (temperature < 20)
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
  if (airHumidity < 40)
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
  else if (airHumidity < 50)
  { //40-50
    return tip[6];
  }
  else if (airHumidity < 60)
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
  if (rain < 15)
  {
    return tip[9];
  }
  else if (rain < 900)
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

#ifdef I2C_LCD_SUPPORT
void siteInit()
{
  #ifdef NRF24_SUPPORT
  lcd.clear();
  printTip(F("Connecting to SAM"), 0);
  if (radioOK)
  {
    while (!firstBlood)
    {
      printTip(F(".    "), 1);
      delay(500);
      printTip(F("..   "), 1);
      delay(500);
      printTip(F("...  "), 1);
      delay(500);
      printTip(F(".... "), 1);
      delay(500);
      printTip(F("....."), 1);
      delay(500);
      printTip(F(".... "), 1);
      delay(500);
      printTip(F("...  "), 1);
      delay(500);
      printTip(F("..   "), 1);
      delay(500);
      printTip(F(".    "), 1);
      delay(500);
    }
  }
  else
  {
    printTip("Radio Failed", 1);
    delay(5000);
  }
  #endif
}
void site1()
{ //Temperature
  printSensorName(F("Temperature"));
  printValue("", temperature, F(" C"), 1);
  printValue("", temperature * 5 / 9 + 32, F(" F"), 2);
  printTip(tempTip().c_str(), 3);
}
void site2()
{ //Humidity
  printSensorName(F("Humidity"));
  printValue("", airHumidity, F("%"), 1);
  printTip(humidityTip().c_str(), 3);
}
void site3()
{ //Earth Hum
  printSensorName(F("Earth Humidity"));
  printValue("", map(earthHumidity, 0, 1023, 0, 100), F("%"), 1);
}
void site4()
{ //Rain
  printSensorName(F("Rain"));
  printValue("", map(rain, 0, 1023, 0, 100), F("%"), 1);
  printTip(rainTip().c_str(), 2);
}
void site5()
{ //Pressure
  printSensorName(F("Rain"));
  printValue("", pressure, F("HPa"), 1);
}
void site6()
{ //Light Intensity
  printSensorName(F("Light Intensity"));
  printValue("", map(light, 0, 1023, 0, 10), F("%"), 1);
}
void site7()
{ //PM
  printSensorName(F("PM Sensor"));
  printValue(F("Size 2.5nm:"), p25, F("ppm"), 1);
  printValue(F("Size 10nm:"), p10, F("ppm"), 2);
}
void site8()
{ //Gas
  printSensorName(F("Gasses"));
  printValue(F("LPG: "), lpg, F("ppm"), 1);
  printValue(F("Methane: "), methane, F("ppm"), 2);
  printValue(F("Smoke: "), smoke, F("ppm"), 3);
}
void site9()
{ //wind
  printSensorName(F("Wind"));
  printValue(F("Speed: "), windspeed, F("m/s"), 2);
  String printing = F("Direction: ");
  Serial.println(winddirection);
  switch (winddirection)
  {
  case 0:
    printing += F("N");
  case 1:
    printing += F("NE");
  case 2:
    printing += F("E");
  case 3:
    printing += F("SE");
  case 4:
    printing += F("S");
  case 5:
    printing += F("SW");
  case 6:
    printing += F("W");
  case 7:
    printing += F("NW");
  default:
    printing += F("N/A");
  }
  lcd.setCursor(1, 3);
  lcd.print(printing.c_str());
}
void siteFailure()
{
  lcd.clear();
  printTip(F("Something went wrong"), 0);
  printTip(F("Our pros are trying"), 1);
  printTip(F("to fix this problem"), 2);
  printTip(F("Props to MAI!"), 3);
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

void initLCD()
{
  lcd.init();
  lcd.backlight();
  siteInit();
}
#endif

#ifdef ESP8266
#ifdef NRF24_SUPPORT
void radioCallback(void *pArg)
{
  receivedMessage();
}
#endif

#ifdef I2C_LCD_SUPPORT
void lcdCallback(void *pArg)
{
  #ifdef NRF24_SUPPORT
  if (!firstBlood || !radioOK)
  {
    return;
  }
  
  #endif
  displayChange();
}
#endif

void espSetup()
{
#ifdef NRF24_SUPPORT
  os_timer_setfn(&RadioTimer, radioCallback, NULL);
  os_timer_arm(&RadioTimer, 500, true);
#endif
#ifdef I2C_LCD_SUPPORT
  os_timer_setfn(&LCDTimer, lcdCallback, NULL);
  os_timer_arm(&LCDTimer, usualDelay, true);
#endif
}
#endif

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Init begin"));
#ifdef ESP8266
  espSetup();
#endif
#ifdef NRF24_SUPPORT
  initRadio();
#endif

#if defined(SD_SUPPORT) && defined(__AVR__)
  initSD();
#endif

#ifdef I2C_LCD_SUPPORT
  initLCD();
#endif

  Serial.println(F("Init done"));
}

void loop()
{
#if defined(NRF24_SUPPORT) && not defined(INTERRUPT_ENABLED) && not defined(ESP8266)
  receivedMessage();
#endif

  delay(10);
}
