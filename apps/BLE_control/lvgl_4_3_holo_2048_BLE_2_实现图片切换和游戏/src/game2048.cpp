#include "game2048.h"
#include <Arduino.h>
#include "lv_cubic_gui.h"
#include "game2048_controller.h"
#include "app_controller.h"
#include "common.h"
#include <esp32-hal-timer.h>
#include "BLE.h"

extern bool deviceConnected;
extern bool oldDeviceConnected;             //the state of last connection
//extern pServer 
extern String rxload;
 
void taskOne(void *parameter)
{
    while (1)
    {
        //心跳任务
        lv_tick_inc(5);
        delay(5);
    }
    Serial.println("Ending task 1");
    vTaskDelete(NULL);
}

void taskTwo(void *parameter)
{
    while (1)
    {
        // LVGL任务主函数
        screen.routine();
        delay(5);
    }
    Serial.println("Ending task 2");
    vTaskDelete(NULL);
}

GAME2048 game;
//BLE ble;


struct Game2048Rundata
{
    int Normal = 0;       //记录移动的方向
    int BornLocation = 0; //记录新棋子的位置
    int *p;
    int *moveRecord;
    BaseType_t xReturned_task_one = NULL;
    TaskHandle_t xHandle_task_one = NULL;
    BaseType_t xReturned_task_two = NULL;
    TaskHandle_t xHandle_task_two = NULL;

};
static Game2048Rundata *run_data = NULL;
void Game_init(void)
{
 lv_holo_cubic_gui();
 
    randomSeed(analogRead(25));
    // 初始化运行时参数
    run_data = (Game2048Rundata *)calloc(1, sizeof(Game2048Rundata));
    game.init();
    run_data->p = game.getBoard();
    run_data->moveRecord = game.getMoveRecord();

    run_data->xReturned_task_one = xTaskCreate(
        taskOne,                  
        "TaskOne",                 
        10000,                       
        NULL,                        
        1,                         
        &run_data->xHandle_task_one); 

    run_data->xReturned_task_two = xTaskCreate(
        taskTwo,                     
        "TaskTwo",                  
        10000,                      
        NULL,                        
        1,                           
        &run_data->xHandle_task_two); 
    //刷新棋盘显示
    int new1 = game.addRandom();
    int new2 = game.addRandom();
    showBoard(run_data->p);
    //棋子出生动画
    born(new1);
    born(new2);
    // 防止进入游戏时，误触发了向上
    delay(1000);

}

void game_2048_process(AppController *sys, const BLE_Action *act_info)
{
//ble.setupBLE("Escaple_Room");
if (deviceConnected) {
    delay(50);  //delay for aviod the data writen 
   if (RETURN == act_info->active)
    {
        sys->app_exit(); // 退出APP
        return;
    }

    // 具体操作
    if (TURN_RIGHT == act_info->active)
    {
        game.moveRight();
        if (game.comparePre() == 0)
        {
            showAnim(run_data->moveRecord, 4, game.addRandom(),
                     run_data->p);
                     Serial.println("Anim go R");
        }
        Serial.println("game Right!");
		String str=rxload;
        const char *newValue=str.c_str();
        pCharacteristic->setValue(newValue);
		rxload="";
        pCharacteristic->notify();
		//break;
    }
    else if (TURN_LEFT == act_info->active)
    {
        game.moveLeft();
        if (game.comparePre() == 0)
        {
            showAnim(run_data->moveRecord, 3, game.addRandom(),
                     run_data->p);
        }
    Serial.println("game Left!");
	String str=rxload;
    const char *newValue=str.c_str();
    pCharacteristic->setValue(newValue);
	rxload="";
    pCharacteristic->notify();
	//break;
    }
    else if (UP == act_info->active)
    {
        game.moveUp();
        if (game.comparePre() == 0)
        {
            showAnim(run_data->moveRecord, 1, game.addRandom(),
                     run_data->p);
        }
    Serial.println("game Up!");
	String str=rxload;
    const char *newValue=str.c_str();
    pCharacteristic->setValue(newValue);
	rxload="";
    pCharacteristic->notify();
	//break;
    }
    else if (DOWN == act_info->active)
    {
        game.moveDown();
        if (game.comparePre() == 0)
        {
            showAnim(run_data->moveRecord, 2, game.addRandom(),
                     run_data->p);
        }
    Serial.println("game Down!");
	String str=rxload;
    const char *newValue=str.c_str();
    pCharacteristic->setValue(newValue);
	rxload="";
    pCharacteristic->notify();
	//break;
    }

    if (game.judge() == 1)
    {
        //   rgb.setRGB(0, 255, 0);
        Serial.println("you win!");
    }
    else if (game.judge() == 2)
    {
        //   rgb.setRGB(255, 0, 0);
        Serial.println("you lose!");
    }

    // 程序需要时可以适当加延时
    delay(300);
  }

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
}

void game_2048_exit_callback(void)
{
    // 查杀定时器
    if (run_data->xReturned_task_one == pdPASS)
    {
        vTaskDelete(run_data->xHandle_task_one);
    }
    if (run_data->xReturned_task_two == pdPASS)
    {
        vTaskDelete(run_data->xHandle_task_two);
    }

    game_2048_gui_del();
    // 释放资源
    free(run_data);
    run_data = NULL;
}

 /*void game_2048_event_notification(APP_EVENT_TYPE type, int event_id)
{
    // 目前事件主要是wifi开关类事件（用于功耗控制）
    switch (type)
    {
    case APP_EVENT_WIFI_CONN:
    {
        // todo
    }
    break;
    case APP_EVENT_WIFI_AP:
    {
        // todo
    }
    break;
    case APP_EVENT_WIFI_ALIVE:
    {
        // wifi心跳维持的响应 可以不做任何处理
    }
    break;
    default:
        break;
    } 
}
*/
APP_OBJ game_2048_app = {"2048", &number_game, "", Game_init,
                         game_2048_process, game_2048_exit_callback};//,game_2048_event_notification
                         

APP_OBJ game_2048_app1 = {"Start", &app_loading, "", NULL,
                         NULL, NULL};//,game_2048_event_notification
                         
/*
* 随机刷新一个2或4
* 返回刷新的位置
*/
