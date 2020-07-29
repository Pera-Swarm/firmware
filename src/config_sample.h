
// This is the configuration file for the robot
// Rename this file as 'config.h' before use

// ---------------------------------------------------------- MQTT Communication
#ifdef ENABLE_MQTT

#define MQTT_SERVER "xx.xx.xx.xx"
#define MQTT_PORT 1883
#define MQTT_CLIENT "Robot"
#define MQTT_USERNAME "mqtt_username"
#define MQTT_PASSWORD "mqtt_password"

#endif

// ------------------------------------------------------------- WiFi Client API
#ifdef ENABLE_WIFI_CLIENT

#define HOST "http://server.com/";
#define PORT 8081

#endif

// ------------------------------------------------------------ WiFi Credentials
#define WIFI_SSID "wifi_SSID"
#define WIFI_PASS "wifi_password"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
