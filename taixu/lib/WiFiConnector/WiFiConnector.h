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
    bool isConnected();
    void setAction(String str);
    void setIdle(bool idle);

private:
    const char *apSSID;
    const char *apPassword;
    DNSServer dnsServer;
    WebServer server;
    Preferences preferences;
    bool configMode = false;
    bool connected;
    hw_timer_t *timer;
    bool idle = false;
    String action = "keep";

    void enableConfigPage();
    void disableConfigPage();
    bool connectFromSavedConfigs();
    void saveNewWiFiConfig(const String &ssid, const String &password);
    String generateConfigPage();
    void handleRoot();
    void handleAction();
    void handleConnect();
    void handleNotFound();
};

#endif