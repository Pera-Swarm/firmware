#pragma once

#include "features.h"
#include "mqtt/def_mqtt.h"
#include "mqtt/mqtt_functions.h"

#include "config/global_variables.h"

typedef struct Proximity{
   uint16_t Front, Right, Left, BackRight, BackLeft;
} proximity_t;

extern uint8_t proximity_lock;
extern struct Proximity proximity_virt;

void proximity_read(proximity_t* proximity);
void mqtt_proximity_handle(char* msg);
