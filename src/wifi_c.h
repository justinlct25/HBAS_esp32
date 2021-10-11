#ifndef _WIFI_C_H
#define _WIFI_C_H

#include <NTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

extern char wifi_ssid[1000];
extern char wifi_password[100];

void wifi_init();
void wifi_reconnect();
bool wifi_stat();
int wifi_strength();

#endif