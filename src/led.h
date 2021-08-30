#ifndef _LED_H_
#define _LED_H_

#include <Arduino.h>
#include "cled.h"
#include "bat.h"
#include "deepsleep.h"

extern CLED blueled;
extern CLED yellowled;

void led_operate();

extern bool isjoin;
extern bool isalert;
extern bool alerted;

extern bool joining;
extern bool umsging;
extern bool cmsging;

extern bool istof;
extern bool isintmpu;
extern bool isextmpu;


#endif
