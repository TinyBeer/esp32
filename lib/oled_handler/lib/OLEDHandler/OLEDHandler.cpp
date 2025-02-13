#include "OLEDHandler.h"

OLED_Handler::OLED_Handler(uint8_t clock, uint8_t data, uint8_t reset) : u8g2(U8G2_R0, clock, data, reset)
{
    u8g2.begin();
    u8g2.setFont(u8g2_font_ncenB10_tr); // 设置字体
    u8g2.clearBuffer();                 // 清除屏幕缓冲区
}

void OLED_Handler::writeString(String s, int line)
{
    if (line == 1)
    {
        u8g2.drawStr(0, 8 * 6, s.c_str());
    }
    else
    {
        u8g2.drawStr(0, 8 * 8, s.c_str());
    }
    u8g2.sendBuffer();
}

void OLED_Handler::flushScreen()
{
    // 清空屏幕
    u8g2.clearBuffer();
    u8g2.sendBuffer();
}

void OLED_Handler::drawPicture(const uint8_t *bitmap)
{
    u8g2.firstPage();
    do
    {
        // 绘制一个与屏幕大小相同的填充矩形
        u8g2.drawXBM(0, 0, 128, 64, bitmap);
    } while (u8g2.nextPage());
}
