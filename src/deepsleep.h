#ifndef _DEEPSLEEP_H_
#define _DEEPSLEEP_H_

#include <Arduino.h>
#include "bat.h"
#include "lora.h"
#include "encode_msg.h"
#include "buzzer.h"
#include "wifi_c.h"
#include <sys/time.h>
#include <string>
#include "esp_wifi.h"

#define uS 1000000UL
//#define short_sleep_time  1800UL 
#define short_sleep_time  1800UL 
// #define short_sleep_time  300UL 
//#define long_sleep_time  3600UL 
#define long_sleep_time  3600UL                             // for testing
// #define lowvolt 3.5
// #define highvolt 3.58
// #define lowvolt 3.9
// #define highvolt 4.1
#define lowvolt 4.5
#define highvolt 4.58

//#define charge_interval 1000 * 60 * 10
// #define charge_interval 300000	                        // for testing
#define charge_interval 10000	                        

#define GPS_GET_TIME 60000

extern RTC_DATA_ATTR int bootCount;
extern RTC_DATA_ATTR bool issleep;
// extern RTC_DATA_ATTR bool ischarge;

void print_wakeup_reason();
void deepsleep_handler();
void deepsleep_routine();
void routine_low_battery_sleep(int time_to_sleep);
void wake_up_task_before_sleep(int time_limit, int try_round);


#endif