
#ifdef ENABLE_MQTT

#define MQTT_WAIT_TIMEOUT 10000

void mqtt_wait(uint8_t *lock){
    long start_time = millis();
    enter_critical();

    // loop until reply or timeout
    while((*lock == 1) && (millis() - start_time) < MQTT_WAIT_TIMEOUT){
        mqtt_handle();
    }

    exit_critical();
}

// Whatever need to hold during mqtt_blocking call
void enter_critical(){
    //Serial.println(F("Entering to a critical section"));

    // stop moving
    motors.pause();
}

// Whatever need to resume after mqtt_blocking call
void exit_critical(){
    // start moving back
    motors.resume();

    //Serial.println(F("Exiting from a critical section"));
}


void beginMQTT(){

    // Need to setup WiFi before configure MQTT
    beginWiFi();

    //Serial.printf("%s %d\n", , );
    client.setServer(MQTT_SERVER, MQTT_PORT);

    //Serial.printf("%s %d %s %s\n", , , , );
    client.connect(MQTT_CLIENT+ROBOT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    client.setCallback(mqtt_onMessageArrived);

    Serial.println(F(">> MQTT\t\t:enabled"));

    if (!client.connected()) reconnect();
    else subscribeDefault();

    // Say a live
    mqtt_robot_live();
}

void subscribeDefault(){

    // ROBOT TOPICS ------------------------------------------------------------

    // robot/msg/{robotId}
    sprintf(tempString1, "%s/" TOPIC_ROBOT_MSG, CHANNEL, ROBOT_ID);
    //Serial.println(tempString1);
    mqtt_subscribe(tempString1);

    // robot/msg/broadcast
    sprintf(tempString1, "%s/" TOPIC_ROBOT_BROADCAST, CHANNEL);
    mqtt_subscribe(tempString1);



    // DISTANCE SENSOR ---------------------------------------------------------

    // sensor/distance/{robotId}/?
    sprintf(tempString1, "%s/" TOPIC_DISTANCE_REQ_FROM_SERVER, CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);

    // sensor/distance/{robotId}
    sprintf(tempString1, "%s/" TOPIC_DISTANCE_RESP_FROM_SERVER, CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);

    // localization/{robotId}
    sprintf(tempString1, "%s/" TOPIC_LOCALIZATION, CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);



    // COMMUNICATION -----------------------------------------------------------

    // comm/in/{robotId}
    sprintf(tempString1, "%s/" TOPIC_COMM_IN, CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);

    // comm/out/simple
    sprintf(tempString1, "%s/" TOPIC_COMM_OUT_SIMPLE, CHANNEL);
    mqtt_subscribe(tempString1);

    // comm/out/directional
    // sprintf(tempString1, "%s/" TOPIC_COMM_OUT_DIRECTIONAL, CHANNEL);
    // mqtt_subscribe(tempString1);

    // NEOPIXEL ----------------------------------------------------------------

    // output/neopixel/{robotId}
    sprintf(tempString1, "%s/" TOPIC_NEOPIXEL_IN, CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);

    // LOCALIZATION ------------------------------------------------------------

    // localization/{robotId}
    sprintf(tempString1, "%s/" TOPIC_LOCALIZATION, CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);



    // OTHER TOPICS ------------------------------------------------------------

    // v1/robot/ota/{robotId}
    sprintf(tempString1, "%s/" TOPIC_OTA, CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);

}

void mqtt_subscribe(char* str){
    Serial.printf("sub:\t %s\n", str);
    client.subscribe(str);
}

int mqtt_publish(char* str1, char* str2, boolean retained){
    int resp = client.publish(str1, str2, retained);
    Serial.printf("pub:\t %s > %s (resp: %d)\n", str1, str2, resp);
    return resp;
}


// This should be call frequently to check newly published messages
void mqtt_handle(){
    if (!client.connected()) reconnect();
    client.loop();
}

void reconnect() {
    //uint8_t reconnectCount = 0;

    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");

        // Attempt to connect
        if (client.connect(MQTT_CLIENT + ROBOT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("connected");
            subscribeDefault();

        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again");
            delay(250);
        }
    }

}

#else

void beginMQTT(){
    Serial.println(F(">> MQTT\t\t:disabled"));
}
void mqttPublish(){}
void subscribe(){}
void callback(char* topic, byte* message, unsigned int length){}
void mqtt_handle(){}
void mqtt_connect(){}

#endif
