#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "features.h"
#include "config/pins.h"

#define NEOPIXEL_LED_COUNT 21
#define NEOPIXEL_BRIGHTNESS 50 // [0:255]
#define DELAYVAL 150

extern Adafruit_NeoPixel neopixel;

void beginNeoPixel();

void pixelUpdateLED(uint8_t i, int r, int g, int b);
void neoPixelTest();

void pixelShowColor(int r, int g, int b);
void pixelColorWave(int r, int g, int b);
void colorWipe(uint32_t color, int wait);
void pixelOff();

// Functions written for testing the performance of the LEDs
void whiteOverRainbow(int whiteSpeed, int whiteLength);
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops);
