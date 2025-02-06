#include "OTAHandler.h"

void OTAHandler::begin(const char *otaPassword)
{
    // 初始化 OTA
    ArduinoOTA.setPort(3232); // 设置 OTA 端口
    if (otaPassword)
    {
        ArduinoOTA.setPassword(otaPassword); // 设置 OTA 密码（可选）
    }

    ArduinoOTA
        .onStart([]()
                 {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH) {
                type = "sketch";
            } else { // U_SPIFFS
                type = "filesystem";
            }
            Serial.println("Start updating " + type); })
        .onEnd([]()
               { Serial.println("\nEnd"); })
        .onProgress([](unsigned int progress, unsigned int total)
                    { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
        .onError([](ota_error_t error)
                 {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

    ArduinoOTA.begin();
    Serial.println("OTA Ready");
}

void OTAHandler::handle()
{
    ArduinoOTA.handle();
}