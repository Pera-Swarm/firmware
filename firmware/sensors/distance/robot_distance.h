#pragma once

#include <Arduino.h>
#include <Wire.h>

#if defined(DISTANCE_VL53LX0)
#include <Adafruit_VL53L0X.h>
#endif

#define ANALOG_RESOLUTION 12
#define DISTANCE_SAMPLES 10
#define DISTANCE_SENSOR_FILTER_RATE 0.99

// With VL53LX0, if the distance reeading is more than this, distance function returns -1
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

    #if defined(DISTANCE_VL53LX0)
    Adafruit_VL53L0X tof = Adafruit_VL53L0X();
    #endif

private:
    float alpha = 0.2f;
    float average = 25.0f;
    float rawDistance = 0.0f;
    uint32_t rawReading = 0;
    uint32_t lastTime = 0 ;
};

#endif
