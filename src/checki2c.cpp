#include "checki2c.h"

bool istof = false;
bool isintmpu = false;
bool isextmpu = false;

void i2cdev_restore(){
    if(!istof){
        Wire.endTransmission(true);
        tof_init();
    }
    if(!isintmpu){
        Wire.endTransmission(true);
        gyro_init(1);
    }
    if(!isextmpu){
        Wire.endTransmission(true);
        gyro_init(2);
    }

    istof = false;
    Wire.beginTransmission(TOF_ADDR);
    if (Wire.endTransmission () == 0) istof = true;
    isintmpu = false;
    Wire.beginTransmission(INTMPU_ADDR);
    if (Wire.endTransmission () == 0) isintmpu = true;
    isextmpu = false;
    Wire.beginTransmission(EXTMPU_ADDR);
    if (Wire.endTransmission () == 0) isextmpu = true;
  
}

void showi2cstate(){
    Serial.printf("%-10s: %d %-10s: %d %-10s: %d\r\n","istof",istof,"isintmpu",isintmpu,"isextmpu",isextmpu);
}

void showi2cdev(){
    Serial.print("i2c address: ");
    for (uint8_t i = 8; i < 127; i++){
      Wire.beginTransmission (i);
      if (Wire.endTransmission () == 0){
        Serial.printf("%d:0x%X ",i,i);
      }
    }
  Serial.print("\r\n");
}