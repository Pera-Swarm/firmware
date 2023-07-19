#include "display.h"

#ifdef ENABLE_SSD1306_DISPLAY

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

void beginDisplay()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_SSD1306))
    {
        Serial.println(F(">> SSD1306\t:allocation failed"));
        return;
    }

    Serial.println(F(">> SSD1306\t:enabled"));
    delay(250);
    display_clear();

    // dispaly_drawTestChar();
    // delay(2000);
    // display_clear();
    // display_print(0, "Hello");
    // display_print(1, "World");
}

void display_clear()
{
    display.clearDisplay();
}
void display_print(uint8_t line, String string_array)
{ // display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 20 + line * 20);
    display.println(string_array);
    display.display();
    Serial.printf("\t Display: %u > %s \n", line, string_array.c_str());
}
void display_print(uint8_t line, int number)
{ // display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 20 + line * 20);
    display.println(number);
    display.display();
    Serial.printf("\t Display: %u > %d \n", line, number);
}

void display_drawTest()
{
    display.drawCircle(display.width() / 2, display.height() / 2, 25, WHITE);
    display.drawLine(20, display.height() / 2, display.width() - 20, display.height() / 2, WHITE);
    display.drawLine(display.width() / 2, 0, display.width() / 2, display.height(), WHITE);
    display.display();
    delay(1);
}
void dispaly_drawTestChar()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);

    // Not all the characters will fit on the display. This is normal.
    // Library will draw what it can and the rest will be clipped.
    for (int16_t i = 0; i < 256; i++)
    {
        if (i == '\n')
            display.write(' ');
        else
            display.write(i);
    }

    display.display();
    delay(2000);
}

#else
// If SSD1306_DISPLAY isn't defined --------------------------------------------

void beginDisplay()
{
    Serial.println(F(">> SSD1306\t:disabled"));
}
void display_clear() {}
void display_print(uint8_t line, String string_array) {}
void display_print(uint8_t line, int number) {}

void display_drawTest() {}
void dispaly_drawTestChar() {}

#endif
