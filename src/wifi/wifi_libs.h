#pragma once

// ---------------------------------------------------------------- WiFi Station
#ifdef ENABLE_WIFI
#include "station/robot_wifi.h"
#endif

// ------------------------------------------------------ WiFi Communication API
#ifdef ENABLE_WIFI_MONITOR
#include "monitor/robot_monitor.h"
#endif

// ------------------------------------------------------------- WiFi Client API
#ifdef ENABLE_WIFI_CLIENT
#include "client/tobot_http_client.h"
#endif

// ------------------------------------------------------------------ OTA Upload
#ifdef ENABLE_OTA_UPLOAD
#include "ota/robot_ota.h"
#endif

// ------------------------------------------------------------ ESP Now Protocol
#ifdef ENABLE_ESPNOW
#include "esp_now/robot_ota.h"
#endif
