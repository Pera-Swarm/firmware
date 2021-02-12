#include "robot_monitor.h"
// TODO: Need to update the codes to be compatible with current firmware

#ifdef ENABLE_WIFI_MONITOR

void beginWiFiMonitor() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    int startTime = millis();

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        if( (millis() - startTime)> 10*1000){
            // Restart if unabled to connnect into a WiFi connection within 30 seconds
            Serial.print("\nWiFi connection failed");
            ESP.restart();
        }
    }

    pixelColorWave(0, 50, 0); // Green

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    delay(1000);
    pixelOff();

    //if (MDNS.begin("esp32")) { Serial.println("MDNS responder started"); }

    wifiMonitor.on("/", handleRoot);
    wifiMonitor.on("/status", robotStatus);

    wifiMonitor.onNotFound(handleNotFound);

    // JSON APIs
    wifiMonitor.on("/mag", getMag);
    wifiMonitor.on("/accel", getAccel);
    wifiMonitor.on("/dist", getDist);
    wifiMonitor.on("/color", getColor);

    wifiMonitor.on("/motor", handleMotion);
    wifiMonitor.on("/turn", handleRotation);

    wifiMonitor.on("/pixelLED/all", handlePixelLED);

    wifiMonitor.on("/ir/send", handleIR);

    // REM: Not tested yet
    wifiMonitor.on("/eeprom/write", handleEEPROM_Write);
    wifiMonitor.on("/eeprom/read", handleEEPROM_Read);

    // Only for Course Project
    wifiMonitor.on("/path", handleMotionPath);

    wifiMonitor.begin();

    Serial.println(F(">> WiFi Monitor\t:enabled"));
}

void handleRoot() {
    String message = "Root Directory\n\n";
    wifiMonitor.send(200, "text/plain", message);
}
void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += wifiMonitor.uri();
    message += "\nMethod: ";
    message += (wifiMonitor.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += wifiMonitor.args();
    message += "\n";

    for (uint8_t i = 0; i < wifiMonitor.args(); i++) {
        message += " " + wifiMonitor.argName(i) + ": " + wifiMonitor.arg(i) + "\n";
    }

    wifiMonitor.send(404, "text/plain", message);
}

// General Monitor Functions
void robotStatus() {
    IPAddress ip =  WiFi.localIP();
    sprintf(tempString, "{\"status\":\"Active\",\"version\":\"%s\",\"author\":\"%s\",\"serial\": \"%s\",\"IP\":\"%d.%d.%d.%d\"}", FIRMWARE_VERSION, FIRMWARE_AURTHOR, SERIAL_NUMBER, ip[0], ip[1], ip[2], ip[3]);

    // REM: Need to test this code
    if (wifiMonitor.args() == 1) {
        // update server IP
        //strcpy(host,wifiMonitor.arg(0));
        Serial.println(host);
    }

    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
}

// API Level Functions
void getMag() {

    #ifdef ENABLE_COMPASS_SENSOR
    compass.read();
    Serial.printf("magnetic\tx:%d y:%d z:%d uT\t %f", compass.m.x, compass.m.y, compass.m.z, compass.heading());
    sprintf(tempString, "{\"x\": \"%d\", \"y\": %d, \"z\": \"%d\", \"heading\": \"%f\"}", compass.m.x, compass.m.y, compass.m.z, compass.heading());
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
    #else
    sprintf(tempString, "{\"x\": \"%d\", \"y\": %d, \"z\": \"%d\", \"heading\": \"%f\"}", 0, 0, 0, 0);
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
    #endif

}
void getAccel() {

    #ifdef ENABLE_COMPASS_SENSOR
    compass.read();

    // REM: Incomplete calculations
    float ax = (float)compass.a.x; //* 0.001F * (9.80665F);
    float ay = (float)compass.a.y; //* * 0.001F * (9.80665F);
    float az = (float)compass.a.z; //* * 0.001F * (9.80665F);

    Serial.printf("acceleration\tx:%f y:%f z:%f  m/s^2\n", ax, ay, az);
    sprintf(tempString, "{\"x\": \"%f\", \"y\": %f, \"z\": \"%f\"}", ax, ay, az);
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);

    #else
    Serial.print("acceleration\tN/A");
    sprintf(tempString, "{\"x\": \"%d\", \"y\": %d, \"z\": \"%d\"}", 0, 0, 0);
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);

    #endif
}
void getDist() {

    #ifdef ENABLE_DISTANCE_SENSOR
    int rawDistance = distance.getRawDistance();
    float filteredDistance = distance.getDistanceFloat();

    Serial.printf("Distance\traw:%d filtered:%f \n", rawDistance, filteredDistance);
    sprintf(tempString, "{\"raw\": \"%d\", \"filtered\": \"%f\"}", rawDistance, filteredDistance);
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
    #else
    Serial.print("Distance\tN/A");
    sprintf(tempString, "{\"raw\": \"%d\", \"filtered\": \"%f\"}", 0, 0.0);
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
    #endif
}
void getColor() {

    #ifdef ENABLE_COLOR_SENSOR
    uint16_t red, green, blue, c, colorTemp, lux;
    float r, g, b;

    colorSensor.setInterrupt(false);      // turn on LED
    delay(60);
    colorSensor.getRawData(&red, &green, &blue, &c);
    colorTemp = colorSensor.calculateColorTemperature(red, green, blue);
    lux = colorSensor.calculateLux(red, green, blue);
    colorSensor.setInterrupt(true);     // turn off LED

    r = (red * 256) / (float)c;
    g = (green * 256) / (float)c;
    b = (blue * 256) / (float)c;

    // Only for test
    //pixelShowColor(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]);

    Serial.printf("Color Sensor\tR:%d G:%d B:%d temp:%d lux:%d\n", (int)r, (int)g, (int)b, colorTemp, lux);
    sprintf(tempString, "{\"R\": \"%d\", \"G\": \"%d\", \"B\": \"%d\" , \"temp\": \"%d\" , \"lux\": \"%d\"}", (int)r, (int)g, (int)b, colorTemp, lux);
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);

    #else
    Serial.print("Color Sensor\tN/A");
    sprintf(tempString, "{\"R\": \"%d\", \"G\": %d, \"B\": \"%d\" , \"temp\": \"%d\" , \"lux\": \"%d\",\"code\" : \"%d\"}", 0, 0, 0, 0, 0, 0);
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
    #endif
}

