#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <Arduino.h>
#include "cbuzzer.h"
#include "deepsleep.h"

extern CBUZ buz;
extern bool isalert;
extern float bat;

void buzzer_init();
void buzzer(int);

void buz_operate();

#endif