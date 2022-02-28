#include "joystick.h"
#include "common.h"
#include "Arduino.h"

#define JOYSTICK_XOUT 26
#define JOYSTICK_YOUT 27
#define JOYSTICK_SEL 14
int joystick_flag = 0;
const char *active_type_info[] = {"TURN_RIGHT", "RETURN",
                                  "TURN_LEFT", "UP",
                                  "DOWN", "Enter_App",
                                  "SHAKE", "UNKNOWN"};

JOYSTICK::JOYSTICK()
{
    action_info.isValid = true;
    action_info.active = UNKNOWN;
    action_info.long_time = true;
}

void JOYSTICK::setup_joystick()
{
  pinMode(JOYSTICK_XOUT, INPUT);
  pinMode(JOYSTICK_YOUT, INPUT);
  pinMode(JOYSTICK_SEL, INPUT_PULLUP);
}

JOYSTICK_action *JOYSTICK::JOYSTICKAction()
{
    int xValue = analogRead(JOYSTICK_XOUT);
    int yValue = analogRead(JOYSTICK_YOUT);
    int sel = digitalRead(JOYSTICK_SEL);

    Serial.printf("X: %d | Y: %d | sel: %d\n", xValue, yValue, sel);
    action_info.isValid = true;
    if (xValue > 3000)
    {
        action_info.active = TURN_RIGHT;
        Serial.println("R");
    }
    else if (xValue < 1000)
    {
        action_info.active = TURN_LEFT;
        Serial.println("Left");
    }
    else if (yValue > 3000)
    {
        action_info.active = UP;
        Serial.println("UP");
    }
    else if (yValue < 1000)
    {
        action_info.active = DOWN;
        Serial.println("D");
    }
    else if (!sel)
    {
        if (joystick_flag) {
            action_info.active = RETURN;
            Serial.println("Return");
            joystick_flag = 0;
        }
        else
        {
            action_info.active = Enter_App;
            Serial.println("Enter");
            joystick_flag = 1;
        }
    }
    else
    {
        action_info.isValid = false;
    }
    //    }
        // 操作方向进行调整
       /*  if (UNKNOWN != action_info.active)
        {
            action_info.active = (ACTIVE_TYPE)((action_info.active + 0) % UNKNOWN);
        } */

   /*  Serial.print("ax: ");
    Serial.print(action_info.ax);
    Serial.print("\n");
    delay(2000);
    Serial.print("ay: ");
    Serial.print(action_info.ay);
    Serial.print("\n");
    delay(2000); */

    /*  Serial.print("az: ");
    Serial.print(action_info.az);
    Serial.print("\n");
    */
    return &action_info;
}
