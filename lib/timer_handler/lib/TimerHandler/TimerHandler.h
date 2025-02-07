#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include <Arduino.h>

class TimerHandler
{
public:
    TimerHandler(uint8_t timer, void (*fn)(), uint64_t intervalMs)
    {
        _timer = timerBegin(timer, 80, true);
        timerAttachInterrupt(_timer, fn, true);
        timerAlarmWrite(_timer, intervalMs * 1000, true);
        timerAlarmEnable(_timer);
    }
    ~TimerHandler()
    {
        timerEnd(_timer);
    }
    void start();
    void stop();

private:
    hw_timer_t *_timer;
};

#endif