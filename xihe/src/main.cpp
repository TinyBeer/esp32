#include "Config.h"
#include "LEDHandler.h"
#include "WiFiConnector.h"
#include "OTAHandler.h"
#include "OLEDHandler.h"
#include "ServoHandler.h"
#include <PubSubClient.h>

LEDHandler ledHandler(PIN_LED);
WiFiConnector wifiConnector(APSSID, APPassword); // 创建 WiFiConnector 实例
OTAHandler otaHandler;                           // 创建 OTAHandler 对象
OLED_Handler oledHandler(/* clock=*/PIN_I2C_SCL, /* data=*/PIN_I2C_SDA, /* reset=*/OLED_RESET);
ServoHandler servoHandler(Pin_Servo_PWM); // 创建一个舵机对象
bool screenUpdated = false;

WiFiClient espClient;
PubSubClient client(espClient);

void msgHandler(String msg)
{
    if (msg == "on")
    {
        ledHandler.on();
        servoHandler.write(90);
    }
    else if (msg == "off")
    {
        ledHandler.off();
        servoHandler.write(180);
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
    otaHandler.begin();
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
        if (!screenUpdated)
        {
            screenUpdated = true;
            oledHandler.writeString(String("IP Adress:"), 1);
            oledHandler.writeString(wifiConnector.ip(), 2);
        }
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();
    }
}
