#ifndef _CHECKI2C_H_
#define _CHECKI2C_H_
#include <HardwareSerial.h>
#include <Wire.h>

#define TOF_ADDR 41
#define INTMPU_ADDR 105
#define EXTMPU_ADDR 104

extern bool istof;
extern bool isintmpu;
extern bool isextmpu;

extern void tof_init();
extern void gyro_init(int);

void i2cdev_restore();
void showi2cstate();
void showi2cdev();

#endif