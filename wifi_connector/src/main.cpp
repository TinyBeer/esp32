#include "Config.h"
#include "WiFiConnector.h"

// 创建 WiFiConnector 实例
WiFiConnector wifiConnector(APSSID, APPassword);

void setup()
{
  wifiConnector.begin();
}

void loop()
{
  wifiConnector.loop();
}