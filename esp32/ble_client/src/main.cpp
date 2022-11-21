#include <Arduino.h>
#include <BLEDevice.h>

bool connectToServer(BLEAddress pAddress);
static void characNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                    uint8_t* pData, size_t length, bool isNotify);

//States
#define STATE_DISCONNECTED 0
#define STATE_CONNECTION_VERIF 1
#define STATE_CONNECTED 2

#define ONBOARD_LED 2

#define SERVICE_UUID "s1afc201-1fb5-459e-8fcc-c5c9c331914b"

//#define CHARAC_UUID "c1a1d466-344c-4be3-ab3f-189f80dd7518"
// #define CHARAC_UUID "c2a1d466-344c-4be3-ab3f-189f80dd7518"
 #define CHARAC_UUID "c3a1d466-344c-4be3-ab3f-189f80dd7518"
// #define CHARAC_UUID "c4a1d466-344c-4be3-ab3f-189f80dd7518"
                               

//BLE Server name (the other ESP32 name running the server sketch)
#define BLE_SERVER_NAME "ESP32 BLE SERVER"

//Activate notify
const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

//Address of the peripheral device. Address will be found during scanning...
static BLEAddress *pServerAddress;
static BLEScan* pBLEScan;

/* UUID's of the service, characteristic that we want to read*/
static BLEUUID serviceUUID(SERVICE_UUID);
static BLEUUID characUUID(CHARAC_UUID);

//Characteristicd that we want to read
static BLERemoteCharacteristic* charac;

//General variables
static uint8_t state;
static uint8_t data[255] = {0};
static uint8_t dataLength = 0;
static bool newData = false;

//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 
{
  void onResult(BLEAdvertisedDevice advertisedDevice) 
  {
    if (advertisedDevice.getName() == BLE_SERVER_NAME) 
    { 
      //Check if the name of the advertiser matches
      advertisedDevice.getScan()->stop(); //Scan can be stopped, we found what we are looking for
      pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //Address of advertiser is the one we need
      state = STATE_CONNECTION_VERIF;
    }
  }
};

void setup()
{
  //Init BLE device
  BLEDevice::init("");
 
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());

  state = STATE_DISCONNECTED;

  pinMode(ONBOARD_LED,OUTPUT);
}

void loop() 
{
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (state == STATE_DISCONNECTED)
  {
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
  }
  else if (state == STATE_CONNECTION_VERIF)
  {
    if (connectToServer(*pServerAddress)) 
    {
      //Activate the Notify property of each Characteristic
      charac->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      state = STATE_CONNECTED;
    } 
  }
  else if (state == STATE_CONNECTED)
  {
    if (newData)
    {
      if (data[0] == 1)
      {
        digitalWrite(ONBOARD_LED,HIGH);
      }
      else 
      {
        digitalWrite(ONBOARD_LED,LOW);
      }
      newData = false;
    }
  }
}

//Connect to the BLE Server that has the name, Service, and Characteristics
bool connectToServer(BLEAddress pAddress) 
{
   BLEClient* pClient = BLEDevice::createClient();
 
  // Connect to the remove BLE Server.
  pClient->connect(pAddress);
 
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) 
  {
    return (false);
  }

  // Obtain a reference to the characteristics in the service of the remote BLE server.
  charac = pRemoteService->getCharacteristic(characUUID);
  if (charac == nullptr) 
  {
    return false;
  }
 
  //Assign callback functions for the Characteristics
  charac->registerForNotify(characNotifyCallback);
  return true;
}

//When the BLE Server sends a new reading with the notify property
static void characNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                    uint8_t* pData, size_t length, bool isNotify) 
{
  //store new value
  for (uint8_t i = 0; i < length; i++)
  {
    data[i] = pData[i];
  }
  dataLength = length;
  newData = true;
}