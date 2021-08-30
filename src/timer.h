#ifndef _TIMER_H_
#define _TIMER_H_

#include <Arduino.h>

#define cuptimebuffer 12
#define ctimerbuffer 12

//uptime
extern char cuptime[cuptimebuffer];
char *getuptime();

//timer
extern char timer[ctimerbuffer];
char *gettimer(unsigned long);


#endif