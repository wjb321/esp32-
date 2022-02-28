
#ifndef APP_CONTORLLER_H
#define APP_CONTORLLER_H

#include "Arduino.h"
#include "interface.h"
#include "joystick.h"
//#include "MQTT.h"
#include <list>

#define APP_MAX_NUM 15           // maximum app
#define WIFI_LIFE_CYCLE 60000    // wifi life time
#define EVENT_LIST_MAX_LENGTH 10 // list capacity

struct EVENT_OBJ
{
    const APP_OBJ *from; // APP which send command service
    //APP_EVENT_TYPE type; // app type
    unsigned int id;     // send command service ID 
};

class AppSwitch
{
public:
    AppSwitch();
    ~AppSwitch();
    void Display(void);                     // LCD
    int app_register(APP_OBJ *app);         // rigister app into app_controller
    int app_unregister(const APP_OBJ *app); // deregister app
    int control_center(JOYSTICK_action *act_info);
    void app_exit(void); // exit from app
    // 事件请求
    //int req_event(const APP_OBJ *from, APP_EVENT_TYPE type, int event_id);
    //int req_event_deal(void);             // 事件处理
    //bool wifi_event(APP_EVENT_TYPE type); // wifi事件的处理

private:
    int app_is_legal(const APP_OBJ *app_obj);

private:
    APP_OBJ *appList[APP_MAX_NUM]; //leave 10 places for the app
    unsigned long m_preWifiReqMillis; // 保存上一回请求的时间戳
    unsigned int app_num;
    boolean app_exit_flag; // exit last app flag
    int cur_app_index;     // run now app index 当前运行的APP下标
    int pre_app_index;     // last run app index
};

#endif