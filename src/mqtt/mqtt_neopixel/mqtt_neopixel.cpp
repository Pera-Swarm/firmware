#include "mqtt_neopixel.h"

void mqtt_neopixel_handle(char *msg)
{
    int r = 0, g = 0, b = 0;
    sscanf(msg, "%d %d %d", &r, &g, &b);

    // validate
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    // Serial.printf("Neopixel: %d,%d,%d \n", r, g, b);
    pixelShowColor(r, g, b);
}
