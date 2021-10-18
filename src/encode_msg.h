#ifndef _ENCODE_MSG_H_
#define _ENCODE_MSG_H_

#include <Arduino.h>
#include <gps.h>
#include <string>
#include "deepsleep.h"

extern String sday, smonth, syear;
extern String shour, sminute, ssecond, scentisecond;
extern String sLatitude, sLongitude;
extern String location;
extern String datetime;
extern String date;
extern int ptime;
extern int pdate;
extern int bat2;

//String encode_fmsg(String msgtype);   //legacy function

extern char ctimestamp[15];
extern time_t utctime();
extern char testmsg[70];

char *encode_cmsg(char);



#endif