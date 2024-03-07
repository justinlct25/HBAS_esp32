#include "tof.h"

VL53L0X tof;
int16_t distance;
uint16_t raw_distance;

int eightoneeight_count = 0;

int tof_count;

void tof_init(){
  tof.init();
  tof.setTimeout(500);
}

void getdistance(){
    raw_distance = tof.readRangeSingleMillimeters();
    if(raw_distance <= 8191 && raw_distance > 0){
        distance=(raw_distance-10)/10; //in cm with 10mm error
        if(++tof_count >= 10)
        {
          tof_count = 0;
          //reset
          Serial.println("818 tof reset");
          digitalWrite(23, LOW);
          digitalWrite(23, HIGH);
          tof_init();
        }
    }
    else{
        distance = -1;
        //reset
        Serial.println("-1 tof reset");
        digitalWrite(23, LOW);
        digitalWrite(23, HIGH);
        tof_init();
    }
    
} 

void showtof(){
  Serial.printf("%-10s: %-6hu %-10s: %-6d\r\n","raw_mm",raw_distance,"to_cm",distance);
}
