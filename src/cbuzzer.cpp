#include "cbuzzer.h"

CBUZ::CBUZ(int pin, buz_operation operatation, int freq, int channel, int resolution){
    setPin(pin);
    setOperatation(operatation);
    setFreq(freq);
    setChannel(channel);
    setInterval(0UL);
    setCurrent(0UL);
    setPervious(0UL);
    setTimes(0);

    ledcSetup(channel,freq,resolution);
    ledcAttachPin(pin, channel);
    ledcWrite(channel, 0);
}

void CBUZ::setPin(int pin){
    BUZ_PIN = pin;
    ledcAttachPin(BUZ_PIN, BUZ_CHANNEL);
}
int CBUZ::getPin(){
    return BUZ_PIN;
}

void CBUZ::setOperatation(buz_operation operatation){
    BUZ_OPERATE = operatation;
}
buz_operation CBUZ::getOperatation(){
    return BUZ_OPERATE;
}

void CBUZ::setChannel(int channel){
    BUZ_CHANNEL = channel;
    ledcSetup(BUZ_CHANNEL,BUZ_FREQ,BUZ_RESOLUTION);
}
int CBUZ::getChannel(){
    return BUZ_CHANNEL;
}

void CBUZ::setFreq(int freq){
    BUZ_FREQ = freq;
    ledcSetup(BUZ_CHANNEL,BUZ_FREQ,BUZ_RESOLUTION);
}
int CBUZ::getFreq(){
    return BUZ_FREQ;
}

void CBUZ::setResolution(int resolution){
    BUZ_RESOLUTION = resolution;
    ledcSetup(BUZ_CHANNEL,BUZ_FREQ,BUZ_RESOLUTION);
}
int CBUZ::getResolution(){
    return BUZ_RESOLUTION;
}

void CBUZ::setState(int state){
    BUZ_STATE = state;
}
int CBUZ::getState(){
    return BUZ_STATE;
}

void CBUZ::setInterval(unsigned long interval){
    BUZ_INTERVAL = interval;
}
unsigned long CBUZ::getInterval(){
    return BUZ_INTERVAL;
}

void CBUZ::setCurrent(unsigned long current){
    BUZ_CURRENT = current;
}
unsigned long CBUZ::getCurrent(){
    return BUZ_CURRENT;
}

void CBUZ::setPervious(unsigned long pervious){
    BUZ_PERVIOUS = pervious;
}
unsigned long CBUZ::getPervious(){
    return BUZ_PERVIOUS;
}

void CBUZ::setTimes(int times){
    BUZ_TIMES = BUZ_TIMES + times;
}
int CBUZ::getTimes(){
    return BUZ_TIMES;
}

void CBUZ::cbuz_routine(){
    /*int res = 0;
    for(int i = getResolution()-1;i>=0;i--) res = (1 << i);*/
    switch(getOperatation()){
        default:
        case BUZ_OFF:
          setState(0);
          ledcWrite(BUZ_CHANNEL, BUZ_STATE);
          break;
        case BUZ_ON: 
          setState(255);
          ledcWrite(BUZ_CHANNEL, BUZ_STATE);
          break;
        /*case BUZ_TOGGLE:
        case BUZ_OFF_TOGGLE:
          Serial.println("BUZ_TOGGLE");
          setState(255);
          setInterval(1000UL);
          break;*/
    }
    /*buz_operation e = getOperatation();
    if(e == BUZ_TOGGLE || e == BUZ_OFF_TOGGLE){
        Serial.println(getTimes());
        setCurrent(millis());
        if (getCurrent() - getPervious() >= getInterval()) {
            setPervious(millis());
            setState(~getState());
            if(getTimes()>0) ledcWrite(BUZ_CHANNEL, BUZ_STATE);
            setTimes(1);
        }
        if(getTimes()>10){
            setTimes(-310);
        }
    }*/
}
