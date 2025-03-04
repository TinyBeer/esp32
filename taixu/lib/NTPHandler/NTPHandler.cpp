#include "NTPHandler.h"

NTPHandler::NTPHandler(const char *server, const int sync_second)
    : _sync_interval(sync_second * 1000), timeClient(ntpUDP, server)
{
}

int NTPHandler::getHour()
{
    // 初始化 NTP 客户端
    timeClient.begin();
    timeClient.setTimeOffset(28800); // 设置时区偏移量，北京时间为 UTC+8，即 8 * 3600 = 28800 秒
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= _sync_interval)
    {
        previousMillis = currentMillis;

        // 更新 NTP 时间
        timeClient.update();
    }
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);
    int currentHour = ptm->tm_hour;
    return currentHour;
}
