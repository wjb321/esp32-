#ifndef INTERFACE_H
#define INTERFACE_H

class AppSwitch;
struct Imu_Action;

struct APP_OBJ
{
    const char *app_name;  // app name
    const void *app_image; // app image
    const char *app_info;  // basic infomation 
    void (*app_init)();    // game init
    void (*control_center)(AppSwitch *sys, const Imu_Action *act_info);    //app main program entry pointer
    void (*exit_callback)(); //exit callback func
   // void (*on_event)(APP_EVENT_TYPE type, int event_id); // 事件通知
};

#endif