#include <Arduino.h>
#include "Config.h"
#include "WiFiConnector.h"
#include "OTAHandler.h"

WiFiConnector wifiConnector; // 创建 WiFiConnector 对象
OTAHandler otaHandler;       // 创建 OTAHandler 对象

void setup()
{
  Serial.begin(115200);

  // 连接 Wi-Fi
  wifiConnector.connect(WIFI_SSID, WIFI_PASSWORD);

  // 初始化 OTA
  otaHandler.begin(OTA_PASSWORD);
}

void loop()
{
  // 处理 OTA 事件
  otaHandler.handle();
}