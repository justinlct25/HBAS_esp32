#ifndef _CBUZZER_H_
#define _CBUZZER_H_
#include <Arduino.h>

typedef enum
{
    BUZ_OFF,
    BUZ_ON,
    BUZ_TOGGLE,
    BUZ_OFF_TOGGLE
}buz_operation;

class CBUZ{
public:
  CBUZ(int, buz_operation, int, int, int);
  void setPin(int);
  int getPin();
  void setOperatation(buz_operation);
  buz_operation getOperatation();
  void setChannel(int);
  int getChannel();
  void setFreq(int);
  int getFreq();
  void setResolution(int);
  int getResolution();
  void setState(int);
  int getState();
  void setInterval(unsigned long);
  unsigned long getInterval();
  void setCurrent(unsigned long);
  unsigned long getCurrent();
  void setPervious(unsigned long);
  unsigned long getPervious();
  void setTimes(int);
  int getTimes();

  void cbuz_routine();

private:
  int BUZ_PIN;
  buz_operation BUZ_OPERATE;
  int BUZ_CHANNEL;
  int BUZ_FREQ;
  int BUZ_RESOLUTION;
  int BUZ_STATE;
  unsigned long BUZ_INTERVAL;
  unsigned long BUZ_CURRENT;
  unsigned long BUZ_PERVIOUS;
  int BUZ_TIMES;
};

#endif