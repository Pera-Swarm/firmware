#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "features.h"
#include "config/pins.h"

#define SCREEN_WIDTH 128 // width, in pixels
#define SCREEN_HEIGHT 64 // height, in pixels

extern Adafruit_SSD1306 display;

void beginDisplay();

void display_clear();
void display_print(uint8_t line, String string_array);
void display_print(uint8_t line, int number);

void display_drawTest();
void dispaly_drawTestChar();
