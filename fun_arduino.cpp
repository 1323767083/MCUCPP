#include "fun.hpp"
#ifdef AVR
	int ssop::open_serial(const char *device)
  {
      begin(9600);
      return true;
  }
	void ssop::close_serial()
  {
      return;
  }

	int ssop::write_port(char * buffer, unsigned int size)
  {
      unsigned int bytesSent = print((char *)buffer);
      if (bytesSent==size)
          return 0;
      else
          return -1;
  }
	int ssop::read_port(char * buffer, unsigned int size)
  {
      unsigned int i;
      int received=0;
      char readch;
      for  (i=0;i<size;i++)
      {
          readch=read();
          if (readch!=EOF)
          {
              received++;
              * (buffer+i)=readch;
          }
          else
              break;
      }
        return received;
  }
ssop::ssop(int Rx, int Tx):SoftwareSerial(Rx, Tx){

}

ssop ssOnenet(8,9);
ssop ssGPS(10,11);

void print_out(char const *str, bool flag_add_return, bool flag_last_int, int last_int)
{
  char tempstr[20];

  if (flag_last_int)
    if (flag_add_return)
    {
      Serial.print(str);
      itoa(last_int,tempstr,10);
      Serial.println(tempstr);
      //sprintf (tempstr,"%s %d \n",str, last_int);
    }
    else
    {
      Serial.print(str);
      itoa(last_int,tempstr,10);
      Serial.print(tempstr);
      //sprintf (tempstr,"%s %d",str, last_int);
    }
  else
    if (flag_add_return)
      Serial.println(str);
      //sprintf (tempstr,"%s \n",str);
    else
      Serial.print(str);
      //sprintf (tempstr,"%s",str);
  return;
}

void Delay_ms(int ms)
{
	delay(ms);
}
#endif
