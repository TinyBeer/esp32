#include "WiFiConnector.h"

void WiFiConnector::connect(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
}

String WiFiConnector::getIPAddress()
{
    return WiFi.localIP().toString();
}