#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "../core/macros.h"
#include "../../Configuration.h"
#include "../../PinoutConfiguration.h"

#include "SanityCheck.h"

#include "../core/serial.h"

#define RESET() digitalWrite(PINOUT_HARDWARE_RESET_PIN, LOW)