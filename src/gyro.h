#ifndef _GYRO_H_
#define _GYRO_H_

#include <Arduino.h>
#include "MPU6050.h" //Electronic cats

#define OUTPUT_READABLE_ACCELGYRO
extern MPU6050 a1;
extern MPU6050 a2;

extern int16_t ax, ay, az, ax2, ay2, az2;
extern double x, y, z;
extern double x2, y2, z2;

extern int minVal;
extern int maxVal;

extern int globx;
extern int offsetx;
extern int stibx;
extern int stickrot;

void gyro_init();
void gyro_init(int);
void gyro_update();
void calrot3();
void showgyro();

#endif