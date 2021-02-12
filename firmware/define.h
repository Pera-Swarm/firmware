#pragma once

#include <Arduino.h>
#include "config/pins.h"

// Directions, do not remove
#define LEFT 4
#define RIGHT 6
#define FORWARD 8
#define BACKWARD 2

// Rotation Directions
#define CW 0
#define CCW 1

#define ROBOT_V4

enum color {COLOR_NONE, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
enum {BEGIN, STOP, TEST, START, IDEAL, WAIT,MODE1, MODE2, MODE3, MODE4, MODE5, MODE6, MODE7};

uint8_t mode = BEGIN;
uint8_t buttonStatus = 0;
uint8_t ROBOT_ID = 0;

char tempString1[255];       // Helps to build strings
char tempString2[255];       // Helps to build strings

// Local Server's IP address
// String host = "";

// ---------------------------------------------------------------------- EEPROM
#include "modules/memory/robot_memory.h"
SW_Memory memory;

// ----------------------------------------------------------- NeoPixel LED Ring
#include "./modules/neopixel/robot_neopixel.h"
Adafruit_NeoPixel neopixel(NEOPIXEL_LED_COUNT, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

// --------------------------------------------------------------- Motor Section
#include "modules/motors/robot_motors.h"
SW_Motors motors;

// ----------------------------------------------------- Distance Sensor Section
#include "sensors/distance/robot_distance.h"
SW_Distance distance;

// ------------------------------------------------------------- Compass Section
#include "sensors/compass/robot_lsm303.h"
SW_LSM303::vector<int16_t> running_min = {-524,   -547,   -407};
SW_LSM303::vector<int16_t> running_max = { +328,   +327,   +282};
SW_LSM303 compass(running_min, running_max);

// ------------------------------------------ GPIO and I2C Port Expander Section
#include "modules/gpio/robot_gpio.h"
SW_GPIO gpio;

// -------------------------------------------------------- Color Sensor Section
#include "sensors/color/robot_tcs34725.h"
SW_TCS34725 colorSensor = SW_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);

// ------------------------------------------------------------ IR Transreceiver
#include "sensors/infared/infared.h"
SW_Infared ir;
