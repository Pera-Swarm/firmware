/**
* @brief    ESP32 SharpIR Sensor library
* @author   Nuwan Jaliyagoda
* @version  1.0.1
* @url      https://github.com/NuwanJ/ESP32SharpIR
*
* Licensed under, Creative Commons Attribution-ShareAlike 4.0 International License
* ------------------------------------------------------------------------------
*/

#include "../features.h"
#include "../src/pins.h"
#include "../src/SW_Distance.h"

#ifdef ENABLE_DISTANCE_SENSOR

SW_Distance::SW_Distance() {
   analogReadResolution(ANALOG_RESOLUTION);
}

void SW_Distance::begin(){
   Serial.println(F(">> Dist.Sensor\t:enabled"));
}
void SW_Distance::setFilterRate(float rate) {
   if (rate > 0 && rate < 1) this->alpha = rate;
}

uint8_t SW_Distance::getRawDistance( bool avoidBurstRead ) {
   uint8_t distance ;

   //wait for sensor's sampling time
   if (!avoidBurstRead) while ( millis() <= this->lastTime + 20 ) {}
   this->lastTime = millis();

   distance = (28400) / (1 + analogRead(PIN_DIST_SENSOR));  // To avoid divide by zero error
   if (distance > 80) return 81;
   else if (distance < 10) return 9;
   else return distance;
}
float SW_Distance::getDistanceFloat(bool avoidBurstRead ) {
   //wait for sensor's sampling time
   if ( !avoidBurstRead ) while ( millis() <= this->lastTime + 20 ) {}
   this->lastTime = millis();

   // exponential smoothing
   for(int i=0;i<DISTANCE_SAMPLES;i++){
      this->average = (this->alpha * this->getRawDistance())  + ((1 - this->alpha) * this->average);
      delay(20);
   }

   return this->average;
}
uint8_t SW_Distance::getDistanceInt(bool avoidBurstRead ) {
   return (uint8_t)getDistanceFloat(avoidBurstRead);
}

void SW_Distance::test() {
   Serial.printf("Distance Sensor:\n\traw:%d float:%f int:%d\n\n", this->getRawDistance(), this->getDistanceFloat(), this->getDistanceInt());
}

#else

SW_Distance::SW_Distance(){}

void SW_Distance::begin(){
   Serial.println(F(">> Dist.Sensor\t:disabled"));
}
void SW_Distance::setFilterRate(float rate){}
uint8_t SW_Distance::getRawDistance( bool avoidBurstRead ){}
float SW_Distance::getDistanceFloat(bool avoidBurstRead ){}
uint8_t SW_Distance::getDistanceInt(bool avoidBurstRead ){}
void SW_Distance::test(){
   Serial.println(F(">> Dist.Sensor\t:disabled"));
}

#endif
