#include "NBIOT.hpp"
u16 check_NBIOT_Cmd_result(char *response, const char * reply_success, const char * reply_fail,struct pluse_comands *pm)
{
	bool flag_found_return=false;
	u16  return_code=check_NBIOT_Cmd_result___unknown;
	
	char *temp;
	char msg[lbuffer_len];  //only plus response & ok error //reboot hsa long message
	pm->NSOCLI_received=0;
	pm->NSONMI_received=0; 
	temp=response+gbuffer_len;
	while (*temp==(char) 0)
	{
		if (temp>= response)
			temp--;
		else
			return check_NBIOT_Cmd_result___respnse_not_end;
	}
	while(response<temp)
	{
		response=sgetline(response, msg);

		if (strlen(msg)==0)
			continue;
		if (!flag_found_return)
		{
			if (reply_fail!=NULL)
				if(strstr(msg, reply_fail) != NULL)
				{
					return_code=0;
					flag_found_return=true;
					continue;
				}
			if (reply_success!=NULL)
				if(strstr(msg, reply_success) != NULL)
				{
					return_code=1;
					flag_found_return=true;
					continue;
				}
		}
		if (strstr(msg, NSONMI_response) != NULL)
		{
			//char response[]="+NSONMI:2,4";
			pm->NSONMI_received=1;
			//sscanf(&msg[8], "%d,%d",&pm->NSONMI_received_socket,&pm->NSONMI_to_recieve_byte);
            sscanf(&msg[8], format_Decimal_2d,&pm->NSONMI_received_socket,&pm->NSONMI_to_recieve_byte);
			continue;
		}
		if (strstr(msg, NSOCLI_response) != NULL)
		{
			//char response[]="+NSOCLI:2";
			pm->NSOCLI_received=1;
			//sscanf(&msg[8], "%d",&pm->NSOCLI_received_socket);
            sscanf(&msg[8], format_Decimal_1d,&pm->NSOCLI_received_socket);
			continue;
		}
	}
	return return_code;
}


u16 NBIOT_Cmd ( char *cmd, const char *reply_success, const char *reply_fail, u32 waittime,struct pluse_comands *pm)
{
  	int received;
  	if (*(cmd+strlen(cmd)-1)!='\n')
  	{
		return NBIOT_Cmd___not_end_return;
    }	
    
    print_out(cmd, false,false,0);
  	if (ssOnenet.write_port(cmd, strlen(cmd))==-1)
  	{
   		return NBIOT_Cmd___write_port_fail;
  	}
    if ( ( reply_success == 0 ) && ( reply_fail == 0 ) )
        return 1;
    Delay_ms(waittime);
	memset(gbuff, 0, gbuffer_len);
	received = ssOnenet.read_port(gbuff, gbuffer_len);
	if (received < 0) {
    	return NBIOT_Cmd___read_port_fail;
	}
	return check_NBIOT_Cmd_result(gbuff, reply_success,reply_fail,pm);
}


u16 checkNBIOT(void)
{
    return NBIOT_Cmd((char *)cmd_checkNBIOT, OK_response, NULL, 1000, &pms);
}

u16 NB_RESET(void)
{
    return NBIOT_Cmd((char *)cmd_NB_RESET, OK_response, NULL, 10000, &pms);
}


u16 Get_IMEI(void)
{
    if(1==NBIOT_Cmd((char *)cmd_Get_IMEI, (char*)OK_response, (char*)ERROR_response, 100,&pms))
    {
        memcpy((char *)NBIOT_MODULE_INFO.IMEI, gbuff + 8, 15);
        return 1 ;
    }
    return 0 ;
}


u16 Get_IMSI(void)
{
	if(1==NBIOT_Cmd((char *)cmd_Get_IMSI, (char*)OK_response, (char*)ERROR_response, 2000, &pms))
    {
        memcpy((char *)NBIOT_MODULE_INFO.IMSI, gbuff + 2, 15);
        return 1 ;
    }

    return 0 ;
}


