
#include "robot_motors.h"
#include "features.h"
#include "config/pins.h"

#ifndef ENABLE_MOTORS

SW_Motors::SW_Motors() {}
SW_Motors::~SW_Motors() {}

void SW_Motors::begin()
{
    Serial.println(">> Motors\t:disabled");
}
void SW_Motors::write(int16_t leftSpeed, int16_t rightSpeed) {}
void SW_Motors::stop() {}
void SW_Motors::stop(int16_t d) {}

void SW_Motors::pause() {}
void SW_Motors::resume() {}

void SW_Motors::test() {}

#endif
//-------------------------------------------------- end of ifndef ENABLE_MOTORS
