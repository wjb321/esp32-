#include <Arduino.h>
#include "display.h"
#include "lvgl.h"
#include "lv_conf.h"
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include "lv_port_indev.h"
#include "lv_cube_gui.h"
#include "lv_demo_encoder.h"
#include "common.h"
#include "MQTT.h"
#include <time.h>
#include <joystick.h>
#include "app_switch.h"
#include "app_switch_gui.h"
#include "game2048_controller.h"
#include "game2048.h"
#include <WiFi.h>
#include <ArduinoJson.h>
/* #define SSID "ubilab_wifi"
#define PWD "ohg4xah3oufohreiPe7e"  */

/* terry creats wifi by phone(makes computer has 2.4GHz wifi not 5Ghz)，amd my computer creates
a mew wifi named below for esp32 to connect(2.4Ghz) and then fill the debugger with the given mqtt server ip from 
up(it is gotten from my computer ipconfig)because now my computer now is the server for locals, later i can change 
it into the server ip where escape room has */
/* #define SSID "whatever"
#define PWD "youlike!!" */
/* extern int xValue ;
extern int yValue;
extern int sel; */

Display screen;
JOYSTICK joystick;
JOYSTICK_action *act_info;
//IMU mpu;
//IMU mpu_control;
//Imu_Action *act_info;          // 存放mpu6050返回的数据
//MQTT_Action *act_info;          // MQTT commands or BLE commands
AppSwitch *app_Switch; // APP controller

long lastInterval = 0;

//MQTT mqtt_control;

//extern String rxload;// message;
void setup()
{
    Serial.begin(115200);
   
    /*mqtt configure*/
    Serial.print("Connecting to ");
    //Serial.println(SSID);
    //WiFi.setHostname(NAME);
  /*   WiFi.begin(SSID, PWD);
    
    // Wait while not connected
     while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500); 
        Serial.print("Connecting to Wifi.");
    } */
    // Print IP
    //Serial.println("\nWiFi connected.\nIP Adress: ");
    //Serial.println(WiFi.localIP());
    joystick.setup_joystick();
    //mqtt_control.setupMQTT("game_control");
    randomSeed(2);
   
    /*** Init screen ***/
	screen.init();
	screen.setBackLight(0.2);
    Serial.println("Setup done");
    

    lv_port_indev_init();
	/*** Init on-board RGB ***/
    app_Switch = new AppSwitch();
	app_Switch->app_register(&escape_room);
    app_Switch->app_register(&game_2048_app);
    app_Switch->control_center(&joystick.action_info);
}

void loop(){
    
 /*    mqtt.loop();
    if (mqtt.state()) {
        mqtt_control.setupMQTT("game_control");
    }  */
    
	screen.routine();
   
    if (millis() - lastInterval > 2000) {
        act_info = joystick.JOYSTICKAction();
        app_Switch->control_center(act_info); // 运行当前进程
        lastInterval = millis();
    }
   /*  if (act_info->active == RETURN) {
        Serial.println("return delay");
        delay(1000);
    }
 */
}

