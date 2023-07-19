#pragma once

#include "features.h"
#include "mqtt/def_mqtt.h"
#include "mqtt/mqtt_functions.h"

#include "sensors/color/color.h"
#include "config/global_variables.h"

typedef struct Color{
    uint8_t R,G,B,C;
} color_t;

extern uint8_t color_lock;
extern struct Color color_virt;

void mqtt_color_handle(char* msg);
void color_read(color_t* color);
