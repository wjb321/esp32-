#include <Arduino.h>
#include "imu.h"
#include "lv_port_indev.h"

IMU mpu;

void setup()
{
	Serial.begin(115200);
	
	randomSeed(2);//analogRead(25)	
	/*** Init IMU as input device ***/
	lv_port_indev_init();
	mpu.init();
}


int flag = 0;	
void loop(){
	while(1){
		mpu.update(200);
		 Serial.printf("AccelX:%d\n", mpu.getAccelX());
		 delay(3000);
		Serial.printf("AccelY:%d\n", mpu.getAccelY());
		delay(3000);

		if(mpu.getAccelY() >= 5000 && flag == 0){
		
			flag = 1;
			Serial.println("Right!");
			Serial.printf("AccelY:%d\n", mpu.getAccelY());
		}else if(mpu.getAccelY() <= -5000 && flag == 0){	
			flag = 1;
			Serial.println("Left!");
			Serial.printf("AccelY:%d\n", mpu.getAccelY());
		}else if(mpu.getAccelX() >= 9000 && flag == 0){	
			flag = 1;
			Serial.println("Up!");
            Serial.printf("GyroX:%d\n", mpu.getGyroX());
		}else if(mpu.getAccelX() <= -2000 && flag == 0){
			flag = 1;
			Serial.println("Down!");
            Serial.printf("GyroX:%d\n", mpu.getGyroX());
		}else if(mpu.getAccelX() <= 7000 && mpu.getAccelX() >= -1000 && mpu.getAccelY() <= 5000 && mpu.getAccelY() >= -5000){
			flag = 0;
			continue;
		}else{
			continue;
		}
	}
}