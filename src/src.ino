
#include "features.h"   // Enable or Diasable services
#include "define.h"     // Configurations for version 4 PCB
#include "config.h"   // Prepare this file before use


//#define WIFI_SSID "Eng-Student"
//#define WIFI_PASS "3nG5tuDt"

//const char* ssid = WIFI_SSID;
//const char* password = WIFI_PASS;

#include "config_sample.h"   // Sample configurations

void setup() {

    Serial.begin(115200);
    beginMemory();

    //memory.setRobotId(10);

    beginLED();
    pixelColorWave(0, 0, 50);

    gpio.begin();
    motors.begin();
    motors.enableEncoders();
    distance.begin();
    //compass.begin();
    colorSensor.begin();

    beginInfared();
    beginWiFiMonitor();
    beginOTA();
    beginMQTT();

    pixelOff();
    gpio.blinkLED(3, 500);

    //beginESPNow();

    Serial.printf("Robot: %d\n\n", memory.getRobotId());

    delay(2500);

    //i2c_scan();
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
        //memory.setRobotId(31);
        memory.setErrorCorrection(LEFT, 0);
        memory.setErrorCorrection(RIGHT, 0);
        Serial.println("WARNING!\nPlease configure this microcontroller with configurations before use.\n\n");

        /*#if defined(ENABLE_WIFI_MONITOR)
        beginWiFiMonitor();
        while (1) {
        wifiMonitor.handleClient();
        delay(100);
    }
    #endif
    */
}
}

void i2c_scan(){
    Serial.println ();
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

void motorFunctionTest(){

   motors.encoderReset();

    Serial.println("Go Forward");
    motors.write(150,150);
    delay(500);
    motors.stop();
    delay(2000);

    Serial.println("Turn CW");
    motors.write(150,-150);
    delay(500);
    motors.stop();
    delay(2000);

    Serial.println("Turn CCW");
    motors.write(-150,150);
    delay(500);
    motors.stop();
    delay(2000);

    motors.encoderPrint();



}
