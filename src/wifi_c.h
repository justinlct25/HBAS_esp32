#ifndef _WIFI_C_H
#define _WIFI_C_H

#include <NTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

extern const char *ssid;
extern const char *password;

void wifi_init();
void wifi_reconnect();
bool wifi_stat();
int wifi_strength();

#endif