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

#ifdef WHEEL_ENCODER

void SW_Motors::enableEncoders(){
    this->enR.setCount(0);
    this->enL.setCount(0);

    this->enR.attachCounter(PIN_ENCODER_R);
    this->enL.attachCounter(PIN_ENCODER_L);
    Serial.println(">> Encoders\t:enabled");
}
void SW_Motors::encoderReset(){
    this->enL.clearCount();
    this->enR.clearCount();
}
uint SW_Motors::encoderAverage(){
    return (this->enL.getCount() + this->enR.getCount())/2;
}
uint SW_Motors::getEncoderReading(uint8_t wheel){
    if(wheel == LEFT) return this->enL.getCount();
    if(wheel == RIGHT) return this->enR.getCount();
    return 0;
}
void SW_Motors::encoderPrint(){
    Serial.printf("   Encoder L:%d R:%d\n", this->enL.getCount(), this->enR.getCount());
    delay(100);
}
//------------------------------------------------------ end of if WHEEL_ENCODER
#else
void SW_Motors::enableEncoders(){
    Serial.println(">> Encoders\t:disabled");
}
void SW_Motors::encoderReset(){}
uint SW_Motors::encoderAverage(){ return 0;}
uint SW_Motors::getEncoderReading(uint8_t wheel){ return 0;}
void SW_Motors::encoderPrint(){}

//---------------------------------------------------- end of else WHEEL_ENCODER
#endif
