#ifndef _DATA_TRACER_H_
#define _DATA_TRACER_H_

#include <Arduino.h>
#include "bluetooth.h"

extern int globx;
extern int stickrot;
extern int16_t distance;
extern float bat;
extern int bat2;
extern String sLatitude;
extern String sLongitude;
extern double sLat;
extern double sLong;

extern int recordCounter;
extern int recordCounterB;
extern int recordCounterS;
extern char arecord[500][60];

extern bool isperson;
extern bool isbrake;
extern bool isalert;
extern bool isjoin;
extern bool isretry;
extern bool isend;
extern bool islateset;
extern bool isstartup;
extern bool alerted;

extern bool joining;
extern bool umsging;
extern bool cmsging;
extern bool amsging;
extern bool bmsging;
extern bool amsgsuc;

extern bool istof;
extern bool isintmpu;
extern bool isextmpu;

extern const char *ptr;
extern char h_globx[4];
extern char h_stickrot[4];
extern char h_distance[5];
extern char h_bat[4];
extern char h_latitude[10];
extern char h_longitude[11];
extern bool radar_state();

void showstatus();
void showrecord();
void showallbool();

extern BluetoothSerial bt;
void BTshowstatus();
void BTshowrecord();
void BTshowallbool();


#endif