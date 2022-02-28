#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <I2Cdev.h>
#include <MPU6050.h>
#include "lv_port_indev.h"
extern int xValue ;
extern int yValue;
extern int sel;

#ifdef __cplusplus
enum ACTIVE_TYPE
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

struct JOYSTICK_action
{
    ACTIVE_TYPE active;
    boolean isValid;
    boolean long_time;
};

class JOYSTICK
{

public:
    JOYSTICK_action action_info;

public:
    JOYSTICK();
    void setup_joystick();
    JOYSTICK_action *JOYSTICKAction();
};

#endif
#endif
