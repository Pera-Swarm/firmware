#pragma once

#include "features.h"
#include "mqtt/mqtt.h"
#include "algorithms/algorithm.h"

void mqtt_robot_live();
void mqtt_robot_msg(char *msg, int *value);
