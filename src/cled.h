#ifndef _CLED_H_
#define _CLED_H_
#include <Arduino.h>

typedef enum 
{
  LED_OFF,
  LED_ON,
  LED_TOGGLE,
  LED_FAST_TOGGLE,
  LED_SLOW_TOGGLE
}led_operation;

class CLED{
public:
  CLED(int, led_operation);
  CLED(int, led_operation, unsigned long);
  CLED(int, led_operation, unsigned long, unsigned long, unsigned long);
  void setPin(int);
  int getPin();
  void setOperatation(led_operation);
  led_operation getOperatation();
  void setState(bool);
  bool getState();
  void setInterval(unsigned long);
  unsigned long getInterval();
  void setCurrent(unsigned long);
  unsigned long getCurrent();
  void setPervious(unsigned long);
  unsigned long getPervious();

  void cled_opertate(int,led_operation);
  void cled_routine();

private:
  int LED_PIN;
  led_operation LED_OPERATE;
  bool LED_STATE;
  unsigned long LED_INTERVAL;
  unsigned long LED_CURRENT;
  unsigned long LED_PERVIOUS;
};

#endif