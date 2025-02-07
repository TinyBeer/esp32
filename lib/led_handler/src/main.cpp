#include "LEDHandler.h"
#include "Config.h"
LEDHandler led(PIN_LED);

void setup()
{
    led.on();
}

void loop()
{
    // 主循环可以处理其他任务
    delay(100);
}
