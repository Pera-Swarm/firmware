/**
* @brief    ESP32 SharpIR Sensor library
* @author   Nuwan Jaliyagoda
* @version  1.0.1
* @url      https://github.com/NuwanJ/ESP32SharpIR
*
* Licensed under, Creative Commons Attribution-ShareAlike 4.0 International License
* ------------------------------------------------------------------------------
*/

#ifndef DistSensor_h
#define DistSensor_h

#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

#define ANALOG_RESOLUTION 12
#define DISTANCE_SAMPLES 10
#define DISTANCE_SENSOR_FILTER_RATE 0.99

class SW_Distance{
public:

    SW_Distance();

    void begin();
    uint16_t getRawDistance( bool avoidBurstRead = true );
    float getDistanceFloat(bool avoidBurstRead = true);
    uint16_t getDistanceInt(bool avoidBurstRead = true);
    void setFilterRate(float rate);
    void test();

    #ifdef DISTANCE_VL53LX0
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
