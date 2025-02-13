#ifndef OLEDHandler_H
#define OLEDHandler_H
#include <U8g2lib.h>
class OLED_Handler
{
public:
    OLED_Handler(uint8_t clock, uint8_t data, uint8_t reset);
    void writeString(String s, int line);
    void drawPicture(const uint8_t *bitmap);
    void flushScreen();

private:
    U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
};
#endif