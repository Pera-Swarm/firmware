/**
* @brief    ESP32 IR Sensor library
* @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
* @version  1.0.0
* @url      https://github.com/NuwanJ/ESP32-IR
*
* Licensed under, Creative Commons Attribution-ShareAlike 4.0 International License
* ------------------------------------------------------------------------------
*/

#include "features.h"
#include "config/pins.h"
#include "robot_infared.h"

//------------------------------------------------------------------------------
#if defined(ENABLE_INFARED)
//------------------------------------------------------------------------------

SW_Infared::SW_Infared() {
    this->setupTx();
    this->setupRx();
}
SW_Infared::~SW_Infared(){
    // Empty
}

void SW_Infared::begin(){
    Serial.println(">> IR\t\t:enabled");
    //if (ENABLE_SERIAL_COMMUNICATION) Serial.printf("\t\t: real tick set to: Tx: %fns | Rx: %fns\n\n", this->realTickTx, this->realTickRx);
}

void SW_Infared::setupTx(){
    if ((this->rmt_send = rmtInit(PIN_IR_TX, true, RMT_MEM_64)) == NULL){
        if (ENABLE_SERIAL_COMMUNICATION) Serial.println(">> error: Infared (Tx module failed)\n");
    }

    // Configure tick widths of the transmiter
    this->realTickTx = rmtSetTick(this->rmt_send, 200);

    //-- Generate carrier waveform of 38KHz -------------------------------------
    for (int i = 0; i < 128; i++) {
        this->dataTx[i].val = 0x80010000 + ((32) << 17) + ((32) << 1);
    }

}
void SW_Infared::setupRx(){
    for (int i = 0; i < IR_SENSOR_COUNT; i++) {
        if ((this->rmt_rx[i] = rmtInit(this->pin_rmtRx[i], false, RMT_MEM_64)) == NULL){
            if (ENABLE_SERIAL_COMMUNICATION) Serial.println(">> error: Infared (Rx module failed)\n");

        }

        this->realTickRx = rmtSetTick(this->rmt_rx[i], 5000); // x10 uS
    }

    //rmtRead(this->rmt_rx[0], this->irReceive_0);
    //rmtRead(this->rmt_rx[1], this->irReceive_1);
    //rmtRead(this->rmt_rx[2], this->irReceive_2);
    //rmtRead(this->rmt_rx[3], this->irReceive_3);

}

unsigned int SW_Infared::parse(rmt_data_t* items, size_t len, uint8_t rId) {
    if (len == IR_BIT_LEN + 2 ) {
        rmt_data_t* it = &items[0];
        unsigned int value = 0;

        if (IR_DEBUG) Serial.print("Receive\t< "); // LSB First

        // trim the first and last framing bits
        for (size_t i = 1; i < (len - 1); i++) {
            it = &items[i];

            if ((it->duration0 < 200) && (it->duration1 < 200) && (it->duration0 > 20) && (it->duration1 > 20)) {
                if (IR_DEBUG) Serial.printf("%d", (it->duration0 > it->duration1));
                bitWrite(value, i - 1, (it->duration0 > it->duration1));

            } else {
                if (IR_DEBUG) Serial.printf("(%d,%d)", it->duration0, it->duration1);
            }
        }

        if (IR_DEBUG) Serial.printf("      << %ud (%d)\n", value, rId);
        return value;

    }else{
        return 0;
    }
}

void SW_Infared::sendWaveform(unsigned int value){
    this->sendWaveform(value, IR_BIT_LEN); // default: 32bits
}
void SW_Infared::sendWaveform(unsigned int value, int len) {
    uint8_t bitI;
    if (IR_DEBUG) Serial.print("Send \t> "); // LSB First

    // Store the last value send through IR LEDs
    rmtValueSend = value;

    // Sends a starting bit of 10 ticks
    this->sendBit(PULSE_FRAME_TICKS, PULSE_FRAME_TICKS);

    // send signal bits
    for (uint8_t i = 0; i < len; i++) {
        bitI = bitRead(value, i);

        if (bitI == 1) this->sendBit(PULSE_HIGH_TICKS, PULSE_LOW_TICKS);
        else this->sendBit(PULSE_LOW_TICKS, PULSE_HIGH_TICKS);
        if (IR_DEBUG) Serial.print(bitI);
    }
    // Sends a ending bit of 10 ticks
    this->sendBit(PULSE_FRAME_TICKS, PULSE_FRAME_TICKS);
    if (IR_DEBUG) Serial.printf("      << %ud\n", value);

}
void SW_Infared::sendBit(short pulseHigh, short pulseLow) {
    rmtWrite(rmt_send, dataTx, pulseHigh);
    delayMicroseconds(26 * (pulseHigh + pulseLow));
}

