#ifndef AVR
#include <iostream>
#include <cstring>

#include "fun.hpp"
#include "mqtt.hpp"
#include "rest.hpp"

char serial_str[20]="\\\\.\\COM";     //"\\\\.\\COM3"
/*
char prototype[5];
char serial_number_s[2];
int state;
bool flag_finished=false;
char finished_msg[20];

void setup() {
	if (strcmp(prototype,"mqtt")==0)
	{
		strcat(serial_str,serial_number_s);
		if (ssOnenet.open_serial(serial_str)==0 )
			return;
		state=nbiot_init;
	}

	if (strcmp(prototype,"get")==0)
	{
		strcat(serial_str,serial_number_s);
		if (ssOnenet.open_serial(serial_str)==0 )
			return;
		state=rest_nbiot_init;
	}

}

void loop() {
	int fun_return_code;
	if (not flag_finished)
	{
		Delay_ms(2000);
		print_out(finished_msg,true,false,0);
	}
	if (strcmp(prototype,"mqtt")==0)
	{
		state=mqtt_flow(state);
		if (state==-1)
		{
			ssOnenet.close_serial();
			flag_finished=true;
			strcpy(finished_msg,"Error happened");
			return;
		}
	}
	if (strcmp(prototype,"get")==0)
	{
		state=rest_flow(state, &fun_return_code);
		if (state==-1)
		{
			if (fun_return_code!=0)
			{
				//print_out("Error with Code", true, true,fun_return_code);
				strcpy(finished_msg,"Error happened");
				flag_finished=true;
			}
			else
			{
				print_out("Success", true, false,0);
				strcpy(finished_msg,"Success");
				flag_finished=true;
			}
			return;
		}
	}
}

int main(int argc, char *argv[]) {
	strcpy(prototype,argv[1]);
	strcpy(serial_number_s,argv[2]);
	setup();
	while (true)
	{
		loop();
	}

}
*/

int main(int argc, char *argv[]) {
	int state;
	int fun_return_code;
	if (strcmp(argv[1],"mqtt")==0)
	{
		strcat(serial_str,argv[2]);
		if (ssOnenet.open_serial(serial_str)==0 )
			return 0;
		state=nbiot_init;
		while (true)
		{
			state=mqtt_flow(state,&fun_return_code);
			if (state==-1)
				break;
		}
		ssOnenet.close_serial();
		return 0; 
	}
	if (strcmp(argv[1],"get")==0)
	{
		strcat(serial_str,argv[2]);
		if (ssOnenet.open_serial(serial_str)==0 )
			return 0;
		state=rest_nbiot_init;
		while (true)
		{
			state=rest_flow(state, &fun_return_code);
			if (state==-1)
			{
				if (fun_return_code!=0)
					print_out("Error with Code", true, true,fun_return_code);
				else
					print_out("Success", true, false,0);
				break;
			}
			else
			{
				sprintf(lbuff, format_return_code,fun_return_code );
				print_out(lbuff,true,false,0);
			}
		}
		ssOnenet.close_serial();
		return 0;

	}
}

#endif
