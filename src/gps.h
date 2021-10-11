#ifndef _GPS_H_
#define _GPS_H_

#include <Arduino.h>
#include "TinyGPS++.h"
#include <ArduinoNvs.h>


extern TinyGPSPlus tgps;

void gps_init();
void gps_coolstart();
void gps_warmstart();
void gps_hotstart();
void gps_standby();
void gps_wakeup();
void tinygps();
void showgpsinfo();
time_t utctime();

extern String sday, smonth, syear;
extern String shour, sminute, ssecond, scentisecond;
extern String location;
extern String datetime;
extern String date;
extern int ptime;
extern int pdate;

#endif