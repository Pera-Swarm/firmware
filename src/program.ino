
#include "features.h"   // Enable or Diasable services
#include "define.h"     // Configurations for version 4 PCB
#include "./config.h"   // Prepare this file before use

void setup() {
   Serial.begin(115200);
   beginMemory();

   beginLED();
   pixelColorWave(0, 0, 50);

   gpio.begin();
   motors.begin();
   motors.enableEncoders();
   distance.begin();
   compass.begin();
   colorSensor.begin();

   beginInfared();
   beginWiFiMonitor();
   beginOTA();
   beginMQTT();

   pixelOff();
   gpio.blinkLED(3, 500);

   beginESPNow();

   delay(2500);


}

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

// =============================================================================
// Load parameters which are stored in the EEPROM
// =============================================================================

void beginMemory() {
   memory.begin();

   if(memory.getMemoryStatus()){
      ROBOT_ID = memory.getRobotId();
      motors.rightCorrection =  memory.getErrorCorrection(RIGHT);
      motors.leftCorrection = memory.getErrorCorrection(LEFT);

   }else{
      // Write default values, if memory isn't configured before
      memory.setRobotId(31);
      memory.setErrorCorrection(LEFT, 0);
      memory.setErrorCorrection(RIGHT, 0);
      Serial.println("WARNING!\nPlease configure this microcontroller with configurations before use.\n\n");

      #if defined(ENABLE_WIFI_MONITOR)
      beginWiFiMonitor();
      while (1) {
         wifiMonitor.handleClient();
         delay(100);
      }
      #endif
   }
}
