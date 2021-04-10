#pragma once

#include "features.h"   // Enable or Diasable services
#include "define.h"     // Configurations for version 4 PCB
#include "core.h"
#include "config/config.h"   // Prepare this file before use

extern uint8_t mode;
extern int ROBOT_ID;

void setup();
void beginMemory();
void i2c_scan();
