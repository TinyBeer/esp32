#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "Config.h"

// 使用软件 I2C 方式初始化 U8g2 对象
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/PIN_I2C_SCL, /* data=*/PIN_I2C_SDA, /* reset=*/OLED_RESET);

void setup()
{
  // 初始化串口通信，波特率为 115200
  Serial.begin(115200);

  // 初始化 U8g2 库
  u8g2.begin();

  // 设置字体
  u8g2.setFont(u8g2_font_ncenB08_tr);
}

void loop()
{
  // 清空屏幕缓冲区
  u8g2.firstPage();
  do
  {
    // 在屏幕上绘制文本
    u8g2.drawStr(0, 20, "Hello, ESP32!");
    u8g2.drawStr(0, 40, "SSD1306 OLED");
  } while (u8g2.nextPage());

  // 延时 2 秒
  delay(2000);
}
