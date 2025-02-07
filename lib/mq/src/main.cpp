#include <WiFi.h>
#include <PubSubClient.h>

// 替换为你的网络信息
const char *ssid = "102";
const char *password = "zhd691231";

// 替换为你的 MQTT 服务器信息
const char *mqttServer = "192.168.10.103";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";

// 定义要订阅的主题
const char *topic = "test";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  delay(10);
  // 连接到 WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  // 尝试重新连接到 MQTT 服务器
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // 创建一个随机的客户端 ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // 尝试连接
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword))
    {
      Serial.println("connected");
      // 订阅主题
      client.subscribe(topic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 等待 5 秒后重试
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}