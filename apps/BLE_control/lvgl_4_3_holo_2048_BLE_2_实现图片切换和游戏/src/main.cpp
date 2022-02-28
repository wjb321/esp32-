#include <Arduino.h>
#include "display.h"
#include "lvgl.h"
#include "lv_conf.h"
//#include "imu.h"
//#include "rgb_led.h"
#include "lv_port_indev.h"//
#include "lv_cubic_gui.h"
#include "lv_demo_encoder.h"
#include "game2048.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "common.h"//
#include <time.h>
#include <BLE.h>
#include "app_controller.h"
#include "app_controller_gui.h"
//include "game2048.h"
#include "game2048_controller.h"

/*** Component objects ***/
Display screen;
//IMU mpu;
//Pixel rgb;
BLE_Action *act_info;          // 存放mpu6050返回的数据
AppController *app_contorller; // APP控制器
BLE ble;
//GAME2048 game;
//int *p;
//int *m;
//long time1;
extern String rxload;
/*end of configuration of BLE
1)BLE initialize
2)backlight setting
3)lvgl gui configuration*/
void setup()
{
    Serial.begin(115200);
    ble.setupBLE("Escape_Room");//setup bluetooth name
    randomSeed(2);//analogRead(25)
	
    /*** Init screen ***/
	screen.init();
	screen.setBackLight(0.2);
    
	 lv_port_indev_init();
	/*** Init on-board RGB ***/
	//rgb.init();
	//rgb.setBrightness(64).setRGB(153, 170, 255);
    //lv_holo_cubic_gui();
    app_contorller = new AppController();
    app_contorller->app_register(&game_2048_app);
	app_contorller->app_register(&game_2048_app1);
    app_contorller->main_process(&ble.action_info);


    /*show the game UI and delete after some time*/
	//load_png();
	//load_png1();
   // clear();
  // lv_holo_cubic_gui();
   /*there are four parameters:
   1) as the pointer to task entry function, it should be in loop(no return)
   2) name be descriptive to the task, mainly facilitate debugging
   3) the size of task stack specifized as bytes
   4) pointer to the parameter that will be used
   5) priority of the task
   6) pass back a handle by which the task can be referenced
   see up two tasks*/
    //xTaskCreate(taskOne,"TaskOne",10000,NULL,1,NULL);                 
    //xTaskCreate(taskTwo,"TaskTwo",10000,NULL,1,NULL);
     
   
	/*** Init 2048 game ***/
	//game.init();
	//p = game.getBoard();
	//m = game.getMoveRecord();
	
     
	/*refresh input values cause in the very beginning there 
	will be 2 initial values*/
	//int new1 = game.addRandom();
	//int new2 = game.addRandom();
	//showBoard(p);
	

	//animations
	//born(new1);
	//born(new2);
	//wait for the anaimation finished
	//delay(300);
	//time1 = millis();
	//xTaskCreate(taskOne,"TaskOne",10000,NULL,1,NULL);                 
    //xTaskCreate(taskTwo,"TaskTwo",10000,NULL,1,NULL);
   // delay(300);
}


//int flag = 0;	//avoid wrong action
////int Normal = 0;	//moving direction
//int BornLocation = 0; //new born value location

void loop(){
/* 	while(1){
		if (deviceConnected) {
		delay(50);  //delay for aviod the data writen 
		if((rxload == "L" || rxload == "l" ) && flag == 0){	
			game.moveLeft();
			if (game.comparePre() == 0) { 
				showAnim(m,3,game.addRandom(),p);
			}
			flag = 1;
			Serial.println("Left!");
			String str=rxload;
            const char *newValue=str.c_str();
            pCharacteristic->setValue(newValue);
			rxload="";
            pCharacteristic->notify();
			break;
		}else if((rxload == "R" || rxload == "r") && flag == 0){	
			game.moveRight();
			if (game.comparePre() == 0) {
				showAnim(m,4,game.addRandom(),p);
			}
			flag = 1;
			Serial.println("right!");
			String str=rxload;
            const char *newValue=str.c_str();
            pCharacteristic->setValue(newValue);
			rxload="";
            pCharacteristic->notify();
			break;
		}else if((rxload == "U" || rxload == "u") && flag == 0){	
			game.moveUp();
			if (game.comparePre() == 0) {
				showAnim(m,1,game.addRandom(),p);
			}
			flag = 1;
			Serial.println("Up!");
			String str=rxload;
            const char *newValue=str.c_str();
            pCharacteristic->setValue(newValue);
            pCharacteristic->notify();
			rxload="";
			break;
		}else if((rxload == "D" || rxload == "d") && flag == 0){	
			game.moveDown();
			if (game.comparePre() == 0) {
				showAnim(m,2,game.addRandom(),p);
			}
			flag = 1;
			Serial.println("Down!");
			String str=rxload;
            const char *newValue=str.c_str();
            pCharacteristic->setValue(newValue);
			rxload="";
            pCharacteristic->notify();
			break;
		}else{
			flag = 0;
			continue;
		}

		if(game.Judge() == 1){
			rgb.setRGB(0, 255, 0);
			Serial.println("you win!");
		}else if(game.Judge() == 2){
			rgb.setRGB(255, 0, 0);
			Serial.println("you lose!");
		}
        delay(30);
	}
	// disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
	
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // buffering time for bluetooth
        pServer->startAdvertising(); // rebroadcasting
        Serial.println(" startAdvetising ");
        oldDeviceConnected = deviceConnected;
    }

    // connecting  
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
	} */
	screen.routine();
    act_info = ble.BLEAction(rxload);
    app_contorller->main_process(act_info); // 运行当前进程
}