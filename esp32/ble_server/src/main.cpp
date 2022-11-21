#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

/*************************
 * BLE : based on 
 * // https://randomnerdtutorials.com/esp32-ble-server-client/#ESP32-BLE-Server
 * 
 * Limits : max 4 clients per server, 3 clients by default
 * To add more clients, a change in the sdk must be made before compiling on a new computer. Follow the link below.
 * https://arduino.stackexchange.com/questions/73288/connect-more-than-4-ble-clients-to-esp32-ble-server
 * 
 * Helpful links :
 * uuid generator : https://www.uuidgenerator.net/
 * ***********************/

//General defines
#define ONBOARD_LED 2
#define BLE_SERVER_NAME "ESP32 BLE SERVER"

// BLE Services
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
BLECharacteristic charac_1("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ);
BLEDescriptor descrip_1(BLEUUID((uint16_t)0x2902));

//Variables
int devicesConnected = 0;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
	
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    devicesConnected++;
  };
  void onDisconnect(BLEServer* pServer) {
    devicesConnected--;
    if (devicesConnected < 0)
    {
      devicesConnected = 0;
    }
  }
};
	
void setup()
{
  //Create server
  BLEDevice::init(BLE_SERVER_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Services
  BLEService *service_1 = pServer->createService(SERVICE_UUID);
  service_1->addCharacteristic(&charac_1);
  descrip_1.setValue("charac 1");
  charac_1.addDescriptor(&descrip_1);
  service_1->start();

  // Start advertising services
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
}

bool test = true;

void loop()
{
  if (devicesConnected > 0)
  {
    if ((millis() - lastTime) > timerDelay) {

      if (test)
      {
        static const char* msg = "0";
        test = false;
        charac_1.setValue(msg);
      }
      else
      {
        static const char* msg = "1";
        test = true;
        charac_1.setValue(msg);
      }
      
      charac_1.notify();

      lastTime = millis();
    }
  }
}



