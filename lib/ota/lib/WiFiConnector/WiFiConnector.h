#ifndef WIFICONNECTOR_H
#define WIFICONNECTOR_H

#include <WiFi.h>

class WiFiConnector
{
public:
    // 连接 Wi-Fi
    void connect(const char *ssid, const char *password);

    // 获取当前 IP 地址
    String getIPAddress();
};

#endif