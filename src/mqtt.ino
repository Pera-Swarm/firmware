
#ifdef ENABLE_MQTT

void beginMQTT(){
    client.setServer(MQTT_SERVER, MQTT_PORT);
    mqtt_connect();
    Serial.println(F(">> MQRR\t:enabled"));
}

void mqttPub_live(){

    sprintf(tempString, "{\"id\":\"%d\",\"reality\":\"1\"}", ROBOT_ID);
    int resp = client.publish(CHANNEL TOPIC_ROBOT_LIVE, tempString, true);

    if(resp==1){
        //successReport(SUCCESS_MQTT_PUBLISH);
    }else {
        //errorReport(ERROR_MQTT_PUBLISH_FAILED);}
    }

}

void subscribe(){
    //client.subscribe(TOPIC_MSG);
    //client.subscribe(TOPIC_OTA);
}

void subscribeDefault(){
    char topic_temp[64];

    // robot/msg/{robotId}
    sprintf(topic_temp, "%s/robot/msg/%d", CHANNEL, ROBOT_ID);
    client.subscribe(topic_temp);

    // robot/msg/broadcast
    client.subscribe(CHANNEL "/" TOPIC_ROBOT_BROADCAST);

    // sensor/distance/{robotId}/?
    sprintf(topic_temp, "%s/sensor/distance/%d/?", CHANNEL, ROBOT_ID);
    client.subscribe(topic_temp);

    // sensor/distance/{robotId}
    sprintf(topic_temp, "%s/sensor/distance/%d", CHANNEL, ROBOT_ID);
    client.subscribe(topic_temp);

    // localization/{robotId}
    sprintf(topic_temp, "%s/localization/%d", CHANNEL, ROBOT_ID);
    client.subscribe(topic_temp);

    // comm/in/{robotId}
    sprintf(topic_temp, "%s/comm/in//%d", CHANNEL, ROBOT_ID);
    client.subscribe(topic_temp);

    // v1/robot/ota/{robotId}
    sprintf(topic_temp, "%s/robot/ota/%d", CHANNEL, ROBOT_ID);
    client.subscribe(topic_temp);


}

void callback(char* topic, byte* message, unsigned int length) {
    char msg[length+1];
    msg[length] = '\0';

    for (int i = 0; i < length; i++) {
        msg[i] =  (char)message[i];
    }
    Serial.printf("\n>> topic: %s msg:%s\n", topic, msg);


    /*
    if (String(topic).equals(TOPIC_MSG)) {

    //scanf("%d %d", &x &y);

    //
    // Need to implement
    //
}else if (String(topic).equals(TOPIC_OTA)) {
Serial.printf("\n>> topic: %s msg:%s\n", topic, msg);
//
// Need to implement
//
}
*/
}

// This should be call frequently to check newly published messages
void mqtt_handle(){
    if (!client.connected()) mqtt_connect();
    client.loop();
}

void mqtt_connect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect(MQTT_CLIENT + ROBOT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            client.setCallback(callback);
            subscribeDefault();
            Serial.println("connected");
            //successReport(SUCCESS_MQTT_CONNECTED);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 3 seconds");
            //errorReport(ERROR_MQTT_RECONNECT_FAILED);

            // Wait 3 seconds before retrying
            delay(3000);
        }
    }
}

#else

void beginMQTT(){
    Serial.println(F(">> MQTT\t:disabled"));
}
void mqttPublish(){}
void subscribe(){}
void callback(char* topic, byte* message, unsigned int length){}
void mqtt_handle(){}
void mqtt_connect(){}

#endif
