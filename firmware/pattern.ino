
enum pattern{P_BEGIN, P_WAIT, P_RUN };

int patternState = P_BEGIN;

// Loop in the pattern
void pattern_loop(){

    if(patternState==P_RUN){
        pattern_execute();

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
}

// functional part of the pattern
void pattern_execute(){
    Serial.println("pattern: execute");
    delay(500);
}

// instruct to start the pattern
void pattern_start(){
    patternState = P_RUN;
    Serial.println("pattern: start");
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
