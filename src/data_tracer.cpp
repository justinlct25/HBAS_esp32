#include "data_tracer.h"

const char *ptr = "-->";
char h_globx[4];
char h_stickrot[4];
char h_distance[5];
char h_bat[4];
char h_latitude[10];
char h_longitude[11];

void showstatus(){
  sprintf(h_globx,"%x",globx);
  sprintf(h_stickrot,"%x",stickrot);
  sprintf(h_distance,"%x",distance);
  sprintf(h_bat,"%x",bat2);
  sprintf(h_latitude,"%x",(int)(((sLatitude.toFloat())+90)*1000000));
  sprintf(h_longitude,"%x",(int)(((sLatitude.toFloat())+180)*1000000));
  Serial.println("----------------Status--------------------");
  Serial.printf("%-12s: %-10d %-s %-8s\r\n","ramp level",globx,ptr,h_globx);
  Serial.printf("%-12s: %-10d %-s %-8s\r\n","handbrake",stickrot,ptr,h_stickrot);
  Serial.printf("%-12s: %-10d %-s %-8s\r\n","distance",distance,ptr,h_distance);
  Serial.printf("%-12s: %-10.4f %-s %-8s\r\n","battery",bat,ptr,h_bat);
  Serial.printf("%-12s: %-10.6f %-s %-8s\r\n","latitude",sLat,ptr,h_latitude);
  Serial.printf("%-12s: %-10.6f %-s %-8s\r\n","longitude",sLong,ptr,h_longitude);
  //Serial.printf("%-12s: %-10d\r\n","Radar",radar_state());
  Serial.println("------------------End---------------------");

}

void showrecord(){
  Serial.printf("=========%d record found=========\r\n",recordCounter);
  for(int i=0; i<recordCounter; i++){
    Serial.printf("%-3d: %-s\r\n",i,arecord[i]);
  }
  Serial.printf("=================================\r\n");
}

void showallbool(){
  Serial.println("---------------All bool-------------------");
  Serial.printf("%-10s: %d %-10s: %d %-10s:  %d\r\n","isperson",isperson,"isbrake",isbrake,"isalert",isalert);
  Serial.printf("%-10s: %d %-10s: %d %-10s:  %d\r\n","isjoin",isjoin,"amsging",amsging,"bmsging",bmsging);
  Serial.printf("%-10s: %d %-10s: %d %-10s: %d\r\n","istof",istof,"isintmpu",isintmpu,"isextmpu",isextmpu);
  Serial.printf("%-10s: %d %-10s: %d %-10s:  %d\r\n","islateset",islateset,"isstartup",isstartup,"alerted",alerted);
  Serial.println("------------------------------------------");

}

//bluetooth
void BTshowstatus(){
  sprintf(h_globx,"%x",globx);
  sprintf(h_stickrot,"%x",stickrot);
  sprintf(h_distance,"%x",distance);
  sprintf(h_bat,"%x",bat2);
  sprintf(h_latitude,"%x",(int)(((sLatitude.toFloat())+90)*1000000));
  sprintf(h_longitude,"%x",(int)(((sLatitude.toFloat())+180)*1000000));
  bt.println("----------------Status--------------------");
  bt.printf("%-12s: %-10d %-s %-8s\r\n","ramp level",globx,ptr,h_globx);
  bt.printf("%-12s: %-10d %-s %-8s\r\n","handbrake",stickrot,ptr,h_stickrot);
  bt.printf("%-12s: %-10d %-s %-8s\r\n","distance",distance,ptr,h_distance);
  bt.printf("%-12s: %-10.2f %-s %-8s\r\n","battery",bat,ptr,h_bat);
  bt.printf("%-12s: %-10.6f %-s %-8s\r\n","latitude",sLat,ptr,h_latitude);
  bt.printf("%-12s: %-10.6f %-s %-8s\r\n","longitude",sLong,ptr,h_longitude);
  bt.println("------------------End---------------------");

}

void BTshowrecord(){
  bt.printf("=========%d record found=========\r\n",recordCounter);
  for(int i=0; i<recordCounter; i++){
    bt.printf("%-3d: %-50s\r\n",i,arecord[i]);
  }
  bt.printf("=================================\r\n");
}

void BTshowallbool(){
  bt.println("---------------All bool-------------------\r\n");
  bt.printf("%-10s: %d %-10s: %d %-10s:  %d\r\n","isperson",isperson,"isbrake",isbrake,"isalert",isalert);
  bt.printf("%-10s: %d %-10s: %d %-10s:  %d\r\n","isjoin",isjoin,"isretry",isretry,"isend",isend);
  bt.printf("%-10s: %d %-10s: %d %-10s:  %d\r\n","islateset",islateset,"isstartup",isstartup,"alerted",alerted);
  bt.println("------------------------------------------");

}

