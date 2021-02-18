#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiMulti.h>

WebServer wifiMonitor(80);

// Basic Functions -------------------------------------------------------------
void beginWiFiMonitor();
void handleRoot();
void handleNotFound();
void robotStatus();

// API Level Functions ---------------------------------------------------------
void getMag();
void getAccel();
void getDist();
void getColor();

void handleMotion();
void handleRotation();

void handlePixelLED();
void handleIR();

void handleEEPROM_Write();
void handleEEPROM_Read();
