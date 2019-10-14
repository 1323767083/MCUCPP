#ifdef AVR
#include <Arduino.h>
#else
#include <iostream>
#include <cstring>
#endif
#include "gdef.hpp"
#include "gerror.hpp"
#include "msg.hpp"
#include "NBIOT.hpp"

#ifndef mgtt_h
#define mgtt_h

#ifndef AVR
typedef char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
#endif

#define MQTT_CONNECT	1
#define MQTT_CONNACK	2
#define MQTT_PUBLISH	3

extern bool decode_variable_length(char * msg, int *variable_len, int *num_of_bite);
extern bool encode_variable_length(char * varibale_len_str, int len);
extern void build_1Data1value_json(char * dataname, float tvalue, char *result);
extern void build_1Data2value_location_json(char * dataname, float lot, float lat, char *result);
extern u16 prepare_mqtt_connect_message(char *mqtt_message,char *PROID,char *AUTH_INFO,char *DEVID);
extern u16 prepare_mqtt_Send_message(char *mqtt_messageHEX,char *SendData,short length);
extern u16 ana_mqtt_command(char *msg, char* cmduuid, char *cmd);
extern int CONNACK(char * msg);
extern int handle_mqtt_response(char *msg);
extern int mqtt_flow(int state, int * return_code);
#endif 
