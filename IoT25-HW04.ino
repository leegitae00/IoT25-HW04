#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("BLE Device Connected");
  }

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("BLE Device Disconnected");
  }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work...");

  BLEDevice::init("202034072 이기태");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

)
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

pCharacteristic->setValue("Hello from ESP32 BLE");

pService->start();

BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
pAdvertising->start();

Serial.println("BLE advertising started. You can connect via BLE scanner app.");
}

void loop()
{

  if (deviceConnected && Serial.available())
  {
    String msg = Serial.readStringUntil('\n');
    pCharacteristic->setValue(msg.c_str());
    pCharacteristic->notify();
    Serial.print("Sent via BLE: ");
    Serial.println(msg);
  }

  delay(100);
}
