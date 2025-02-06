#ifndef CONFIG_H
#define CONFIG_H

// Wi-Fi 配置
const char *WIFI_SSID = "8hao";
const char *WIFI_PASSWORD = "00000000";

// OTA 配置
const char *OTA_PASSWORD = "admin";

// 巴法云配置
const char *MQ_SERVER = "bemfa.com";
const int MQ_PORT = 9501;
const char *MQ_USER = "";
const char *MQ_PWD = "";
const char *MQ_CLIENT_ID = "09b775b46e534b2bae8ef82106955e12";
const char *MQ_TOPIC = "led002";

const int keyPin = 0;
const int ledPin = 2;
const int servoPin = 13;

#endif