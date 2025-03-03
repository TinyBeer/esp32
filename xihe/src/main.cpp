#include "Config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ServoHandler.h"
#include <esp_sleep.h>

ServoHandler servoHandler(Pin_Servo_PWM); // 创建一个舵机对象

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
    else
    {
        // keep status
    }
}

void setup()
{
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);
    Serial.begin(115200);

    // 连接WiFi
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Adress:");
    Serial.println(WiFi.localIP());
    if (WiFi.status() == WL_CONNECTED)
    {
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
    esp_sleep_enable_timer_wakeup(5 * 1000000);
}
void loop()
{
}
