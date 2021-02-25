#pragma once

#include <Arduino.h>

#ifdef ENABLE_MQTT
#include <WiFi.h>
#include <WiFiMulti.h>
#include <PubSubClient.h>

#include "config/global_variables.h"
#include "features.h"
#include "def_mqtt.h"
#include "mqtt_functions.h"
#include "mqtt_global_variables.h"

#include "mqtt/mqtt_color/mqtt_color.h"
#include "mqtt/mqtt_distance/mqtt_distance.h"
#include "mqtt/mqtt_proximity/mqtt_proximity.h"

#include "mqtt/mqtt_robot/mqtt_robot.h"
#include "mqtt/mqtt_neopixel/mqtt_neopixel.h"
#include "mqtt/mqtt_comm/mqtt_comm.h"

#endif
