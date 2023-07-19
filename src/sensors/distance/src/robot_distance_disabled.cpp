#include "features.h"
#include "robot_distance_disabled.h"

#ifndef ENABLE_DISTANCE_SENSOR

SW_Distance::SW_Distance(){}

void SW_Distance::begin(){
    Serial.println(F(">> Dist.Sensor\t:disabled"));
}
void SW_Distance::setFilterRate(float alpha){}
uint16_t SW_Distance::getRawDistance( bool avoidBurstRead ){return 0;}
float SW_Distance::getDistanceFloat(bool avoidBurstRead ){return 0;}
uint16_t SW_Distance::getDistanceInt(bool avoidBurstRead ){return 0;}
void SW_Distance::test(){
    Serial.println(F(">> Dist.Sensor\t:disabled"));
}

#endif
