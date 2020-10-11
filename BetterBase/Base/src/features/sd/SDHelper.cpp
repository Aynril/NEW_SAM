#include "../../inc/BaseConfig.h"

#if ENABLED(SD_SUPPORT)
#include "SDHelper.h"

File logFile;
bool SDinitialized = false;

void initSDCard()
{
  PRINT_DEBUG_LN("Initializing SD card...");

  if (!SD.begin(PINOUT_SD_CS))
  {
    PRINT_DEBUG_LN("initialization failed!");
  }
  else
  {
    PRINT_DEBUG_LN("initialization done.");
    SDinitialized = true;
    logFile = SD.open("data.csv", FILE_WRITE);
    if (!logFile)
    {
      PRINT_DEBUG_LN("error opening data.csv");
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
      PRINT_DEBUG_LN("writing to file");
      logFile.println(msg);
      logFile.close();
      PRINT_DEBUG_LN("writing done");
    }
    else
    {
      PRINT_DEBUG_LN("error opening file");
    }
  }
}

#endif