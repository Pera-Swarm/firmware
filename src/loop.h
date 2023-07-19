#pragma once

#include <Arduino.h>

#include "define.h"
#include "features.h"

#include "config/pins.h"
#include "mqtt/mqtt.h"
#include "algorithms/algorithm.h"

int modeController();
void loopModes();
void loop();
void mode_sensorTest();