unsigned int SW_Infared::encodedValue(unsigned int value)  {

    char bitArray[32];
    int count = 0;

    for (int i = 1; i <= 32; ++i) {
        if (ceil(log2(i)) != floor(log2(i)))  {
            bitArray[32 - i] = bitRead(value, count++) + '0';
        } else {
            bitArray[32 - i] = '0';
        }
    }

    // Debugging -------
    if (ENABLE_SERIAL_COMMUNICATION) {
        for (int i = 0; i < 32; ++i) {
            Serial.print(bitArray[i]);
        }
        Serial.println();
    }
    // -----------------

    for (int i = 0; i < 6; ++i)  {
        int evenCount = 0;
        for (int j = 1; j <= 32; ++j)  {
            if (ceil(log2(j)) != floor(log2(j)) && bitRead(j, i) == 1)  {
                if (bitArray[32 - j] == '1')
                ++evenCount;
            }
        }
        if (evenCount % 2 == 0)bitArray[32 - ((int)(pow(2, i)))] = '0';
        else bitArray[32 - ((int)(pow(2, i)))] = '1';
    }

    // Debugging -------
    if (ENABLE_SERIAL_COMMUNICATION) {
        for (int i = 0; i < 32; ++i) {
            Serial.print(bitArray[i]);
        }
        Serial.println();
    }
    // -----------------

    return this->charToInt(bitArray, 32);

}
unsigned int SW_Infared::decodedValue(unsigned int value)  {

    char checkBitArray[6];
    int count = 5;

    for (int i = 0; i < 6; ++i)  {
        if (ENABLE_SERIAL_COMMUNICATION) Serial.printf("%d : ", i);

        int evenCount = 0;

        for (int j = 0; j < 32; ++j)  {
            if (bitRead((j + 1), i) == 1)  {
                if (ENABLE_SERIAL_COMMUNICATION) Serial.printf("%d (%d), ", j, bitRead(value, j));
                if (bitRead(value, j) == 1)
                ++evenCount;
            }
        }
        if (ENABLE_SERIAL_COMMUNICATION)  Serial.println();

        if (evenCount % 2 == 0) checkBitArray[count--] = '0';
        else checkBitArray[count--] = '1';
    }

    if (ENABLE_SERIAL_COMMUNICATION){
        Serial.println("-----------Check Bits----------");
        for (int i = 0; i < 6; ++i) {
            Serial.print(checkBitArray[i]);
        }
        Serial.println();
    }

    unsigned int checkValue = charToInt(checkBitArray, 6);

    if (ENABLE_SERIAL_COMMUNICATION) Serial.println(checkValue);

    if (checkValue == 0)  {
        return value;

    } else  {
        char bitArray[32];
        int count = 0;
        for (int i = 1; i <= 32; ++i)  {
            bitArray[32 - i] = bitRead(value, count++) + '0';
        }

        if (ENABLE_SERIAL_COMMUNICATION) {
            for (int i = 0; i < 32; ++i) {
                Serial.print(bitArray[i]);
            }
            Serial.println();
        }

        bitArray[32 - checkValue] = !(bitArray[32 - checkValue] - '0') + '0';

        if (ENABLE_SERIAL_COMMUNICATION) {
            for (int i = 0; i < 32; ++i) {
                Serial.print(bitArray[i]);
            }
            Serial.println();
        }

        return this->charToInt(bitArray, 32);
    }
}
unsigned int SW_Infared::charToInt(char* bitArray, int len) {
    // Usage: charToInt("1001010111100100", 16)
    unsigned int value = 0;
    for (int i = 0; i < len; i++) {
        value = (value << 1) | (bitArray[i] == '1');
    }
    return value;
}

void SW_Infared::sendTestSignal() {
    unsigned int randNumber = esp_random();
    this->sendWaveform(randNumber, IR_BIT_LEN);
}

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

SW_Infared::SW_Infared() {
    Serial.println(F(">> IR\t\t:disabled"));
}
SW_Infared::~SW_Infared(){}

void SW_Infared::setupTx(){}
void SW_Infared::setupRx(){}

void SW_Infared::begin(){
    Serial.println(F(">> IR\t\t:disabled"));
}

unsigned int SW_Infared::parse(rmt_data_t* items, size_t len, uint8_t rId) {return 0;}

void SW_Infared::sendWaveform(unsigned int value){}
void SW_Infared::sendWaveform(unsigned int value, int len) {}
void SW_Infared::sendBit(short pulseHigh, short pulseLow) {}

unsigned int SW_Infared::decodedValue(unsigned int value)  {return 0;}
unsigned int SW_Infared::charToInt(char* bitArray, int len) {return 0;}

void SW_Infared::sendTestSignal() {
    Serial.println(F(">> IR\t:disabled"));
}

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
