#ifndef DISPLAY_H
#define DISPLAY_H

#include <lvgl.h>

#define LCD_BL_PIN 2
#define LCD_BL_PWM_CHANNEL 0

#ifdef __cplusplus 
class Display
{
private:

public:
    void init();
    void routine();
    void setBackLight(float);
};
#endif
#endif