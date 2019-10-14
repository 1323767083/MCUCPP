#include "rest.hpp"
void  prepare_get_cmd_hex(char *hexout,char *strout,char * content, int socket)
{
	//const char content[] = "IMEI=863716040605724&IMSI=460046670705364";
    sprintf(strout, HttpGet, tecent_api, content, tecent_server_ip, tecent_server_port);
    sprintf(hexout+strlen(hexout),format_Decimal_len,strlen(strout));
    Str2Hex(strout,hexout+strlen(hexout));
    *(hexout+strlen(hexout))='\n';
}

int ana_get_received(char *in)
{
	//"HTTP/1.0 200 OK"  //485454502F312E3120323030204F4B0D0A  "HTTP/1.1 200 OK"
	char *ip;

	if (Hex2Str(in, lbuff)!=1)
	{
		return 0;
	}
	ip=get_str_before_sep(lbuff, ' ', gbuff); //http/1.0 or http/1.1
	ip=get_str_before_sep(ip, ' ', gbuff); //200
	if (strcmp(gbuff, rest_msg_200)!=0)
	{
		return 0;
	}
	ip=sgetline(ip, gbuff);  //OK
	if (strcmp(gbuff, rest_msg_OK)!=0)
	{
		return 0;
	}
	return 1;
}

int ana_get_responsed(char *in, char *out)
{
	char *ip;
	if (Hex2Str(in, out)!=1)
	{
		return 0;
	}
	ip=out;
	while (ip!=NULL)
	{
		ip=sgetline(ip, in);
		if (strlen(in)==0)  //\r\n
		{
			strcpy(in,ip);
			strcpy(out,in);
			break;
		}
		else
			continue;

	}
	return 1;
}

int rest_flow(int state, int *return_code)
{
	int next_state=-1;
	int remaining_len,NSORF_response_return_code;
	bool cmd_send_result=0;
	char *cp;
	*return_code=0;
	init_globals();
	switch (state)
	{
		case rest_nbiot_init:
			if (initNBIOT()==1)
			{
				next_state=rest_nbiot_tcp;
				*return_code=rest_success_NBIOT_init;
			}
			else
			{
				*return_code=rest_init_nbiot_fail;
				next_state=-1;
			}
			break;
		case rest_onenet_connect:
			if (Get_Enter_Net_Status()==1)
			{
				next_state=rest_nbiot_tcp;
				*return_code=rest_success_connect;
			}
			else
			{
				next_state=rest_nbiot_init;
				*return_code=rest_fail_connect;
			}
			break;
		case rest_nbiot_tcp:
			if (NB_Create_TCP(tecent_server_ip, tecent_server_port, &socket_used)==1)
			{
				//print_out ("socket is ", true,true,socket_used);
				next_state=rest_get;
				*return_code=rest_success_tcp;
			}
			else
			{
				next_state=rest_onenet_connect;
				*return_code=rest_fail_tcp;
			}
			break;
		case rest_get:
			sprintf(gbuff,cmd_Send_Data,socket_used);
			prepare_get_cmd_hex(gbuff,lbuff,(char *)"IMEI=863716040605724&IMSI=460046670705364", socket_used);
			cmd_send_result=NBIOT_Cmd(gbuff, (char*)OK_response, (char*)ERROR_response, 10000,&pms);
			if (cmd_send_result!=1)
			{
				*return_code=rest_fail_send_get_command;
				next_state=-1;
				break;
			}
			if (pms.NSOCLI_received==1)
			{
				*return_code=rest_receive_socket_close;
				next_state=-1;
				break;
			}
			if (pms.NSONMI_received!=1)
			{
				*return_code=rest_receive_no_msg;
				next_state=-1;
				break;
			}
			next_state=rest_get_response_head;
			*return_code=rest_success_send_get;
			break;

		case rest_get_response_head:
			sprintf(gbuff,cmd_Receive_Data,socket_used,pms.NSONMI_to_recieve_byte);
			cmd_send_result = NBIOT_Cmd(gbuff, (char*)OK_response, (char*)ERROR_response, 1000,&pms);
			if (cmd_send_result!=1)
			{
				*return_code=rest_receive_msg_unproper;
				next_state=-1;
				break;
			}
			cp=NSORF_response(gbuff ,&remaining_len, &NSORF_response_return_code,tecent_server_ip,tecent_server_port);
			if (cp==NULL)
			{
				*return_code=rest_receive_msg_no_inform;
				next_state=-1;
				break;
			}
			if (ana_get_received(cp)!=1)  //  get receive properly
			{
				*return_code=rest_receive_msg_ana_fail;
				next_state=-1;
				break;
			}
			if (remaining_len==0)
			{
				*return_code=rest_get_response_not_received;
				next_state=-1;
				break;
			}
			pms.NSONMI_received=1;
			pms.NSONMI_to_recieve_byte=remaining_len;
			next_state=rest_get_response_body;
			*return_code=rest_success_get_response_head;
			break;
		case rest_get_response_body:
			if (pms.NSONMI_received!=1)
			{
				*return_code=rest_get_response_body_before_head;
				next_state=-1;
				break;
			}

			sprintf(gbuff,cmd_Receive_Data,socket_used,pms.NSONMI_to_recieve_byte);
			cmd_send_result = NBIOT_Cmd(gbuff, (char*)OK_response, (char*)ERROR_response, 10000,&pms);
			if (cmd_send_result!=1)
			{
				*return_code=rest_receive_msg_body_unproper;
				next_state=-1;
				break;
			}
			cp=NSORF_response(gbuff ,&remaining_len, &NSORF_response_return_code,tecent_server_ip,tecent_server_port);
			if (cp==NULL)
			{
				*return_code=rest_receive_msg_body_no_inform;
				next_state=-1;
				break;
			}
			if (ana_get_responsed(cp, lbuff)!=1)  //get get data data
			{
				*return_code=rest_receive_msg_body_ana_fail;
				next_state=-1;
				break;
			}
			//printf("get received message: %s \n",lbuff);
			next_state=rest_close_socket;
			*return_code=rest_success_get_response_body;
			break;
		case rest_close_socket:
			Close_Socket(socket_used);
			next_state=reset_success_state;
			*return_code=rest_success_close_socket;
			break;

		case reset_success_state:
			next_state=-1;  //-1 means quit
			*return_code=0;
			break;
		default:
			//print_out ("unknown ",true,true,state);
			*return_code=rest_unknown_state;
			next_state=-1;//-1 means quit
			break;
	}
	return next_state;
}

