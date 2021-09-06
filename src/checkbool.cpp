#include "checkbool.h"
#include "wifiap_server.h"

bool isperson = false;
bool isbrake = false;
bool isalert = false;

// device id
int device_id = 3;

// limit parameter
int lim_distance = 35;
int lim_angle = 40; //ref: ~ 47  ref: ~105

void getNVSConfig(){
  NVS.begin();
  device_id = NVS.getInt("device_id");
  if(device_id!=0){
    // char chare;
    sprintf(wifi_ap_ssid, "HbAs_%x", device_id);
    // wifi_ap_ssid = 
  }
  lim_distance = NVS.getInt("lim_distance");
  lim_angle = NVS.getInt("lim_angle");
}

void person()
{
  if ( (distance <= lim_distance && distance >= 0) && distance <= 818 )
  {
    isperson = true;
  }
  else
  {
    isperson = false;
    if(distance == -1) isperson = true;
  }
}

void checkrot2()
{
  if (stickrot < lim_angle)
  {
    isbrake = false;
    if (!isperson && !isbrake)
    {
      isalert = true;
      if(!istof && !isintmpu && !isextmpu) isalert = false;
    }
    else
    {
      isalert = false;
    }
  }
  else
  {
    isbrake = true;
    isalert = false;
  }
}
