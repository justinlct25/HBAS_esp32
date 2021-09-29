#ifndef _TOF_H_
#define _TOF_H_

#include <Arduino.h>
#include <VL53L0X.h>

extern VL53L0X tof;
extern int16_t distance;
extern uint16_t raw_distance;
extern int eightoneeight_count;

void tof_init();
void getdistance();
void showtof();



#endif
