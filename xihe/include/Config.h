#ifndef CONFIG_H
#define CONFIG_H

// Pin Define
#define PIN_KEY 0
#define PIN_LED 2
#define Pin_Servo_PWM 12 // 舵机

// BLE
#define BLE_Server "Taixu"
#define BLE_Client "Xihe"

// 服务和特征 16 位短 UUID
#define SERVICE_UUID_16 0x9001
#define CHARACTERISTIC_UUID_16_READ 0x9002

#define ScanSecond 5

#endif