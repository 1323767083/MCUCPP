
#ifdef AVR
#include <Arduino.h>
#else
#include <iostream>
#include <cstring>
#endif

#include "fun.hpp"
#include "gerror.hpp"
#include "command.hpp"
#include "msg.hpp"

#ifndef gdef_h
#define gdef_h
#ifndef AVR
typedef char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
#endif

struct pluse_comands
{
	u8 NSONMI_received,NSOCLI_received;
	int  NSONMI_received_socket,NSOCLI_received_socket;
	int  NSONMI_to_recieve_byte;
};


struct param{
	char IMEI[16];
	char IMSI[16];
	//int Init_NB_Status;
	//int signalCSQ;
	u8 Init_NB_Status;
	u8 signalCSQ;

};
enum mqtt_states {base_config,nbiot_init, onenet_connect,nbiot_tcp,mqtt_connect, mqtt_send,mqtt_receive,nbiot_close_socket,wrong_state,wait_state};
enum rest_state {rest_nbiot_init,rest_onenet_connect,rest_nbiot_tcp,rest_get,rest_get_response_head, rest_get_response_body,rest_close_socket,reset_success_state};

#define lbuffer_len 501
#define gbuffer_len 1001 
#define max_len_cmduuid 50 
#define max_len_cmd 50 


extern struct pluse_comands pms;
extern struct param NBIOT_MODULE_INFO;
extern char lbuff[lbuffer_len],gbuff[gbuffer_len];
extern int socket_used;
extern char PROID[];
extern char AUTH_INFO[];
extern char DEVID[];

extern const char mqtt_server_ip[20];
extern const int mqtt_server_port;

extern const char tecent_server_ip[20];
extern const int tecent_server_port;
extern const char tecent_api[20];


extern int half_Hex2Value(char half_hex);
extern int Hex2Value(char high, char low);
extern int Hex2Str(char *hex, char *str);
extern int Str2Hex(char *str, char *hex);
extern char * get_str_before_sep(char *instr, char sep, char *outstr);
extern char * sgetline(char * ip, char *op);

extern void init_globals();



#endif 
