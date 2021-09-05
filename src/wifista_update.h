#ifndef _WIFI_UPDATE_H
#define _WIFI_UPDATE_H

#include <Arduino.h>
#include <wifi_c.h>
#include <http.h>
#include <version.h>
#include <ota.h>

extern unsigned long previousWifiMillis;
extern long Wifiinterval;

void wifista_update();

#endif