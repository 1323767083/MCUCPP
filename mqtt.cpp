#include "mqtt.hpp"
bool decode_variable_length(char * msg, int *variable_len, int *num_of_bite)
{
	int i,tempvalue,len=0,multiplier=1;
	for (i=0;i<4;i++)
	{
		tempvalue=Hex2Value(msg[2+2*i], msg[3+2*i]);
		if (tempvalue <0)
			return false;
		if (tempvalue < 128)
		{
			len+=tempvalue*multiplier;
			break;
		}
		else
			if (i<3)
			{
				len+=(tempvalue-128)*multiplier;
				multiplier=multiplier*128;
			}
			else
				return false;
	}
	*variable_len=len;
	*num_of_bite=i+1;
	return true;
}
bool encode_variable_length(char * varibale_len_str, int len)
{
	int i,tempvalue;
    char tempstr[3];
	*varibale_len_str='\0';
	for (i=0;i<4;i++)
	{
		tempvalue=len % 128;
		len= len /128;
		if (len >0)
			tempvalue=tempvalue |128;
		sprintf(tempstr, format_1Hex_len,tempvalue);
		strcat(varibale_len_str,tempstr);
		if (len ==0)
			return true;
	}
	return false;
}


void build_1Data1value_json(char * dataname, float tvalue, char *result)
{
	sprintf (result, template_1d1v, dataname,(double)tvalue);
}

void build_1Data2value_location_json(char * dataname, float lot, float lat, char *result)
{
	sprintf (result, template_1d2v, dataname,(double)lot,(double)lat);
}

//generate MQTT connect package
u16 prepare_mqtt_connect_message(char *mqtt_message,char *PROID,char *AUTH_INFO,char *DEVID)
{
        u16 PROID_length = strlen(PROID);
        u16 DEVID_length = strlen(DEVID);
        u16 AUTH_INFO_length = strlen(AUTH_INFO);
        //u16 MQTT_length = strlen("MQTT");
        u16 packetLen;
        
        packetLen = 12;//fix part length
        if(PROID_length > 0)
                packetLen = packetLen + 2 + PROID_length;
        if(DEVID_length > 0)
                packetLen = packetLen+ 2 + DEVID_length;
        if(AUTH_INFO_length > 0)
                packetLen = packetLen+ 2 + AUTH_INFO_length;

        sprintf((char*)(mqtt_message+strlen((char*)mqtt_message)), format_Decimal_len,packetLen);

		//Fixed Header
		//  10  1: means MQTT Packet Type   0��default 
        strcpy((char*)(mqtt_message+strlen((char*)mqtt_message)),mqtt_msg_packet_type_connect);
        
		encode_variable_length((char*)(mqtt_message+strlen((char*)mqtt_message)), (packetLen-2));

        //  variable header beggining part 
		//  0004        ProtocolName Length is 4
		//  4D515454    MQTT
		//  04			Protocol Level should be 4 
		//  C0   		Connect Flag	User flag��1��	Passwordflag��1��	WillRetainFlag��0��	WillQos Flag ��00��	WillFlag��0��	CleanSessionflag��0��	Reserve��0�� 
		//  FFFF        KeepAlive  120�� min 120 if less than 120 max 65535  which is FFFF    low byte fist 0100  means 1
        
		//strcat(mqtt_message,"00044D51545404C00178");
		strcat(mqtt_message,mqtt_connect_variable_head_1part);

		sprintf((char*)(mqtt_message+strlen((char*)mqtt_message)), format_2Hex_len,((0xff00&(DEVID_length))>>8),(0xff&DEVID_length));
        Str2Hex((char*)DEVID,(char*)(mqtt_message+strlen((char*)mqtt_message)));
		
        sprintf((char*)(mqtt_message+strlen((char*)mqtt_message)), format_2Hex_len,((0xff00&(PROID_length))>>8),(0xff&PROID_length));
		Str2Hex((char*)PROID,(char*)(mqtt_message+strlen((char*)mqtt_message)));
		
        sprintf((char*)(mqtt_message+strlen((char*)mqtt_message)), format_2Hex_len,((0xff00&(AUTH_INFO_length))>>8),(0xff&AUTH_INFO_length));
        Str2Hex((char*)AUTH_INFO,(char*)(mqtt_message+strlen((char*)mqtt_message)));
		
        strcat(mqtt_message,"\r\n");        
        return packetLen;    
}