u16 Get_Enter_Net_Status(void)
{
    char buffer[10] = {0};
    u16 status;
    status = NBIOT_Cmd((char *)cmd_Get_Enter_Net_Status, (char*)OK_response, (char*)ERROR_response, 5000, &pms);
    memcpy(buffer, gbuff + 9, 1);
    status = atoi(buffer);
    if(1 == status)
        return 1;
    else
        return 0 ;
}

u16 Get_CSQ(void)
{
    int status;
    char csq_buffer[10] = {0};
    //status = NBIOT_Cmd("AT+CSQ\n", "+CSQ", NULL, 100, &pms);
    status = NBIOT_Cmd((char *)cmd_Get_CSQ, CSQ_response, NULL, 100, &pms);
    if(status == 1)
    {
        memcpy(csq_buffer, gbuff + 7, 2);
        return atoi(csq_buffer);
    }
    return 99 ;
}


u16 NB_Create_TCP(const char *server_ip, int port, int *socket )
{
    u16 status;
    //1.create TCP Socket
    status = NBIOT_Cmd((char*)cmd_Create_socket, (char*)OK_response, NULL, 2000, &pms);
    *socket=gbuff[2]-'0';

	//2.if sucess connect to server ip with port 
    if(1 == status)
    {
        sprintf(gbuff,(char*)cmd_Connect_to_server , *socket, server_ip, port);
        status = NBIOT_Cmd(gbuff,(char*) OK_response, NULL, 2000, &pms);
        return status;
    }
    return 0 ;
}



u16 Close_Socket(int socket)
{
    sprintf(gbuff,cmd_Close_Socket,socket );
    return NBIOT_Cmd(gbuff, (char*)OK_response, (char*)ERROR_response, 2000, &pms);
}



