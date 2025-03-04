#include "Config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ServoHandler.h"
#include <esp_sleep.h>

ServoHandler servoHandler(Pin_Servo_PWM); // 创建一个舵机对象
uint64_t sleep_second = 5;

void msgHandler(String msg)
{
    if (msg == "on")
    {
        servoHandler.write(35);
        delay(1000);
        servoHandler.write(90);
        delay(1000);
    }
    else if (msg == "off")
    {
        servoHandler.write(145);
        delay(1000);
        servoHandler.write(90);
        delay(1000);
    }
    else if (msg == "keep_long")
    {
        sleep_second = 10;
    }
}

void setup()
{
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);
    Serial.begin(115200);

    // 连接WiFi
    int attempt_times = 0;
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    while (attempt_times < 10 && WiFi.status() != WL_CONNECTED)
    {
        attempt_times++;
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    WiFi.setTxPower(WIFI_POWER_2dBm);
    WiFi.setSleep(true);
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to WiFi");
        Serial.print("IP Adress:");
        Serial.println(WiFi.localIP());

        HTTPClient http;
        // 开启HTTP连接
        http.begin(HTTP_HOST);
        // 发送HTTP GET请求
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0)
        {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
            msgHandler(payload);
        }
        http.end();
    }
    digitalWrite(PIN_LED, LOW);
    esp_sleep_enable_timer_wakeup(sleep_second * 1000000);
    esp_deep_sleep_start();
}
void loop()
{
}
