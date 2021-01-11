
#include "features.h"   // Enable or Diasable services
#include "define.h"     // Configurations for version 4 PCB
#include "config.h"   // Prepare this file before use

//#include "config_sample.h"   // Sample configurations

void setup() {

    Serial.begin(115200);
    beginMemory();          // NOTE: This should be run as the first thing.

    // This command should be run 'ONLY' at the first run to assign a ID for robot
    // RobotId, leftMotorCorrection, rightMotorCorrection
    //memory.setupRobotWithId(1,15,-15);

    gpio.begin();

    motors.begin();
    motors.enableEncoders();

    beginNeoPixel();
    pixelColorWave(0, 0, 50);

    //distance.begin();
    //compass.begin();
    colorSensor.begin();

    //beginInfared();
    //beginWiFiMonitor();
    //beginOTA();
    //beginESPNow();

    beginMQTT();

    //pixelOff();
    //gpio.blinkLED(3, 500);


    Serial.printf("Robot: %d\n\n", memory.getRobotId());

    //delay(2500);

    //i2c_scan();
}

/*
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


// =============================================================================
// Load parameters which are stored in the EEPROM
// =============================================================================

void beginMemory() {
    memory.begin();

    if(memory.getMemoryStatus()){
        ROBOT_ID = memory.getRobotId();

        #ifdef ENABLE_MOTORS
        motors.rightCorrection =  memory.getErrorCorrection(RIGHT);
        motors.leftCorrection = memory.getErrorCorrection(LEFT);
        #endif

    }else{
        // Write default values, if memory isn't configured before
        //memory.setRobotId(31);
        memory.setErrorCorrection(LEFT, 0);
        memory.setErrorCorrection(RIGHT, 0);
        Serial.println("WARNING!\nPlease configure this microcontroller with configurations before use.\n\n");
    }
}
