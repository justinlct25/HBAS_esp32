#ifndef _LOAR_H_
#define _LORA_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "timeout.h"
#include "data_tracer.h"

#define LORABUSY (strstr(input,"LoRaWAN modem is busy")!=NULL)

//Lora
extern SoftwareSerial lora; //TX,RX

//lorawan send and receive
extern bool isjoin;

extern char cmsg[60];

extern char input[60];
extern char lvalue[60];
extern int inn;
extern char *ln;
extern char *par;
extern bool isack;
extern bool isackb;

extern bool joining;
extern bool umsging;
extern bool cmsging;
extern bool cmsgingb;

extern bool amsging;
extern bool bmsging;

extern bool amsgsuc;
extern bool bmsgsuc;

extern RTC_DATA_ATTR int rssi;
extern RTC_DATA_ATTR int snr;

// void manuallora();
void lora_rountine();
void njoinlora();
void nsendloramsg(char *);
void nsendloracmsg(char *);
void lora_getpayload();
void lora_reset();
void lora_sleep();
void lora_wakeup();
void showlora();
void lora_msginghandle();
void lora_AT_init();

//legacy
//extern bool isretry; 
//extern bool isend;
//extern String tmsg;
//extern String feedback;
//void joinlora();
//void sendloramsg(String);
//void sendloracmsg(String);
//void sendloracmsg(char*);

#endif
