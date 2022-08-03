
#include "setup.h"

uint8_t mode = BEGIN;
int ROBOT_ID;

void setup() {

    Serial.begin(115200);
    // i2c_scan();          // Sacn and print available I2C ports

    beginMemory();          // NOTE: This should be run as the first thing.

    // This commands should be run 'ONLY' at the first run to assign a ID for robot
    // RobotId, leftMotorCorrection, rightMotorCorrection
    memory.setupRobotWithId(5,0,0);

    gpio.begin();
    motors.begin();
    motors.enableEncoders();

    beginNeoPixel();
    // pixelColorWave(0, 0, 50);

    beginDisplay();
    // display_print(0, "Begin...");
    // display_clear();

    distance.begin();
    colorSensor.begin();
    compass.begin();

    // Need to setup WiFi before configure MQTT
    beginWiFi();
    beginMQTT();

    // Not fully completed
    //beginInfared();
    //beginWiFiMonitor();
    //beginOTA();
    //beginESPNow();

    pixelOff();
    gpio.blinkLED(3, 500);

    //delay(2500);

    Serial.printf("\nRobot_%d > Setup Completed!\n\n", memory.getRobotId());
}

// =============================================================================
// Load parameters which are stored in the EEPROM
// =============================================================================

void beginMemory() {
    memory.begin();

    if(memory.getMemoryStatus()){

        #ifdef ENABLE_MOTORS
        motors.rightCorrection =  memory.getErrorCorrection(RIGHT);
        motors.leftCorrection = memory.getErrorCorrection(LEFT);
        #endif

        ROBOT_ID = memory.getRobotId();
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
