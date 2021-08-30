#include "timeout.h"

unsigned long previousTimeoutMillis = 0;
const long Timeoutinterval = 15000; //15 second

bool timeout(unsigned long perviousMillis){
  unsigned long currentMillis = millis();
  if(currentMillis - perviousMillis > Timeoutinterval){
    Serial.println("timeout");
    return false;
  }
  else{
    return true;
  }
}

bool timeout(unsigned long perviousMillis,long customTimeoutinterval){
  unsigned long currentMillis = millis();
  if(currentMillis - perviousMillis > customTimeoutinterval){
    Serial.println("timeout");
    return false;
  }
  else{
    return true;
  }
}


