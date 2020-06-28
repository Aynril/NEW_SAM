
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned short site = 0;

void printSensorName(String text)
{
  lcd.clear();
  //printTime();
  lcd.setCursor(0, 0);
  lcd.print(text.c_str());
}

void printValue(String prefixIn, float value, String unitIn, int lineIndex)
{
  const char *prefix = prefixIn.c_str();
  const char *unit = unitIn.c_str();
  lcd.setCursor(1, lineIndex);
  if (strlen(prefix) > 0)
  {
    lcd.print(prefix);
  }
  lcd.print(value);
  lcd.print(unit);
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
  for (uint8_t i = 0; i < strlen(s) - 19; i++)
  {

    String toPrint = "";

    for (int c = 0; c < 20; c++)
    {
      toPrint += s[i + c];
    }

    lcd.setCursor(0, lineIndex);
    lcd.print(toPrint.c_str());
    /*     if (i == 0)
      delay(600);
    delay(600); */
  }
}