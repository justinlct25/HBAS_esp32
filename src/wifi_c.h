#ifndef _WIFI_C_H
#define _WIFI_C_H

#include <Arduino.h>
#include <WiFi.h>

extern const char *ssid;
extern const char *password;

void wifi_init();
void wifi_reconnect();
bool wifi_stat();
int wifi_strength();

#endif