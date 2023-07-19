#include "mqtt_comm.h"

#ifdef ENABLE_MQTT

// simple communication
void mqtt_comm_out_simple(String msg)
{
    sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_COMM_OUT_SIMPLE);
    sprintf(tempString2, "{\"id\": %d,\"msg\":\"%s\"}", mqtt_robot_id, msg.c_str());
    mqtt_publish(tempString1, tempString2, false);
}

void mqtt_comm_out_simple(String msg, int dist)
{
    mqtt_comm_out_simple(msg.c_str(), dist);
}

void mqtt_comm_out_simple(char *msg, int dist)
{ // Can specify the booadcast distance
    sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_COMM_OUT_SIMPLE);
    sprintf(tempString2, "{\"id\": %d,\"msg\":\"%s\",\"dist\":\"%d\"}", mqtt_robot_id, msg, dist);
    mqtt_publish(tempString1, tempString2, false);
}

// direct communication
void mqtt_comm_out_direct(String msg)
{
    sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_COMM_OUT_SIMPLE);
    sprintf(tempString2, "{\"id\": %d,\"msg\":\"%s\"}", mqtt_robot_id, msg.c_str());
    mqtt_publish(tempString1, tempString2, false);
}

void mqtt_comm_out_direct(String msg, int dist)
{
    mqtt_comm_out_direct(msg.c_str(), dist);
}

void mqtt_comm_out_direct(char *msg, int dist)
{ // Can specify the booadcast distance
    sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_COMM_OUT_SIMPLE);
    sprintf(tempString2, "{\"id\": %d,\"msg\":\"%s\",\"dist\":\"%d\"}", mqtt_robot_id, msg, dist);
    mqtt_publish(tempString1, tempString2, false);
}
#endif
