#ifdef AVR
#include <Arduino.h>
#include "SoftwareSerial.h"
#else
#include <iostream>
#include <cstring>
#include <windows.h>
#endif
#ifndef fun_h
#define fun_h
#ifndef AVR
class ssop
#else
class ssop: public SoftwareSerial
#endif
{
public:
#ifndef AVR
	HANDLE port;
	HANDLE open_serial_port(const char * device, unsigned int baud_rate);
#endif
	char serial[20];
	ssop(int rx, int tx);
	int open_serial(const char *device);
	void close_serial();
	int write_port(char * buffer, unsigned int size);
	int read_port(char * buffer, unsigned int size);

};
extern ssop ssOnenet,ssGPS;
extern void print_out(char const *str, bool flag_add_return, bool flag_last_int, int last_int);
extern void Delay_ms(int ms);
#endif
