#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "Config.h"
#include "img.h"

// 使用软件 I2C 方式初始化 U8g2 对象
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/PIN_I2C_SCL, /* data=*/PIN_I2C_SDA, /* reset=*/OLED_RESET);
void setup()
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr); // 设置字体
  u8g2.clearBuffer();                 // 清除屏幕缓冲区
}
void loop()
{
  for (int i = 0; i < epd_bitmap_allArray_LEN; i++)
  {
    u8g2.firstPage();
    do
    {
      // 绘制一个与屏幕大小相同的填充矩形
      u8g2.drawXBM(0, 0, 128, 64, epd_bitmap_allArray[i]);
    } while (u8g2.nextPage());
    delay(2000);
  }

  for (int i = 0; i < 8; i++)
  {
    // 显示文字
    u8g2.firstPage();
    do
    {
      u8g2.drawStr(0, 8 * i, "2This is some text after images.");
    } while (u8g2.nextPage());
    delay(2000); // 文字显示的时间，单位为毫秒
  }
}