/*
*mainly the control app logic then use this logic as the input of app_switch_gui.
*here use https://github.com/ClimbSnail and https://github.com/AndyXFuture as references. 
*still there are many applications can be extended.
*/
#include "app_switch.h"
#include "app_switch_gui.h"
#include "common.h"
#include "interface.h"
#include "Arduino.h"
#include "joystick.h"

const char *app_event_type_info[] = {"APP_EVENT_WIFI_CONN", "APP_EVENT_WIFI_AP",
                                     "APP_EVENT_WIFI_ALIVE", "APP_EVENT_WIFI_DISCONN",
                                     "APP_EVENT_UPDATE_TIME", "APP_EVENT_NONE"};

//extern String rxload;

AppSwitch::AppSwitch()
{
    app_num = 0;
    app_exit_flag = 0;
    cur_app_index = 0;
    pre_app_index = 0;
    //appList = new APP_OBJ[APP_MAX_NUM];
    //m_wifi_status = false;
    m_preWifiReqMillis = millis();
    // 设置CPU主频
    setCpuFrequencyMhz(80);
    uint32_t freq = getCpuFrequencyMhz(); // In MHz
    // uint32_t freq = getXtalFrequencyMhz(); // In MHz
    Serial.print(F("getCpuFrequencyMhz: "));
    Serial.println(freq);

    app_control_gui_init();
    //LV_IMG_DECLARE(app_loading);
    appList[0] = new APP_OBJ();
    appList[0]->app_image = &EscapeRoom;
    appList[0]->app_name = "EscapeRoom";
    app_contorl_display_scr(appList[cur_app_index]->app_image,
                            appList[cur_app_index]->app_name,
                            LV_SCR_LOAD_ANIM_NONE, true);
     //Display();
}

void AppSwitch::Display()
{
    // appList[0].app_image = &app_loading;
    app_contorl_display_scr(appList[cur_app_index]->app_image,
                            appList[cur_app_index]->app_name,
                            LV_SCR_LOAD_ANIM_NONE, true);
}

AppSwitch::~AppSwitch()
{
    //rgb_thread_del();
}

int AppSwitch::app_is_legal(const APP_OBJ *app_obj)
{
    // APP的合法性检测
    if (NULL == app_obj)
        return 1;
    if (APP_MAX_NUM <= app_num)
        return 2;
    return 0;
}

/*
*everytime call this function, then the applist will be added
*/
int AppSwitch::app_register(APP_OBJ *app) 
{
    int ret_code = app_is_legal(app);
    if (0 != ret_code)
    {
        return ret_code;
    }

    appList[app_num] = app;
    ++app_num;
    return 0; //注册成功
}

int AppSwitch::app_unregister(const APP_OBJ *app) // 将APP从app_controller中去注册（删除）
{
    // todo
    return 0;
}

