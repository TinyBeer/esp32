#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "102";
const char *password = "zhd691231";
int32_t cnt = 0;

WiFiUDP udp;
const int localPort = 4210;

void broadcast_ip()
{
  // 要发送的广播消息
  String msg = "hello";
  // 发送UDP广播
  udp.beginPacket(IPAddress(255, 255, 255, 255), 8888);
  udp.write((uint8_t *)msg.c_str(), msg.length());
  udp.endPacket();

  Serial.println("Broadcast sent");
}

void setup()
{
  Serial.begin(115200);
  // 连接到WiFi网络
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to WiFi");
  udp.begin(localPort);
}

void loop()
{
  char incomingPacket[255];
  int packetSize = udp.parsePacket();
  if (packetSize > 0)
  {
    IPAddress remoteIP = udp.remoteIP();
    int remotePort = udp.remotePort();
    udp.read(incomingPacket, packetSize);
    Serial.printf("Received packet from %s:%d with length %d:\r\n", remoteIP.toString().c_str(), remotePort, packetSize);
    Serial.println(incomingPacket);
  }
  if (cnt > 5000)
  {
    broadcast_ip();
    cnt = 0;
  }
  cnt++;
  delay(1);
}