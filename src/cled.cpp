#include "cled.h"

#define CLOW HIGH
#define CHIGH LOW

CLED::CLED(int pin,led_operation operatation){
  setPin(pin);
  setOperatation(operatation);
  setInterval(0UL);
  setCurrent(0UL);
  setPervious(0UL);
  pinMode(pin,OUTPUT);
  digitalWrite(pin,CLOW);
}
CLED::CLED(int pin,led_operation operatation, unsigned long interval){
  setPin(pin);
  setOperatation(operatation);
  setInterval(interval);
  setCurrent(0UL);
  setPervious(0UL);
  pinMode(pin,OUTPUT);
  digitalWrite(pin,CLOW);
}
CLED::CLED(int pin,led_operation operatation, unsigned long interval, unsigned long current, unsigned long pervious ){
  setPin(pin);
  setOperatation(operatation);
  setInterval(interval);
  setCurrent(current);
  setPervious(pervious);
  pinMode(pin,OUTPUT);
  digitalWrite(pin,CLOW);
}

void CLED::setPin(int pin){
  LED_PIN = pin;
}
int CLED::getPin(){
  return LED_PIN;
}

void CLED::setOperatation(led_operation operatation){
  LED_OPERATE = operatation;
}
led_operation CLED::getOperatation(){
  return LED_OPERATE;
}

void CLED::setState(bool state){
  LED_STATE = state;
}
bool CLED::getState(){
  return LED_STATE;
}

void CLED::setInterval(unsigned long interval){
  LED_INTERVAL = interval;
}
unsigned long CLED::getInterval(){
  return LED_INTERVAL;
}

void CLED::setCurrent(unsigned long current){
  LED_CURRENT = current;
}
unsigned long CLED::getCurrent(){
  return LED_CURRENT;
}

void CLED::setPervious(unsigned long pervious){
  LED_PERVIOUS = pervious;
}
unsigned long CLED::getPervious(){
  return LED_PERVIOUS;
}

void CLED::cled_opertate(int pin, led_operation operatation){
  setPin(pin);
  setOperatation(operatation);
  pinMode(pin,OUTPUT);
  digitalWrite(pin,CLOW);
}

void CLED::cled_routine(){
  switch(getOperatation()){
    default:
    case LED_OFF:
      setState(CLOW);
      digitalWrite(getPin(),getState());
      break;
    case LED_ON:
      setState(CHIGH);
      digitalWrite(getPin(),getState());
      break;
    case LED_TOGGLE:
      setInterval(1000UL); //1s
      break;
    case LED_FAST_TOGGLE:
      setInterval(200UL); //0.2s
      break;
    case LED_SLOW_TOGGLE:
      setInterval(2000UL); //2s
      break;
  }
  led_operation e = getOperatation();
  if(e == LED_TOGGLE || e == LED_FAST_TOGGLE || e == LED_SLOW_TOGGLE){
    unsigned long cur = millis();
    setCurrent(cur);
    if (getCurrent() - getPervious() >= getInterval()) {
        setPervious(cur);
        setState(!getState());
        digitalWrite(getPin(),getState());
      }
  }
}