#ifdef AVR
#include <Arduino.h>
#else
#include <iostream>
#include <cstring>
#endif
#include "gdef.hpp"
#ifndef nbiot_h
#define nbiot_h


extern u16 check_NBIOT_Cmd_result(char *response, const char * reply_success, const char * reply_fail,struct pluse_comands *pm);
extern u16 NBIOT_Cmd ( char *cmd, const char * reply_success, const char * reply_fail, u32 waittime,struct pluse_comands *pm);
extern u16 checkNBIOT(void);
extern u16 NB_RESET(void);
extern u16 Get_IMEI(void);
extern u16 Get_IMSI(void);
extern u16 Get_Enter_Net_Status(void);
extern u16 Get_CSQ(void);
extern u16 NB_Create_TCP(const char *server_ip, int port, int *socket );
extern u16 Close_Socket(int socket);

extern u16 initNBIOT();
extern u16 BASE_Config_NBIOT(int band);



extern char *NSORF_response(char * response,int *remaining_len, int *return_code, const char *desired_ip, int desired_port);

#endif 

 
