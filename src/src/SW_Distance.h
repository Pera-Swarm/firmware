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

#define ANALOG_RESOLUTION 12
#define DISTANCE_SAMPLES 10
#define DISTANCE_SENSOR_FILTER_RATE 0.99

class SW_Distance{
public:

   SW_Distance();

   void begin();
   uint8_t getRawDistance( bool avoidBurstRead = true );
   float getDistanceFloat(bool avoidBurstRead = true);
   uint8_t getDistanceInt(bool avoidBurstRead = true);
   void setFilterRate(float rate);
   void test();

private:
   float alpha = 0.2f;
   float average = 25.0f;
   float rawDistance = 0.0f;
   uint32_t rawReading = 0;
   uint32_t lastTime = 0 ;
};

#endif
