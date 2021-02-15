#include "features.h"
#include "config/pins.h"

#include "robot_distance_VL53L0X.h"

#ifdef ENABLE_DISTANCE_SENSOR

// VL53L0X Sensor --------------------------------------------------------------
#ifdef DISTANCE_VL53LX0

SW_Distance::SW_Distance(){}

void SW_Distance::begin(){

    Wire.begin();
    if (!tof.begin(I2C_VLX53L0X)) {
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
    // VL53L0X_Error status = VL53L0X_ERROR_NONE;

    tof.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        // Serial.printf("Distance (mm): %u [%d]\n",measure.RangeMilliMeter, measure.RangeStatus);

        if( measure.RangeMilliMeter > DISTANCE_MAX_THRESHOLD){
            return DISTANCE_MAX_THRESHOLD;
        }
        if(measure.RangeMilliMeter <= DISTANCE_MIN_THRESHOLD && measure.RangeStatus != 0){
            return DISTANCE_MAX_THRESHOLD;
        }
        // Cottect the distance offset
        return max(measure.RangeMilliMeter +  DISTANCE_OFFSET, 0);

    }

    //Serial.println(" out of range ");
    return DISTANCE_MAX_THRESHOLD;
}

float SW_Distance::getDistanceFloat(bool avoidBurstRead ) {
    return (float)(this->getRawDistance(avoidBurstRead));
}
uint16_t SW_Distance::getDistanceInt(bool avoidBurstRead ) {
    return (uint16_t)getRawDistance(avoidBurstRead);
}

void SW_Distance::test(){
    // Serial.printf("Distance Sensor:\traw:%d float:%2f int:%u\n\n", this->getRawDistance(), this->getDistanceFloat(), this->getDistanceInt());
    Serial.printf("Distance Sensor: %d\n\n", this->getRawDistance());
}

// ------------------------------------------------------- end of VL53L0X Sensor
#endif

#endif
