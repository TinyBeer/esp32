#include "ServoHandler.h"
#include "Config.h"

// 创建一个舵机对象
ServoHandler servo(Pin_Servo_PWM);

void setup()
{
  servo.write(90);
  delay(5000);
}

void loop()
{

  // 从 0 度旋转到 180 度
  for (int angle = 45; angle <= 135; angle++)
  {
    servo.write(angle);
  }

  // 延迟 1 秒
  delay(1000);

  // 从 180 度旋转到 0 度
  for (int angle = 135; angle >= 45; angle--)
  {
    servo.write(angle);
  }

  // 延迟 1 秒
  delay(1000);
}
