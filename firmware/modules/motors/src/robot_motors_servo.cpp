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

//------------------------------------------------------------------------------
#if defined(DRIVE_SERVO)

SW_Motors::SW_Motors() {}
SW_Motors::~SW_Motors() {
    this->detach();
}

void SW_Motors::begin(){
    this->attach();
    this->write(0, 0);
    Serial.println(">> Motors\t:enabled,servoMode");
}
void SW_Motors::write(int16_t leftSpeed, int16_t rightSpeed){
    this->rightMotorSpeed = this->RIGHT_DEFAULT - (rightSpeed/4);
    this->leftMotorSpeed = this->LEFT_DEFAULT + (leftSpeed/4);

    this->servoRight.write(this->rightMotorSpeed);
    this->servoLeft.write(this->leftMotorSpeed);

    //Serial.printf("M: %d %d (servo)\n", leftSpeed, rightSpeed);
}
void SW_Motors::stop(){
    this->write(0,0);
}
void SW_Motors::stop(int16_t d){
    this->stop();
    delay(d);
}

void SW_Motors::pause(){
    // Warning: not tested
    this->servoRight.write(this->RIGHT_DEFAULT);
    this->servoLeft.write(this->LEFT_DEFAULT);
}
void SW_Motors::resume(){
    // Warning: not tested
    this->servoRight.write(this->rightMotorSpeed);
    this->servoLeft.write(this->leftMotorSpeed);
}

void SW_Motors::test(){
    // Not Available
}
void SW_Motors::attach(){
    this->servoRight.attach(PIN_SERVO_RIGHT, SERVO_MIN_US, SERVO_MAX_US);
    this->servoLeft.attach(PIN_SERVO_LEFT, SERVO_MIN_US, SERVO_MAX_US);
}
void SW_Motors::detach(){
    this->servoRight.detach();
    this->servoLeft.detach();
}

#endif
//----------------------------------------------------------- end of DRIVE_SERVO

#endif
