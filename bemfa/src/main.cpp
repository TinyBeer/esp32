#include <Arduino.h>
#include "Config.h"
#include "LEDHandler.h"
#include "WiFiConnector.h"
#include "OTAHandler.h"
#include <PubSubClient.h>
#include <ESP32Servo.h>

LEDHandler ledHandler;
WiFiConnector wifiConnector;
OTAHandler otaHandler;
Servo myServo; // 创建舵机对象

WiFiClient espClient;
PubSubClient client(espClient);

void msgHandler(String msg)
{
  if (msg == "on")
  {
    ledHandler.on();
    myServo.write(90);
  }
  else if (msg == "off")
  {
    ledHandler.off();
    myServo.write(180);
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

void setup()
{
  ledHandler.setup(ledPin);

  myServo.attach(servoPin);

  Serial.begin(115200);

  // 连接 Wi-Fi
  wifiConnector.connect(WIFI_SSID, WIFI_PASSWORD);

  // 初始化 OTA
  otaHandler.begin(OTA_PASSWORD);

  client.setServer(MQ_SERVER, MQ_PORT);
  client.setCallback(callback);
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

void loop()
{
  // 处理 OTA 事件
  otaHandler.handle();

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}