#ifndef APP_GAME_2048_H
#define APP_GAME_2048_H
/*
there are places that different apps can be extended here
for example: weather, alblum, video
*/
//#include "wifi_mqtt.h"
#include "interface.h"
#include <PubSubClient.h>
//#include "WiFiClient.h"
#ifdef __cplusplus
extern APP_OBJ game_2048_app;
extern APP_OBJ escape_room;
extern PubSubClient mqtt;
#endif
#endif