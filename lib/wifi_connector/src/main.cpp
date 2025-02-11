#include "Config.h"
#include "WiFiConnector.h"

// 创建 WiFiConnector 实例
WiFiConnector wifiConnector(APSSID, APPassword);

void setup()
{
  wifiConnector.reset();
  wifiConnector.begin();
}

void loop()
{
  wifiConnector.loop();
  if (!wifiConnector.isConfigMode())
  {
    Serial.println(wifiConnector.ip());
    delay(1000);
  }
}