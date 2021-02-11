#include "wifi_module.h"



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
