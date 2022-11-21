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
#define SERVICE_UUID "s1afc201-1fb5-459e-8fcc-c5c9c331914b"

// BLE Characteristic
#define CHARAC1_UUID "c1a1d466-344c-4be3-ab3f-189f80dd7518"
#define CHARAC2_UUID "c2a1d466-344c-4be3-ab3f-189f80dd7518"
#define CHARAC3_UUID "c3a1d466-344c-4be3-ab3f-189f80dd7518"
#define CHARAC4_UUID "c4a1d466-344c-4be3-ab3f-189f80dd7518"

BLECharacteristic charac_1(CHARAC1_UUID, BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ);
BLECharacteristic charac_2(CHARAC2_UUID, BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ);
BLECharacteristic charac_3(CHARAC3_UUID, BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ);
BLECharacteristic charac_4(CHARAC4_UUID, BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ);

BLEDescriptor descrip_1(BLEUUID((uint16_t)0x2902));
BLEDescriptor descrip_2(BLEUUID((uint16_t)0x2902));
BLEDescriptor descrip_3(BLEUUID((uint16_t)0x2902));
BLEDescriptor descrip_4(BLEUUID((uint16_t)0x2902));

//Variables
int devicesConnected = 0;
bool startAdvertize = true;
BLEServer *pServer;
	
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    devicesConnected++;
    if (devicesConnected < 4)
    {
      startAdvertize = true;
    }
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
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Services
  BLEService *service_1 = pServer->createService(SERVICE_UUID);

  service_1->addCharacteristic(&charac_1);
  service_1->addCharacteristic(&charac_2);
  service_1->addCharacteristic(&charac_3);
  service_1->addCharacteristic(&charac_4);

  descrip_1.setValue("charac 1");
  charac_1.addDescriptor(&descrip_1);
  descrip_2.setValue("charac 2");
  charac_2.addDescriptor(&descrip_2);
  descrip_3.setValue("charac 3");
  charac_3.addDescriptor(&descrip_3);
  descrip_4.setValue("charac 4");
  charac_4.addDescriptor(&descrip_4);

  service_1->start();

  // Start advertising services
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  
}

unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
unsigned long time4 = 0;
static uint8_t mode1 = 0;
static uint8_t mode2 = 0;
static uint8_t mode3 = 0;
static uint8_t mode4 = 0;

void loop()
{
  if (startAdvertize)
  {
    pServer->getAdvertising()->start();
    startAdvertize = false;
  }

  if (devicesConnected > 0)
  {
    //charac 1 blinks at 0.25sec
    if ((millis() - time1) > 1000) {
      mode1 = !mode1; 
      charac_1.setValue(&mode1, 1);
      charac_1.notify();
      time1 = millis();
    }
    //charac 2 blinks at 0.5sec
    if ((millis() - time2) > 2000) {
      mode2 = !mode2; 
      charac_2.setValue(&mode2, 1);
      charac_2.notify();
      time2 = millis();
    }
    //charac 3 blinks at 1sec
    if ((millis() - time3) > 3000) {
      mode3 = !mode3; 
      charac_3.setValue(&mode3, 1);
      charac_3.notify();
      time3 = millis();
    }
    //charac 4 blinks at 2sec
    if ((millis() - time4) > 4000) {
      mode4 = !mode4; 
      charac_4.setValue(&mode4, 1);
      charac_4.notify();
      time4 = millis();
    }
  }
}



