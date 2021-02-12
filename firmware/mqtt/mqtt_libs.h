#include <Arduino.h>

#pragma once

// --------------------------------------------------------------
#ifdef ENABLE_MQTT
#include <WiFi.h>
#include <WiFiMulti.h>

#include <PubSubClient.h>
#include "./mqtt/def_mqtt.h"

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

uint8_t dist_lock = 0;
uint16_t dist_virt=0;


#define MQTT_WAIT_TIMEOUT 10000

#endif
// --------------------------------------------------------------

void beginMQTT();

void mqtt_wait(uint8_t *lock);
void enter_critical();
void exit_critical();

void subscribeDefault();

void mqtt_subscribe(char* str);
int mqtt_publish(char* str1, char* str2, boolean retained);
void mqtt_onMessageArrived(char* topic, byte* message, unsigned int length);

void mqtt_handle();
void reconnect() ;

void mqtt_robot_live();
void mqtt_robot_msg(char* msg, int* value);

int distance_read();

void mqtt_comm_out(char* msg);
