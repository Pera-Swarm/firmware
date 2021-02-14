#pragma once

#include <Arduino.h>

enum robot_state{ROBOT_BEGIN, ROBOT_WAIT, ROBOT_RUN };
extern int robotState;

// Loop in the pattern
void algorithm_loop();

void algorithm_setup();

// functional part of the pattern
void algorithm_execute(char* msg);

// instruct to start the pattern
void algorithm_start();

// reset the pattern variables and state
void algorithm_reset();

// stop the execution of the pattern
void algorithm_stop();
