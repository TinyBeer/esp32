#include "TouchSensor.h"

TouchSensor::TouchSensor(
    const int touchPin,
    const int touchThreshold,
    const unsigned long doubleClickInterval,
    const unsigned long pressDuration)
    : touchPin(touchPin),
      touchThreshold(touchThreshold),
      doubleClickInterval(doubleClickInterval),
      pressDuration(pressDuration)
{
    // 初始化触摸引脚中断
    touchAttachInterrupt(touchPin, []() {}, touchThreshold);
}

TouchSensor::ClickEnum TouchSensor::check()
{
    {
        if (isTouched())
        {
            if (!isTouching)
            {
                // 触摸开始，记录开始时间
                touchStartTime = millis();
                isTouching = true;
                delay(10);
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
            doubleClickDetected = false;
            return DoubleClick;
        }

        // 处理单击事件
        if (singleClickDetected && (millis() - lastTouchEndTime > doubleClickInterval))
        {
            singleClickDetected = false;
            return SingleClick;
        }
    }
    return NoClick;
}

bool TouchSensor::isTouched()
{
    return (touchRead(touchPin) < touchThreshold);
}
