
// This is the configuration file for the robot
// Fill all the required the parameters and 
// rename this file as 'config.h' before use

#ifndef _ROBOT_CONFIG_H
#define _ROBOT_CONFIG_H

// ---------------------------------------------------------- MQTT Communication
#ifdef ENABLE_MQTT

#define MQTT_SERVER "192.168.8.1"
#define MQTT_PORT 1883
#define MQTT_CLIENT "Robot"
#define MQTT_USERNAME "mqtt_username"
#define MQTT_PASSWORD "mqtt_password"

#endif

// ------------------------------------------------------------- WiFi Client API
#ifdef ENABLE_WIFI_CLIENT

#define HOST "http://192.168.8.1/";
#define PORT 8081

#endif

// ------------------------------------------------------------ WiFi Credentials
#define WIFI_SSID "wifi_SSID"
#define WIFI_PASS "wifi_password"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

#endif
