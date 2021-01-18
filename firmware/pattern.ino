
enum pattern{P_BEGIN, P_WAIT, P_RUN };
int patternState = P_BEGIN;

int currentHopId = -1;
boolean colorUpdated = false;

// Loop in the pattern
void pattern_loop(){

    if(patternState==P_RUN){
        // @Override by pattern
        //pattern_execute();
        delay(50);

    }else if(patternState==P_BEGIN){
        pattern_setup();
        patternState = P_WAIT;

    }else{
        // wait
        delay(100);
    }
}

void pattern_setup(){
    Serial.println("pattern: setup");
    pattern_start();
}

// functional part of the pattern
void pattern_execute(char* msg){

    Serial.printf("pattern: execute %s \n", msg);

    int hopId=0, hopR=0, hopG=0, hopB=0;
    int formatLen = sscanf(msg, "%d %d %d %d", &hopId, &hopR, &hopG, &hopB);

    if(formatLen==4){
        printf("pattern_exec updated: %d len:%d \n", (colorUpdated==1) ? 1 : 0, formatLen);

        if (colorUpdated) {
            // a reverse message, don't forward
            pixelShowColor(0,0,0);

        } else {
            if (hopId > currentHopId) {

                pixelShowColor(hopR,hopG,hopB);

                currentHopId = hopId;
                colorUpdated = true;

                delay(2000);

                // Send it to the next robot
                sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_COMM_OUT_SIMPLE);
                sprintf(tempString2, "{\"id\":\"%d\",\"msg\":\"%d %d %d %d\"}", ROBOT_ID, hopId+1, hopR, hopG, hopB);
                mqtt_publish(tempString1, tempString2, false);

            }
        }
    }else{
        Serial.printf("invalid msg format %d \n", formatLen);
    }
    delay(50);
}

// instruct to start the pattern
void pattern_start(){
    patternState = P_RUN;
    Serial.println("pattern: start");
    currentHopId = -1;
    colorUpdated = false;
    pixelShowColor(0,0,0);
}

// reset the pattern variables and state
void pattern_reset(){
    patternState = P_BEGIN;
    Serial.println("pattern: reset");
}

// stop the execution of the pattern
void pattern_stop(){
    patternState = P_WAIT;
    Serial.println("pattern: wait");
}
