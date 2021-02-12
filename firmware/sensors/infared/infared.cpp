#include "infared.h"
// TODO: Need to update the codes to be compatible with current firmware

#ifdef ENABLE_INFARED

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

#else

void beginInfared() {
   // Nothing to do in here
}
void parseRmtData(rmt_data_t* items, size_t len, uint8_t rId) {}

#endif
