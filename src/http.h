#ifndef _HTTP_H
#define _HTTP_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <wifi_c.h>

extern const char *URL;
extern String latestVersion;
extern String result;
extern String payload;
extern int httpResponseCode;

String http_version_check();

#endif