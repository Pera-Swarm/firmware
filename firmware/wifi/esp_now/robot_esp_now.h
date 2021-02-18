#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define NUMSLAVES 8
#define CHANNEL 1

#define PRINT_SCAN_RESULTS
//#define DELETE_BEFORE_PAIR

#define ESP_NOW_SLAVE
//#define ESP_NOW_MASTER

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

// -- Basic Functions ----------------------------------------------------------
void beginESPNow();
void setupESPNow(uint8_t mode);
void scanForSlave();
bool attachSlaves();
void deletePeer();

// -- Callback Functions -------------------------------------------------------
void esp_onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status);
void boradcast_Dataframe(espnow_message *outgoingMsg);
void espNowTest();

// -- Client Configuration -----------------------------------------------------
void configDeviceAP();

// -- Utility Functions --------------------------------------------------------
void print_MAC(const uint8_t *mac_addr);
void print_responseResult(esp_err_t addStatus);
