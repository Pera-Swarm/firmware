
#ifdef ENABLE_NEOPIXEL_RING

void beginNeoPixel() {
    neopixel.begin();
    neopixel.clear();
    neopixel.setBrightness(NEOPIXEL_BRIGHTNESS);
    neopixel.show();
    Serial.println(F(">> NeoPixel\t:enabled"));
}

void pixelUpdateLED(uint8_t i, int r, int g, int b){
    neopixel.setPixelColor(i, neopixel.Color(r, g, b));
    neopixel.show();
}
void neoPixelTest() {

    colorWipe(neopixel.Color(255,   0,   0)     , 50); // Red
    colorWipe(neopixel.Color(  0, 255,   0)     , 50); // Green
    colorWipe(neopixel.Color(  0,   0, 255)     , 50); // Blue
    whiteOverRainbow(75, 5);
    rainbowFade2White(3, 3, 1);
}

void pixelShowColor(int r, int g, int b) {

    if(r==0 && g==0 && b==0){
        pixelOff();
    }else{
        neopixel.clear();
        neopixel.fill(neopixel.Color(r, g, b), 0, NEOPIXEL_LED_COUNT);
        delay(500);
        neopixel.show();
    }
}
void pixelColorWave(int r, int g, int b) {

    neopixel.clear();
    for (int i = 1; i <= NEOPIXEL_LED_COUNT / 2; i++) {
        neopixel.setPixelColor(i, neopixel.Color(r, g, b));
        neopixel.setPixelColor(NEOPIXEL_LED_COUNT - 1 - i, neopixel.Color(r, g, b));
        neopixel.show();
        delay(DELAYVAL);
    }
    delay(DELAYVAL * 5);
    pixelOff();
}
void colorWipe(uint32_t color, int wait) {
    for (int i = 1; i < neopixel.numPixels(); i++) {
        neopixel.setPixelColor(i, color);         //  Set pixel's color (in RAM)
        neopixel.show();                          //  Update strip to match
        delay(wait);                           //  Pause for a moment
    }
}
void pixelOff() {
    neopixel.clear();
    neopixel.fill(neopixel.Color(0,0,0), 0, NEOPIXEL_LED_COUNT);
    delay(500);
    neopixel.show();
}

// Functions written for testing the performance of the LEDs
void whiteOverRainbow(int whiteSpeed, int whiteLength) {

    if (whiteLength >= neopixel.numPixels()) whiteLength = neopixel.numPixels() - 1;

    int      head          = whiteLength - 1;
    int      tail          = 0;
    int      loops         = 3;
    int      loopNum       = 0;
    uint32_t lastTime      = millis();
    uint32_t firstPixelHue = 0;

    for (;;) {
        for (int i = 1; i < neopixel.numPixels(); i++) {
            if (((i >= tail) && (i <= head)) ||
            ((tail > head) && ((i >= tail) || (i <= head)))) {
                neopixel.setPixelColor(i, neopixel.Color(255,255,255));
            } else {
                int pixelHue = firstPixelHue + (i * 65536L / neopixel.numPixels());
                neopixel.setPixelColor(i, neopixel.gamma32(neopixel.ColorHSV(pixelHue)));
            }
        }
        neopixel.show();
        firstPixelHue += 40;

        if ((millis() - lastTime) > whiteSpeed) {
            if (++head >= neopixel.numPixels()) {
                head = 0;
                if (++loopNum >= loops) return;
            }
            if (++tail >= neopixel.numPixels()) tail = 0;
            lastTime = millis();
        }
    }
}
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
    int fadeVal = 0, fadeMax = 100;
    for (uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops * 65536; firstPixelHue += 256) {
        for (int i = 1; i < neopixel.numPixels(); i++) {
            uint32_t pixelHue = firstPixelHue + (i * 65536L / neopixel.numPixels());
            neopixel.setPixelColor(i, neopixel.gamma32(neopixel.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
        }
        neopixel.show();
        delay(wait);

        if (firstPixelHue < 65536) {
            if (fadeVal < fadeMax) fadeVal++;
        } else if (firstPixelHue >= ((rainbowLoops - 1) * 65536)) {
            if (fadeVal > 0) fadeVal--;
        } else {
            fadeVal = fadeMax;
        }
    }
    delay(500);
}

// --------------------------------------------- end of ENABLE_NEOPIXEL_RING
#elif defined(ENABLE_NEOPIXEL_RING_RMT)
// This implementation is still under experiments

void beginNeoPixel() {
    if ((rmt_send2 = rmtInit(PIN_NEOPIXEL_LED, true, RMT_MEM_64)) == NULL) {
        Serial.println(">> PixelRing\t:ERROR");
    } else {
        Serial.println(">> PixelRing\t:enabled");
    }

    float realTick = rmtSetTick(rmt_send2, 100);
    Serial.printf("real tick set to: %fns\n", realTick);
}
void neoPixelTest() {

    int color[] =  { 0x55, 0x11, 0x77 };  // RGB value
    int led_index = 0;
    int led, col, bit;
    int i = 0;

    for (int k = 0; k < 20; k++) {
        for (led = 1; led < NEOPIXEL_LED_COUNT; led++) {
            for (col = 0; col < 3; col++ ) {
                for (bit = 0; bit < 8; bit++) {
                    if ( (color[col] & (1 << (7 - bit))) && (led == led_index) ) {
                        led_data[i].level0 = 1;
                        led_data[i].duration0 = 8;
                        led_data[i].level1 = 0;
                        led_data[i].duration1 = 4;
                    } else {
                        led_data[i].level0 = 1;
                        led_data[i].duration0 = 4;
                        led_data[i].level1 = 0;
                        led_data[i].duration1 = 8;
                    }
                    i++;
                }
            }
        }
        // make the led travel in the pannel
        if ((++led_index) >= NEOPIXEL_LED_COUNT) {
            led_index = 0;
        }

        // Send the data
        rmtWrite(rmt_send2, led_data, NEOPIXEL_LED_COUNT);

        delay(100);
    }
}

void pixelShowColor(int r, int g, int b) {}
void pixelColorWave(int r, int g, int b) {}
void colorWipe(uint32_t color, int wait) {}
void pixelOff() {}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {}
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {}

// ----------------------------------------- end of ENABLE_NEOPIXEL_RING_RMT
#else

void beginNeoPixel() {
    Serial.println(F(">> NeoPixel\t:disabled"));
}
void pixelUpdateLED(uint8_t i, int r, int g, int b){}
void neoPixelTest() {}

void pixelShowColor(int r, int g, int b) {}
void pixelColorWave(int r, int g, int b) {}
void colorWipe(uint32_t color, int wait) {}
void pixelOff() {}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {}
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {}

// ------------------------------------------------------------- end of else
#endif
