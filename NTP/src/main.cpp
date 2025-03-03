#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// 替换为你的 Wi-Fi 网络信息
const char *ssid = "8hao";
const char *password = "00000000";

// NTP 服务器设置
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// 不同时间段的休眠间隔
const int normalInterval = 5000;    // 正常休眠间隔 5 秒
const int extendedInterval = 30000; // 延长休眠间隔 30 秒

// 记录上一次 HTTP 请求的时间
unsigned long previousMillis = 0;

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

  // 初始化 NTP 客户端
  timeClient.begin();
  timeClient.setTimeOffset(28800); // 设置时区偏移量，北京时间为 UTC+8，即 8 * 3600 = 28800 秒

  // 获取当前时间
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int currentHour = ptm->tm_hour;
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= normalInterval)
  {
    previousMillis = currentMillis;

    // 更新 NTP 时间
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);
    int currentHour = ptm->tm_hour;
    Serial.print("current hour: ");
    Serial.println(currentHour);
  }
}
