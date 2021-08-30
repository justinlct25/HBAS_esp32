#ifndef _RADAR_H
#define _RADAR_H

#include <Arduino.h>

extern uint8_t radar_out;
extern uint8_t radar_tx;
extern uint8_t radar_rx;

void init_radar();
bool radar_state();

#endif