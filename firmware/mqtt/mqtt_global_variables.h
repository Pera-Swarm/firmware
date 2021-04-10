#pragma once

#include <Arduino.h>

#ifdef ENABLE_MQTT
#include <WiFi.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>

#define MQTT_WAIT_TIMEOUT 10000

extern WiFiClient espClient;
extern long lastMsg;
extern PubSubClient client;

#endif

extern int mqtt_robot_id;


// Helps to build strings
extern char tempString1[255];
extern char tempString2[255];
