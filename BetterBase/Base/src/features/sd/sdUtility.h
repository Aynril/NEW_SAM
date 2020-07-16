#include "../../inc/BaseConfig.h"

#ifndef SD_UTILITY_H
#define SD_UTILITY_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

File logFile;

void initSDCard()
{
#ifdef DEBUG_FEATURE
  Serial.print("Initializing SD card...");
#endif

  if (!SD.begin(SDPin))
  {
#ifdef DEBUG_FEATURE
    Serial.println("initialization failed!");
#endif
  }
  else
  {
#ifdef DEBUG_FEATURE
    Serial.println("initialization done.");
#endif
    SDinitialized = true;
    logFile = SD.open("data.csv", FILE_WRITE);
    if (!logFile)
    {
#ifdef DEBUG_FEATURE
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
#ifdef DEBUG_FEATURE
      Serial.println("writing to file");
#endif
      logFile.println(msg);
      logFile.close();
#ifdef DEBUG_FEATURE
      Serial.println("writing done");
#endif
    }
    else
    {
#ifdef DEBUG_FEATURE
      Serial.println("error opening file");
#endif
    }
  }
}

#endif // !SD_UTILITY_H
