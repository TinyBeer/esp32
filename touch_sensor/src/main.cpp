#include <Arduino.h>

// 定义触摸引脚，这里使用T0，对应GPIO4
const int touchPin = T0;
// 触摸阈值，可根据实际情况调整
const int touchThreshold = 40;
// 单击和双击的最大间隔时间（毫秒）
const unsigned long doubleClickInterval = 300;
// 触摸最短持续时间（毫秒），用于过滤短时间误触
const unsigned long pressDuration = 50;

// 记录上一次触摸结束的时间
unsigned long lastTouchEndTime = 0;
// 记录当前触摸开始的时间
unsigned long touchStartTime = 0;
// 标记当前是否正在触摸
bool isTouching = false;
// 标记是否检测到单击事件
bool singleClickDetected = false;
// 标记是否检测到双击事件
bool doubleClickDetected = false;

// 检测是否触摸的函数
bool isTouched()
{
  return (touchRead(touchPin) < touchThreshold);
}

void setup()
{
  Serial.begin(115200);
  // 初始化触摸引脚中断
  touchAttachInterrupt(touchPin, []() {}, touchThreshold);
}

void loop()
{
  if (isTouched())
  {
    if (!isTouching)
    {
      // 触摸开始，记录开始时间
      touchStartTime = millis();
      isTouching = true;
    }
  }
  else
  {
    if (isTouching)
    {
      // 触摸结束，计算触摸持续时间
      unsigned long touchDuration = millis() - touchStartTime;
      if (touchDuration >= pressDuration)
      {
        unsigned long currentTime = millis();
        if (currentTime - lastTouchEndTime <= doubleClickInterval)
        {
          // 满足双击时间间隔，判定为双击
          doubleClickDetected = true;
          singleClickDetected = false; // 取消可能的单击标记
          lastTouchEndTime = 0;
        }
        else
        {
          // 不满足双击时间间隔，先标记可能是单击
          singleClickDetected = true;
          lastTouchEndTime = currentTime;
        }
      }
      isTouching = false;
    }
  }

  // 处理双击事件
  if (doubleClickDetected)
  {
    Serial.println("Double click detected!");
    doubleClickDetected = false;
  }

  // 处理单击事件
  if (singleClickDetected && (millis() - lastTouchEndTime > doubleClickInterval))
  {
    Serial.println("Single click detected!");
    singleClickDetected = false;
  }

  delay(10);
}