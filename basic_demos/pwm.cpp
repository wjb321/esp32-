#include <Arduino.h>
#include "../lib/Motor/Motor.h"
/*setup()里面应该包含相应的设置函数，不然就进不了库函数中去*/
#define PWM_EN 1

int interruptCounter = 0;
hw_timer_t * timer = NULL;

void IRAM_ATTR TimerEvent() 
/*
中断服务函数，必须返回空值，并且，诶有输入参数的函数，
为了将代码分配到IRAM中，中断处理函数应该具有IRAM属性
*/
{
  Serial.println(interruptCounter++);
  if(interruptCounter > 5 )
  {
    interruptCounter = 1;
  }
}


void setup() 
{
  Serial.begin(115200);

#if  PWM_EN
 Motor_Init();
#endif


timer = timerBegin(0, 80, true);
/*
timerBegin
参数，
1，定时器编号 0-3
2，预分频计数器数值（其基础频率是80， 那么如果设置为80， 则其技术频率为1M， 80分频的单位是微秒）
3. 计数向上 还是向下，向上为true， 反之为false
*/

timerAlarmEnable(timer);

/*
timerAttach
绑定定时器中断处理函数
参数1， 指向已经初始化定时器的指针（本例是timer）
2， 中断服务函数的函数指针
3， 表示中断触发类型是边沿触发（true），还是电平
*/

timerAttachInterrupt(timer, &TimerEvent, true);
/*
timerAlarmWriter:
制定触发定时器中断的计数器值，分别有三个函数
1.  指向已经初始化定时器的指针（本例是timer）
2， 触发中断的计数值，这里是相当用于1秒钟
3， 定时器在生产中断时是否重新加载的标志，如果是true，就是下次依然使能中断
*/
timerAlarmWrite(timer, 1000000, true);
}

void loop() {
  // put your main code here, to run repeatedly: 
  #if PWM_EN
  PWM_SetDuty(200 * interruptCounter, 200 * interruptCounter);

  #endif
} 
