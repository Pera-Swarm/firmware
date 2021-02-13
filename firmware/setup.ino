
#include "features.h"   // Enable or Diasable services
#include "define.h"     // Configurations for version 4 PCB
#include "core.h"
#include "config/config.h"   // Prepare this file before use


//#include "config_sample.h"   // Sample configurations

void setup() {

    Serial.begin(115200);
    beginMemory();          // NOTE: This should be run as the first thing.

    // This command should be run 'ONLY' at the first run to assign a ID for robot
    // RobotId, leftMotorCorrection, rightMotorCorrection
    // memory.setupRobotWithId(2,0,0);

    gpio.begin();

    motors.begin();
    motors.enableEncoders();

    beginNeoPixel();
    pixelColorWave(0, 0, 50);

    distance.begin();
    colorSensor.begin();
    compass.begin();
    
    // Not fully enabled
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


void i2c_scan(){
    Serial.println ("I2C scanner. Scanning ...");
    byte count = 0;

    Wire.begin();
    Serial.println ("...");
    for (byte i = 8; i < 120; i++)
    {
        //Serial.println (i);
        Wire.beginTransmission (i);
        if (Wire.endTransmission () == 0)
        {
            Serial.print ("Found address: ");
            Serial.print (i, DEC);
            Serial.print (" (0x");
            Serial.print (i, HEX);
            Serial.println (")");
            count++;
            delay (1);  // maybe unneeded?
        } // end of good response
    } // end of for loop

    Serial.println ("Done.");
    Serial.print ("Found ");
    Serial.print (count, DEC);
    Serial.println (" device(s).");
}
