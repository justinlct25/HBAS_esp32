#ifndef _CHECKBOOL_H_
#define _CHECKBOOL_H_

#include <Arduino.h>
#include <ArduinoNvs.h>

extern int16_t distance;
extern int stickrot;
extern bool isperson;
extern bool isbrake;
extern bool isalert;
extern bool istof;
extern bool isintmpu;
extern bool isextmpu;

extern int device_id;
extern int lim_distance;
extern int lim_angle;
extern char update_server_url[200];

extern bool smallerbrakeangle;


extern void buzzer();

void getNVSConfig();
void checkrot2();
void person();


#endif