#ifndef BLE_H
#define BLE_H
//#ifdef __cplusplus
//#include <iostream.h>
//using namespace std;
#include "Arduino.h"
#include "game2048.h"
#include "lv_port_indev.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

extern const char *active_type_info[];
extern BLECharacteristic *pCharacteristic;
extern bool deviceConnected;
extern bool oldDeviceConnected;             //the state of last connection
//extern uint8_t txValue = 0;
//extern String rxload="BlackWalnutLabs";
extern BLEServer *pServer;  
enum ACTIVE_TYPE
{
    TURN_RIGHT = 0,
    RETURN,
    TURN_LEFT,
    UP,
    DOWN,
    GO_FORWORD,
    SHAKE,
    UNKNOWN
};

struct BLE_Action
{
    ACTIVE_TYPE active;
    boolean isValid;
    boolean long_time;
};


class BLE
{     
public: 
    BLE_Action action_info;
public:
    BLE();
    void setupBLE(String BLEName);
    BLE_Action *BLEAction(String rxload);
};


//#endif // c++
#endif