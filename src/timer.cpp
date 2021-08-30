#include "timer.h"

 char cuptime[cuptimebuffer];
 char timer[ctimerbuffer];

char *getuptime()
{
  unsigned long uptime = millis() / 1000UL;
  unsigned long second = uptime % 60;
  unsigned long minute = ((uptime-second) / 60) % 60;
  unsigned long hour = ((uptime-minute*60-second) / 3600);
  if(hour<=0 && minute<=0){
      sprintf(cuptime,"%lu",second);
  }
  else{
      if(hour<=0){
          sprintf(cuptime,"%lu:%lu",minute,second);
      }
        else{
            if(hour>0){
                sprintf(cuptime,"%lu:%lu:%lu",hour,minute,second);
            }
        }
  }
  return cuptime;
}

char *gettimer(unsigned long pervMillis){
  unsigned long lasttime= (millis() - pervMillis) /1000UL;
  unsigned long second = lasttime % 60;
  unsigned long minute = ((lasttime-second) / 60) % 60;
  unsigned long hour = ((lasttime-minute*60-second) / 3600);
  if(hour<=0 && minute<=0){
      sprintf(timer,"%lu",second);
  }
  else{
      if(hour<=0){
          sprintf(timer,"%lu:%lu",minute,second);
      }
        else{
            if(hour>0){
                sprintf(timer,"%lu:%lu:%lu",hour,minute,second);
            }
        }
  }
  return timer;
}