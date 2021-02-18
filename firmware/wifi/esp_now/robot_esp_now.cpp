#include "robot_esp_now.h"

#ifdef ENABLE_ESPNOW

void beginESPNow(){

    // -- For Client Mode -------------------------------------------------------
    #ifdef ESP_NOW_SLAVE

    configDeviceAP();
    setupESPNow(1);
    esp_now_register_recv_cb(esp_onDataRecv);

    #endif

    // -- For Master Mode -------------------------------------------------------
    #ifdef ESP_NOW_MASTER
    WiFi.mode(WIFI_STA);
    //Serial.print("\tSTA MAC: "); Serial.println(WiFi.macAddress());
    setupESPNow(0);
    esp_now_register_send_cb(esp_onDataSend);

    #endif

}

void setupESPNow(uint8_t mode) {

    WiFi.disconnect();

    if (esp_now_init() == ESP_OK) {
        if(mode==0){
            Serial.println(F(">> ESP Now\t:enabled,master"));
        }else{
            Serial.println(F(">> ESP Now\t:enabled,slave"));
        }

    } else {
        Serial.println(F(">> ESP Now\t:failed"));
        // Retry InitESPNow, add a counte and then restart?
        delay(1000);
        beginESPNow();
        // or Simply Restart
        //ESP.restart();
    }
}
void scanForSlave() {

    int8_t scanResults = WiFi.scanNetworks();
    memset(slaves, 0, sizeof(slaves));
    espSlaveCount = 0;

    Serial.println("");

    if (scanResults == 0) {
        Serial.println("No WiFi devices in AP Mode found");
    } else {
        Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");

        for (int i = 0; i < scanResults; ++i) {
            String SSID = WiFi.SSID(i);
            int32_t RSSI = WiFi.RSSI(i);
            String BSSIDstr = WiFi.BSSIDstr(i);

            #ifdef PRINT_SCAN_RESULTS
            Serial.print(" + ");
            Serial.println(SSID);
            #endif

            delay(10);
            if (SSID.indexOf("Slave") == 0) {
                //Serial.printf("%s\n", SSID);
                Serial.print("\t");Serial.print(i + 1); Serial.print(": ");
                Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]");
                Serial.print(" ("); Serial.print(RSSI); Serial.println(")");

                int mac[6];

                if (6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
                    for (int ii = 0; ii < 6; ++ii ) {
                        slaves[espSlaveCount].peer_addr[ii] = (uint8_t) mac[ii];
                    }
                }
                slaves[espSlaveCount].channel = CHANNEL;
                slaves[espSlaveCount].encrypt = 0;
                espSlaveCount++;
            }
        }

        if (espSlaveCount > 0) {
            Serial.printf("%d Slave(s) found, processing..\n", espSlaveCount);
        } else {
            Serial.println("No Slave Found, trying again.");
        }

        WiFi.scanDelete();   // clean up RAM
    }
}
bool attachSlaves() {

    if (espSlaveCount > 0) {
        for (int i = 0; i < espSlaveCount; i++) {
            Serial.print("Processing: ");
            for (int ii = 0; ii < 6; ++ii ) {
                Serial.print((uint8_t) slaves[i].peer_addr[ii], HEX);
                if (ii != 5) Serial.print(":");
            }
            bool exists = esp_now_is_peer_exist(slaves[i].peer_addr);

            if (exists) {
                Serial.println("Already Paired");
            } else {
                esp_err_t addStatus = esp_now_add_peer(&slaves[i]);
                print_responseResult(addStatus);
                delay(100);
            }
        }
        return true;

    } else {
        Serial.println("No Slave found to process");
        return false;
    }

}
void deletePeer() {

    /*
    esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);

    Serial.print("Slave Delete Status: ");

    if (delStatus == ESP_OK) {
    Serial.println("Success");

} else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
Serial.println("ESPNOW Not Init");

} else if (delStatus == ESP_ERR_ESPNOW_ARG) {
Serial.println("Invalid Argument");

} else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
Serial.println("Peer not found.");

} else {
Serial.println("Not sure what happened");

}
*/
}


// -- Callback Functions -------------------------------------------------------
void esp_onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status) {

    Serial.print("Last Packet Sent to\t\t: ");
    print_MAC(mac_addr);
    Serial.print("Last Packet Send Status\t: ");
    print_responseResult(status);
    Serial.println();
}
void esp_onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    Serial.print("Last Packet Recv from\t: ");
    print_MAC(mac_addr);

    memcpy(&incomingMsg, data, sizeof(incomingMsg));
    Serial.print("Bytes received\t\t: ");
    Serial.println(data_len);

    // This should be updated with the struct "espnow_message"
    Serial.printf("id: %d command: %d \n", incomingMsg.id, incomingMsg.command);
    Serial.printf("A:%d, B:%d, C:%d\n\n", incomingMsg.valueA, incomingMsg.valueB, incomingMsg.valueC);

}

void boradcast_Dataframe(espnow_message *outgoingMsg){

    Serial.print("\n>> Send : ");

    for (int i = 0; i < espSlaveCount; i++) {
        const uint8_t *peer_addr = slaves[i].peer_addr;
        esp_now_send(peer_addr, (uint8_t *)outgoingMsg, sizeof(espnow_message));
        delay(100);
    }
}

void espNowTest() {

    if (espSlaveCount > 0){
        attachSlaves();

        outgoingMsg.id = 1;
        outgoingMsg.command = 0;
        outgoingMsg.valueA = 10;
        outgoingMsg.valueB = 20;
        outgoingMsg.valueC = 30;

        boradcast_Dataframe(&outgoingMsg);
    } else {
        // No slave found to process
    }

    delay(3000);
}

// -- Client Configuration -----------------------------------------------------
void configDeviceAP() {

    WiFi.mode(WIFI_AP);

    String Prefix = "Slave:";
    String Mac = WiFi.macAddress();
    String SSID = Prefix + Mac;
    String Password = "123456789";
    bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL , 0);

    if (!result) {
        Serial.println("\tAP Config failed.");
    } else {
        Serial.println("\tBroadcasting with AP: " + String(SSID));
    }

    Serial.print("\tAP MAC: "); Serial.println(WiFi.softAPmacAddress());
}

// -- Utility Functions --------------------------------------------------------
void print_MAC(const uint8_t *mac_addr){
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);
}
void print_responseResult(esp_err_t addStatus){
    if (addStatus == ESP_OK) {
        Serial.println("Pair success");
    } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        Serial.println("ESPNOW Not Init");
    } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Add Peer - Invalid Argument");
    } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
    } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
    } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
    } else if (addStatus == ESP_ERR_ESPNOW_INTERNAL) {
        Serial.println("Internal Error");
    } else if (addStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
        Serial.println("Peer not found.");
    } else {
        Serial.println("Not sure what happened");
    }
}

#else

void beginESPNow(){
    Serial.println(F(">> ESP Now\t:disabled"));
}
void setupESPNow(){}
void scanForSlave(){}
bool attachSlaves(){
    return false;
}
void deletePeer(){}
void boradcast_Dataframe(int *outgoingMsg){}
void esp_onDataSend(const uint8_t *mac_addr, int status){}
void esp_onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len){}

void espNowTest(){}
void configDeviceAP(){}

void print_MAC(const uint8_t *mac_addr){}
void print_responseResult(int addStatus){}

#endif
