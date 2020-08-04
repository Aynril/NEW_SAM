#pragma once

#include "../inc/BaseConfig.h"

#ifdef DEBUG_FEATURE
#define PRINT_DEBUG(x) Serial.print(x)
#define PRINT_DEBUG_DEC(x) Serial.print(x, DEC)
#define PRINT_DEBUG_HEX(x) Serial.print(x, HEX)
#define PRINT_DEBUG_LN(x) Serial.println(x)
#else
#define PRINT_DEBUG(x)
#define PRINT_DEBUG_DEC(x)
#define PRINT_DEBUG_HEX(x)
#define PRINT_DEBUG_LN(x)
#endif