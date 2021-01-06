/**
* @brief    Swarm Robot Motor Controller Library
* @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
* @version  1.0.0
* @url      N/A
*
* ------------------------------------------------------------------------------
*/

#include "../features.h"
#include "../src/def_pins.h"
#include "../src/SW_Motors.h"

//------------------------------------------------------------------------------
#if defined(DRIVE_SERVO)
//------------------------------------------------------------------------------

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

   this->servoRight.write(this->RIGHT_DEFAULT - (rightSpeed/4));
   this->servoLeft.write(this->LEFT_DEFAULT + (leftSpeed/4));

   //Serial.printf("M: %d %d (servo)\n", leftSpeed, rightSpeed);
}
void SW_Motors::stop(){
   this->write(0,0);
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


#ifdef WHEEL_ENCODER
void SW_Motors::enableEncoders(){
   this->enR.setCount(0);
   this->enL.setCount(0);

   this->enR.attachCounter(PIN_ENCODER_R);
   this->enL.attachCounter(PIN_ENCODER_L);
}
void SW_Motors::encoderReset(){
   this->enL.clearCount();
   this->enR.clearCount();
}
int SW_Motors::encoderAverage(){
   return (this->enL.getCount() + this->enR.getCount())/2;
}

void SW_Motors::encoderPrint(){
   Serial.printf("Encoder L:%d R:%d\n", this->enL.getCount(), this->enR.getCount());
   delay(100);
}
#else
void SW_Motors::enableEncoders(){}
void SW_Motors::encoderReset(){}
int SW_Motors::encoderAverage(){ return -1;}
void SW_Motors::encoderPrint(){}
#endif











//------------------------------------------------------------------------------
#elif defined(DRIVE_PWM)
//------------------------------------------------------------------------------

#define LEDC_RESOLUTION_BITS  8
#define LEDC_BASE_FREQ     5000
#define LEDC_CHANNEL_A     12
#define LEDC_CHANNEL_B     13

SW_Motors::SW_Motors() {

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



   if(leftSpeed>30)leftSpeed += leftCorrection;
   if(rightSpeed>30)rightSpeed += rightCorrection; // drift

   //Serial.printf("M: %d %d\n", leftSpeed, rightSpeed);

   // ------ map  the speed with correct & acceptable range --------------------
   leftSpeed = constrain(leftSpeed, -1 * MAX_MOTOR_SPEED , MAX_MOTOR_SPEED);
   rightSpeed = constrain(rightSpeed , -1 * MAX_MOTOR_SPEED , MAX_MOTOR_SPEED);

   // ------ motor rotating directions -----------------------------------------
   this->leftMotorDir = (leftSpeed >= 0) ? 1 : 0;
   this->rightMotorDir = (rightSpeed >= 0) ? 1 : 0;

   //------- check motor directions --------------------------------------------
   if (this->leftMotorDir !=  this->leftMotorDirOld) {
      // Direction changed
      digitalWrite(PIN_MOT_A, (this->leftMotorDir) ? HIGH : LOW);
      this->leftMotorDirOld = this->leftMotorDir;
   }
   if (this->rightMotorDir !=  this->rightMotorDirOld) {
      // Direction changed
      digitalWrite(PIN_MOT_B, (this->rightMotorDir) ? LOW : HIGH);
      this->rightMotorDirOld = this->rightMotorDir;
   }
   //---------------------------------------------------------------------------

   this->rightMotorSpeed = abs(rightSpeed);
   this->leftMotorSpeed = abs(leftSpeed);

   // Analog write function for ESP32
   ledcWrite(LEDC_CHANNEL_A, this->leftMotorSpeed);
   ledcWrite(LEDC_CHANNEL_B, this->rightMotorSpeed);

}
void SW_Motors::stop(){
   this->write(0,0);
}
void SW_Motors::test(){
   for (int i = 0; i < 255; i++) {
      this->write(i, i);
      delay(100);
   }
   this->write(0, 0);
   delay(500);

   for (int i = 255; i > 0; i--) {
      this->write(i, i);
      delay(100);
   }

   this->write(0, 0);
   delay(500);

}

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
int SW_Motors::encoderAverage(){
   return (this->enL.getCount() + this->enR.getCount())/2;
}
void SW_Motors::encoderPrint(){
   Serial.printf("Encoder L:%d R:%d\n", this->enL.getCount(), this->enR.getCount());
   delay(100);
}
#else
void SW_Motors::enableEncoders(){
   Serial.println(">> Encoders\t:disabled");
}
void SW_Motors::encoderReset(){}
int SW_Motors::encoderAverage(){ return -1;}
void SW_Motors::encoderPrint(){}
#endif


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
