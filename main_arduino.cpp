#ifdef AVR
#include <Arduino.h>
#include "mqtt.hpp"
#include "rest.hpp"
#include "fun.hpp"
#include "msg.hpp"


int state=nbiot_init;
int return_code;
void setup() {
	Serial.begin(9600);
    if (ssOnenet.open_serial("")==0 )
        return;
}

void loop() {
    Serial.write(hello_world);
    delay(1000);
    state=mqtt_flow(state,&return_code);
	//if (state==-1)
	//	break;
}
#endif
