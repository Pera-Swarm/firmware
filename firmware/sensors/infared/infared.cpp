#include "infared.h"
// TODO: Need to update the codes to be compatible with current firmware

#ifdef ENABLE_INFARED

SW_Infared ir;

void beginInfared() {
    rmtRead(ir.rmt_rx[0], irReceive_0);
    rmtRead(ir.rmt_rx[1], irReceive_1);
    rmtRead(ir.rmt_rx[2], irReceive_2);
    rmtRead(ir.rmt_rx[3], irReceive_3);
    ir.begin();
}

void parseRmtData(rmt_data_t* items, size_t len, uint8_t rId) {
    // IR Protocols can be programmed in here

    unsigned int value = ir.parse(items, len, rId);
    //Serial.println(value);

}


/*
// NOTE: These code block is originally placed in setup.ino, and moved to here
// Functionality isn't checked yet

#if defined(ENABLE_INFARED)

// Interrupts for Remote Rx Event
extern "C" void irReceive_0(uint32_t *data, size_t len) {
parseRmtData((rmt_data_t*) data, len, 0);
}
extern "C" void irReceive_1(uint32_t *data, size_t len) {
parseRmtData((rmt_data_t*) data, len, 1);
}
extern "C" void irReceive_2(uint32_t *data, size_t len) {
parseRmtData((rmt_data_t*) data, len, 2);
}
extern "C" void irReceive_3(uint32_t *data, size_t len) {
parseRmtData((rmt_data_t*) data, len, 3);
}
#endif
*/


#else

void beginInfared() {
    // Nothing to do in here
}
void parseRmtData(rmt_data_t* items, size_t len, uint8_t rId) {}

#endif
