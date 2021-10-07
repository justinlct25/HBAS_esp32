#include "gps.h"

String sLatitude, sLongitude;
String sday, smonth, syear;
String shour, sminute, ssecond, scentisecond;
String location;
String datetime;
String date;
int ptime;
int pdate;
double sLat, sLong;

char ctimestamp[15];
unsigned long lutimestamp;


TinyGPSPlus tgps;

void tinygps()
{
    while (Serial2.available() > 0)
    {
        if (tgps.encode(Serial2.read()))
        {
            if (tgps.location.isValid())
            {
                sLatitude = String((double)tgps.location.lat(), 6);
                sLongitude = String((double)tgps.location.lng(), 6);
                NVS.setString("latitude", sLatitude);
                NVS.setString("longitude", sLongitude);
            }
            else
            {
                sLatitude = "0";
                sLongitude = "0";
            }
            if (tgps.date.isValid())
            {
                sday = tgps.date.day();
                smonth = tgps.date.month();
                syear = tgps.date.year();
                if (tgps.date.day() < 10)
                    sday = "0" + String(tgps.date.day());
                if (tgps.date.month() < 10)
                    smonth = "0" + String(tgps.date.month());
                if (tgps.date.year() < 10)
                    syear = "0" + String(tgps.date.year());
                pdate = int(tgps.date.year() * 1e04 + tgps.date.month() * 1e02 + tgps.date.day());
            }
            else
            {
                sday = "00";
                smonth = "00";
                syear = "00";
            }
            if (tgps.time.isValid())
            {
                shour = tgps.time.hour();
                sminute = tgps.time.minute();
                ssecond = tgps.time.second();
                scentisecond = tgps.time.centisecond();
                if (shour.toInt() < 10)
                    shour = "0" + String(tgps.time.hour());
                if (tgps.time.minute() < 10)
                    sminute = "0" + String(tgps.time.minute());
                if (tgps.time.second() < 10)
                    ssecond = "0" + String(tgps.time.second());
                if (tgps.time.centisecond() < 10)
                    scentisecond = "0" + String(tgps.time.centisecond());
                ptime = int(shour.toInt() * 3600 + tgps.time.minute() * 60 + tgps.time.second());
            }
            else
            {
                shour = "00";
                sminute = "00";
                ssecond = "00";
                scentisecond = "00";
            }
        }
    }
}

void showgpsinfo()
{
    location = "Location : " + sLatitude + "," + sLongitude;
    datetime = "Data/Time: " + syear + "/" + smonth + "/" + sday + " " + shour + ":" + sminute + ":" + ssecond + "." + scentisecond;
    Serial.println(location);
    Serial.println(datetime);
}

time_t utctime(){
  int year=tgps.date.year(), month=tgps.date.month(), day=tgps.date.day(), hour=tgps.time.hour(), min=tgps.time.minute(), sec=tgps.time.second();
  struct tm t;
  time_t timeinfo;
  t.tm_year=year-1900;
  t.tm_mon=month-1;
  t.tm_mday=day;
  t.tm_hour=hour+8;
  t.tm_min=min;
  t.tm_sec=sec;
  timeinfo=mktime(&t);
  sprintf(ctimestamp,"%lu",timeinfo);
  //Serial.printf("long utc: %lu\r\n",(unsigned long)timeinfo);
  //Serial.printf("  ch utc:%s\r\n",ctimestamp);
  //Serial.printf(" hex utc:%x\r\n",(unsigned int)timeinfo);

  return timeinfo;
}