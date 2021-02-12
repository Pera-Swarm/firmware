#include "robot_http_client.h"

#ifdef ENABLE_WIFI_CLIENT

// --- Robot as a Client -------------------------------------------------------

void httpPrint(String input) {
    // if ((wifiMulti.run() == WL_CONNECTED)) {
    //    IPAddress ip =  WiFi.localIP();
    //    HTTPClient http;
    //    Serial.print("[HTTP] begin...\n");
    //
    //    http.begin("192.168.43.182", 3000, "/post");
    //    http.addHeader("Content-Type", "application/json");
    //
    //    sprintf(tempString, "{\"robotIP\":\"%d.%d.%d.%d\",\"text\":\"%s\"}", ip[0], ip[1], ip[2], ip[3], &input);
    //    int httpResponseCode = http.POST(tempString); //Send the actual POST request
    //
    //    // start connection and send HTTP header
    //    if (httpResponseCode > 0) {
    //       String response = http.getString();                       //Get the response to the request
    //       Serial.println(httpResponseCode);   //Print return code
    //       Serial.println(response);           //Print request answer
    //    } else {
    //       Serial.print("Error on sending POST: ");
    //       Serial.println(httpResponseCode);
    //    }
    // } else {
    //    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    // }
    // http.end();
}

int httpGET(String URL, String parameters){

    URL.concat(parameters);

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        http.begin(URL); // Ex: "http://68.183.188.135:8080/test"
        http.addHeader("Content-Type", "text/plain");

        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);           //Print request answer

        } else {
            Serial.printf("Error on sending GET: %d \n", httpResponseCode);
        }

        http.end();
        return httpResponseCode;
    }
    else {
        Serial.println("Error in WiFi connection");
        delay(5000);
        return 0;
    }
}
int httpPOST(String URL){

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        http.begin(URL); // Ex: "http://68.183.188.135:8080/test"
        http.addHeader("Content-Type", "text/plain");

        // Add post headers
        http.addHeader("postEn", "true");

        int httpResponseCode = http.POST("");

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);   //Print return code
            Serial.println(response);           //Print request answer

        } else {
            Serial.printf("Error on sending GET: %d \n", httpResponseCode);
        }

        http.end();
        return httpResponseCode;
    }
    else {
        Serial.println("Error in WiFi connection");
        delay(5000);
        return 0;
    }
}

#else
void httpPrint(String input){}

int httpGET(String URL){
    return -1;
}
int httpPOST(String URL){
    return -1;
}

#endif
