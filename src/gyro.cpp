#include "gyro.h"

int16_t ax, ay, az, ax2, ay2, az2;
double x, y, z;
double x2, y2, z2;

int minVal = 265;
int maxVal = 402;

int globx = 0;
int offsetx = 0;
int stibx = 0;
int stickrot = 0;

#define OUTPUT_READABLE_ACCELGYRO
MPU6050 a1(0x69);
MPU6050 a2(0x68);

void gyro_init()
{
  a1.initialize();
  a2.initialize();
}

void gyro_init(int num){
  if(num == 1) a1.initialize();
  if(num == 2) a2.initialize();
}

void gyro_update()
{
  a1.getAcceleration(&ax, &ay, &az);
  a2.getAcceleration(&ax2, &ay2, &az2);
}

void calrot3(){
  //mpu 1
  int xAng = map(ax, minVal, maxVal, -90, 90);
  int yAng = map(ay, minVal, maxVal, -90, 90);
  int zAng = map(az, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  globx = y;
  offsetx = 0;
  if (globx >= 0 && globx <= 180)
  {
    offsetx = globx;
  }
  else if (globx > 180 && globx <= 360)
  {
    offsetx = globx - 360;
  }

  //mpu2
  int xAng2 = map(ax2, minVal, maxVal, -90, 90);
  int yAng2 = map(ay2, minVal, maxVal, -90, 90);
  int zAng2 = map(az2, minVal, maxVal, -90, 90);

  x2 = RAD_TO_DEG * (atan2(-yAng2, -zAng2) + PI);
  y2 = RAD_TO_DEG * (atan2(-xAng2, -zAng2) + PI);
  z2 = RAD_TO_DEG * (atan2(-yAng2, -xAng2) + PI);

  stibx = x2;
  if (stibx >= 0 && stibx <= 180){
    stibx = x2;
  }
  else if (stibx > 180 && stibx <= 360)
  {
    stibx = x2 - 360;
  }

  //stickrot
  if(offsetx<=0){
    stickrot = abs(stibx) - abs(offsetx);
  }
  else if(offsetx>0){
    if(stibx<=0){
      stickrot = abs(stibx) + abs(offsetx);
    }
    else{
      stickrot = abs(stibx) - abs(offsetx);
    }
      
  }
  


}

void showgyro(){
  Serial.printf("%-10s: %-6d %-10s: %-6d\r\n","raw_x1",globx,"ofs_x1",offsetx);
  Serial.printf("%-10s: %-6.0f %-10s: %-6d\r\n","raw_x2",x2,"ofs_x2",stibx);
  Serial.printf("%-10s: %-6d\r\n","stickrot",stickrot);
}
