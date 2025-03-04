#ifndef CONFIG_H
#define CONFIG_H

// Pin Define
#define PIN_KEY 0
#define PIN_LED 2
#define Pin_Servo_PWM 12 // 舵机

#define LED_PWM_Channel 0
#define LED_PWM_Freq 5000
#define LED_PWM_Resolution 8
#define LED_PWM_Brightness_ON 10
#define LED_PWM_Brightness_OFF 0

// WiFi
#define WIFI_SSID "ESP32_Config_AP"
#define WIFI_PWD ""

// 要请求的URL
#define HTTP_HOST "http://192.168.4.1/action"

#endif