#ifdef AVR
#include <Arduino.h>
#else
#include <iostream>
#include <cstring>
#endif

#include "gdef.hpp"
#include "msg.hpp"
#include "gerror.hpp"
#include "NBIOT.hpp"

#ifndef rest_h
#define rest_h
extern int rest_flow(int state, int *error_code);
#endif
