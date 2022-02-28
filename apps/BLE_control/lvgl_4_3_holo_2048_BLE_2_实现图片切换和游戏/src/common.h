#ifndef __COMMON_H__
#define __COMMON_H__

#define AIO_VERSION "1.9.3"

#include "Arduino.h"
//#include "config.h"
//#include "driver/sd_card.h"
#include "display.h"
//#include "driver/ambient.h"
//#include "imu.h"
//#include "network.h"
//#include <Preferences.h>

//extern SdCard tf;
//extern Pixel rgb;
//extern Config g_cfg;      // 全局配置文件
//extern Network g_network; // 网络连接
//extern Preferences prefs; // 声明Preferences对象
#ifdef __cplusplus
extern Display screen;    // 屏幕对象

//extern Ambient ambLight;  // 光纤传感器对象

boolean doDelayMillisTime(unsigned long interval,
                          unsigned long *previousMillis,
                          boolean state);

#define GFX 

#if GFX 0
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS -1 // Not connected
#define TFT_DC 2
#define TFT_RST 4 // Connect reset to ensure display initialises
#include <Arduino_GFX_Library.h>
extern Arduino_HWSPI *bus;
extern Arduino_ST7789 *tft;

#else
#include <TFT_eSPI.h>
/*
TFT pins should be set in path/to/Arduino/libraries/TFT_eSPI/User_Setups/Setup24_ST7789.h
*/
extern TFT_eSPI *tft;
#endif
#endif
#endif