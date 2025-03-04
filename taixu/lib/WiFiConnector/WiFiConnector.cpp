#include "WiFiConnector.h"

WiFiConnector::WiFiConnector(const char *apSSID, const char *apPassword)
    : apSSID(apSSID), apPassword(apPassword), server(80)
{
    pinMode(Pin_WiFi_LED, OUTPUT);
    digitalWrite(Pin_WiFi_LED, HIGH);
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, []()
                         { digitalWrite(Pin_WiFi_LED, !digitalRead(Pin_WiFi_LED)); }, true);
    timerAlarmWrite(timer, flush_interval_ms * 1000, true);
}

void WiFiConnector::begin()
{
    Serial.begin(115200);

    // 初始化 Preferences
    preferences.begin("wifi_config", false);
    preferences.end();
    delay(2000);
    // 启动 AP 模式
    WiFi.softAP(apSSID, apPassword);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // 为根路径、扫描请求和连接请求注册处理函数
    server.on("/", [this]()
              { this->handleRoot(); });
    server.on("/action", [this]()
              { this->handleAction(); });
    server.on("/connect", [this]()
              { this->handleConnect(); });
    server.onNotFound([this]()
                      { this->handleNotFound(); });

    // 启动 Web 服务器
    server.begin();
    Serial.println("Web server started");

    // 尝试从保存的配置中连接 WiFi
    if (!connectFromSavedConfigs())
    {
        enableConfigPage();
    }
    else
    {
        connected = true;
    }
    WiFi.setTxPower(WIFI_POWER_2dBm);
    WiFi.setSleep(WIFI_PS_MIN_MODEM);
}

void WiFiConnector::reset()
{
    preferences.begin("wifi_config", false);
    preferences.clear();
    preferences.end();
}

void WiFiConnector::loop()
{
    if (configMode)
    {
        // 处理 DNS 请求
        dnsServer.processNextRequest();
    }
    // 处理 Web 服务器请求
    server.handleClient();
}

String WiFiConnector::ip()
{
    return WiFi.localIP().toString();
}

bool WiFiConnector::isConfigMode()
{
    return configMode;
}

bool WiFiConnector::isConnected()
{
    return connected;
}

void WiFiConnector::setAction(String str)
{
    action = str;
}

void WiFiConnector::setIdle(bool flag)
{
    idle = flag;
}

void WiFiConnector::enableConfigPage()
{
    Serial.println("CONFIG MODE");
    // 启动 DNS 服务器
    dnsServer.start(53, "*", WiFi.softAPIP());

    configMode = true;
    connected = false;
    timerAlarmEnable(timer);
}

void WiFiConnector::disableConfigPage()
{
    configMode = false;
    dnsServer.stop();
    timerAlarmDisable(timer);
    digitalWrite(Pin_WiFi_LED, LOW);
}

bool WiFiConnector::connectFromSavedConfigs()
{
    preferences.begin("wifi_config", true);
    int configCount = preferences.getInt("config_count", 0);
    for (int i = 0; i < configCount; i++)
    {
        String ssidKey = "ssid_" + String(i);
        String pwdKey = "password_" + String(i);
        String ssid = preferences.getString(ssidKey.c_str(), "");
        String password = preferences.getString(pwdKey.c_str(), "");
        // if (!ssid.isEmpty() && !password.isEmpty())
        if (!ssid.isEmpty())
        {
            Serial.printf("try to connet ssid[%s] with password[%s]...\n", ssid, password);
            WiFi.begin(ssid.c_str(), password.c_str());
            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 10)
            {
                delay(1000);
                attempts++;
            }
            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.printf("Connected to saved WiFi: %s\nLocal IP: ", ssid);
                Serial.println(WiFi.localIP());
                digitalWrite(Pin_WiFi_LED, LOW);
                preferences.end();
                return true;
            }
        }
    }
    preferences.end();
    return false;
}

void WiFiConnector::saveNewWiFiConfig(const String &ssid, const String &password)
{
    preferences.begin("wifi_config", false);
    int configCount = preferences.getInt("config_count", 0);
    String ssidKey = "ssid_" + String(configCount);
    String pwdKey = "password_" + String(configCount);
    preferences.putString(ssidKey.c_str(), ssid);
    preferences.putString(pwdKey.c_str(), password);
    preferences.putInt("config_count", configCount + 1);
    preferences.end();
}

String WiFiConnector::generateConfigPage()
{
    String page = "<html>";
    page += "<head>";
    page += "<style>";
    page += "body {";
    page += "    display: flex;";
    page += "    justify-content: center;";
    page += "    align-items: center;";
    page += "    height: 100vh;";
    page += "    margin: 0;";
    page += "    font-size: 1.2em;";
    page += "}";
    page += "form {";
    page += "    border: 1px solid #ccc;";
    page += "    padding: 20px;";
    page += "    border-radius: 5px;";
    page += "    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);";
    page += "}";
    page += "input {";
    page += "    padding: 10px 20px;";
    page += "    font-size: 1em;";
    page += "    cursor: pointer;";
    page += "}";
    page += "input {";
    page += "    width: 100%;";
    page += "    padding: 10px;";
    page += "    margin-bottom: 15px;";
    page += "    border: 1px solid #ccc;";
    page += "    border-radius: 3px;";
    page += "}";
    page += "</style>";
    page += "</head>";
    page += "<body>";
    page += "<div>";
    page += "<h1>WiFi Configuration</h1>";
    page += "<form method='post' action='/connect'>";
    page += "SSID: <input type='text' name='ssid'><br>";
    page += "Password: <input type='password' name='password'><br>";
    page += "<input type='submit' value='Connect'>";
    page += "</form>";
    page += "</div>";
    page += "</body>";
    page += "</html>";
    return page;
}

void WiFiConnector::handleRoot()
{
    String page = generateConfigPage();
    server.send(200, "text/html", page);
}

void WiFiConnector::handleAction()
{
    server.send(200, "text/plain", action);
    if (idle)
    {
        action = "keep";
    }
    else
    {
        action = "keep_long";
    }
}

void WiFiConnector::handleConnect()
{
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
        String targetSSID = server.arg("ssid");
        String targetPassword = server.arg("password");

        WiFi.disconnect();
        WiFi.begin(targetSSID.c_str(), targetPassword.c_str());

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10)
        {
            delay(1000);
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            connected = true;
            saveNewWiFiConfig(targetSSID, targetPassword);
            String ip = WiFi.localIP().toString();
            server.send(200, "text/plain", "Connected to " + targetSSID + " IP: " + ip + "\r\nExit Config Mode in 5 second");
            delay(5000);
            Serial.printf("Connected to WiFi: %s IP: %s\n", targetSSID, ip.c_str());
            disableConfigPage();
        }
        else
        {
            server.send(200, "text/plain", "Failed to connect to " + targetSSID);
        }
    }
    else
    {
        server.send(400, "text/plain", "Missing SSID or password");
    }
}

void WiFiConnector::handleNotFound()
{
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}