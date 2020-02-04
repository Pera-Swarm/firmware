
#ifdef ENABLE_NEOPIXEL_RING

void beginLED() {
   strip.begin();
   strip.setBrightness(NEOPIXEL_BRIGHTNESS);
   strip.clear();
   strip.show();
   Serial.println(F(">> NeoPixel\t:enabled"));
}
void neoPixelTest() {
   // Fill along the length of the strip in various colors...
   colorWipe(strip.Color(255,   0,   0)     , 50); // Red
   colorWipe(strip.Color(  0, 255,   0)     , 50); // Green
   colorWipe(strip.Color(  0,   0, 255)     , 50); // Blue
   colorWipe(strip.Color(  0,   0,   0, 255), 50); // True white (not RGB white)
   whiteOverRainbow(75, 5);
   pulseWhite(5);
   rainbowFade2White(3, 3, 1);
}

void pixelShowColor(int r, int g, int b) {
   strip.clear();
   strip.fill(strip.Color(r, g, b), 0, NEOPIXEL_LED_COUNT);  // // fill(uint32_t c=0, uint16_t first=0, uint16_t count=0);
   delay(500);
   strip.show();
}
void pixelColorWave(int r, int g, int b) {

   strip.clear();
   for (int i = 0; i < NEOPIXEL_LED_COUNT / 2; i++) { // For each pixel...
      strip.setPixelColor(i, strip.Color(r, g, b));
      strip.setPixelColor(NEOPIXEL_LED_COUNT - 1 - i, strip.Color(r, g, b));
      strip.show();
      delay(DELAYVAL);
   }
   delay(DELAYVAL * 5);
}
void colorWipe(uint32_t color, int wait) {
   for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
      strip.show();                          //  Update strip to match
      delay(wait);                           //  Pause for a moment
   }
}
void pixelOff() {
   strip.clear();
   delay(500);
   strip.show();
}

// Functions written for testing the performance of the LEDs
void whiteOverRainbow(int whiteSpeed, int whiteLength) {

   if (whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

   int      head          = whiteLength - 1;
   int      tail          = 0;
   int      loops         = 3;
   int      loopNum       = 0;
   uint32_t lastTime      = millis();
   uint32_t firstPixelHue = 0;

   for (;;) {
      for (int i = 0; i < strip.numPixels(); i++) {
         if (((i >= tail) && (i <= head)) ||
         ((tail > head) && ((i >= tail) || (i <= head)))) {
            strip.setPixelColor(i, strip.Color(0, 0, 0, 255));
         } else {
            int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
            strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
         }
      }
      strip.show();
      firstPixelHue += 40;

      if ((millis() - lastTime) > whiteSpeed) {
         if (++head >= strip.numPixels()) {
            head = 0;
            if (++loopNum >= loops) return;
         }
         if (++tail >= strip.numPixels()) {
            tail = 0;
         }
         lastTime = millis();
      }
   }
}
void pulseWhite(uint8_t wait) {
   for (int j = 0; j < 256; j++) {
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
      delay(wait);
   }

   for (int j = 255; j >= 0; j--) {
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
      delay(wait);
   }
}
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
   int fadeVal = 0, fadeMax = 100;
   for (uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops * 65536;
      firstPixelHue += 256) {

         for (int i = 0; i < strip.numPixels(); i++) {
            uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
            strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
         }

         strip.show();
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

   #elif defined(ENABLE_NEOPIXEL_RING_2)


   void beginLED() {
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
         for (led = 0; led < NEOPIXEL_LED_COUNT; led++) {
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
   void pulseWhite(uint8_t wait) {}
   void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {}


   #else

   void beginLED() {
      Serial.println(F(">> NeoPixel\t:enabled"));
   }
   void neoPixelTest() {}

   void pixelShowColor(int r, int g, int b) {}
   void pixelColorWave(int r, int g, int b) {}
   void colorWipe(uint32_t color, int wait) {}
   void pixelOff() {}

   void whiteOverRainbow(int whiteSpeed, int whiteLength) {}
   void pulseWhite(uint8_t wait) {}
   void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {}

   #endif
