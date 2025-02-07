#include "TimerHandler.h"

void TimerHandler::start()
{
    timerAlarmEnable(_timer);
}

void TimerHandler::stop()
{
    timerAlarmDisable(_timer);
}
