#pragma once
#include "../../inc/BaseConfig.h"

#include <SPI.h>
#include <SD.h>

void initSDCard();
void saveData(String msg);
