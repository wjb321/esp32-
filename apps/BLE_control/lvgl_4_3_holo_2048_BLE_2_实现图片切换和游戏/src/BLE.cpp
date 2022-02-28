#include "BLE.h"
#include "game2048.h"
#include "common.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Arduino.h"
//#include <iostream.h>
//using namespace std;
//
/*here are basic steps for configuring the BLE
1) define the name
2) build a server
3) establish a call back function
4) start a service
5) start characteristics with notify and write*/
const char *active_type_info[] = {"TURN_RIGHT", "RETURN",
                                  "TURN_LEFT", "UP",
                                  "DOWN", "GO_FORWORD",
                                  "SHAKE", "UNKNOWN"};
bool deviceConnected = false;
bool oldDeviceConnected = false;             //the state of last connection
BLEServer *pServer = NULL; 
uint8_t txValue = 0;
String rxload="BlackWalnutLabs";
 
BLECharacteristic *pCharacteristic;
//extern String rxload;

BLE::BLE()
{
    action_info.isValid = true;
    action_info.active = UNKNOWN;
    action_info.long_time = true;
} 
 
/*realize the call back function and show it in serial also assign it to temp variable rxLoad*/
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue(); //call the function and get xValue
		if (rxValue.length() > 0) {
        rxload="";
        for (int i = 0; i < rxValue.length(); i++)
         {
          rxload +=(char)rxValue[i];
          Serial.print(rxValue[i]);
         }
         Serial.println("");
      }
    }
}; 
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
void BLE::setupBLE(String BLEName){
    
  const char *ble_name=BLEName.c_str();
  BLEDevice::init(ble_name);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX,BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

BLE_Action *BLE::BLEAction(String rxload)
{
 if (!action_info.isValid)
        {
            if (rxload == "L" || rxload == "l" )
            {
                action_info.isValid = 1;
                action_info.active = TURN_LEFT;
            }
            else if (rxload == "R" || rxload == "r")
            {
                action_info.isValid = 1;
                action_info.active = TURN_RIGHT;
            }
            else if (rxload == "U" || rxload == "u")
            {
                action_info.isValid = 1;
                action_info.active = UP;
            }
            else if (rxload == "D" || rxload == "d")
            {
                action_info.isValid = 1;
                action_info.active = DOWN;
            }
            else if (rxload == "s" || rxload == "S")
            {
                // 震动检测
                action_info.isValid = 1;
                action_info.active = SHAKE;
            }
             else if (rxload == "i" || rxload == "I")
            {
                // 震动检测
                action_info.isValid = 1;
                action_info.active = GO_FORWORD;
            }
            else if (rxload == "Q" || rxload == "q")
                {
                    action_info.isValid = 1;
                    action_info.active = RETURN;
                    //encoder_state = LV_INDEV_STATE_REL;
                }
            else
            {
                action_info.isValid = 0;
            }
            if (UNKNOWN != action_info.active)
            {
            action_info.active = (ACTIVE_TYPE)((action_info.active + 0) % UNKNOWN);
            }
    }
 return &action_info;
}
