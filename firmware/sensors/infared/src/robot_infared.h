/**
* @brief    ESP32 IR Sensor library
* @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
* @version  1.0.0
* @url      https://github.com/NuwanJ/ESP32-IR
*
* Licensed under, Creative Commons Attribution-ShareAlike 4.0 International License
* ------------------------------------------------------------------------------
*/

#include <Arduino.h>

#ifndef SW_IR_H
#define SW_IR_H

#if defined(ENABLE_INFARED)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp32-hal.h"
#endif

#include "config/pins.h"

#define IR_SENSOR_COUNT 4
#define IR_BIT_LEN 32
#define PULSE_HIGH_TICKS 20
#define PULSE_LOW_TICKS 10
#define PULSE_FRAME_TICKS 10
#define IR_DEBUG 1

class SW_Infared {
private:

    const uint8_t pin_rmtRx[IR_SENSOR_COUNT] = {PIN_IR_RX_0, PIN_IR_RX_1, PIN_IR_RX_2, PIN_IR_RX_3};

    rmt_obj_t* rmt_send = NULL;

    int flagRmtRecieve = 0;
    int rmtValueReceived = 0;
    int rmtValueSend = 0;
    rmt_data_t dataTx[128];
    float realTickRx, realTickTx;
    char sendBits[64];

    void setupTx();
    void setupRx();

    unsigned int charToInt(char* bitArray, int len);

    unsigned int encodedValue(unsigned int value);
    unsigned int decodedValue(unsigned int value);

public:
    SW_Infared();
    ~SW_Infared();

    void begin();

    unsigned int parse(rmt_data_t* items, size_t len, uint8_t rId);
    rmt_obj_t* rmt_rx[IR_SENSOR_COUNT];

    void sendWaveform(unsigned int value);
    void sendWaveform(unsigned int value, int len);
    void sendBit(short pulseHigh, short pulseLow);

    void sendTestSignal();

};

#endif
