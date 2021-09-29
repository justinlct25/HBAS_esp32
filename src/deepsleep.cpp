#include "deepsleep.h"

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR bool issleep = false;
// RTC_DATA_ATTR bool issleep = true;						// test only
RTC_DATA_ATTR bool ischarge = false;


TaskHandle_t lora_task_b_wake_up;
void lora_task_bcode_wake_up(void *);

int charge_stopped;

void print_wakeup_reason()
{
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause();
	switch (wakeup_reason)
	{
	case ESP_SLEEP_WAKEUP_EXT0:
		Serial.println("Wakeup caused by external signal using RTC_IO");
		break;
	case ESP_SLEEP_WAKEUP_EXT1:
		Serial.println("Wakeup caused by external signal using RTC_CNTL");
		break;
	case ESP_SLEEP_WAKEUP_TIMER:
		Serial.println("Wakeup caused by timer");
		break;
	case ESP_SLEEP_WAKEUP_TOUCHPAD:
		Serial.println("Wakeup caused by touchpad");
		break;
	case ESP_SLEEP_WAKEUP_ULP:
		Serial.println("Wakeup caused by ULP program");
		break;
	default:
		Serial.printf("Wakeup was not caused by deep sleep: %d\r\n", wakeup_reason);
		break;
	}
}

void deepsleep_handler()
{
	print_wakeup_reason();
	getbattery();
	Serial.printf("Boot number: %d\r\n", ++bootCount);
	Serial.printf("Current battery: %f\r\n", bat);
	if (issleep)
	{
		if (bat >= lowvolt && digitalRead(16) || bat >= highvolt)
		{
			Serial.print("Is charging or get enough battery\r\nLeave deep sleep mode\r\n");
			issleep = false;
			Serial.println("self-reboot");
			ESP.restart(); //may not require
		}
		else
		{

			int start_time = millis();
			int time_interval = 40000;
			int attempts = 5;
			while (!isjoin && millis() - start_time <= time_interval){
				lora_rountine();
			}

			if (!isjoin){
				for(int i = 0; i < attempts; i++){
					njoinlora();
					start_time = millis();
					while (!isjoin && millis() - start_time <= time_interval){
						lora_rountine();
					}
					if (isjoin) break;
				}
			}
			if (isjoin) {
				Serial.println("Startup low battery deep sleep triggered with SUCCESS lora join");
				wake_up_task_before_sleep(time_interval, attempts);
				delay(2000);

				time_t now;
				struct tm timeDetails;
				time(&now);
				localtime_r(&now, &timeDetails);
				int year = timeDetails.tm_year;
				int weekday;
				year == 70 ? weekday = 10 : weekday = timeDetails.tm_wday;
				Serial.printf("year: %i", year);
				int sleep_time;

				switch (weekday)
				{
					case 0:
					case 6:
						sleep_time = long_sleep_time * 4;
						break;
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					default:
						sleep_time = long_sleep_time;
						break;
				}
				Serial.printf("today is %i, sleep_time is: %i\r\n", weekday, sleep_time);
				delay(2000);
				routine_low_battery_sleep(sleep_time);
			} else {
				Serial.println("Startup low battery deep sleep triggered with UNSUCCESS lora join");
				delay(2000);
				routine_low_battery_sleep(short_sleep_time);

			}
			//++bootCount;
		}
	}
}

void deepsleep_routine()
{
	//GPIO16: HIGH - 插電	LOW - 未插電
	//未插電時記錄當前時間
	if (digitalRead(16)){
		charge_stopped = millis();
	}
	//電量 < lowvolt && 未插電時
	if (bat < lowvolt && !digitalRead(16))
	{
		Serial.println("Routine low battery deep sleep trigger");
		routine_low_battery_sleep(long_sleep_time);
	}
	//電量 > low && 未插電時
	else if (bat > lowvolt && !digitalRead(16) && millis() - charge_stopped >= charge_interval )
	{
		Serial.println("I should buzz then sleep");
		buzzer(3);
		routine_low_battery_sleep(long_sleep_time);
	}
}

void routine_low_battery_sleep(int sleep_time)
{
	issleep = true;
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 1);
	esp_sleep_enable_timer_wakeup(sleep_time * uS);
	Serial.printf("Setup ESP32 to sleep for every %i Seconds\r\n", sleep_time);
	Serial.flush();
	Serial.println("Enter Deep Sleep Mode");
	digitalWrite(17, LOW);
	esp_deep_sleep_start();
}

void lora_task_bcode_wake_up(void * parameter){
  	Serial.println("loratask b");
	nsendloramsg(encode_cmsg('B'));
	bmsging = true;
  	vTaskDelete(NULL);
}

void wake_up_task_before_sleep(int time_limit, int try_round){
	Serial.println("inside wake_up_task_before_sleep");
	delay(5000);					// this is needed to ensure firm lora connection
	xTaskCreate(lora_task_bcode_wake_up,"lora_task_b_wake_up",5000,NULL,2,&lora_task_b_wake_up);
	umsging = true;

	int start_time = millis();
	// while (umsging ){
	while (true && millis() - start_time <= time_limit){
		lora_rountine();
		// Serial.print(millis() - start_time);
	}
	Serial.printf("umsging state: %i", umsging);
	Serial.printf("sent\r\n");
	if (umsging){
		Serial.println("b message send fail, going to re-send");
		for (int i = 0; i < try_round; i ++){
			xTaskCreate(lora_task_bcode_wake_up,"lora_task_b_wake_up",5000,NULL,2,&lora_task_b_wake_up);
			start_time = millis();
			while (umsging && millis() - start_time <= time_limit){
				lora_rountine();
			}
			if(!umsging) break;
		}
	}
}

