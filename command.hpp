#ifndef command_h
#define command_h
#ifdef AVR
const char cmd_checkNBIOT[5] PROGMEM=               "AT\n";
const char  cmd_NB_RESET[10] PROGMEM=               "AT+NRB\n";
const char  cmd_Get_IMEI[15] PROGMEM=               "AT+CGSN=1\n";
const char  cmd_Get_IMSI[15] PROGMEM=               "AT+CIMI\n";
const char  cmd_Get_Enter_Net_Status[15] PROGMEM=    "AT+CGATT?\n";
const char  cmd_Get_CSQ[15] PROGMEM=                "AT+CSQ\n";
const char  cmd_Create_socket[30] PROGMEM= 			"AT+NSOCR=STREAM,6,56000,1\n";
const char  cmd_Connect_to_server[30] PROGMEM=      "AT+NSOCO=%d,%s,%d\n";
const char  cmd_Close_Socket[15] PROGMEM=           "AT+NSOCL=%d\n";
const char  cmd_Send_Data[15] PROGMEM=              "AT+NSOSD=%d,";   //"AT+NSOSD=%d,%d"     //"AT+NSOSD=%d,%d,%s"
const char  cmd_Receive_Data[20] PROGMEM=           "AT+NSORF=%d,%d\n";


const char  cmd_Set_minimum_power[15] PROGMEM=       "AT+CFUN=0\n";
const char  cmd_Set_NBAND[15] PROGMEM=               "AT+NBAND=%d\n";
const char  cmd_base_config_1[35] PROGMEM=           "AT+NCONFIG=AUTOCONNECT,TRUE\n";
const char  cmd_base_config_2[45] PROGMEM=           "AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE\n";
const char  cmd_base_config_3[45] PROGMEM=           "AT+NCONFIG=CR_0859_SI_AVOID,TRUE\n";

const char  OK_response[5] PROGMEM=                  "OK";
const char  ERROR_response[10] PROGMEM=              "ERROR";
const char  CSQ_response[10] PROGMEM=                "+CSQ";
const char  NSONMI_response[10] PROGMEM=             "+NSONMI";
const char  NSOCLI_response[10] PROGMEM=             "+NSOCLI";

#else
const char cmd_checkNBIOT[5] =               "AT\n";
const char  cmd_NB_RESET[10] =               "AT+NRB\n";
const char  cmd_Get_IMEI[15] =               "AT+CGSN=1\n";
const char  cmd_Get_IMSI[15] =               "AT+CIMI\n";
const char  cmd_Get_Enter_Net_Status[15]=    "AT+CGATT?\n";
const char  cmd_Get_CSQ[15]=                "AT+CSQ\n";
const char  cmd_Create_socket[30]= 			"AT+NSOCR=STREAM,6,56000,1\n";
const char  cmd_Connect_to_server[30]=      "AT+NSOCO=%d,%s,%d\n";
const char  cmd_Close_Socket[15]=           "AT+NSOCL=%d\n";
const char  cmd_Send_Data[15]=              "AT+NSOSD=%d,";   //"AT+NSOSD=%d,%d"     //"AT+NSOSD=%d,%d,%s"
const char  cmd_Receive_Data[20]=           "AT+NSORF=%d,%d\n";


const char  cmd_Set_minimum_power[15]=       "AT+CFUN=0\n";
const char  cmd_Set_NBAND[15]=               "AT+NBAND=%d\n";
const char  cmd_base_config_1[35]=           "AT+NCONFIG=AUTOCONNECT,TRUE\n";
const char  cmd_base_config_2[45]=           "AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE\n";
const char  cmd_base_config_3[45]=           "AT+NCONFIG=CR_0859_SI_AVOID,TRUE\n";

const char  OK_response[5]=                  "OK";
const char  ERROR_response[10]=              "ERROR";
const char  CSQ_response[10]=                "+CSQ";
const char  NSONMI_response[10]=             "+NSONMI";
const char  NSOCLI_response[10]=             "+NSOCLI";
#endif
#endif
