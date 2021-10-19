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
int lim_angle = 40;


void getNVSConfig(){
  NVS.begin();
  int temp_device_id = NVS.getInt("device_id");
  if(temp_device_id!=0){
    sprintf(wifi_ap_ssid, "HbAs_%d", temp_device_id);
    device_id = temp_device_id;
  };
  int temp_lim_distance = NVS.getInt("lim_distance");
  if(temp_lim_distance>0){
    lim_distance = temp_lim_distance;
  };
  int temp_lim_angle = NVS.getInt("lim_angle");
  if(temp_lim_angle>0){
    lim_angle = temp_lim_angle;
  };
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
