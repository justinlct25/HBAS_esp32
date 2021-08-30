#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <BluetoothSerial.h>

extern BluetoothSerial bt;

extern char buf[50];
extern char cvalue[20];
extern int inc;
extern char *line;
extern char *param;

extern unsigned long quit_time;
extern unsigned long extent_time;
extern bool isbtoff;

void bt_init();
void bt_routine();
void bt_quit();

#endif

