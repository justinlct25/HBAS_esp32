#include "bat.h"

ESP32AnalogRead adc;
ESP32AnalogRead adc2;

float bat = 0;
int bat2 = 0;
int bat_counter = 0;

float bat_temp = 0;

void bat_init()
{
  adc.attach(34);
}

void getbattery(){
  bat_temp = adc.readVoltage() * 2;
  if(bat_temp > 3.0 && bat_temp < 5.0)
  {
    bat = bat_temp;
    bat2 = int(bat * 100);
    // Serial.println(bat);
    // Serial.println(bat2);
  }
  else
  {
    Serial.println("bat read error?");
  }
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
  // if(bat_counter<bat_sample_interval) {
  //   Serial.printf("battery counter: %d  last sample: %f\r\n",bat_counter,bat);
  // }
  // else{
    Serial.printf("battery: %f\r\n",bat);
  // }
}
