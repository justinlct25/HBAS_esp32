#include "wifista_update.h"

bool staUpdate = false;
unsigned long previousWifiMillis = 0;
// long Wifiinterval = 3600000; //1 hour
// long Wifiinterval = 100000; //1 mins
// long Wifiinterval = 10000; //0.5 mins
long Wifiinterval = 5000; //0.25 mins


void wifista_update(){
    // wifi connection and ota
    unsigned long currentWifiMillis = millis();
    if(currentWifiMillis - previousWifiMillis >= Wifiinterval){
      previousWifiMillis = currentWifiMillis;
      Serial.println("Check wifi status");
      if(!wifi_stat()){
        wifi_reconnect();
        Serial.println("Reconnecting to wifi");
        Serial.printf("wifi status: %d , RSSI: %d\r\n",wifi_stat(),wifi_strength());
      }else{ 
        Serial.printf("latest version: %s\r\n",http_version_check());
        version_check();
        if(!islateset){
          Serial.println("got lateset version");
          Serial.println("offbuzz");
          if(!isstartup){
            ota_update();
            isstartup = true;
          }else{
            ota_status();
          }
        }
      }
    }
}