#pragma once

#define CONFIGURATION_H_VERSION 001000

#ifndef __BASE_FIRMWARE__
#define __BASE_FIRMWARE__
#endif

#include "inc/BaseConfig.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if ENABLED(LOW_POWER_FUNCTION_SUPPORTED)
#include "features/LowPower.h"
#endif