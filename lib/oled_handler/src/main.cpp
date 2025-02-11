#include <Arduino.h>
#include "Config.h"
#include "OLED_Handler.h"
#include "img.h"

OLED_Handler oled(/* clock=*/PIN_I2C_SCL, /* data=*/PIN_I2C_SDA, /* reset=*/OLED_RESET);
void setup()
{
}
void loop()
{
  for (int i = 0; i < epd_bitmap_allArray_LEN; i++)
  {
    oled.drawPicture(epd_bitmap_allArray[i]);
    delay(5000);
  }
  oled.flushScreen();
  oled.writeString(String("IP Adress:"), 1);
  oled.writeString(String("255.255.255.255"), 2);
  delay(5000); // 文字显示的时间，单位为毫秒
}