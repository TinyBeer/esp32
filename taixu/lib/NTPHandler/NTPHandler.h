#ifndef NTP_HANDLER
#define NTP_HANDLER

#include <NTPClient.h>
#include <WiFiUdp.h>

class NTPHandler
{
public:
    NTPHandler(const char *server, const int sync_second);
    int getHour();

private:
    WiFiUDP ntpUDP;
    NTPClient timeClient;
    const int _sync_interval;
    unsigned long previousMillis = 0;
};

#endif