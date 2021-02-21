#pragma once

// #include "mqtt.h"
#include "mqtt/mqtt_functions.h"

// simple communication
void mqtt_comm_out_simple(String msg);
void mqtt_comm_out_simple(char* msg, int dist);
void mqtt_comm_out_simple(String msg, int dist);

// direct communication
void mqtt_comm_out_direct(String msg);
void mqtt_comm_out_direct(char* msg, int dist);
void mqtt_comm_out_direct(String msg, int dist);