//generate mqtt data package
u16 prepare_mqtt_Send_message(char *mqtt_messageHEX,char *SendData,short length)
{
        u16 packetLen=length+10;
        
        sprintf((char*)(mqtt_messageHEX+strlen((char*)mqtt_messageHEX)),format_Decimal_len,packetLen);
		strcpy((char*)(mqtt_messageHEX+strlen((char*)mqtt_messageHEX)),mqtt_msg_packet_type_send);
        encode_variable_length((char*)(mqtt_messageHEX+strlen((char*)mqtt_messageHEX)), (packetLen));
		//30 // MQTT should be =3  DupFlag=0 QosLevel=0 Retain=0 
		//43 //remainedLength=67 

		//strcat(mqtt_messageHEX,	   "000324647001");
        strcat(mqtt_messageHEX,	   mqtt_send_variable_head_1part);
		//0003// TopicNameLen=3 
		//246470//TopicName=$dp 
		//01//datatype 1 1:Json
        
		//003B//data length
		sprintf((char*)(mqtt_messageHEX+strlen((char*)mqtt_messageHEX)), format_2Hex_len,((0xff00&(length))>>8),(0xff&length));
		
        Str2Hex((char*)SendData,(char*)(mqtt_messageHEX+strlen((char*)mqtt_messageHEX)));
	
		strcat(mqtt_messageHEX,"\n");
        return packetLen;    
} 



// mqtt_command_sampe="3040002A24637265712F32323662643435642D366230342D353732392D393833302D363038663434376632643763 68656C6C6F5F6A6F686E5F68755F323232303030"
u16 ana_mqtt_command(char *msg, char* cmduuid, char *cmd)
{
	u16 len_topic,len_msg, len_cmduuid,len_command;
	//u8 tempu8;
	char temp[101];  //101 = #define max_len_hex_cmduuid 50  *2 +1 1 for'\0'
	char extract_command_topic[14];
	if (*msg!='3')
		return ana_mqtt_command___NOT_PUBLISH_MESSAGE; //return -1; // not public message
	msg++;
	//skip flags
	msg++;
	char high, low;
	high=*msg++;
	low=*msg++;
	len_msg=Hex2Value(high,low);	
	//len_hex_msg=Hex2Value(*msg++,*msg++);	
	
	high=*msg++;
	low=*msg++;
	len_topic=Hex2Value(high,low);	
	high=*msg++;
	low=*msg++;
	len_topic+=Hex2Value(high,low)+len_topic*256;

	len_cmduuid=len_topic-6;  				    // 6 means $creq/
	len_command=len_msg-2-len_topic;  	        //2 means 2 byte for len of topic 
	strncpy(extract_command_topic,msg , 12); 	// 12 hex lenth of $creq/    equal to 6*2
	extract_command_topic[12]='\0';
	if (strcmp(extract_command_topic,command_topic)!=0)
	{
		return ana_mqtt_command___NOT_command_topic; // mis match command topic $creq/
	}
	msg+=12;
	if (len_cmduuid > (u16) max_len_cmduuid)
	{
		return ana_mqtt_command___Exceend_max_len_CMUUID; // cmuuid hex lenth exceed max max_len_hex_cmduuid*2
	}
	strncpy(temp,msg,len_cmduuid*2);		//   len_cmduuid*2  is hex string length
	temp[len_cmduuid*2]='\0';
	if (Hex2Str(temp,cmduuid)!=1)
		return ana_mqtt_command___CMUUID_Error; // cmd uuid hex 2 str error
	msg+=len_cmduuid*2;
	if (strlen(msg)!=len_command*2)
	{
		return  ana_mqtt_command___msg_len_error; // commd length mis match
	}
	if (strlen(msg) > (u16) max_len_cmd*2)
		return ana_mqtt_command___msg_len_exceed_max; // cmd hex lenth exceed max_len_cmd
	if (Hex2Str(msg,cmd)!=1)
		return ana_mqtt_command___msg_hex2str_error; // cmd hex 2 str error
	return 1;  // success extract commad string
}

