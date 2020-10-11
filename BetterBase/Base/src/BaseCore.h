#pragma once

#define CONFIGURATION_H_VERSION 001000

#ifndef __BASE_FIRMWARE__
#define __BASE_FIRMWARE__
#endif

#include "inc/BaseConfig.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if ENABLED(LOW_POWER_MODE)
#include "features/LowPower.h"
#endif

#if ENABLED(SD_SUPPORT)
#include "features/sd/SDHelper.h"
#endif

#if ENABLED(RADIO_SUPPORT)
#include "features/radio/radio.h"
#endif

//#include <LowPower.h>