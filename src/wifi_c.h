#ifndef _WIFI_C_H
#define _WIFI_C_H

#include <NTPClient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Time.h>

extern ESP32Time rtc;
extern const char *ssid;
extern const char *password;
extern const String weekDays[7];
extern const String months[12];

void wifi_init();
void wifi_reconnect();
bool wifi_stat();
int wifi_strength();

#endif