int CONNACK(char * msg)
{
	//char msg[]="20020000";
	int len, num_of_bite;
	int result;
	if (decode_variable_length(msg, &len, &num_of_bite))
		if ((int) strlen(msg)== 2*(len+2))
		{
			result=Hex2Value(msg[2*(len+2)-2], msg[2*(len+2)-1]);
			return result; 
		}
		else
			return CONNACK___meg_len_mismatch;
	else
	    return CONNACK___decode_length_error;
}


int handle_mqtt_response(char *msg)
{
	int mqqt_msg_type,return_code;
	char cmduuid[max_len_cmduuid];
	char cmd[max_len_cmd];
	mqqt_msg_type=msg[0]-'0';

	switch (mqqt_msg_type)
	{
		case MQTT_PUBLISH: //server publish to client 
			return_code=ana_mqtt_command(msg, cmduuid, cmd);
			if (return_code==1)
			{
				print_out(OK_response, true, false,0);
				return 1; 
			}
			else
			{
				print_out(ERROR_response, true, true,return_code);
				return return_code; 
			} 
			break; 
		case MQTT_CONNACK:
			
			return_code=CONNACK(msg);
			if (return_code==0)
			{
				print_out (msg_handle_mqtt_response[1],true,true,return_code); 
				return 1;
			}
			else
				return return_code; 
			break;
		default:
			print_out((char const *)msg_handle_mqtt_response[2], true, true,mqqt_msg_type);
			break;
	}
	return handle_mqtt_response___response_not_support;
}
int mqtt_flow(int state, int *return_code)
{
	int next_state=wrong_state;
	int len_mess;

	bool cmd_send_result=0;	
	char *cp;
	int remaining_len;
	int NSORF_response_return_code;
	switch (state)
	{
		case base_config:
			BASE_Config_NBIOT(8);
			next_state=nbiot_init;
			*return_code=mqtt_success_BASE_Config_NBIOT;
			break;
		case nbiot_init:  //init
			if (initNBIOT()==1)
			{
				next_state=nbiot_tcp;
				*return_code=mqtt_success_NBIOT_init;
			}
			else
			{
				next_state=wrong_state;
				*return_code=mqtt_fail_NBIOT_init;
			}
			break;
		case onenet_connect: //check onenet connected
			if (Get_Enter_Net_Status()==1)
			{
				next_state=nbiot_tcp;
				*return_code=mqtt_success_NBIOT_connect;
			}
			else
			{
				next_state=nbiot_init;
				*return_code=mqtt_fail_NBIOT_connect;
			}
			break;
		case nbiot_tcp: //tcp
			if (NB_Create_TCP(mqtt_server_ip, mqtt_server_port, &socket_used)==1)
			{
				//print_out ("socket is ", true,true,socket_used);
				next_state=mqtt_connect;
				*return_code=mqtt_success_tcp;
			}
			else
			{
				next_state=onenet_connect;
				*return_code=mqtt_fail_tcp;
			} 
			break;
		case mqtt_connect: //connect
			//sprintf(RXBuffer,cmd_ifs[3],socket_used);
			sprintf(gbuff,cmd_Send_Data,socket_used);
			len_mess=prepare_mqtt_connect_message(gbuff,PROID,AUTH_INFO,DEVID);
			if  (len_mess>=gbuffer_len)
			{
				//print_out("send message exceed RXBUFFER_LEN, quit program", true,true,len_mess);
				*return_code=mqtt_fail_connect_msglen_exceed_max;
				next_state=wrong_state;
				break;
			}	
		
			cmd_send_result=NBIOT_Cmd(gbuff, (char*)OK_response, (char*)ERROR_response, 1000,&pms);
			//print_out ("connect result",true,true,cmd_send_result);
			if (cmd_send_result!=1)
			{
				next_state=wrong_state;
				*return_code=mqtt_fail_connect_return_fail;
				break;
			}
			if (pms.NSOCLI_received)
			{
				next_state=nbiot_tcp;
				*return_code=mqtt_fail_connect_socket_closed;
				break;
			}
			if (pms.NSONMI_received)
			{
				next_state=mqtt_receive;
				*return_code=mqtt_success_connect_and_received_msg;
				break;
			}
			next_state=wait_state;
			*return_code=mqtt_success_connect;
			break;

		case mqtt_send:  //send
			
			build_1Data1value_json((char *)label_temperature, 100, lbuff);

			sprintf(gbuff,cmd_Send_Data,socket_used);
			len_mess=prepare_mqtt_Send_message(gbuff,lbuff,strlen(lbuff));
			if  (len_mess>=gbuffer_len)
			{
				//print_out("send message exceed RXBUFFER_LEN, quit program", true,true,len_mess);
				*return_code=mqtt_fail_send_msglen_exceed_max;
				next_state=wrong_state;
				break;

			}	
			cmd_send_result = NBIOT_Cmd(gbuff, (char*)OK_response, (char*)ERROR_response, 1000,&pms);
			//print_out ("send result ", true,true,cmd_send_result);
			if (!cmd_send_result)
			{
				next_state=nbiot_close_socket;
				*return_code=mqtt_fail_send_return_fail;
				break;
			}
			if (pms.NSOCLI_received)
			{
				next_state=nbiot_tcp;
				*return_code=mqtt_fail_send_socket_closed;
				break;
			}
			if (pms.NSONMI_received)
			{
				next_state=mqtt_receive;
				*return_code=mqtt_success_send_and_recieved_msg;
				break;
			}
			next_state=wait_state;
			*return_code=mqtt_success_send;
			break;
		case mqtt_receive:  //receive
			if (pms.NSONMI_received!=1)
			{
				next_state=wrong_state;
				*return_code=mqtt_fail_receive_state_without_msg;
				break;
			}
			//sprintf(RXBuffer,"AT+NSORF=%d,%d\n",socket_used,pms.NSONMI_pto_recieve_byte);
			sprintf(gbuff,cmd_Receive_Data,socket_used,pms.NSONMI_to_recieve_byte);
			cmd_send_result = NBIOT_Cmd(gbuff, (char*)OK_response, (char*)ERROR_response, 1000,&pms);
			//print_out ("recieve result",true,true,cmd_send_result);

			remaining_len=0;
			if (cmd_send_result==1)
			{
				cp=NSORF_response(gbuff ,&remaining_len, &NSORF_response_return_code,mqtt_server_ip,mqtt_server_port);
				if (cp==NULL)
				{
					next_state=mqtt_receive;
					*return_code=mqtt_fail_receive_ana_NSORF_response;
					break;
				}
				else
				{
					handle_mqtt_response(cp);
					next_state=wait_state;
					*return_code=mqtt_success_receive_handle_NSORF_response;
					break;
				}
			}
			if (pms.NSOCLI_received==1)
			{
				next_state=nbiot_tcp;
				*return_code=mqtt_fail_receive_socket_close;
				break;
			}
			if (pms.NSONMI_received==1 )
			{
				next_state=mqtt_receive;
				*return_code=mqtt_success_receive_more_to_receive;
				break;
			}
			if (remaining_len!=0)
			{
				pms.NSONMI_received=1;
				pms.NSONMI_to_recieve_byte=remaining_len;
				next_state=mqtt_receive;
				*return_code=mqtt_success_receive_remain_to_receive;
				break;
			}
			*return_code=mqtt_success_receive;
			next_state=wait_state;
			break;
		case nbiot_close_socket:
			Close_Socket(socket_used);
			next_state=nbiot_tcp;
			*return_code=mqtt_success_close_socket;
			break;
		case wait_state:
			Delay_ms(20000);
			next_state=mqtt_send;
			*return_code=mqtt_success_wait;
			break;
		case wrong_state:
			next_state=-1;
			*return_code=mqtt_fail_to_quit;
			break;
		default:
			//print_out ("unknonw ",true,true,state);
			next_state=wrong_state;
			*return_code=mqtt_fail_from_switch_defualt;
			break;
	}
	return next_state;
}
