#include "robot_wifi.h"
#include "modules/neopixel/neopixel.h"

#ifdef ENABLE_WIFI

void beginWiFi() {

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    uint startTime = millis();

    Serial.print(F(">> WiFi\t\t:enabled\n"));
    Serial.print(F("       Connecting"));

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        if ((millis() - startTime) > 20 * 1000) {
            // allow 20sec, if not connected, reset
            ESP.restart();
        }
    }
    Serial.print(F("\n       Connected !\n\n"));

    #ifdef NEOPIXEL_INDICATIONS
    pixelColorWave(50, 0, 50);  // purple
    #endif
}

#else

void beginWiFi() {
    Serial.print(F(">> WiFi\t\t:disabled"));
}

#endif
