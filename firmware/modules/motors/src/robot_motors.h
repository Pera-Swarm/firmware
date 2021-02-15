/**
* @brief    Swarm Robot Motor Controller Library
* @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
* @version  1.0.0
* @url      N/A
*
* ------------------------------------------------------------------------------
*/

#ifndef SW_MOTORS_H
#define SW_MOTORS_H

#include <Arduino.h>
#include <Robot_Servo.h>
#include <Robot_Encoder.h>

#include "config/definitions.h"

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2400

#define MAX_MOTOR_SPEED 255
#define MIN_MOTOR_SPEED 50

#define ENCODER_COUNT_PER_UNIT (1/0.3)

class SW_Motors {

private:
    void attach();
    void detach();

    Servo servoRight;
    Servo servoLeft;

    uint8_t RIGHT_DEFAULT = 90;
    uint8_t LEFT_DEFAULT = 90;

    boolean leftMotorDir = 1, rightMotorDir = 1;
    boolean leftMotorDirOld = 0, rightMotorDirOld = 0;
    int16_t rightMotorSpeed = 0, leftMotorSpeed = 0;

    Robot_Encoder enR;
    Robot_Encoder enL;

public:
    SW_Motors();
    ~SW_Motors();

    int8_t leftCorrection=0;
    int8_t rightCorrection=0;

    void begin();
    void write(int16_t left, int16_t right);
    void stop();
    void stop(int16_t delay); // Go for 'delay' stop

    void pause();
    void resume();

    void test();

    void enableEncoders();
    void encoderReset();
    void encoderPrint();

    uint encoderAverage();
    uint getEncoderReading(uint8_t wheel);
};

#endif
