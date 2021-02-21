#include "algorithms/algorithm.h"


int modeController(){
    buttonStatus = gpio.readModeButton();

    //printf("MODE %d\n", buttonStatus);

    if(buttonStatus==1) return mode= MODE1;
    if(buttonStatus==2) return mode= MODE2;
    if(buttonStatus==3) return mode= MODE3;
    return  BEGIN;
}


void loopModes() {

    switch (mode) {
        case BEGIN:
        mode=modeController();
        delay(200);

        break;

        // Mode 1 --------------------------------------------------------------
        case MODE1:
        algorithm_loop();

        break;

        // Mode 2 --------------------------------------------------------------
        case MODE2:
        // What should happen on mode 2
        // distance.test();
        // mqtt_comm_out_direct("Hello", 100);
        // motors.test();
        // Color c;
        // color_read(&c);
        // delay(5000);
        delay(2500);

        // mode = WAIT;

        break;

        // Mode 3 --------------------------------------------------------------
        case MODE3:
        // What should happen on mode 3
        motors.write(200,200);
        delay(1000);
        //mode = BEGIN;

        break;

        // Mode WAIT --------------------------------------------------------------
        case WAIT:
        delay(100);
        break;
    }

}

void loop(){

    // Main loop, which runs algorithms
    loopModes();

    // Handle OTA requests, if it is enabled
    #ifdef ENABLE_OTA_UPLOAD
    ArduinoOTA.handle();
    #endif

    // WiFi Monitor : Only support on MODE_0
    #ifdef ENABLE_WIFI_MONITOR
    wifiMonitor.handleClient();
    #endif

    #ifdef ESP_NOW_MASTER
    scanForSlave();
    #endif

    #ifdef ENABLE_MQTT
    // printf("handle mqtt\n");
    mqtt_handle();
    #endif

    delay(100);
}


void mode_sensorTest(){
    distance.test();
    compass.test();
    colorSensor.test();
    //ir.sendTestSignal();
}

void mode_ramdomMotion(){
    // Random move mode

    // motors.write(200, 200);
    // while (1) {
    //     int d = distance.getDistanceInt();
    //     Serial.println(d);
    //
    //     if (d < 12) {
    //         int timeout = random(-1500, 1500);
    //
    //         if (timeout < 0) {
    //             Serial.println("CCW");
    //             motors.write(-80, 80);
    //             delay((-1 * timeout) + 500);
    //         } else {
    //             Serial.println("CW");
    //             motors.write(80, -80);
    //             delay(timeout + 500);
    //         }
    //         motors.write(200, 200);
    //         delay(10);
    //     }
    // }

}
