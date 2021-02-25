#pragma once

#include "features.h"
#include "mqtt/def_mqtt.h"
#include "mqtt/mqtt_functions.h"

#include "sensors/distance/distance.h"
#include "config/global_variables.h"
#include "modules/display/display.h"

extern uint8_t dist_lock;
extern uint16_t dist_virt;

void mqtt_distance_handle(char* msg);
int distance_read();
