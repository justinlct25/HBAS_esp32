#include "encode_msg.h"

char testmsg[60];

char *encode_cmsg(char msgtype){
  char h_timestamp[10];
  char h_latitude[10];
  char h_longitude[10];
  char h_bat[4];
  char h_msgtype=msgtype;
  memset(testmsg,'\0',60);
  sprintf(h_timestamp,"%x",(unsigned int)utctime());
  // if(issleep){
  //   // Serial.print("NVSgggg");
    
  //   sprintf(h_latitude,"%x",(int)(((NVS.getString("latitude").toFloat())+90)*1000000));
  //   sprintf(h_longitude,"%x",(int)((((NVS.getString("longitude")).toFloat())+180)*1000000));
  //   Serial.println("coordinates got before msgb");
    // Serial.println(String(h_latitude));
    // Serial.println(String(h_longitude));
    // Serial.println(NVS.getString("latitude").toFloat());
    // Serial.println(NVS.getString("longitude").toFloat());

  // }else{
    sprintf(h_latitude,"%x",(int)(((sLatitude.toFloat())+90)*1000000));
    sprintf(h_longitude,"%x",(int)(((sLongitude.toFloat())+180)*1000000));
  // }
  // Serial.println("outside 0");
  // Serial.println(str(h_latitude));
  // Serial.println(str(h_longitude));
  // Serial.println(h_latitude);
  // Serial.println(h_longitude);
  // Serial.println("outside 0");
  // Serial.println(sLatitude);
  // if(strcmp()){
  // if(sLatitude=="0"||sLongitude=="0"){
  // Serial.print("inside 0");
  

  sprintf(h_bat,"%x",bat2);
  // sprintf(testmsg,"%s;%s;%s;%s;%c;%s;%s",h_timestamp,h_latitude,h_longitude,h_bat,h_msgtype,h_rssi,h_snr);
  sprintf(testmsg,"%s;%s;%s;%s;%c;%i;%i",h_timestamp,h_latitude,h_longitude,h_bat,h_msgtype,rssi,snr);
  
  Serial.print("MSG Length : ");
  Serial.println(strlen(testmsg));

  return testmsg;
}

// String encode_fmsg(String msgtype){  //legacy function
//   String s_date=syear+smonth+sday;
//   String s_time=String(ptime);
//   String ftmsg=s_date+";"+s_time+";"+sLatitude+";"+sLongitude+";"+String(bat2)+";"+msgtype;
//   String h_date=String(pdate,HEX);
//   String h_time=String(ptime,HEX);
//   String h_latitude=String(int((sLatitude.toFloat()+90)*1000000),HEX);
//   String h_longitude=String(int((sLongitude.toFloat()+180)*1000000),HEX);
//   String h_battery=String(bat2,HEX);
//   String s=";";
//   String encode_msg=h_date +s+ h_time +s+ h_latitude +s+ h_longitude +s+ h_battery +s+ msgtype;
//   //Serial.printf("ftmsg: %s\r\n",fmsg);
//   //Serial.printf("encode msg: %s\r\n",encode_msg);
//   return encode_msg;
// }

