
#define ROBOT_V4
#define SERIAL_NUMBER "SW00001"
#define FIRMWARE_VERSION "2.0.0"
#define FIRMWARE_AURTHOR "UoP_Comp"

#include "Arduino.h"
#include <Wire.h>
#include "./src/pins.h"

enum color {COLOR_NONE, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
enum {BEGIN, STOP, TEST, START, IDEAL,  MODE1, MODE2, MODE3, MODE4, MODE5, MODE6, MODE7};

// This will enable or disable debug messages for IR communnication
#define IR_DEBUG 1

uint8_t mode = BEGIN;
uint8_t buttonStatus = 0;
uint8_t ROBOT_ID = 0;

// Local Server's IP address
String host = "";

// EEPROM ---------------------------------------------------------------
#include "src/SW_Memory.h"
SW_Memory memory;

// NeoPixel Ring ---------------------------------------------------------------
#ifdef ENABLE_NEOPIXEL_RING
#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXEL_LED    23
#define NEOPIXEL_LED_COUNT  20
#define NEOPIXEL_BRIGHTNESS 50
#define DELAYVAL 150

Adafruit_NeoPixel strip(NEOPIXEL_LED_COUNT, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

#endif

// Motor Section ---------------------------------------------------------------
#include "src/SW_Motors.h"
SW_Motors motors;

// SharpIR Distance Sensor Section ---------------------------------------------
#include "src/SW_Distance.h"
SW_Distance distance;

// Compass Section -------------------------------------------------------------
#include "src/SW_LSM303.h"
SW_LSM303::vector<int16_t> running_min = {-524,   -547,   -407};
SW_LSM303::vector<int16_t> running_max = { +328,   +327,   +282};
SW_LSM303 compass(running_min, running_max);

// GPIO and I2C Port Expander Section ------------------------------------------
#include "src/SW_GPIO.h"
SW_GPIO gpio;

// Color Sensor Section --------------------------------------------------------
#include "src/SW_TCS34725.h"
SW_TCS34725 colorSensor = SW_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_4X);

// Infared Section -------------------------------------------------------------
#include "src/SW_Infared.h"
#define IR_DEBUG 1   // This will enable or disable debug messages for IR communnication
SW_Infared ir;



// OTA Upload -----------------------------------------------------------------------------

#ifdef ENABLE_OTA_UPLOAD

#include<string>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#endif

// WiFi Communication API ------------------------------------------------------

#ifdef ENABLE_WIFI_MONITOR
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiMulti.h>

// For HTTP Server
WebServer wifiMonitor(80);
char tempString[128];       // Helps to build the reply strings

#endif

#ifdef ENABLE_WIFI_CLIENT

#include <HTTPClient.h>
#define HOST "http://68.183.188.135/";
#define PORT 8081

#endif


#define WIFI_NETWORK_2
// Network 1 : NotConnected
// Network 2 : Techyon Lab
// Network 3 : Eng-Student
// Network 4 : Pasan-WiFi

// List of available WiFi Networks


#if defined(WIFI_NETWORK_1)       // Network 1 --------------
#define WIFI_SSID "Eng-Student"
#define WIFI_PASS "3nG5tuDt"

#elif defined(WIFI_NETWORK_2)       // Network 2 --------------
#define WIFI_SSID "Techyon Lab"
#define WIFI_PASS "iot@ceykod"

#elif defined(WIFI_NETWORK_3)         // Network 3 --------------
#define WIFI_SSID "NotConnected"
#define WIFI_PASS "iot@ceykod2"

#elif defined(WIFI_NETWORK_4)       // Network 4 --------------
#define WIFI_SSID "Pasan-Wifi"
#define WIFI_PASS "password"

#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
