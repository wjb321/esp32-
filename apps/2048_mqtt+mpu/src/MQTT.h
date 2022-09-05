#ifndef MQTT_H
#define MQTT_H
#include "Arduino.h"
#include "game2048.h"
#include "lv_port_indev.h"

//extern const char *active_type_info[];

/* enum ACTIVE_TYPE
{
    TURN_RIGHT = 0,
    RETURN,
    TURN_LEFT,
    UP,
    DOWN,
    Enter_App,
    SHAKE,
    UNKNOWN
};
 */
/* struct MQTT_Action
{
    ACTIVE_TYPE active;
    boolean isValid;
    boolean long_time;
}; */

class MQTT
{     
//public: 
    //MQTT_Action action_info;
public:
    //MQTT();
    void setupMQTT(String MqttControlName);
    //MQTT_Action *MQTTAction(String rxload);
};

#endif