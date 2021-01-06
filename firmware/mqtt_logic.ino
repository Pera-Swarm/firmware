
void beginWiFi() {

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("");

    int startTime = millis();

    Serial.print("WiFi:");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        if ((millis() - startTime) > 20 * 1000) {
            // allow 20sec, if not connected, reset
            ESP.restart();
        }
    }
    Serial.println();
}


#ifdef ENABLE_MQTT

void beginMQTT(){

    // Need to setup WiFi before configure MQTT 
    beginWiFi();

    //Serial.printf("%s %d\n", , );
    client.setServer(MQTT_SERVER, MQTT_PORT);

    //Serial.printf("%s %d %s %s\n", , , , );
    client.connect(MQTT_CLIENT+ROBOT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    client.setCallback(mqtt_onMessageArrived);

    Serial.println(F(">> MQTT\t:enabled"));

    if (!client.connected()) reconnect();
    else subscribeDefault();

    // Say a live
    mqttPub_live();
}

void mqttPub_live(){
    sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_ROBOT_LIVE);
    sprintf(tempString2, "{\"id\":\"%d\",\"reality\":\"1\"}", ROBOT_ID);
    mqtt_publish(tempString1, tempString2, false);
}


void subscribeDefault(){
    //Serial.println("subscribing to default topics");

    // robot/msg/{robotId}
    sprintf(tempString1, "%s/robot/msg/%d", CHANNEL, ROBOT_ID);
    //Serial.println(tempString1);
    mqtt_subscribe(tempString1);

    // robot/msg/broadcast
    sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_ROBOT_BROADCAST);
    //Serial.println(tempString1);
    mqtt_subscribe(tempString1);


    /*
    // sensor/distance/{robotId}/?
    sprintf(tempString1, "%s/sensor/distance/%d/?", CHANNEL, ROBOT_ID);
    client.subscribe(tempString1);

    // sensor/distance/{robotId}
    sprintf(tempString1, "%s/sensor/distance/%d", CHANNEL, ROBOT_ID);
    client.subscribe(tempString1);

    // localization/{robotId}
    sprintf(tempString1, "%s/localization/%d", CHANNEL, ROBOT_ID);
    client.subscribe(tempString1);
    */


    // comm/in/{robotId}
    sprintf(tempString1, "%s/comm/in/%d", CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);

    /*
    // v1/robot/ota/{robotId}
    sprintf(tempString1, "%s/robot/ota/%d", CHANNEL, ROBOT_ID);
    mqtt_subscribe(tempString1);
    */
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
    Serial.println(F(">> MQTT\t:disabled"));
}
void mqttPublish(){}
void subscribe(){}
void callback(char* topic, byte* message, unsigned int length){}
void mqtt_handle(){}
void mqtt_connect(){}

#endif
