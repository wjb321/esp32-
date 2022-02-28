#include "game2048.h"
#include <Arduino.h>
#include "lv_cube_gui.h"
#include "game2048_controller.h"
#include "app_switch.h"
#include "common.h"
#include <esp32-hal-timer.h>
#include "joystick.h"
#include "MQTT.h"
#include <ArduinoJson.h>
//extern String rxload;
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



struct Game2048Rundata
{
    int Normal = 0;       //record the moving direction
    int BornLocation = 0; //record the new number location
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

void game_2048_process(AppSwitch *sys, const JOYSTICK_action *act_info)
{
  StaticJsonDocument<300>  JSONbuffer;// arduino 6
  delay(50);  //delay for aviod the data writen 
   if (RETURN == act_info->active)
    {   
        Serial.println("1,out start:   ");
        sys->app_exit(); // 退出APP
        Serial.println("   game exit");
        return;
    }
    // 具体操作
    if (TURN_RIGHT == act_info->active)
    {
        game.moveRight();
        if (game.comparePre() == 0)
        {
            showAnim(run_data->moveRecord, 4, game.addRandom(), run_data->p);
        }
        Serial.println("game Right!");
        //rxload ="";
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
   // rxload ="";
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
    //rxload ="";
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
    //rxload ="";
    }
    if (game.judge() == 1)
    {
    
        JSONbuffer["method"] = "trigger";
        JSONbuffer["state"] = "off";
        JSONbuffer["data"] = "skipped";
        char JSONmessageBuffer[200];
        serializeJson(JSONbuffer, JSONmessageBuffer);
        //JSONbuffer.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println("Sending message to MQTT topic..");
        //Serial.println(JSONbuffer);
        Serial.println(mqtt.state());
        if (mqtt.publish("5/safe/control", JSONmessageBuffer) == true) {
            Serial.println("Success sending message");
        } else {
            Serial.println("Error sending message");
        }
        load_png(); //interface after win the game
        delay(5000);
        sys->app_exit();

    }
    else if (game.judge() == 2)
    {
        JSONbuffer["method"] = "trigger";
        JSONbuffer["state"] = "on";
        char JSONmessageBuffer_1[100];
        serializeJson(JSONbuffer, Serial);
        //JSONbuffer.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        Serial.println("Sending message to MQTT topic..");
        //Serial.println(JSONbuffer);
        //const uint8_t* loads = JSONbuffer;
        if (mqtt.publish("5/safe/control", JSONmessageBuffer_1) == true) {
            Serial.println("Success sending message");
        } else {
            Serial.println("Error sending message");
        }
        Game_init();
    }

    // 程序需要时可以适当加延时
    delay(300);
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

APP_OBJ game_2048_app = {"GAME", &number_game, "", Game_init,
                         game_2048_process, game_2048_exit_callback};//,game_2048_event_notification
                         

APP_OBJ escape_room = {"EscapeRoom", &EscapeRoom, "", NULL, NULL, NULL};//,game_2048_event_notification 

