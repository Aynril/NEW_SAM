#include <Arduino.h>
#include <config.h>

#ifdef I2C_LCD_SUPPORT
#include <lcd.h>
#endif

#ifdef ESP8266

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h>

const String FirmwareVer = {"1.2"};
#define URL_fw_Version "https://raw.githubusercontent.com/Aynril/NEW_SAM/platformio/BetterDisplay/versions.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/Aynril/NEW_SAM/platformio/BetterDisplay/firmware.bin"

HTTPClient http;

void FirmwareUpdate()
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

  client->setInsecure();
  http.begin(*client, URL_fw_Version); // check version URL
  delay(100);
  int httpCode = http.GET(); // get data from version file
  delay(100);
  String payload;
  if (httpCode == HTTP_CODE_OK) // if version received
  {
    payload = http.getString(); // save received version
    Serial.println(payload);
  }
  else
  {
    Serial.print("error in downloading version file:");
    Serial.println(httpCode);
  }

  http.end();
  if (httpCode == HTTP_CODE_OK) // if version received
  {
    if (payload.equals(FirmwareVer))
    {
      Serial.println("Device already on latest firmware version");
    }
    else
    {
      Serial.println("New firmware detected");
      #ifdef I2C_LCD_SUPPORT
      printTip("Updating Firmware", 0);
      printTip("Cur:" + FirmwareVer + " to " + payload, 1);
      printTip("KEEP POWER ON", 2);
      #endif

      // The line below is optional. It can be used to blink the LED on the board during flashing
      // The LED will be on during download of one buffer of data from the network. The LED will
      // be off during writing that buffer to flash
      // On a good connection the LED should flash regularly. On a bad connection the LED will be
      // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
      // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
      ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

      t_httpUpdate_return ret = ESPhttpUpdate.update(*client, URL_fw_Bin, "");

      switch (ret)
      {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
      }
    }
  }
}

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
void wifiInitSite() {
  lcd.clear();
  printTip(F("Connecting to WiFi"), 0);
  printTip(F("Search MAI Mirror"), 1);
  printTip(F("in WiFi Settings"), 2);
}
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
  WiFiManager wifiManager;
  #ifdef I2C_LCD_SUPPORT
  wifiInitSite();
  #endif

  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);

  wifiManager.setTimeout(90);
  //reset settings - for testing
  //wifiManager.resetSettings();


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("MAI Mirror")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Serial.println("Connected to WiFi");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  FirmwareUpdate();
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
#ifdef I2C_LCD_SUPPORT
  initLCD();
#endif
#ifdef ESP8266
  espSetup();
#endif
#ifdef I2C_LCD_SUPPORT
  siteInit();
#endif
#ifdef NRF24_SUPPORT
  initRadio();
#endif

#if defined(SD_SUPPORT) && defined(__AVR__)
  initSD();
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
