
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#define usualDelay 5000       //default delay of side change

float values[] = {
  17,         //temp in °C
  37,         //hum in %
  530,        //earthHum in x/1023
  267,        //rain in x/1023
  1003,       //pressure in HPa
  200,        //lightIntensity in x/1023
  20,         //pm25 in ppm
  17,         //pm10 in ppm
  0,          //lpg in ppm
  0,          //co in ppm
  0,          //smoke in ppm
  5,          //windspeed in m/s
  2           //winddirection; 0 for North, 1 for NorthEast... 7 for NorthWest
};

PROGMEM const String tip[20] = {
  "Heat up living space",                                               //[0] TempTipBelow15nr1
  "Dress up properly for cold weather",                                 //[1] TempTipBelow15nr2
  "Heat living space but remember to shock-ventilate and close doors",  //[2] TempTip16to20
  "Optimal temperature for living space",                               //[3] TempTipOver20

  "Build plant humidifier to increase wellbeing",     //[4] HumidityTip0to40nr1
  "Plant plants that thrive in these circumstances",  //[5] HumidityTip0to40nr2
  "Plant plants that thrive in these circumstances",  //[6] HumidityTip40To50
  "Optimal for both people and plants",               //[7] HumidityTip50To60
  "",                                                 //[8] HumidityTipOver60

  "Water plants out of water butt",               //[9] RainTip0
  "Get your laundry inside",                      //[10] RainTipOver0nr1
  "Close windows and doors of living area",       //[11] RainTipOver0nr2
  "How about using rain as water source?",        //[12] RainTipOver0nr3
  "Houseplants might get damaged if put outside", //[13] RainTipOver1000nr1
  "Put up water butt",                            //[14] RainTipOver1000nr2
  "Get your laundry inside fast",                 //[15] RainTipOver1000nr3
  "Close windows and doors of living area",       //[16] RainTipOver1000nr4

  "Get outside to collect some vitamin D",        //[17] LightTipNr1
  "How about installing solar panels?",           //[18] LightTipNr2

  "Try to wear a cap if you leave the house!",    //[19] WindspeedTipOver15kmH

};

PROGMEM const String sensorName[] = {
  "Temperature",
  "Humidity",
  "Earth Humidity",
  "Rain",
  "Pressure",
  "Light Intensity",
  "PM Sensor",
  "Gas Sensor",
  "Wind"
};

LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned short site = 0;

void printSensorName(String text) {
  lcd.clear();
  //printTime();
  lcd.setCursor(0, 0);
  lcd.print(text.c_str());
}
void printValue(char* prefix, float value, char* unit, int lineIndex, bool hasTip) {
  lcd.setCursor(1, lineIndex);
  if (prefix != "") {
    lcd.print(prefix);
  }
  lcd.print(value);
  lcd.print(unit);
  if (!hasTip) {
    delay(usualDelay);
  }
}
void printTip(char* s, int lineIndex) {
  if (strlen(s) <= 20) {
    lcd.setCursor(0, lineIndex);
    lcd.print(s);
    return;
  }
  for (int i = 0; i < strlen(s) - 19; i++) {

    String toPrint = "";

    for (int c = 0; c < 20; c++) {
      toPrint += s[i + c];
    }

    lcd.setCursor(0, lineIndex);
    lcd.print(toPrint.c_str());
    if (i == 0) delay(600);
    delay(600);
  }
  delay(usualDelay / 4);
}

void siteInit() {
  lcd.clear();
  printTip("Connecting to SAM", 0);
  for (int z = 0; z < 3; z++) {
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
void site1() {//Temperature
  printSensorName(sensorName[0]);
  printValue("", values[0], " C", 1, true);
  printValue("", values[0] * 5 / 9 + 32, " F", 2, true);
  printTip(tempTip().c_str(), 3);
}
void site2() {//Humidity
  printSensorName(sensorName[1]);
  printValue("", values[1], "%", 1, true);
  printTip(humidityTip().c_str(), 3);
}
void site3() {//Earth Hum
  printSensorName(sensorName[2]);
  printValue("", map(values[2], 0, 1023, 0, 100), "%", 1, false);
}
void site4() {//Rain
  printSensorName(sensorName[3]);
  int lol = 0;
  printValue("", map(values[3], 0, 1023, 0, 100), "%", 1, true);
  printTip(rainTip().c_str(), 2);
}
void site5() {//Pressure
  printSensorName(sensorName[4]);
  printValue("", values[4], "HPa", 1, false);
}
void site6() {//Light Intensity
  printSensorName(sensorName[5]);
  printValue("", map(values[5], 0, 1023, 0, 10), "%", 1, true);
}
void site7() {//PM
  printSensorName(sensorName[6]);
  printValue("Size 2.5nm:", values[6], "ppm", 1, true);
  printValue("Size 10nm:", values[7], "ppm", 2, false);
}
void site8() {//Gas
  printSensorName(sensorName[7]);
  printValue("LPG: ", values[8], "ppm", 1, true);
  printValue("CO: ", values[9], "ppm", 2, true);
  printValue("Smoke: ", values[10], "ppm", 3, false);
}
void site9() {//wind
  printSensorName(sensorName[8]);
  printValue("Speed: ", values[11], "m/s", 2, false);
  String printing = "Direction: ";
  Serial.println(int(values[12]));
  switch (int(values[12])) {
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
void siteFailure() {
  lcd.clear();
  printTip("Something went wrong", 0);
  printTip("Our pros are trying", 1);
  printTip("to fix this problem", 2);
  printTip("Props to MAI!", 3);
}

void displayChange() {
  site++;
  if (site > 9) {
    site = 1;
  }
  switch (site) {
    case 1: site1();
      return;
    case 2: site2();
      return;
    case 3: site3();
      return;
    case 4: site4();
      return;
    case 5: site5();
      return;
    case 6: site6();
      return;
    case 7: site7();
      return;
    case 8: site8();
      return;
    case 9: site9();
      return;
    default: siteFailure();
      return;
  }
}

void setup2() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  siteInit();
}

String tempTip() {
  if (values[0] < 15) {
    if (random(1)) {
      return tip[0];
    }
    else {
      return tip[1];
    }
  }
  else if (values[0] < 20) { //16 - 20
    return tip[2];
  }
  else { //>20
    return tip[3];
  }
}

String humidityTip() {
  if (values[1] < 40) { //-40
    if (random(1)) {
      return tip[4];
    }
    else {
      return tip[5];
    }
  }
  else if (values[1] < 50) { //40-50
    return tip[6];
  }
  else if (values[1] < 60) { //50 - 60
    return tip[7];
  }
  else { //>60
    return tip[8];
  }
}

String rainTip() {
  if (values[3] < 15) {
    return tip[9];
  }
  else if (values[3] < 900) {
    if (random(1)) {
      return tip[10];
    }
    else {
      if (random(1)) {
        return tip[11];
      }
      else {
        return tip[12];
      }
    }
  }
  else {
    if (random(1)) {
      if (random(1)) {
        return tip[13];
      }
      else {
        return tip[14];
      }
    }
    else {
      if (random(1)) {
        return tip[15];
      }
      else {
        return tip[16];
      }
    }
  }
}

String lightTip() {
  if (random(1)) {
    return tip[17];
  }
  else {
    return tip[18];
  }
}