void handleMotion() {

    int spd;

    if (wifiMonitor.args() > 0) {
        spd = (wifiMonitor.args() > 1) ? wifiMonitor.arg(1).toInt() : 200;

        if (wifiMonitor.arg(0).compareTo("stop") == 0) {
            Serial.println("motor/stop");
            motors.stop();

        } else if (wifiMonitor.arg(0).compareTo("forward") == 0) {
            Serial.println("motor/forward");
            motors.write(spd, spd);

        } else if (wifiMonitor.arg(0).compareTo("backward") == 0) {
            Serial.println("motor/backward");
            motors.write(-1 * spd, -1 * spd);

        } else if (wifiMonitor.arg(0).compareTo("rotCW") == 0) {
            Serial.println("motor/rotCW");
            motors.write(spd, -1 * spd);

        } else if (wifiMonitor.arg(0).compareTo("rotCCW") == 0) {
            Serial.println("motor/rotCCW");
            motors.write(-1 * spd, spd);

        } else {
            Serial.println("motor/stop");
            motors.stop();
        }

        sprintf(tempString, "{\"status\":\"success\"}");

    } else {
        sprintf(tempString, "{\"status\":\"incomplete\"}");
    }
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
}
void handleRotation() {

    if (wifiMonitor.args() > 0) {
        float angle = wifiMonitor.arg(0).toFloat();

        // REM: Incomplete operation
        Serial.print("motor/rotate: ");
        Serial.println(angle);

        sprintf(tempString, "{\"status\":\"success\",\"angle\":\"%f\"}", angle);
    } else {
        sprintf(tempString, "{\"status\":\"incomplete\"");
    }

    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
}

void handlePixelLED() {
    #ifdef ENABLE_NEOPIXEL_RING
    uint8_t r, g, b;

    if (wifiMonitor.args() > 0) {
        r = wifiMonitor.arg(0).toInt();
        g = wifiMonitor.arg(1).toInt();
        b = wifiMonitor.arg(2).toInt();
        pixelShowColor(r, g, b);
    }

    sprintf(tempString, "{\"status\":\"success\"}");
    #else
    sprintf(tempString, "{\"status\":\"disabled\"}");
    #endif

    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
}

void handleIR() {

    #ifdef ENABLE_INFARED
    if (wifiMonitor.args() > 0) {
        ir.sendWaveform(wifiMonitor.arg(0).toInt());
    }
    sprintf(tempString, "{\"status\":\"done\"}");
    #else
    sprintf(tempString, "{\"status\":\"disabled\"}");
    #endif

    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);

}

void handleEEPROM_Write() {

    #ifdef ENABLE_MEMORY

    if (wifiMonitor.args() > 0) {
        int addr = wifiMonitor.arg(0).toInt();
        int value = wifiMonitor.arg(1).toInt();

        Serial.printf("eepromWrite(%d,%d);\n", addr, value);

        if (addr == SW_Memory::addr::EEPROM_LEFT_CORRECTION) {
            motors.leftCorrection = value;
            memory.setErrorCorrection(LEFT, value);
        } else if (addr == SW_Memory::addr::EEPROM_RIGHT_CORRECTION) {
            motors.rightCorrection = value;
            memory.setErrorCorrection(RIGHT, value);
        } else{
            memory.write(addr, value);
        }
    }

    sprintf(tempString, "{\"status\":\"done\"}");
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);

    #else
    sprintf(tempString, "{\"status\":\"disabled\"}");
    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
    #endif
}
void handleEEPROM_Read() {

    #ifdef ENABLE_MEMORY
    sprintf(tempString, "{\"status\":\"success\",\"rbtId\":\"%d\",\"lftSC\":\"%d\",\"rgtSC\":\"%d\"}",
    memory.getRobotId(), memory.getErrorCorrection(LEFT), memory.getErrorCorrection(RIGHT));
    Serial.print(tempString);
    #else
    sprintf(tempString, "{\"status\":\"disabled\"}");
    #endif

    wifiMonitor.sendHeader("Access-Control-Allow-Origin", "*");
    wifiMonitor.send(200, "application/json", tempString);
}

#else

void beginWiFiMonitor() {
    Serial.println(F(">> WiFi Monitor\t:disabled"));
}

void robotStatus() {}
void getAngleToMonitor() {}
void getColorToMonitor() {}
void getDistanceToMonitor() {}

void getMag() {}
void getAccel() {}
void getDist() {}
void getColor() {}

void handleMotion() {}
void handleRotation() {}
void handlePixelLED() {}
void handleIR() {}

void handleEEPROM_Write() {}
void handleEEPROM_Read() {}

#endif
