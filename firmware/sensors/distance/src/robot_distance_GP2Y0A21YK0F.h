#pragma once

#include <Arduino.h>
#include <Wire.h>

#define ANALOG_RESOLUTION 12
#define DISTANCE_SAMPLES 10
#define DISTANCE_SENSOR_FILTER_RATE 0.99

#define DISTANCE_MAX_THRESHOLD 800
#define DISTANCE_BURST_DELAY 20
#define DISTANCE_OFFSET 30

class SW_Distance{
public:
    SW_Distance();

    void begin();
    uint16_t getRawDistance( bool avoidBurstRead = true );
    float getDistanceFloat(bool avoidBurstRead = true);
    uint16_t getDistanceInt(bool avoidBurstRead = true);
    void setFilterRate(float alpha);
    void test();

private:
    float alpha = 0.2f;
    float average = 25.0f;
    float rawDistance = 0.0f;
    uint32_t rawReading = 0;
    uint32_t lastTime = 0 ;
};
