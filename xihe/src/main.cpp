#include "Config.h"
#include "ServoHandler.h"
#include <esp_sleep.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// 转换为 128 位 UUID
BLEUUID SERVICE_UUID = BLEUUID((uint16_t)SERVICE_UUID_16);
BLEUUID CHARACTERISTIC_UUID_READ = BLEUUID((uint16_t)CHARACTERISTIC_UUID_16_READ);
BLEScan *pBLEScan;               // 扫描结果
std::string targetDeviceAddress; // 目标设备地址
bool targetDeviceFound = false;
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks // 扫描回调函数
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        Serial.println(advertisedDevice.haveServiceUUID());
        Serial.println(advertisedDevice.isAdvertisingService(SERVICE_UUID));
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(SERVICE_UUID))
        {
            targetDeviceFound = true;
            targetDeviceAddress = advertisedDevice.getAddress().toString();
            pBLEScan->stop(); // 找到目标设备，停止扫描
        }
    }
};

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
    esp_sleep_enable_timer_wakeup(5 * 1000000);
    BLEDevice::init(BLE_Client);
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(ScanSecond); // 扫描 5 秒

    if (targetDeviceFound)
    {
        BLEClient *pClient = BLEDevice::createClient();
        BLEAddress targetAddress(targetDeviceAddress);
        if (pClient->connect(targetAddress))
        {
            BLERemoteService *pRemoteService = pClient->getService(SERVICE_UUID);
            if (pRemoteService != nullptr)
            {
                BLERemoteCharacteristic *pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_READ);
                if (pRemoteCharacteristic != nullptr)
                {
                    if (pRemoteCharacteristic->canRead())
                    {
                        std::string value = pRemoteCharacteristic->readValue();
                        msgHandler(String(value.c_str()));
                    }
                }
            }
            pClient->disconnect();
        }
    }
    delay(500);
    digitalWrite(PIN_LED, HIGH);
    esp_deep_sleep_start();
}

void loop()
{
}
