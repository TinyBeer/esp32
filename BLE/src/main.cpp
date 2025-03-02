#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// 定义 LED 引脚
const int ledPin = 2;

// 服务 UUID 和特征 UUID
// 16 位短 UUID
#define SERVICE_UUID_16 0x9001
#define CHARACTERISTIC_UUID_16_READ 0x9002
// 转换为 128 位 UUID
BLEUUID SERVICE_UUID = BLEUUID((uint16_t)SERVICE_UUID_16);
BLEUUID CHARACTERISTIC_UUID_READ = BLEUUID((uint16_t)CHARACTERISTIC_UUID_16_READ);

// 特征值
BLECharacteristic *pCharacteristicRead;
bool deviceConnected = false;
bool ledState = false;

// 特征读取回调函数
class MyReadCallbacks : public BLECharacteristicCallbacks
{
  void onRead(BLECharacteristic *pCharacteristic)
  {
    // 反转 LED 状态
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);

    // 更新读取特征的值
    pCharacteristic->setValue(ledState ? "1" : "0");
    Serial.print("LED state reversed. New state: ");
    Serial.println(ledState ? "ON" : "OFF");
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

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // 设置 LED 引脚为输出模式

  // 创建 BLE 设备
  BLEDevice::init("Taixu");

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
  pCharacteristicRead->setValue(ledState ? "1" : "0");

  // 启动服务
  pService->start();

  // 开始广播
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0); // 设置最小连接间隔
  BLEDevice::startAdvertising();
  Serial.println("Waiting for a client connection...");
}

void loop()
{
  if (deviceConnected)
  {
    // 可以在这里添加其他逻辑
  }
  delay(100);
}