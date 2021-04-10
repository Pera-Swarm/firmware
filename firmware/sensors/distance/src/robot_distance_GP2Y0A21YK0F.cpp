#include "features.h"
#include "config/pins.h"

#include "robot_distance_GP2Y0A21YK0F.h"

#ifdef ENABLE_DISTANCE_SENSOR

// GP2Y0A21YK0F Sensor ---------------------------------------------------------
#ifdef DISTANCE_GP2Y0A21YK0F

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
#endif
#endif
