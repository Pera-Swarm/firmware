/**
* @brief    Swarm Robot Motor Controller Library
* @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
* @version  1.0.0
* @url      N/A
*
* ------------------------------------------------------------------------------
*/

#include "robot_motors.h"
#include "features.h"
#include "config/pins.h"

#if defined(ENABLE_MOTORS)
#if defined(DRIVE_PWM)

#define LEDC_RESOLUTION_BITS  8
#define LEDC_BASE_FREQ        5000
#define LEDC_CHANNEL_A        8
#define LEDC_CHANNEL_B        9

SW_Motors::SW_Motors() {
    // Motor constructor
}

SW_Motors::~SW_Motors() {
    ledcDetachPin(PIN_PWM_A);
    ledcDetachPin(PIN_PWM_B);
}

void SW_Motors::begin(){
    pinMode(PIN_MOT_A, OUTPUT);
    pinMode(PIN_MOT_B, OUTPUT);

    ledcSetup(LEDC_CHANNEL_A, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);
    ledcSetup(LEDC_CHANNEL_B, LEDC_BASE_FREQ, LEDC_RESOLUTION_BITS);

    ledcAttachPin(PIN_PWM_A, LEDC_CHANNEL_A);
    ledcAttachPin(PIN_PWM_B, LEDC_CHANNEL_B);

    ledcWrite(LEDC_CHANNEL_A, 0);
    ledcWrite(LEDC_CHANNEL_B, 0);

    Serial.println(">> Motors\t:enabled,pwmMode");

    this->write(0, 0);
}
void SW_Motors::write(int16_t leftSpeed, int16_t rightSpeed){

    // Adjustment to remove the drift
    if(leftSpeed>30) leftSpeed += leftCorrection;
    else if(leftSpeed<-30) leftSpeed -= leftCorrection;

    if(rightSpeed>30) rightSpeed += rightCorrection;
    else if(rightSpeed<-30) rightSpeed -= rightCorrection;

    // map the speed with correct & acceptable range
    leftSpeed = constrain(leftSpeed, -1 * MAX_MOTOR_SPEED , MAX_MOTOR_SPEED);
    rightSpeed = constrain(rightSpeed , -1 * MAX_MOTOR_SPEED , MAX_MOTOR_SPEED);

    // Serial.printf("M: %d %d\n", leftSpeed, rightSpeed);

    // motor rotating directions
    this->leftMotorDir = (leftSpeed >= 0) ? 1 : 0;
    this->rightMotorDir = (rightSpeed >= 0) ? 1 : 0;

    // check motor directions
    if (this->leftMotorDir !=  this->leftMotorDirOld) {
        // Direction changed
        digitalWrite(PIN_MOT_A, (this->leftMotorDir) ? LOW : HIGH);
        this->leftMotorDirOld = this->leftMotorDir;
    }
    if (this->rightMotorDir !=  this->rightMotorDirOld) {
        // Direction changed
        digitalWrite(PIN_MOT_B, (this->rightMotorDir) ? LOW : HIGH);
        this->rightMotorDirOld = this->rightMotorDir;
    }

    this->rightMotorSpeed = abs(rightSpeed);
    this->leftMotorSpeed = abs(leftSpeed);

    // Analog write function for ESP32
    ledcWrite(LEDC_CHANNEL_A, this->leftMotorSpeed);
    ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);

}
void SW_Motors::stop(){
    this->write(0,0);
}

void SW_Motors::stop(int16_t d){
    delay(d);
    this->write(0,0);
}

void SW_Motors::pause(){
    // pause motors for while
    ledcWrite(LEDC_CHANNEL_A, 0);
    ledcWrite(LEDC_CHANNEL_B, 0);
}
void SW_Motors::resume(){
    // turn on motors again
    ledcWrite(LEDC_CHANNEL_A, this->leftMotorSpeed);
    ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);

}

void SW_Motors::test(){

    // Clockwise rotation
    Serial.println(F("robot: CCW"));
    this->write(0, 200);
    delay(500);
    this->stop(1500);
    this->encoderPrint();

    // Counter Clockwise rotation
    Serial.println(F("robot: CW"));
    this->write(200, 0);
    delay(500);
    this->stop(1500);
    this->encoderPrint();
    this->encoderReset();

    // Forward movement
    Serial.println(F("robot: forward++"));
    for (int i = 0; i < 255; i++) {
        this->write(i, i);
        delay(25);
    }
    delay(500);

    Serial.println(F("robot: forward--"));
    for (int i = 255; i > 0; i--) {
        this->write(i, i);
        delay(25);
    }
    this->stop(500);
    this->encoderPrint();
    delay(2000);

    // Backward movement
    Serial.println(F("robot: backward++"));
    for (int i = 0; i < 255; i++) {
        this->write(-i, -i);
        delay(25);
    }
    delay(500);

    Serial.println(F("robot: backward--"));
    for (int i = 255; i > 0; i--) {
        this->write(-i, -i);
        delay(25);
    }
    this->stop(500);
    this->encoderPrint();
    delay(2000);
    this->encoderReset();

}
#endif
//------------------------------------------------------------- end of DRIVE_PWM

#endif
