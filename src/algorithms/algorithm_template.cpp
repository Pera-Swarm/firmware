#include "algorithm.h"
#include "config/global_variables.h"
#include "mqtt/mqtt.h"

#ifdef ALGO_TEMPLATE // replace this with the name of your algorithm
// -----------------------------------------------------------------------------

int robotState = ROBOT_BEGIN;

// Define your global variables here -----

// ---------------------------------------

void algorithm_setup(){
    Serial.println("algorithm: setup");
    // Define what need to be setup in here...

}

void algorithm_loop(){
    // Robot state machine is defined in here

    if(robotState==ROBOT_RUN){
        algorithm_execute();
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
    // Define Interrupt Handlers in here

    if(interrupt == INT_COMM_IN){
        // Communication Interrupt
    }
}

void algorithm_execute(){
    // Deine the algorithm in here...
}

// instruct to start the pattern
void algorithm_start(){
    robotState = ROBOT_RUN;
    Serial.println("algorithm: start");
    // Define the tasks to be done at the start of the algorithm in here...
}

void algorithm_reset(){
    // Define the tasks to be done at reset in here...

    robotState = ROBOT_BEGIN;
    Serial.println("algorithm: reset");
}

void algorithm_stop(){
    // Define the tasks to be done to stop an algorithm in here...

    robotState = ROBOT_WAIT;
    Serial.println("algorithm: wait");
}

#endif