u16 initNBIOT()
{
    u16 status_return;
    NBIOT_MODULE_INFO.Init_NB_Status=0;
    while(1)
    {
        switch(NBIOT_MODULE_INFO.Init_NB_Status)
        {
               case 0:
                status_return = checkNBIOT();
                if(1 != status_return)
                {
                    print_out(nb_init_messp[0],true,false,0);
                    return initNBIOT___checkNBIOT_fail;
                }
                else
                {
                    //print_out("NB model hardware connected",true,false,0);
                    print_out(nb_init_messp[0*2+1],true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 1 ;
                }
                break ;
            case 1:
            	NBIOT_MODULE_INFO.Init_NB_Status = 2;
				break ;
				/*
                status_return = BASE_Config_NBIOT(8);
                if(false == status_return)
                {
                    NBIOT_MODULE_INFO.Init_NB_Status = 0 ;
                    print_out("NBIOT base config fail...\n");
                }
                else
                {
                    NBIOT_MODULE_INFO.Init_NB_Status = 2 ;
                    print_out("NBIOT base config success...\n");
                }
				*/
                break ;

            case 2:
                status_return = NB_RESET();
                if(1 != status_return)
                {
					print_out(nb_init_messp[2*2],true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 0 ;
                }
                else
                {
                    print_out(nb_init_messp[2*2+1],true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 3 ;
                }
                break ;
            case 3:
                status_return = Get_IMEI();
                if(1 != status_return)
                {
                    print_out(nb_init_messp[3*2],true,false,0);
					NBIOT_MODULE_INFO.Init_NB_Status = 0 ;
                }
                else
                {
                    NBIOT_MODULE_INFO.Init_NB_Status = 4 ;
                    print_out(nb_init_messp[3*2+1],false,false,0);
					print_out(NBIOT_MODULE_INFO.IMEI,true,false,0);
                }
                break ;
            case 4:
                status_return = Get_IMSI();
                if(1 != status_return)
                {
                    print_out(nb_init_messp[4*2+0],true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 0 ;
                }
                else
                {
                	print_out(nb_init_messp[4*2+1],false,false,0);
					print_out(NBIOT_MODULE_INFO.IMSI,true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 5 ;
                }
                break ;

            case 5:
                status_return = Get_Enter_Net_Status();
                if(1!= status_return)
                {
                    print_out(nb_init_messp[5*2+0],true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 5 ;
                }
                else
                {
                    print_out(nb_init_messp[5*2+1],true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 6 ;
                }
                break ;

            case 6:
                NBIOT_MODULE_INFO.signalCSQ = Get_CSQ();
                if(99 == NBIOT_MODULE_INFO.signalCSQ)
                {
                    print_out(nb_init_messp[6*2+0],true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 5 ;
                }
                else
                {
                    char tempstr[20]; 
                    itoa(NBIOT_MODULE_INFO.signalCSQ,tempstr,10);
					print_out(nb_init_messp[6*2+1],false,false,0);
					print_out(tempstr,true,false,0);
                    NBIOT_MODULE_INFO.Init_NB_Status = 7 ;
                }
                break ;
            default:
                break ;
        }

        if(7 == NBIOT_MODULE_INFO.Init_NB_Status)
        {
			break ;
        }
    }
    return 1;
}

//NBIOT base config===>band 5==>china telecom 8===>china mobile
//bool BASE_Config_NBIOT(char *device, int band)
u16 BASE_Config_NBIOT(int band)
{
    char buffer[50] = {0};
    u16 status;

	struct pluse_comands pms;
	
	status = checkNBIOT(); //not sure whether this should start before following command to make following success 
	print_out(msg_BASE_Config_NBIOT[0],true, true,status);
	// set minimum power is drawn
    status = NBIOT_Cmd((char *)cmd_Set_minimum_power, OK_response, ERROR_response, 100,&pms);

    //set nband 
	sprintf(buffer, cmd_Set_NBAND, band);
    status = NBIOT_Cmd(buffer, (char*)OK_response, NULL, 100,&pms);
    print_out(msg_BASE_Config_NBIOT[1],true, true,status);

    //restart
	NB_RESET(); 

    //autoconnect
    //status = NBIOT_Cmd("AT+NCONFIG=AUTOCONNECT,TRUE\n", "OK", NULL, 100,&pms);
    status = NBIOT_Cmd((char *)cmd_base_config_1, (char*)OK_response, NULL, 100,&pms);
	print_out(msg_BASE_Config_NBIOT[2],true, true,status);
	
    //set item 2 and item 3
    //status = NBIOT_Cmd("AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE\n", "OK", NULL, 100,&pms);
    status = NBIOT_Cmd((char *)cmd_base_config_2, (char*)OK_response, NULL, 100,&pms);
    print_out(msg_BASE_Config_NBIOT[3],true, true,status);

    //status = NBIOT_Cmd("AT+NCONFIG=CR_0859_SI_AVOID,TRUE\n", "OK", NULL, 100,&pms);
    status = NBIOT_Cmd((char *)cmd_base_config_3, (char*)OK_response, NULL, 100,&pms);
    print_out(msg_BASE_Config_NBIOT[4],true, true,status);
	return status ;
}

char *NSORF_response(char * response,int *remaining_len, int *return_code, const char *desired_ip, int desired_port)
{
	int length;
	char buffer[20];
	char *cp;
	cp=get_str_before_sep(response, ',', buffer);
	if (cp==NULL)
	{
		*return_code=NSORF_response___format_error;
		return NULL;
	}
	//socket=atoi(buffer);
	if (atoi(buffer)!=socket_used)
	{
		*return_code=NSORF_response___socket_mismatch;
	}
	cp=get_str_before_sep(cp, ',', buffer);
	if (cp==NULL)
	{
		*return_code=NSORF_response___format_error;
		return NULL;
	}
	//if (strcmp(mqtt_server_ip,buffer)!=0)
	if (strcmp(desired_ip,buffer)!=0)
	{
		*return_code=NSORF_response___IP_error;
		return NULL;
	}
	cp=get_str_before_sep(cp, ',', buffer);
	if (cp==NULL)
	{
		*return_code= NSORF_response___format_error;
		return NULL;
	}
		
	//if (mqtt_server_port!=atoi(buffer))
	if (desired_port!=atoi(buffer))
	{
		*return_code= NSORF_response___port_error;
		return NULL; 
	}

	cp=get_str_before_sep(cp, ',', buffer);
	if (cp==NULL)
	{
		*return_code= NSORF_response___format_error;
		return NULL;
	}
	length=atoi(buffer);
	*remaining_len=atoi(cp+length*2+1);  //end of msg +, 
	*(cp+length*2)='\0';
	return cp; 
}







