#ifndef TOUCHSENSOR_H
#define TOUCHSENSOR_H
#include <Arduino.h>

class TouchSensor
{
public:
    enum ClickEnum
    {
        NoClick,
        SingleClick,
        DoubleClick

    };
    // 构造函数，用于初始化触摸传感器的参数
    TouchSensor(
        const int touchPin = T0,
        const int touchThreshold = 40,
        const unsigned long doubleClickInterval = 300,
        const unsigned long pressDuration = 50);
    ClickEnum check();

private:
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
    bool isTouched();
};

#endif