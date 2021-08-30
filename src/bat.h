#ifndef _BAT_H_
#define _BAT_H_

#include <Arduino.h>
#include <ESP32AnalogRead.h>

//battery sample
#define bat_sample_interval 60

extern ESP32AnalogRead adc;

extern float bat;
extern int bat2;
extern int bat_counter;

void bat_init();
void getbattery();
void samplebattery();
void showbattery();

#endif