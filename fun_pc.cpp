
#ifndef AVR
#include "fun.hpp"
HANDLE ssop::open_serial_port(const char * device, unsigned int baud_rate)
{
  	port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,OPEN_EXISTING, 0, NULL);
	if (port == INVALID_HANDLE_VALUE)
  	{
		print_out("INVALID_HANDLE_VALUE ", false,false,0);
		print_out((char *)device,true,false,0);
    	return INVALID_HANDLE_VALUE;
  	}

  	// Flush away any bytes previously read or written.
  	BOOL success = FlushFileBuffers(port);
  	if (!success)
  	{
    	print_out("Failed to flush serial port",true,false,0);
		CloseHandle(port);
    	return INVALID_HANDLE_VALUE;
  	}

	// Configure read and write operations to time out after 100 ms.
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant    = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier  = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant   = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds


  	success = SetCommTimeouts(port, &timeouts);
  	if (!success)
  	{
    	print_out("Failed to set serial timeouts",true,false,0);
    	CloseHandle(port);
    	return INVALID_HANDLE_VALUE;
  	}

  	DCB state;
  	state.DCBlength = sizeof(DCB);
  	success = GetCommState(port, &state);
  	if (!success)
  	{
    	print_out("Failed to get serial settings", true,false,0);
    	CloseHandle(port);
    	return INVALID_HANDLE_VALUE;
  	}

  	state.BaudRate = baud_rate;
 	//state.BaudRate = CBR_9600;  // Setting BaudRate = 9600
	state.ByteSize = 8;         // Setting ByteSize = 8
	state.StopBits = 1 ;// Setting StopBits = 1
	state.Parity   = NOPARITY;  // Setting Parity = None

  	success = SetCommState(port, &state);
  	if (!success)
  	{
    	print_out("Failed to set serial settings", true,false,0);
    	CloseHandle(port);
    	return INVALID_HANDLE_VALUE;
  	}
   	return port;
}
int ssop::open_serial(const char *device)
{
    //const char device[] = "\\\\.\\COM3";
 	//uint32_t baud_rate = 9600;
	strcpy(serial,device);
 	unsigned int baud_rate = 9600;
  	port = open_serial_port(serial, baud_rate);
  	if (port == INVALID_HANDLE_VALUE) { return 0; }
  	return 1;
}
void ssop::close_serial()
{
	CloseHandle(port);
}

int ssop::write_port(char * buffer, unsigned int size)
{
	DWORD written;
  	BOOL success = WriteFile(port, buffer, size, &written, NULL);
 	if (!success)
  	{
    	print_out("Failed to write to port",true,false,0);
    	return -1;
  	}

  	if (written != size)
  	{
    	print_out("Failed to write all bytes to port",true,false,0);
    	return -1;
  	}
  	return 0;
}

int ssop::read_port(char * buffer, unsigned int size)
{
  	DWORD received;
  	BOOL success = ReadFile(port, buffer, size, &received, NULL);
  	if (!success)
  	{
    	print_out("Failed to read from port", true,false,0);
    	return -1;
  	}
  	return received;
}

ssop::ssop(int rx, int tx)
{
	port=HANDLE (0);
	return;
}

ssop ssOnenet(0,0),ssGPS(0,0);

void print_out(char const *str, bool flag_add_return, bool flag_last_int, int last_int)
{
	if (flag_last_int)
		if (flag_add_return)
			printf ("%s %d \n",str, last_int);
		else
			printf ("%s %d",str, last_int);
	else
		if (flag_add_return)
			printf ("%s \n",str);
		else
			printf ("%s",str);
	return;
}

void Delay_ms(int ms)
{
	Sleep(ms);
}


#endif
