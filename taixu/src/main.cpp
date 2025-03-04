#include "Config.h"
#include "WiFiConnector.h"
#include "OTAHandler.h"
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiConnector wifiConnector(APSSID, APPassword); // 创建 WiFiConnector 实例
OTAHandler otaHandler;                           // 创建 OTAHandler 对象
WiFiClient espClient;
PubSubClient client(espClient);

// NTP 服务器设置
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const int syncInterval = 300000; // 同步间隔 5分钟

// 记录上一次 HTTP 请求的时间
unsigned long previousMillis = 0;

void msgHandler(String msg)
{
    if (msg == "on")
    {
        wifiConnector.setAction("on");
    }
    else if (msg == "off")
    {
        wifiConnector.setAction("off");
    }
    else
    {
        Serial.printf("unsupported msg:%s\r\n", msg);
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (int i = 0; i < length; i++)
    {
        msg += (char)payload[i];
    }
    Serial.printf("Message arrived [%s]: %s\r\n", topic, msg);

    if (msgHandler)
    {
        msgHandler(msg);
    }
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(MQ_CLIENT_ID, "", ""))
        {
            Serial.println("connected");
            client.subscribe(MQ_TOPIC);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 1 seconds");
            delay(1000);
        }
    }
}

void setup()
{
    wifiConnector.begin();
    // otaHandler.begin();
    client.setServer(MQ_SERVER, MQ_PORT);
    client.setCallback(callback);

    // 初始化 NTP 客户端
    timeClient.begin();
    timeClient.setTimeOffset(28800); // 设置时区偏移量，北京时间为 UTC+8，即 8 * 3600 = 28800 秒
}

void loop()
{
    wifiConnector.loop();
    if (wifiConnector.isConnected())
    {
        // 处理 OTA 事件
        otaHandler.handle();
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();

        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= syncInterval)
        {
            previousMillis = currentMillis;

            // 更新 NTP 时间
            timeClient.update();
            unsigned long epochTime = timeClient.getEpochTime();
            struct tm *ptm = gmtime((time_t *)&epochTime);
            int currentHour = ptm->tm_hour;
            Serial.print("current hour: ");
            Serial.println(currentHour);
            wifiConnector.setIdle(currentHour >= 10 && currentHour <= 20);
        }
    }
    delay(10);
}
