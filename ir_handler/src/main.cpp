#define IR_Transmiter
#define IR_Receiver

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "Config.h"

// 定义红外发射引脚
const uint16_t IR_PIN = 2;
// 创建 IRsend 对象
IRsend irsend(IR_PIN);

// 定义红外接收引脚
const int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;

// 定义按键引脚
const int BUTTON_PIN = 0;

// 定义长按时间阈值（单位：毫秒）
const unsigned long LONG_PRESS_TIME = 1000;

// 记录按键按下的时间
unsigned long pressStartTime = 0;

// 记录按键状态
bool isButtonPressed = false;

// 定义接收到的红外开机码
const uint32_t POWER_ON_CODE = 0xEA80B0;

void setup()
{
  Serial.begin(115200);
  irsend.begin();      // 初始化红外发射模块
  irrecv.enableIRIn(); // 初始化红外接收模块

  // 设置按键引脚为输入模式，并启用上拉电阻
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{

#ifdef IR_Receiver
  if (irrecv.decode(&results))
  {
    for (int i = 0; i < results.rawlen; i++)
    {
      Serial.print(results.rawbuf[i]);
      Serial.print(", ");
    }
    Serial.println(results.decode_type);
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
#endif

#ifdef IR_Transmiter
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW)
  { // 按键按下
    if (!isButtonPressed)
    { // 按键首次按下
      isButtonPressed = true;
      pressStartTime = millis(); // 记录按下时间
      irsend.sendNEC(POWER_ON_CODE, 24);
    }
    else
    { // 按键持续按下
      if (millis() - pressStartTime >= LONG_PRESS_TIME)
      { // 长按判断
        Serial.println("Long press detected, sending long press IR signal...");
        // 这里可以添加长按对应的红外信号发射代码
        irsend.sendNEC(POWER_ON_CODE, 24);
        while (digitalRead(BUTTON_PIN) == LOW)
          ; // 等待按键释放
        isButtonPressed = false;
      }
    }
  }
#endif
  delay(100);
}
