#include "checkbool.h"
#include "wifiap_server.h"
#include "http.h"
#include "wifi_c.h"

bool isperson = false;
bool isbrake = false;
bool isalert = false;

// device id
int device_id = 3;

// update server url
char update_server_url[200];

// limit parameter (東風)
int lim_distance = 35;
int lim_angle = 40; //ref: ~ 47  ref: ~105

void getNVSConfig(){
  NVS.begin();
  device_id = NVS.getInt("device_id");
  if(device_id!=0){
    sprintf(wifi_ap_ssid, "HbAs_%x", device_id);
  }
  lim_distance = NVS.getInt("lim_distance");
  lim_angle = NVS.getInt("lim_angle");
  String tempSsid = NVS.getString("wifi_ssid");
  sprintf(wifi_ssid, "%s", tempSsid);
  String tempPw = NVS.getString("wifi_password");
  sprintf(wifi_password, "%s", tempPw);
  String nvs_url = NVS.getString("server_url");
  sprintf(update_server_url, "%s", nvs_url);
  sprintf(version_url, "http://%s/version",nvs_url);
  sprintf(bin_url, "http://%s/bin/",nvs_url);
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
