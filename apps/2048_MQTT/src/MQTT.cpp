#include "MQTT.h"
#include "game2048.h"
#include "common.h"
#include "Arduino.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#define SERVER_IP "192.168.1.113" // IP or DNS name
//#define SERVER_PORT 2000 // Port >= 2000
//#define MQTT_SERVER_IP "192.168.137.1"
#define MQTT_SERVER_IP "192.168.137.1"
#define MQTT_PORT 1883 // Port of MQTT Server
#define NAME "Puzzle5"
#define MAX_MSG  50

WiFiClient client;
WiFiClient mqttClient;
PubSubClient mqtt(mqttClient);
String rxload = "hello";

const char *active_type_info[] = {"TURN_RIGHT", "RETURN",
                                  "TURN_LEFT", "UP",
                                  "DOWN", "Enter_App",
                                  "SHAKE", "UNKNOWN"};



String mqttCallback(char* topic, byte* message, unsigned int length);


MQTT::MQTT()
{
    action_info.isValid = true;
    action_info.active = UNKNOWN;
    action_info.long_time = true;
} 

void MQTT::setupMQTT(String MqttControlName)
{
    const char *control_name=MqttControlName.c_str();
    // Setting up mDNS
     if (!MDNS.begin(NAME)) {
     Serial.println("Error setting up MDNS responder!");
    }
    MDNS.addService("_escape", "_tcp", 2000); 
    
    /* // Connect the client
    if (client.connect(SERVER_IP, SERVER_PORT))
    {
        Serial.println("Connected to server");
        client.println("welcome Puzzle 5");
    }
    else {
        Serial.println("Cannot connect to server");
    }
 */
    mqtt.setCallback(mqttCallback);
    mqtt.setServer(MQTT_SERVER_IP, MQTT_PORT);
    // MQTT connect
    if (mqtt.connect(NAME)) 
    {
        Serial.println("Connected to MQTT server");
        // Publish a message
        const char * topic = "5/safe/control";
        bool retained = false;
        mqtt.publish(topic, "5/safe/control", retained);
    } 
    else {
        Serial.println("Cannot connect to MQTT server");
    }
    mqtt.subscribe(control_name); //,MQTTQOS2
}
  

MQTT_Action *MQTT::MQTTAction(String rxload)
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
                action_info.isValid = 1;
                action_info.active = Enter_App;
            }
            else if (rxload == "q" || rxload == "Q")
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

char msg[MAX_MSG] = {'\0'};
String mqttCallback(char* topic, byte* message, unsigned int length) {
    // Convert to cstring
    int len = min((int)length, (int)(MAX_MSG-1));
    memcpy(&msg[0], message, len);
    msg[len] = '\0';
    // Print incoming msg
    rxload = String(msg);
    Serial.printf("MQTT Message on topic！！！！！: %s: %s\n", topic, msg );//rxload message
    //String rxload = &msg;
    Serial.printf("rxload value:", rxload);
    return rxload;
}