/*
* here realize the centrol control:
* MQTT commands as the input
* start page only supports left or right move commands
* game page supports games in "i" command, and then here are up, left, right, down, quit commands
* if act_info is not getintogame, then it supports app switch, so play with this ani: app_contorl_display_scr
* 1 == app_exit_flag means the game starts
*/
int AppSwitch::control_center(JOYSTICK_action *act_info)
{   
    lv_scr_load_anim_t anim_type = LV_SCR_LOAD_ANIM_NONE;
/*     if (UNKNOWN != act_info->active)
    {
        //Serial.print(F("act_info->active:MQTT "));
        Serial.println(active_type_info[act_info->active]);
    } */
    if (0 == app_exit_flag)
    {   
        if ( appList[cur_app_index]->app_image == &EscapeRoom) //0 == app_exit_flag
        {
            // 当前没有进入任何app
            if (TURN_LEFT == act_info->active)
            {
                anim_type = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
                pre_app_index = cur_app_index;
                cur_app_index = (cur_app_index + 1) % app_num;
                Serial.println(String("Current App: ") + appList[cur_app_index]->app_name);
                //BLE part
                //rxload="";
            }
            else if (TURN_RIGHT == act_info->active)
            {
                anim_type = LV_SCR_LOAD_ANIM_MOVE_LEFT;
                pre_app_index = cur_app_index;
                // 以下等效与 processId = (processId - 1 + APP_NUM) % 4;
                cur_app_index = (cur_app_index + 1) % app_num;
                //cur_app_index = (cur_app_index - 1 + app_num) % app_num; // 此处的3与p_processList的长度一致
                Serial.println(String("Current App: ") + appList[cur_app_index]->app_name);
                /* app_contorl_display_scr(appList[cur_app_index]->app_image,
                                        appList[cur_app_index]->app_name,
                                        LV_SCR_LOAD_ANIM_MOVE_LEFT, false); */
                //delay(300);
                //BLE part
            // rxload="";
            }
        }
    else
    {
        if(TURN_LEFT == act_info->active)
        {
            anim_type = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
            pre_app_index = cur_app_index;
            cur_app_index = (cur_app_index + 1) % app_num;
            Serial.println(String("Current App: ") + appList[cur_app_index]->app_name);
	        //BLE part
	       // rxload="";
        }
        else if (TURN_RIGHT == act_info->active)
        {
            anim_type = LV_SCR_LOAD_ANIM_MOVE_LEFT;
            pre_app_index = cur_app_index;
            // 以下等效与 processId = (processId - 1 + APP_NUM) % 4;
            // +3为了不让数据溢出成负数，而导致取模逻辑错误
            cur_app_index = (cur_app_index + 1) % app_num;
            //cur_app_index = (cur_app_index - 1 + app_num) % app_num; // 此处的3与p_processList的长度一致
            Serial.println(String("Current App: ") + appList[cur_app_index]->app_name);
	        //rxload="";
        }
        else if (Enter_App == act_info->active)
        {
            app_exit_flag = 1; // 进入app
            if (NULL != appList[cur_app_index]->app_init)
            {
                (*(appList[cur_app_index]->app_init))(); // 执行APP初始化
            }
        }

        if (Enter_App != act_info->active) // && UNKNOWN != act_info->active
        {
            //未输入I 的时候就会一直在执行app图像的界面（不进入游戏内部）
            app_contorl_display_scr(appList[cur_app_index]->app_image,
                                    appList[cur_app_index]->app_name,
                                    anim_type, false);
            delay(300);
        }
    }
        
    }
    else
    {
        // 运行APP进程 等效于把控制权交给当前APP
        (*(appList[cur_app_index]->control_center))(this, act_info);
    }
    act_info->active = UNKNOWN;
    act_info->isValid = 0;
    return 0;

}
void AppSwitch::app_exit()
{
    app_exit_flag = 0; // 退出APP
    if (NULL != appList[cur_app_index]->exit_callback)
    {
        // 执行APP退出回调
        (*(appList[cur_app_index]->exit_callback))();
    }
    Serial.print(F("2,game exit anim starts:   "));
     app_contorl_display_scr(appList[cur_app_index]->app_image,
                            appList[cur_app_index]->app_name,
                            LV_SCR_LOAD_ANIM_NONE, true); 
    //LV_IMG_DECLARE(youwin);
    // 测试退出游戏后是否能显示图片。结果显示成功，applist[cur_app_index]
   /*  lv_obj_t * gif_bg1 = lv_img_create(lv_scr_act(),NULL);// delete the app_window
    lv_img_set_src(gif_bg1,appList[cur_app_index]->app_image);
    lv_obj_align(gif_bg1, NULL, LV_ALIGN_CENTER, 0, 0); 
    Serial.print(F("   end game exit anim")); */

    //Serial.print(F("game exit anim whole"),appList[cur_app_index]);
    //Serial.print("cur_app_index",cur_app_index);
    //Serial.print("cur_app_name",app_name);
    // 设置CPU主频
    setCpuFrequencyMhz(80);
}