
#include "../../features.h"
#include "../../config/pins.h"
#include "robot_distance.h"

#ifdef ENABLE_DISTANCE_SENSOR

// VL53L0X Sensor --------------------------------------------------------------
#ifdef DISTANCE_VL53LX0

SW_Distance::SW_Distance(){}

void SW_Distance::begin(){

    Wire.begin();
    if (!tof.begin(0x2A)) {
        Serial.println(F(">> Dist.Sensor\t:not attached, VL53L0X"));
    }else{
        Serial.println(F(">> Dist.Sensor\t:enabled, VL53L0X"));
    }
}
void SW_Distance::setFilterRate(float alpha){}

uint16_t SW_Distance::getRawDistance( bool avoidBurstRead ){
    if (!avoidBurstRead) while ( millis() <= this->lastTime + DISTANCE_BURST_DELAY ) {}
    this->lastTime = millis();

    VL53L0X_RangingMeasurementData_t measure;
    tof.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        //Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);

        if( measure.RangeMilliMeter > DISTANCE_MAX_THRESHOLD) return -1;
        else return (measure.RangeMilliMeter +  DISTANCE_OFFSET);

    } else {
        //Serial.println(" out of range ");
        return -1;
    }
}
float SW_Distance::getDistanceFloat(bool avoidBurstRead ) {
    return (float)(this->getRawDistance(avoidBurstRead));
}
uint16_t SW_Distance::getDistanceInt(bool avoidBurstRead ) {
    return (uint16_t)getRawDistance(avoidBurstRead);
}

void SW_Distance::test(){
    Serial.printf("Distance Sensor:\traw:%d float:%2f int:%d\n\n", this->getRawDistance(), this->getDistanceFloat(), this->getDistanceInt());
    //Serial.printf("Distance Sensor: %d\n\n", this->getRawDistance());
}

// ------------------------------------------------------- end of VL53L0X Sensor
#endif

#endif
