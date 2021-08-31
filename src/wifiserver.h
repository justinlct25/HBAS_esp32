
// #include <Arduino.h>
#include <ArduinoNvs.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <checkbool.h>
#include <ESPmDNS.h>
#include <sstream>
#include <bat.h>
#include <version.h>

extern const char* wifi_ap_ssid;
extern const char* wifi_ap_password;
extern const char index_html[] PROGMEM;
extern const char* PARAM_DISTANCE;
extern const char* PARAM_ANGLE;



void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);
void notFound(AsyncWebServerRequest *request);
void wifiServer_run();
void wifiwebsocket_init();
void SerialPrintLimitDistanceAngle();
void webSocketMeasureInfo();
void webSocketLoggerInfo();