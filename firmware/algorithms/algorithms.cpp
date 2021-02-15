#include "algorithms.h"

#include "modules/neopixel/neopixel.h"
#include "modules/motors/motors.h"

#include "config/global_variables.h"
#include "mqtt/mqtt.h"

// enum robot_state{ROBOT_BEGIN, ROBOT_WAIT, ROBOT_RUN };
int robotState = ROBOT_BEGIN;

void algorithm_setup(){
    Serial.println("algorithm: setup");
    //algorithm_start();
}

// Loop in the algorithm
void algorithm_loop(){

    if(robotState==ROBOT_RUN){
        algorithm_execute();
        delay(1000);

    }else if(robotState==ROBOT_BEGIN){
        algorithm_setup();
        robotState = ROBOT_WAIT;

    }else{
        // wait
        delay(100);
    }
}

// functional part of the pattern
void algorithm_execute(){

    int d = distance_read();
    Serial.printf("algo_dist: %d\n", d);

    if(d < 15){
        int random = (rand() % 2000)-1000;
        int sign = (random > 0) ? 1 : -1;

        Serial.printf("random: %d, sign: %d \n", random, sign);

        // Go back for 1 second
        motors.write(-150,-150);
        motors.stop(1000);

        while(distance_read() < 20) {
            // TODO: avid infinity loop in here
            Serial.println("rotate until dist < 50\n");
            motors.write(50*sign,-50*sign);
            motors.stop(1000);
        }

        // turn for 2 second
        motors.write(50*sign,-50*sign);
        motors.stop(2000);

    }else{
        motors.write(200,200);
        delay(1000);
    }

}

// instruct to start the pattern
void algorithm_start(){
    robotState = ROBOT_RUN;
    Serial.println("algorithm: start");
}

// reset the pattern variables and state
void algorithm_reset(){
    robotState = ROBOT_BEGIN;
    Serial.println("algorithm: reset");
}

// stop the execution of the pattern
void algorithm_stop(){
    robotState = ROBOT_WAIT;
    Serial.println("algorithm: wait");
}
