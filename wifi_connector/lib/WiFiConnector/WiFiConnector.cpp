#include "WiFiConnector.h"

WiFiConnector::WiFiConnector(const char *apSSID, const char *apPassword)
    : apSSID(apSSID), apPassword(apPassword), server(80) {}

void WiFiConnector::begin()
{
    Serial.begin(115200);

    // 初始化 Preferences
    preferences.begin("wifi_config", false);
    preferences.end();

    // 尝试从保存的配置中连接 WiFi
    if (!connectFromSavedConfigs())
    {
        // 启动 AP 模式
        WiFi.softAP(apSSID, apPassword);
        Serial.print("AP IP address: ");
        Serial.println(WiFi.softAPIP());
    }

    // 启动 DNS 服务器
    dnsServer.start(53, "*", WiFi.softAPIP());

    // 为根路径、扫描请求和连接请求注册处理函数
    server.on("/", [this]()
              { this->handleRoot(); });
    server.on("/scan", [this]()
              { this->handleScan(); });
    server.on("/connect", [this]()
              { this->handleConnect(); });
    server.onNotFound([this]()
                      { this->handleNotFound(); });

    // 启动 Web 服务器
    server.begin();
    Serial.println("Web server started");
}

void WiFiConnector::loop()
{
    // 处理 DNS 请求
    dnsServer.processNextRequest();
    // 处理 Web 服务器请求
    server.handleClient();
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
        if (!ssid.isEmpty() && !password.isEmpty())
        {
            WiFi.begin(ssid.c_str(), password.c_str());
            int attempts = 0;
            while (WiFi.status() != WL_CONNECTED && attempts < 10)
            {
                delay(1000);
                attempts++;
            }
            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println("Connected to saved WiFi: " + ssid);
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

String WiFiConnector::generateConfigPage(int networkCount)
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
    page += "button, input[type='submit'] {";
    page += "    padding: 10px 20px;";
    page += "    font-size: 1em;";
    page += "    cursor: pointer;";
    page += "}";
    page += "select, input[type='password'] {";
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
    page += "<button onclick=\"window.location.href='/scan'\">Refresh Scan</button><br><br>";
    page += "<form method='post' action='/connect'>";
    page += "Select WiFi: <select name='ssid'>";
    for (int i = 0; i < networkCount; ++i)
    {
        page += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
    }
    page += "</select><br>";
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
    int n = WiFi.scanNetworks();
    String page = generateConfigPage(n);
    server.send(200, "text/html", page);
}

void WiFiConnector::handleScan()
{
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
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
            saveNewWiFiConfig(targetSSID, targetPassword);
            server.send(200, "text/plain", "Connected to " + targetSSID);
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