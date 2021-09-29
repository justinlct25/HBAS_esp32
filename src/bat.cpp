#include "bat.h"

ESP32AnalogRead adc;

float bat = 0;
int bat2 = 0;
int bat_counter = 0;

void bat_init()
{
  adc.attach(34);
}

void getbattery(){
  bat = adc.readVoltage() * 2 ;
  bat2 = int(bat * 100);
}

void samplebattery(){
  if(bat_counter<bat_sample_interval) {
    bat_counter++;
  }
  else{
    getbattery();
    bat_counter = 0;

  }
}

void showbattery(){
  if(bat_counter<bat_sample_interval) {
    Serial.printf("battery counter: %d  last sample: %f\r\n",bat_counter,bat);
  }
  else{
    Serial.printf("battery: %f\r\n",bat);
  }
}
