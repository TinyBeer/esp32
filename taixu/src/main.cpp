#include "Config.h"
#include "WiFiConnector.h"
// #include "OTAHandler.h"
#include <PubSubClient.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

WiFiConnector wifiConnector(APSSID, APPassword); // 创建 WiFiConnector 实例
// OTAHandler otaHandler;                           // 创建 OTAHandler 对象
WiFiClient espClient;
PubSubClient client(espClient);

// 转换为 128 位 UUID
BLEUUID SERVICE_UUID = BLEUUID((uint16_t)SERVICE_UUID_16);
BLEUUID CHARACTERISTIC_UUID_READ = BLEUUID((uint16_t)CHARACTERISTIC_UUID_16_READ);

// 特征值
BLECharacteristic *pCharacteristicRead;
bool deviceConnected = false;

// 特征读取回调函数
class MyReadCallbacks : public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic *pCharacteristic)
    {
        pCharacteristic->setValue("keep");
    }
};

// 客户端连接和断开连接的回调函数
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        pServer->startAdvertising();
    }
};

void msgHandler(String msg)
{
    if (msg == "on")
    {
        pCharacteristicRead->setValue("on");
    }
    else if (msg == "off")
    {
        pCharacteristicRead->setValue("off");
    }
    else
    {
        Serial.printf("unsupported msg:%s\r\n", msg);
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (int i = 0; i < length; i++)
    {
        msg += (char)payload[i];
    }
    Serial.printf("Message arrived [%s]: %s\r\n", topic, msg);

    if (msgHandler)
    {
        msgHandler(msg);
    }
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(MQ_CLIENT_ID, "", ""))
        {
            Serial.println("connected");
            client.subscribe(MQ_TOPIC);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 1 seconds");
            delay(1000);
        }
    }
}

void setup()
{
    BLEDevice::init(BLE_Server);
    // 创建 BLE 服务器
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // 创建 BLE 服务
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // 创建用于读取的 BLE 特征
    pCharacteristicRead = pService->createCharacteristic(
        CHARACTERISTIC_UUID_READ,
        BLECharacteristic::PROPERTY_READ);
    pCharacteristicRead->setCallbacks(new MyReadCallbacks());
    pCharacteristicRead->setValue("keep");

    // 启动服务
    pService->start();

    // 开始广播
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // 设置最小连接间隔
    BLEDevice::startAdvertising();
    Serial.println("Waiting for a client connection...");

    wifiConnector.begin();
    // otaHandler.begin();
    client.setServer(MQ_SERVER, MQ_PORT);
    client.setCallback(callback);
}

void loop()
{
    wifiConnector.loop();
    if (wifiConnector.isConnected())
    {
        // 处理 OTA 事件
        // otaHandler.handle();
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();
    }
}
