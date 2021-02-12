#pragma once

#define ROBOT_V4
#define SERIAL_NUMBER "SW00001"
#define FIRMWARE_VERSION "2.0.0"
#define FIRMWARE_AURTHOR "UoP_Comp"

enum color {COLOR_NONE, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
enum {BEGIN, STOP, TEST, START, IDEAL, WAIT,MODE1, MODE2, MODE3, MODE4, MODE5, MODE6, MODE7};

uint8_t mode = BEGIN;
uint8_t buttonStatus = 0;
uint8_t ROBOT_ID = 0;

char tempString1[255];       // Helps to build strings
char tempString2[255];       // Helps to build strings

// Local Server's IP address
String host = "";

// ---------------------------------------------------------------------- EEPROM
#include "src/SW_Memory.h"
SW_Memory memory;

// --------------------------------------------------------------- NeoPixel Ring
#ifdef ENABLE_NEOPIXEL_RING
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_LED_COUNT  21
#define NEOPIXEL_BRIGHTNESS 50 // [0:255]
#define DELAYVAL 150

Adafruit_NeoPixel neopixel(NEOPIXEL_LED_COUNT, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

#endif

// --------------------------------------------------------------- Motor Section
#include "src/SW_Motors.h"
SW_Motors motors;

// ----------------------------------------------------- Distance Sensor Section
#include "src/SW_Distance.h"
SW_Distance distance;

// ------------------------------------------------------------- Compass Section
#include "src/SW_LSM303.h"
SW_LSM303::vector<int16_t> running_min = {-524,   -547,   -407};
SW_LSM303::vector<int16_t> running_max = { +328,   +327,   +282};
SW_LSM303 compass(running_min, running_max);

// ------------------------------------------ GPIO and I2C Port Expander Section
#include "src/SW_GPIO.h"
SW_GPIO gpio;

// -------------------------------------------------------- Color Sensor Section
#include "src/SW_TCS34725.h"
SW_TCS34725 colorSensor = SW_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);


#include "src/SW_Infared.h"
