
#include "mqtt.h"

#ifdef ENABLE_MQTT

void mqtt_comm_out(char* msg){
    sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_COMM_OUT_SIMPLE);
    sprintf(tempString2, "{\"id\": %d,\"msg\":\"%s\"}", ROBOT_ID, msg);
    mqtt_publish(tempString1, tempString2, false);
}

#endif
