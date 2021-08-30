#ifndef _OTA_H_
#define _OTA_H_

//#include <WiFi.h>
#include <WiFiClient.h>
#include <HttpsOTAUpdate.h>

extern WiFiClient client;
extern bool isstartup;

extern void HttpEvent(HttpEvent_t *);

extern void ota_update();

extern void ota_status();

#endif