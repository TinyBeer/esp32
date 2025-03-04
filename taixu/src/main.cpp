#include "Config.h"
#include "WiFiConnector.h"
#include "OTAHandler.h"
#include "NTPHandler.h"
#include <PubSubClient.h>

WiFiConnector wifiConnector(APSSID, APPassword); // 创建 WiFiConnector 实例
OTAHandler otaHandler;                           // 创建 OTAHandler 对象
WiFiClient espClient;
PubSubClient client(espClient);
NTPHandler ntpHandler("pool.ntp.org", 300);
int count = 0;

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
        count %= 6000;
        if (count == 0)
        {
            int currentHour = ntpHandler.getHour();
            Serial.print("current hour: ");
            Serial.println(currentHour);
            wifiConnector.setIdle(currentHour >= 10 && currentHour <= 22);
        }
        count++;
    }
    delay(10);
}
