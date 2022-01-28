#include <Arduino.h>
#define EXT_ISR_MODE 2
/*
here are mainly realize the external interrupts by using macro define
1, read the given pin values when it is touched or not 
2, call the touchattachevent func in order to inform that there is a touch event
3,
*/
void TouchEvent()
{
  Serial.printf("Touch event.\r\n"); // higher than the given threshold: 40 then shows 
}

void  PinIntEvent()
{
  Serial.printf("Pinint Event. \r\n");
}

void setup() 
{
Serial.begin(115200);
#if 1 == EXT_ISR_MODE  // this is committed
 touchAttachInterrupt(T0, TouchEvent, 40);//touch event make it change
#elif 2 == EXT_ISR_MODE
  pinMode(0, INPUT_PULLUP);// initialize its pin
  // connect with pin0 and set the value to ground and it works
  attachInterrupt(0, PinIntEvent, FALLING);  // falling is a mode, there are several choices
#endif

}

void loop() {

#if 0 == EXT_ISR_MODE
  Serial.printf("touch: %d\r\n", touchRead(T0));
#endif
delay(1000);
}