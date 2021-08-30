#include "deepsleep.h"

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR bool issleep = false;
// RTC_DATA_ATTR bool issleep = true;						// test only
RTC_DATA_ATTR bool ischarge = false;
int start_time;

TaskHandle_t lora_task_b_wake_up;
void lora_task_bcode_wake_up(void *);


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
		if (bat >= lowvolt && digitalRead(16))
		{
			Serial.print("Is charging or get enoguh battery\r\nLeave deep sleep mode\r\n");
			issleep = false;
			Serial.println("self-reboot");
			ESP.restart(); //may not require
		}
		else
		{
			int start_time = millis();
			int time_interval = 12000;
			int attempts = 5;
			while (!isjoin && millis() - start_time <= time_interval){
				lora_rountine();
			}
			// todays edit
			if (!isjoin){
				for(int i = 0; i < attempts; i++){
					njoinlora();
					while (!isjoin && millis() - start_time <= time_interval){
						lora_rountine();
					}
					if (isjoin) break;
				}
			}
			wake_up_task_before_sleep(time_interval, attempts);
			delay(2000);

			//++bootCount;
			Serial.println("Startup low battery deep sleep trigger");
			routine_low_battery_sleep();
		}
	}
}

void deepsleep_routine()
{
	if (bat < lowvolt && !digitalRead(16))
	{
		Serial.println("Routine low battery deep sleep trigger");
		routine_low_battery_sleep();
	}
	else if (bat > lowvolt && !digitalRead(16))
	{
		Serial.println("I should buzz then sleep");
		Serial.print("start_time: ");
		Serial.println(start_time);
		if (millis() - start_time >= charge_interval)
		{
			start_time = millis();
			buzzer(3);
			routine_low_battery_sleep();
		}
	}
}

void routine_low_battery_sleep()
{
	issleep = true;
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 1);
	esp_sleep_enable_timer_wakeup(sleeptime * uS);
	Serial.printf("Setup ESP32 to sleep for every %lu Seconds\r\n", sleeptime);
	Serial.flush();
	Serial.println("Enter Deep Sleep Mode");
	digitalWrite(17, LOW);
	esp_deep_sleep_start();
}

void lora_task_bcode_wake_up(void * parameter){
  	Serial.println("loratask b");

	Serial.println("wake up LoRa battery msg sent");
	nsendloramsg(encode_cmsg('B'));
	bmsging = true;

  	vTaskDelete(NULL);
}

void wake_up_task_before_sleep(int time_limit, int try_round){
	xTaskCreate(lora_task_bcode_wake_up,"lora_task_b_wake_up",5000,NULL,2,&lora_task_b_wake_up);
	umsging = true;
	start_time = millis();
	while (umsging && millis() - start_time <= time_limit){
		lora_rountine();
	}
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

