#ifndef WIFICONNECTOR_H
#define WIFICONNECTOR_H

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>

const int Pin_WiFi_LED = 2;
const uint64_t flush_interval_ms = 500;
class WiFiConnector
{
public:
    WiFiConnector(const char *apSSID, const char *apPassword);
    void begin();
    void reset();
    void loop();
    String ip();
    bool isConfigMode();

private:
    const char *apSSID;
    const char *apPassword;
    DNSServer dnsServer;
    WebServer server;
    Preferences preferences;
    bool configMode;
    hw_timer_t *timer;

    void enableConfigPage();
    void disableConfigPage();
    bool connectFromSavedConfigs();
    void saveNewWiFiConfig(const String &ssid, const String &password);
    String generateConfigPage(int networkCount);
    void handleRoot();
    void handleScan();
    void handleConnect();
    void handleNotFound();
};

#endif