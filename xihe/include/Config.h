#ifndef CONFIG_H
#define CONFIG_H

#define PIN_KEY 0
#define PIN_LED 2

// 服务 UUID 和特征 UUID
// 16 位短 UUID
#define SERVICE_UUID_16 0x9001
#define CHARACTERISTIC_UUID_16_READ 0x9002

// OLED
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22
#define OLED_RESET -1 // 定义复位引脚（-1 表示与 Arduino 复位引脚共用）
// 定义 OLED 屏幕的尺寸
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 舵机
#define Pin_Servo_PWM 12

const int keyPin = 0;
const int ledPin = 2;
const int servoPin = 13;

#endif