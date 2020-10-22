
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned short site = 0;

char *currentTip = (char *)String("Loading..").c_str();
volatile int currentTipIndex = 0;
volatile int currentLineIndex = 0;
volatile bool youMayContinue = true;

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
  currentTip = (char *)s;
  currentLineIndex = lineIndex;
  currentTipIndex = 0;

  if (strlen(s) <= 20)
  {
    lcd.setCursor(0, lineIndex);
    lcd.print(s);
    youMayContinue = true;
    return;
  }
  else
  {
    //lcd.autoscroll();
    lcd.setCursor(0, lineIndex);
    String toPrint = "";
    for (int c = 0; c < 20; c++)
    {
      toPrint += currentTip[currentTipIndex + c];
    }
    lcd.print(toPrint.c_str());
    youMayContinue = false;
  }
}

void shiftIfNeeded()
{
  if (strlen(currentTip) <= 20)
  {
    youMayContinue = true;
    return;
  }
  if (currentTipIndex <= 1)
  {
    currentTipIndex++;
    lcd.setCursor(0, currentLineIndex);
    String toPrint = "";
    for (int c = 0; c < 20; c++)
    {
      toPrint += currentTip[c];
    }
    lcd.print(toPrint.c_str());
    return;
  }

  if (currentTipIndex >= (int)strlen(currentTip) - 19)
  {
    youMayContinue = true;
    currentTipIndex = 0;
    return;
  }

  String toPrint = "";

  for (int c = 0; c < 20; c++)
  {
    toPrint += currentTip[currentTipIndex + c];
  }

  lcd.setCursor(0, currentLineIndex);
  lcd.print(toPrint.c_str());
  currentTipIndex++;
}