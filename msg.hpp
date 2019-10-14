#ifndef msg_h
#define msg_h

#ifndef AVR
const char  nb_init_messp[14][100] = {
	"NB module hardware fail..",   //0*2
	"NB model hardware connected", //0*2+1
	"",
	"",
	"Module reset fail",			//2*2
	"Module reset success",			//2*2+1
	"Fail get model IMEI",          //3*2
	"model IMEI: ",                 //3*2+1
	"SIM card not found, can not connect to network", //4*2
	"SIM IMSI: ", 									  //4*2+1
	"Fail to connect to the network, please reset the module", //5*2
	"Success connect module to the network", 					//5*2+1
	"No Signal",					//6*2
	"signal: ",						//6*2+1
};
const char msg_BASE_Config_NBIOT[5][30] ={
	"",
	"AT+NBAND ",
	"AT+NCONFIG result ",
	"AT+NCONFIG result ",
	"AT+NCONFIG result "
};


const char format_Decimal_len[4]="%d,";
const char format_Decimal_1d[5]="%d";
const char format_Decimal_2d[10]="%d,%d";

const char format_2Hex_len[9]="%02x%02x";
const char format_1Hex_len[5]="%02X";
const char format_return_code[20]="return_code %d";


const char mqtt_msg_packet_type_connect[3]="10";
const char mqtt_msg_packet_type_send[3]="30";
const char mqtt_connect_variable_head_1part[21]="00044D51545404C00178";
const char mqtt_send_variable_head_1part[13]="000324647001";
const char label_temperature[15]="temperature";

const char rest_msg_200[5]="200";
const char rest_msg_OK[5]="OK";


const char msg_handle_mqtt_response[3][40] ={"PUBLISH revceived","CONNACK revceived ","handle_server_response not support"};

const char template_1d1v[100] ="{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%.2f}]}]}";

const char template_1d2v[100] ="{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%.2f,\"lat\":%.2f]}]}";

const char command_topic[14] ="24637265712F";  //$creq/

const char HttpGet[80] = "GET %s?%s HTTP/1.1\r\nHost: %s:%d\r\n Connection: Keep-Alive\r\n\r\n";

const char proper_Get_response_head[35] ="485454502F312E3020323030204F4B0D0A"; //"HTTP/1.0 200 OK"  //485454502F312E3120323030204F4B0D0A  "HTTP/1.1 200 OK"


#else

const char  nb_init_messp[14][100] PROGMEM = {
	"NB module hardware fail..",   //0*2
	"NB model hardware connected", //0*2+1
	"",
	"",
	"Module reset fail",			//2*2
	"Module reset success",			//2*2+1
	"Fail get model IMEI",          //3*2
	"model IMEI: ",                 //3*2+1
	"SIM card not found, can not connect to network", //4*2
	"SIM IMSI: ", 									  //4*2+1
	"Fail to connect to the network, please reset the module", //5*2
	"Success connect module to the network", 					//5*2+1
	"No Signal",					//6*2
	"signal: ",						//6*2+1
};
const char msg_BASE_Config_NBIOT[5][30] PROGMEM ={
	"",
	"AT+NBAND ",
	"AT+NCONFIG result ",
	"AT+NCONFIG result ",
	"AT+NCONFIG result "
};

const char format_Decimal_len[4] PROGMEM ="%d,";
const char format_Decimal_1d[5] PROGMEM ="%d";
const char format_Decimal_2d[10] PROGMEM ="%d,%d";
const char format_2Hex_len[9] PROGMEM ="%02x%02x";
const char format_1Hex_len[5] PROGMEM="%02X";
const char format_return_code[20] PROGMEM ="return_code %d";


const char mqtt_msg_packet_type_connect[3] PROGMEM ="10";
const char mqtt_msg_packet_type_send[3] PROGMEM ="30";
const char mqtt_connect_variable_head_1part[21] PROGMEM ="00044D51545404C00178";
const char mqtt_send_variable_head_1part[13] PROGMEM ="000324647001";
const char label_temperature[15] PROGMEM ="temperature";

const char rest_msg_200[5] PROGMEM ="200";
const char rest_msg_OK[5] PROGMEM ="OK";


const char msg_handle_mqtt_response[3][40] PROGMEM ={"PUBLISH revceived","CONNACK revceived ","handle_server_response not support"};

const char template_1d1v[100] PROGMEM ="{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":%.2f}]}]}";

const char template_1d2v[100] PROGMEM ="{\"datastreams\":[{\"id\":\"%s\",\"datapoints\":[{\"value\":{\"lon\":%.2f,\"lat\":%.2f]}]}";

const char command_topic[14] PROGMEM ="24637265712F";  //$creq/

const char HttpGet[80] PROGMEM = "GET %s?%s HTTP/1.1\r\nHost: %s:%d\r\n Connection: Keep-Alive\r\n\r\n";

const char proper_Get_response_head[35] PROGMEM ="485454502F312E3020323030204F4B0D0A"; //"HTTP/1.0 200 OK"  //485454502F312E3120323030204F4B0D0A  "HTTP/1.1 200 OK"

const char hello_world[15] PROGMEM ="Hello world";
#endif

#endif

