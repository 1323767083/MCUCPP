#include "gdef.hpp"


struct pluse_comands pms;
struct param NBIOT_MODULE_INFO;
char lbuff[lbuffer_len],gbuff[gbuffer_len];
int socket_used;
char PROID[]="274544";
char AUTH_INFO[]="LmvXwrlle4wiTeo5QxqmiAfqrfA=";
char DEVID[]="543973786";

#ifdef AVR
const char mqtt_server_ip[20] PROGMEM ="183.230.40.39";
const int mqtt_server_port  PROGMEM =6002;
const char tecent_server_ip[20] PROGMEM  ="134.175.16.85";
const int tecent_server_port PROGMEM =6001;
const char tecent_api[20] PROGMEM ="/UEinform";

#else
const char mqtt_server_ip[20]="183.230.40.39";
const int mqtt_server_port=6002;
const char tecent_server_ip[20]="134.175.16.85";
const int tecent_server_port=6001;
const char tecent_api[20]="/UEinform";
#endif


int half_Hex2Value(char half_hex)
{
		if (half_hex>='0' && half_hex<='9')
		{
			return half_hex-'0';
		}
		if (half_hex>='A' && half_hex<='F')
		{
			return half_hex-'A'+10;
		}
		return -1;

}

int Hex2Value(char high, char low)
{
	int value=0, temp_value=0;
	char c2[2];
	c2[0]=high;
	c2[1]=low;
	int i;
	for (i=0;i<2;i++)
	{
		value=value*16;
		temp_value=half_Hex2Value(c2[i]);
		if (temp_value<0)
			return -1;
		value+=temp_value;
	}
	return value;
}

int Hex2Str(char *hex, char *str)
{
	int temp;
	int high, low;
	while(*hex)
	{
		high=*hex++;
		low=*hex++;
    	temp= Hex2Value(high, low);
    	if (temp<0)
    		return temp;
    	*str++=(char)temp;
	}
	*str='\0';
    return 1;
}

int Str2Hex(char *str,char *hex)
{
    if (NULL==str || NULL==hex){
            return -1;
    }
    if (0==strlen(str)){
            return -2;
    }
    *hex = '\0';
    char tempstr[3];
	while(*str)
    {
    	sprintf (tempstr,format_1Hex_len,*str);
    	strcat(hex,tempstr);
		str++;
	}
    return 0;
}


char* sgetline(char * ip, char *op)
{
	while (*ip!='\n' && *ip!='\0')
	{
		if(*ip!='\r')
		{
			*op++=*ip++;
		}
		else
			ip++;
	}
	*op='\0';
	ip++;
	return ip;
}
char *get_str_before_sep(char *instr, char sep, char *outstr)
{
	char *endstr, *cstr;
	endstr=instr+strlen(instr);
	cstr=instr;
	while (cstr<endstr)
	{
		if (*cstr!=sep)
		{
			*outstr++=*cstr++;
		}
		else
		{
			*outstr='\0';
			//cstr++;
			return ++cstr;
		}
	}
	return NULL;
}

void init_globals()
{
	memset(lbuff,0,lbuffer_len);
	memset(gbuff,0,gbuffer_len);
}
