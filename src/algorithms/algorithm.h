#pragma once

#include <Arduino.h>

// Interrupt type for robot software interrupts
typedef enum robot_interrupts
{
    INT_COMM_IN
} robot_interrupt_t;

// Robot state definitions
enum robot_state
{
    ROBOT_BEGIN,
    ROBOT_WAIT,
    ROBOT_RUN
};
extern int robotState;

// Algorithm loop
void algorithm_loop();

// Algorithm setup
void algorithm_setup();

// Swarm Algorithm should be implemented on here
void algorithm_execute();
// void algorithm_execute(char* msg);

// invoke a software interrupt for the robot
void algorithm_interrupt(robot_interrupt_t interrupt, char *msg);

// -- Robot experiment state machine functions ---------------------------------

// instruct to start the experiment
void algorithm_start();

// reset the experiment variables and state
void algorithm_reset();

// stop the execution of the experiment
void algorithm_stop();

// -----------------------------------------------------------------------------
