#include "mqtt.h"

WiFiClient espClient;
PubSubClient mqttclient(espClient);
const char* mqtt_server = "218.253.145.198";
uint16_t mqtt_port = 1883;
String s=";";

void mqtt_init(){
    mqttclient.setServer(mqtt_server, mqtt_port);
    mqttclient.connect("@#!kjkwlqs");
}

void mqttpub(){
  if (!mqttclient.connected()) {
    Serial.println("mqtt broker not connected. Reconnecting ...");
    mqttclient.connect("@#!kjkwlqs");
  }
  else{
    String data = String(globx) +s+ String(stickrot) +s+ String(bat,6) +s+ String(distance) +s+ //0-3
                  String(radar_state()) +s+ sLatitude +","+ sLongitude +s+ getuptime() +s+            //4-6
                  syear+"/"+smonth+"/"+sday +s+ shour+":"+sminute+":"+ssecond+"."+scentisecond +s+
                  String(stibx);        //
    mqttclient.publish("truck/data", data.c_str());

    String boolean = String(isperson) +s+ String(isbrake) +s+ String(isalert) +s+
                     String(isjoin) +s+ String(/*isretry*/0) +s+ String(/*isend*/0) +s+
                     String(islateset) +s+String(isstartup) +s+ String(alerted);
    mqttclient.publish("truck/bool",boolean.c_str());
  }
}