
#include "radar.h"

uint8_t radar_out = 25;
uint8_t radar_tx = 18;
uint8_t radar_rx = 19;

void init_radar()
{
    pinMode(radar_out, INPUT);
    pinMode(radar_tx, OUTPUT);
    pinMode(radar_rx, OUTPUT);

    //tx high for 3~4m  rx high for 2s
    digitalWrite(radar_tx, HIGH);
    digitalWrite(radar_rx, HIGH);
}

bool radar_state()
{
    bool state = false;
    state = (digitalRead(radar_out)) ? true : false;
    return state;
}

// void radar_set_tx(){

// }

// void radar_set_rx(){
    
// }