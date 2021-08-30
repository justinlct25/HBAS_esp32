#ifndef _MQTT_H_
#define _MQTT_H_

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

//extern WiFiClient espClient;
//extern PubSubClient client(WiFiClient);
extern const char* mqtt_server;
extern uint16_t mqtt_port;
extern String s;

void mqtt_init();
void mqttpub();

extern int globx;
extern int stickrot;
extern int16_t distance;
extern float bat;
extern String sLatitude;
extern String sLongitude;
extern String sday, smonth, syear;
extern String shour, sminute, ssecond, scentisecond;
extern char* getuptime();
extern bool radar_state();

//to be added
//mpu
extern int stibx;
extern char cver[3][2];
extern char nver[3][2];


extern bool isperson;
extern bool isbrake;
extern bool isalert;
extern bool isjoin;
//extern bool isretry; //legacy
//extern bool isend;   //legacy
extern bool islateset;
extern bool isstartup;
extern bool alerted;

#endif