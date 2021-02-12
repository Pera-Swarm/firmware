#pragma once

#include <Arduino.h>
#include "../../src/SW_Infared.h"

// SW_Infared ir;

// This will enable or disable debug messages for IR communnication
#define IR_DEBUG 1

void beginInfared();
void parseRmtData(rmt_data_t* items, size_t len, uint8_t rId);
