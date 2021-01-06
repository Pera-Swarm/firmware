
#define ROBOT_V4
#define SERIAL_NUMBER "SW00001"
#define FIRMWARE_VERSION "2.0.0"
#define FIRMWARE_AURTHOR "UoP_Comp"

#include "Arduino.h"
#include <Wire.h>
#include "./src/def_pins.h"

enum color {COLOR_NONE, COLOR_RED, COLOR_GREEN, COLOR_BLUE};
enum {BEGIN, STOP, TEST, START, IDEAL, WAIT,MODE1, MODE2, MODE3, MODE4, MODE5, MODE6, MODE7};

// This will enable or disable debug messages for IR communnication
#define IR_DEBUG 1

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

#define PIN_NEOPIXEL_LED    23
#define NEOPIXEL_LED_COUNT  21
#define NEOPIXEL_BRIGHTNESS 50
#define DELAYVAL 150

Adafruit_NeoPixel strip(NEOPIXEL_LED_COUNT, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

#endif

// --------------------------------------------------------------- Motor Section
//#include "src/SW_Motors.h"
//SW_Motors motors;

// --------------------------------------------- SharpIR Distance Sensor Section
//#include "src/SW_Distance.h"
//SW_Distance distance;

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

// ------------------------------------------------------------- Infared Section
#include "src/SW_Infared.h"
SW_Infared ir;

// This will enable or disable debug messages for IR communnication
#define IR_DEBUG 1

// ---------------------------------------------------------- MQTT Communication
#ifdef ENABLE_MQTT

#include <WiFi.h>
#include <WiFiMulti.h>

#include <PubSubClient.h>
#include "src/def_mqtt.h"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

#endif

// ------------------------------------------------------------------ OTA Upload

#ifdef ENABLE_OTA_UPLOAD

#include<string>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#endif

// ------------------------------------------------------------ ESP Now Protocol

#ifdef ENABLE_ESPNOW

#include <WiFi.h>
#include <esp_now.h>

#define NUMSLAVES 8
#define CHANNEL 1

#define PRINT_SCAN_RESULTS
//#define DELETE_BEFORE_PAIR

esp_now_peer_info_t slaves[NUMSLAVES] = {};
int espSlaveCount = 0;

typedef struct espnow_message {
    uint8_t id;
    uint8_t command;
    uint8_t valueA;
    uint8_t valueB;
    uint8_t valueC;
} espnow_message;

espnow_message incomingMsg;
espnow_message outgoingMsg;

#define ESP_NOW_SLAVE
//#define ESP_NOW_MASTER

#endif

// ------------------------------------------------------ WiFi Communication API

#ifdef ENABLE_WIFI_MONITOR

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiMulti.h>

WebServer wifiMonitor(80);

#endif

// ------------------------------------------------------------- WiFi Client API

#ifdef ENABLE_WIFI_CLIENT

#include <WiFi.h>
#include <HTTPClient.h>

#endif
