# 1 "C:\\Users\\Nuwan\\AppData\\Local\\Temp\\tmpfyc__vz8"
#include <Arduino.h>
# 1 "E:/University/Semester 7/CO421 - Final Year Project I/FYP_GitRepos/pera-swarm-firmware/firmware/setup.ino"

#include "features.h"
#include "define.h"
#include "core.h"
#include "config/config.h"

uint8_t mode;
uint8_t buttonStatus;
int ROBOT_ID;
void setup();
void beginMemory();
void i2c_scan();
int modeController();
void loopModes();
void loop();
void mode_sensorTest();
void mode_ramdomMotion();
#line 13 "E:/University/Semester 7/CO421 - Final Year Project I/FYP_GitRepos/pera-swarm-firmware/firmware/setup.ino"
void setup() {

    Serial.begin(115200);


    beginMemory();





    gpio.begin();
    motors.begin();
    motors.enableEncoders();

    beginNeoPixel();


    beginDisplay();



    distance.begin();
    colorSensor.begin();
    compass.begin();


    beginWiFi();
    beginMQTT();







    pixelOff();
    gpio.blinkLED(3, 500);



    Serial.printf("\nRobot_%d > Setup Completed!\n\n", memory.getRobotId());
}





void beginMemory() {
    memory.begin();

    if(memory.getMemoryStatus()){

        #ifdef ENABLE_MOTORS
        motors.rightCorrection = memory.getErrorCorrection(RIGHT);
        motors.leftCorrection = memory.getErrorCorrection(LEFT);
        #endif

        ROBOT_ID = memory.getRobotId();
    }else{


        memory.setErrorCorrection(LEFT, 0);
        memory.setErrorCorrection(RIGHT, 0);
        Serial.println("WARNING!\nPlease configure this microcontroller with configurations before use.\n\n");
    }
}


void i2c_scan(){
    Serial.println ("I2C scanner. Scanning ...");
    byte count = 0;

    Wire.begin();
    Serial.println ("...");
    for (byte i = 8; i < 120; i++)
    {

        Wire.beginTransmission (i);
        if (Wire.endTransmission () == 0)
        {
            Serial.print ("Found address: ");
            Serial.print (i, DEC);
            Serial.print (" (0x");
            Serial.print (i, HEX);
            Serial.println (")");
            count++;
            delay (1);
        }
    }

    Serial.println ("Done.");
    Serial.print ("Found ");
    Serial.print (count, DEC);
    Serial.println (" device(s).");
}
# 1 "E:/University/Semester 7/CO421 - Final Year Project I/FYP_GitRepos/pera-swarm-firmware/firmware/loop.ino"
#include "algorithms/algorithm.h"


int modeController(){
    buttonStatus = gpio.readModeButton();



    if(buttonStatus==1) return mode= MODE1;
    if(buttonStatus==2) return mode= MODE2;
    if(buttonStatus==3) return mode= MODE3;
    return BEGIN;
}


void loopModes() {

    switch (mode) {
        case BEGIN:
        mode=modeController();
        delay(200);

        break;


        case MODE1:
        algorithm_loop();

        break;


        case MODE2:

        distance.test();




        delay(500);



        break;


        case MODE3:

        motors.write(200,200);
        delay(1000);


        break;


        case WAIT:
        delay(100);
        break;
    }

}

void loop(){


    loopModes();


    #ifdef ENABLE_OTA_UPLOAD
    ArduinoOTA.handle();
    #endif


    #ifdef ENABLE_WIFI_MONITOR
    wifiMonitor.handleClient();
    #endif

    #ifdef ESP_NOW_MASTER
    scanForSlave();
    #endif

    #ifdef ENABLE_MQTT
    mqtt_handle();
    #endif

    delay(100);
}


void mode_sensorTest(){
    distance.test();
    compass.test();
    colorSensor.test();

}

void mode_ramdomMotion(){
# 121 "E:/University/Semester 7/CO421 - Final Year Project I/FYP_GitRepos/pera-swarm-firmware/firmware/loop.ino"
}