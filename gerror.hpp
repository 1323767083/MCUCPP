#ifndef gerror_h
#define gerror_h
#define check_NBIOT_Cmd_result___unknown  			1001
#define check_NBIOT_Cmd_result___respnse_not_end  	1002

#define NBIOT_Cmd___not_end_return					1011
#define NBIOT_Cmd___write_port_fail					1012
#define NBIOT_Cmd___read_port_fail					1013

#define initNBIOT___checkNBIOT_fail					1021

#define NSORF_response___IP_error    				1031
#define NSORF_response___port_error					1032
#define NSORF_response___format_error               1033
#define NSORF_response___socket_mismatch			1034

#define ana_mqtt_command___NOT_PUBLISH_MESSAGE      1041
#define ana_mqtt_command___NOT_command_topic		1042
#define ana_mqtt_command___Exceend_max_len_CMUUID	1043
#define ana_mqtt_command___CMUUID_Error				1044
#define ana_mqtt_command___msg_len_error			1045
#define ana_mqtt_command___msg_hex2str_error		1046
#define ana_mqtt_command___msg_len_exceed_max		1047

#define CONNACK___decode_length_error				1051 
#define CONNACK___meg_len_mismatch					1052

#define handle_mqtt_response___response_not_support 1061

#define ana_get_responsed___not_found_0D0A			1071
#define ana_get_responsed___remaining_not_0			1072


#define rest_init_nbiot_fail						1110
#define rest_fail_send_get_command					1111
#define rest_receive_socket_close					1112
#define rest_receive_no_msg							1113
#define rest_receive_msg_unproper					1114
#define rest_receive_msg_no_inform					1115
#define rest_receive_msg_ana_fail					1116
#define rest_get_response_not_received				1117
#define rest_get_response_body_before_head			1118
#define rest_receive_msg_body_unproper				1119
#define rest_receive_msg_body_no_inform				1120
#define rest_receive_msg_body_ana_fail				1121
#define rest_unknown_state							1122
#define rest_fail_connect                           1123
#define rest_fail_tcp                               1124

#define mqtt_fail_NBIOT_init                        1140
#define mqtt_fail_NBIOT_connect                     1141
#define mqtt_fail_tcp                               1142
#define mqtt_fail_connect_msglen_exceed_max         1143
#define mqtt_fail_connect_return_fail               1144
#define mqtt_fail_connect_socket_closed             1145
#define mqtt_fail_send_msglen_exceed_max            1146
#define mqtt_fail_send_return_fail                  1147
#define mqtt_fail_send_socket_closed                1148
#define mqtt_fail_receive_state_without_msg         1149
#define mqtt_fail_receive_ana_NSORF_response        1150
#define mqtt_fail_receive_socket_close              1151
#define mqtt_fail_to_quit                           1152
#define mqtt_fail_from_switch_defualt               1153




#define rest_success_NBIOT_init                     8010
#define rest_success_connect                        8011
#define rest_success_tcp                            8012
#define rest_success_send_get                       8013
#define rest_success_get_response_head              8014
#define rest_success_get_response_body              8015
#define rest_success_close_socket                   8016

#define mqtt_success_BASE_Config_NBIOT              8030
#define mqtt_success_NBIOT_init                     8031
#define mqtt_success_NBIOT_connect                  8032
#define mqtt_success_tcp                            8033
#define mqtt_success_connect_and_received_msg       8034
#define mqtt_success_connect                        8035
#define mqtt_success_send_and_recieved_msg          8036
#define mqtt_success_send                           8037
#define mqtt_success_receive_handle_NSORF_response  8038
#define mqtt_success_receive_more_to_receive        8039
#define mqtt_success_receive_remain_to_receive      8040
#define mqtt_success_receive                        8041
#define mqtt_success_close_socket                   8042
#define mqtt_success_wait                           8043




#endif
