

#include "algorithm.h"

#include "modules/neopixel/neopixel.h"
#include "config/global_variables.h"
#include "mqtt/mqtt.h"

#ifdef ALGO_COLOR_RIPPLE
// -----------------------------------------------------------------------------

int robotState = ROBOT_BEGIN;

int currentHopId = -1;
boolean colorUpdated = false;


void algorithm_setup(){
    Serial.println("algorithm: setup");
}

void algorithm_loop(){

    if(robotState==ROBOT_RUN){
        //algorithm_execute();
        delay(50);

    }else if(robotState==ROBOT_BEGIN){
        algorithm_setup();
        robotState = ROBOT_WAIT;

    }else{
        // wait
        delay(100);
    }
}

void algorithm_interrupt(robot_interrupt_t interrupt, char* msg){
    if(interrupt == INT_COMM_IN){
        Serial.printf("algorithm: execute %s \n", msg);

        int hopId=0, hopR=0, hopG=0, hopB=0;
        int formatLen = sscanf(msg, "%d %d %d %d", &hopId, &hopR, &hopG, &hopB);

        if(formatLen==4){
            printf("algorithm_exec updated: %d len:%d \n", (colorUpdated==1) ? 1 : 0, formatLen);

            if (colorUpdated) {
                // a reverse message, don't forward
                pixelShowColor(0,0,0);

            } else {
                if (hopId > currentHopId) {

                    pixelShowColor(hopR,hopG,hopB);

                    currentHopId = hopId;
                    colorUpdated = true;

                    delay(2000);

                    // Send it to the next robot
                    // sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_COMM_OUT_SIMPLE);
                    sprintf(tempString2, "%d %d %d %d",hopId+1, hopR, hopG, hopB);
                    mqtt_comm_out_simple(tempString2);
                }
            }
        }else{
            Serial.printf("invalid msg format %d \n", formatLen);
        }
    }
}

// functional part of the pattern
void algorithm_execute(){
    delay(50);
}

// instruct to start the pattern
void algorithm_start(){
    robotState = ROBOT_RUN;
    Serial.println("algorithm: start");
    currentHopId = -1;
    colorUpdated = false;
    pixelShowColor(0,0,0);
}

// reset the pattern variables and state
void algorithm_reset(){
    robotState = ROBOT_BEGIN;
    currentHopId = -1;
    colorUpdated = false;
    pixelShowColor(0,0,0);
    Serial.println("algorithm: reset");
}

// stop the execution of the pattern
void algorithm_stop(){
    robotState = ROBOT_WAIT;
    Serial.println("algorithm: wait");
}


#endif
