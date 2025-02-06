#ifndef WIFICONNECTOR_H
#define WIFICONNECTOR_H

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>

class WiFiConnector
{
public:
    WiFiConnector(const char *apSSID, const char *apPassword);
    void begin();
    void loop();

private:
    const char *apSSID;
    const char *apPassword;
    DNSServer dnsServer;
    WebServer server;
    Preferences preferences;

    bool connectFromSavedConfigs();
    void saveNewWiFiConfig(const String &ssid, const String &password);
    String generateConfigPage(int networkCount);
    void handleRoot();
    void handleScan();
    void handleConnect();
    void handleNotFound();
};

#endif