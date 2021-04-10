#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

void httpPrint(String input);
int httpGET(String URL, String parameters);
int httpPOST(String URL);
