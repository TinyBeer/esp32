#ifndef OTAHANDLER_H
#define OTAHANDLER_H

#include <ArduinoOTA.h>

class OTAHandler
{
public:
    // 初始化 OTA
    void begin(const char *otaPassword = nullptr);

    // 处理 OTA 事件
    void handle();
};

#endif