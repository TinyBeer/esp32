#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>

const int ledPin = 2;
const char *ssid = "102";
const char *password = "zhd691231";
const char *mqttServer = "192.168.10.102";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";
const char *topic = "light";
const char *activeTopic = "active";
String clientId = "ESP32Client-switch-1"; // 客户端 ID
hw_timer_t *timer = timerBegin(0, 80, true);
int status = 0;
// 0: 获取mq地址
// 1: 连接mq
// 2: 工作
bool need_broadcast = false;
bool need_active = false;

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP udp;
const int localPort = 4210;

void setup_led()
{
  pinMode(ledPin, OUTPUT);
}
void light_on()
{
  digitalWrite(ledPin, HIGH);
}

void light_off()
{
  digitalWrite(ledPin, LOW);
}

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
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());
  udp.begin(localPort);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String msg;
  for (int i = 0; i < length; i++)
  {
    msg += (char)payload[i];
  }
  Serial.printf("Message arrived [%s]: %s\r\n", topic, msg);

  if (msg == "ON")
  {
    light_on();
  }
  else if (msg == "OFF")
  {
    light_off();
  }
}

void reconnect()
{
  // 尝试重新连接到 MQTT 服务器
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
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

void broadcast_ip()
{
  String msg = "hello";
  udp.beginPacket(IPAddress(255, 255, 255, 255), 8888);
  udp.write((uint8_t *)msg.c_str(), msg.length());
  udp.endPacket();
  Serial.println("Broadcast sent");
}

void IRAM_ATTR timerISR()
{
  String broadcastStr = "false";
  String activeStr = "false";
  if (status == 0)
  {
    need_broadcast = true;
    broadcastStr = "true";
  }
  else
  {
    need_active = true;
    activeStr = "true";
  }

  Serial.printf("定时器触发 status=%d braodcast=%s active=%s\r\n", status, broadcastStr, activeStr);
}
void setup_timer()
{
  timerAlarmWrite(timer, 5000000, true);
  timerAttachInterrupt(timer, &timerISR, true);
  timerAlarmEnable(timer);
}

void setup()
{
  setup_led();
  Serial.begin(115200);
  setup_wifi();
  setup_timer();
}

void loop()
{
  switch (status)
  {
  case 0:
    if (need_broadcast)
    {
      broadcast_ip();
      need_broadcast = false;
    }
    break;
  case 1:
    if (!client.connected())
    {
      reconnect();
    }
    if (need_active)
    {
      client.publish(activeTopic, clientId.c_str());
      need_active = false;
    }
    client.loop();
    break;
  }
  delay(1);

  char incomingPacket[255];
  int packetSize = udp.parsePacket();
  if (packetSize > 0)
  {
    IPAddress remoteIP = udp.remoteIP();
    int remotePort = udp.remotePort();
    udp.read(incomingPacket, packetSize);
    Serial.printf("Received packet from %s:%d with length %d:\r\n", remoteIP.toString().c_str(), remotePort, packetSize);
    Serial.println(incomingPacket);
    mqttServer = remoteIP.toString().c_str();
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    status = 1;
  }
}