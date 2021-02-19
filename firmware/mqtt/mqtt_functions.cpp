#include "mqtt_functions.h"

// Helps to build strings
char tempString1[255];
char tempString2[255];

int mqtt_robot_id;

WiFiClient espClient;
PubSubClient client(espClient);

#ifdef ENABLE_MQTT

void beginMQTT(){

    //Serial.printf("%s %d\n", , );
    client.setServer(MQTT_SERVER, MQTT_PORT);

    //Serial.printf("%s %d %s %s\n", , , , );
    client.connect(MQTT_CLIENT+ROBOT_ID, MQTT_USERNAME, MQTT_PASSWORD);
    client.setCallback(mqtt_onMessageArrived);

    Serial.println(F(">> MQTT\t\t:enabled"));

    if (!client.connected()) mqtt_reconnect();
    else subscribeDefault();

    mqtt_robot_id = memory.getRobotId();

    // Send a 'alive' message to the server
    mqtt_robot_live();
}

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

void subscribeDefault(){

    // ROBOT TOPICS ------------------------------------------------------------

    // robot/msg/{robotId}
    sprintf(tempString1, "%s/" TOPIC_ROBOT_MSG, CHANNEL, mqtt_robot_id);
    //Serial.println(tempString1);
    mqtt_subscribe(tempString1);

    // robot/msg/broadcast
    sprintf(tempString1, "%s/" TOPIC_ROBOT_BROADCAST, CHANNEL);
    mqtt_subscribe(tempString1);

    // DISTANCE SENSOR ---------------------------------------------------------

    // sensor/distance/{robotId}/?
    sprintf(tempString1, "%s/" TOPIC_DISTANCE_REQ_FROM_SERVER, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

    // sensor/distance/{robotId}
    sprintf(tempString1, "%s/" TOPIC_DISTANCE_RESP_FROM_SERVER, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

    // COLOR SENSOR ---------------------------------------------------------

    // sensor/color/{robotId}/?
    sprintf(tempString1, "%s/" TOPIC_COLOR_REQ_FROM_SERVER, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

    // sensor/color/{robotId}
    sprintf(tempString1, "%s/" TOPIC_COLOR_RESP_FROM_SERVER, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

    // COMMUNICATION -----------------------------------------------------------

    // comm/in/{robotId}
    sprintf(tempString1, "%s/" TOPIC_COMM_IN, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

    // comm/out/simple
    //sprintf(tempString1, "%s/" TOPIC_COMM_OUT_SIMPLE, CHANNEL);
    //mqtt_subscribe(tempString1);

    // comm/out/directional
    // sprintf(tempString1, "%s/" TOPIC_COMM_OUT_DIRECTIONAL, CHANNEL);
    // mqtt_subscribe(tempString1);

    // NEOPIXEL ----------------------------------------------------------------

    // output/neopixel/{robotId}
    sprintf(tempString1, "%s/" TOPIC_NEOPIXEL_IN, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

    // LOCALIZATION ------------------------------------------------------------

    // localization/{robotId}
    sprintf(tempString1, "%s/" TOPIC_LOCALIZATION, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

    // OTHER TOPICS ------------------------------------------------------------

    // v1/robot/ota/{robotId}
    sprintf(tempString1, "%s/" TOPIC_OTA, CHANNEL, mqtt_robot_id);
    mqtt_subscribe(tempString1);

}

void mqtt_subscribe(char* str){
    Serial.printf("       sub: %s\n", str);
    client.subscribe(str);
}

int mqtt_publish(char* str1, char* str2, boolean retained){
    int resp = client.publish(str1, str2, retained);
    Serial.printf("       pub: %s > %s (resp: %d)\n", str1, str2, resp);
    return resp;
}


// This should be call frequently to check newly published messages
void mqtt_handle(){
    if (!client.connected()) mqtt_reconnect();
    //Serial.println("*");
    client.loop();
}

void mqtt_reconnect() {
    uint8_t reconnectCount = 0;

    while (!client.connected() && reconnectCount < 10) {
        Serial.print("Attempting MQTT connection...");
        gpio.blinkLED(2, 200);

        // Attempt to connect
        if (client.connect(MQTT_CLIENT + mqtt_robot_id, MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("connected");
            subscribeDefault();

        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again");
            delay(500);
        }
        reconnectCount++;
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
void mqtt_reconnect(){}

#endif
