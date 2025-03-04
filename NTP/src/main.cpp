#include <WiFi.h>
#include "NTPHandler.h"

// 替换为你的 Wi-Fi 网络信息
const char *ssid = "8hao";
const char *password = "00000000";

NTPHandler ntpHandler("pool.ntp.org", 300);

void setup()
{
  Serial.begin(115200);

  // 连接到 Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // 设置 Wi-Fi 为低功耗模式
  WiFi.setSleep(true);
}

void loop()
{

  int currentHour = ntpHandler.getHour();
  Serial.print("current hour: ");
  Serial.println(currentHour);
  delay(3000);
}
