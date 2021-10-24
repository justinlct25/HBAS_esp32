#include "led.h"

#define BLUE_LED_PIN 15
#define YELLOW_LED_PIN 4

//led
CLED blueled(BLUE_LED_PIN,LED_OFF);
CLED yellowled(YELLOW_LED_PIN,LED_OFF);

void led_operate(){
    if(isjoin && (!joining && !cmsging && !umsging)){
        blueled.setOperatation(LED_ON);
    }
    else if(alerted && (!joining && !cmsging && !umsging)){
        blueled.setOperatation(LED_SLOW_TOGGLE);
    }
    else if((joining || cmsging || umsging)){
        blueled.setOperatation(LED_FAST_TOGGLE);
    }
    else{
        blueled.setOperatation(LED_OFF);
    }

    if(isalert){
      yellowled.setOperatation(LED_FAST_TOGGLE);
    }
    else if(bat<lowvolt){
      yellowled.setOperatation(LED_SLOW_TOGGLE);
    }
    else if(!istof || !isintmpu || !isextmpu){
      yellowled.setOperatation(LED_OFF);
    }
    else{
      yellowled.setOperatation(LED_ON);
    }
    if(!digitalRead(16))
    {
      digitalWrite(GPIO_NUM_19, LOW);
    }
    else
    {
      digitalWrite(GPIO_NUM_19, HIGH);
    }
}