#include "buzzer.h"

CBUZ buz(5,BUZ_OFF,2500,0,8);

void buzzer_init(){
    ledcSetup(0, 2500, 8);
    ledcAttachPin(5, 0);
    ledcWrite(0, 0);//deflaut off
}

void buzzer(int x) {
  switch (x) {
    case 1:
      ledcWrite(0, 140);
      break;
    case 2:
      ledcWrite(0, 255);
      break;
    case 3:
    {
        int beep_duration{200};
        for (int i = 0; i < 10; i++)
        {
            Serial.print(i);
            ledcWrite(0, 255);
            delay(beep_duration);
            ledcWrite(0, 0);
            delay(100);
            ledcWrite(0, 255);
            delay(beep_duration);
            ledcWrite(0, 0);
            delay(400);
        }
        break;
    }
    default:
      ledcWrite(0, 0);
      break;
  }
}

void buz_operate(){
  if(isalert){
    buz.setOperatation(BUZ_ON);
  }
  else{
    buz.setOperatation(BUZ_OFF);
    //if(bat<lowvolt)buz.setOperatation(BUZ_OFF_TOGGLE);
  }
  
}
