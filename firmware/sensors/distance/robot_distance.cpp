/**
* @brief    ESP32 SharpIR Sensor library
* @author   Nuwan Jaliyagoda
* @version  1.0.1
* @url      https://github.com/NuwanJ/ESP32SharpIR
*
* Licensed under, Creative Commons Attribution-ShareAlike 4.0 International License
* ------------------------------------------------------------------------------
*/

#include "../../features.h"
#include "../../config/pins.h"
#include "robot_distance.h"

// GP2Y0A21YK0F Sensor ---------------------------------------------------------
#if defined(ENABLE_DISTANCE_SENSOR)

#if defined(DISTANCE_GP2Y0A21YK0F)
SW_Distance::SW_Distance() {
    analogReadResolution(ANALOG_RESOLUTION);
}

void SW_Distance::begin(){
    Serial.println(F(">> Dist.Sensor\t:enabled, GP2Y0A21YK0F"));
}
void SW_Distance::setFilterRate(float alpha) {
    if (alpha > 0 && alpha < 1) this->alpha = alpha;
}

uint16_t SW_Distance::getRawDistance( bool avoidBurstRead ) {
    uint8_t distance ;

    //wait for sensor's sampling time
    if (!avoidBurstRead) while ( millis() <= this->lastTime + DISTANCE_BURST_DELAY ) {}
    this->lastTime = millis();

    distance = (284000) / (1 + analogRead(PIN_DIST_SENSOR));  // To avoid divide by zero error
    if (distance > (DISTANCE_MAX_THRESHOLD)) return -1;
    else if (distance < 100) return (99 + DISTANCE_OFFSET);     // can't measure distances less than 10cm
    else return (distance + DISTANCE_OFFSET);
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
uint16_t SW_Distance::getDistanceInt(bool avoidBurstRead ) {
    return (uint8_t)getDistanceFloat(avoidBurstRead);
}

void SW_Distance::test() {
    Serial.printf("Distance Sensor:\n\traw:%d float:%f int:%d\n\n", this->getRawDistance(), this->getDistanceFloat(), this->getDistanceInt());
}
// -------------------------------------------------- end of GP2Y0A21YK0F Sensor


// VL53L0X Sensor --------------------------------------------------------------
#elif defined(DISTANCE_VL53LX0)

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

#if !defined(ENABLE_DISTANCE_SENSOR)

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
#endif
