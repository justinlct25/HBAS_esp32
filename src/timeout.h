#ifndef _TIMEOUT_H_
#define _TIMEOUT_H_

#include <Arduino.h>

extern unsigned long previousTimeoutMillis;
extern const long Timeoutinterval;

bool timeout(unsigned long);
bool timeout(unsigned long,long);


#endif
