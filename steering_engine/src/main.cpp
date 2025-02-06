#include <Arduino.h>
#include <ESP32Servo.h>

Servo myServo; // 创建舵机对象

void setup()
{
  myServo.attach(13); // 将舵机连接到 GPIO13
}

void loop()
{
  myServo.write(0);   // 将舵机转到 0 度
  delay(1000);        // 等待 1 秒
  myServo.write(180); // 将舵机转到 180 度
  delay(1000);        // 等待 1 秒
}