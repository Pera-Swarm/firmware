#pragma once

#include <Arduino.h>
#include "config/pins.h"
#include "config/global_variables.h"

#define ROBOT_V4

enum color {COLOR_NONE, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
enum {BEGIN=0, STOP, TEST, START, IDEAL, WAIT,MODE1, MODE2, MODE3, MODE4, MODE5, MODE6, MODE7};

// EEPROM
#include "modules/memory/memory.h"

// NeoPixel LED Ring
#include "modules/neopixel/neopixel.h"

// Motor Section
#include "modules/motors/motors.h"

// Distance Sensor Section
#include "sensors/distance/distance.h"

// Compass Section
#include "sensors/compass/compass.h"

// GPIO and I2C Port Expander Section
#include "modules/gpio/gpio.h"

// Color Sensor Section
#include "sensors/color/color.h"

// IR Transreceiver
#include "sensors/infared/infared.h"

// SSD1306 LCD display
#include "modules/display/display.h"
