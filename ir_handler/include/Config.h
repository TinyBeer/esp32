#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// 定义按键引脚
const uint8_t PIN_KEY = 0;

// 开机
const uint64_t IRCode_Turn_ON = 0xEA80B0;
// 关机
const uint64_t IRCode_Turn_OFF = 0xEA80B0;
// 模式
const uint64_t IRCode_ShiftMode = 0xEA4070;

// 除湿
const uint64_t IRCode_Dehumidification = 0x3000007356;
// 制冷
const uint64_t IRCode_Refrigeration = 0x2000007656;
// 制热
const uint64_t IRCode_Heating = 0x1000007056;
// 自动
const uint64_t IRCode_Auto = 0x4000007456;
// 通风
const uint64_t IRCode_Ventilation = 0x5300007456;

#endif
