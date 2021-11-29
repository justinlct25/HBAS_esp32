#include "deepsleep.h"

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR bool issleep = false;
// RTC_DATA_ATTR bool issleep = true;						// test only
//RTC_DATA_ATTR bool ischarge = false;


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
	Serial.printf("issleep: %i\r\n", issleep);
    // Serial.print("POWER EXT : ");
    // Serial.println(digitalRead(16));
	if (issleep)
	{
		//new 
		if (bat <= 3.6)
		{
			esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, HIGH);
			esp_deep_sleep_start();
		}
		if (!digitalRead(16) || esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
		{
			Serial.print("Leave deep sleep mode\r\n");
			issleep = false;
			// Serial.println("self-reboot");
			// ESP.restart(); //may not require
		}
		else
		{
			Serial.println("in wake up else statement");
			int start_time = millis();
			int time_interval = 20000;
			int attempts = 5;

			//Serial.printf("isjoin: %i\r\n", isjoin);
			if (!isjoin){
				for(int i = 0; i < attempts; i++){
					Serial.printf("iteration time: %i\r\n", i);
					njoinlora();
					start_time = millis();
					Serial.print("joining: ");
					Serial.println(joining);
					while ( joining && millis() - start_time <= time_interval){
						lora_rountine();
					}
					if (isjoin) break;
					if (i == 4) isjoin = true;
				}
			}
			Serial.printf("isjoin: %i\r\n", isjoin);

			if (isjoin) {
				Serial.println("Startup low battery deep sleep triggered with SUCCESS lora join");
				wake_up_task_before_sleep(time_interval, attempts);
				delay(300);

		// 		time_t now;
		// 		struct tm timeDetails;
		// 		time(&now);
		// 		localtime_r(&now, &timeDetails);
		// 		int year = timeDetails.tm_year;
		// 		int weekday;
		// 		year == 70 ? weekday = 10 : weekday = timeDetails.tm_wday;
		// 		Serial.printf("year: %i", year);
		// 		int sleep_time;

		// 		switch (weekday)
		// 		{
		// 			case 0:
		// 			case 6:
		// 				sleep_time = long_sleep_time * 4;
		// 				break;
		// 			case 1:
		// 			case 2:
		// 			case 3:
		// 			case 4:
		// 			case 5:
		// 			default:
		// 				sleep_time = long_sleep_time;
		// 				break;
		// 		}
		// 		Serial.printf("today is %i, sleep_time is: %i\r\n", weekday, sleep_time);
		// 		delay(2000);
				// routine_low_battery_sleep(sleep_time);
				routine_low_battery_sleep(long_sleep_time);
			} else {
				Serial.println("Startup low battery deep sleep triggered with UNSUCCESS lora join");
				wake_up_task_before_sleep(time_interval, attempts - 3);
				delay(300);
				routine_low_battery_sleep(long_sleep_time);
			}
		// 	//++bootCount;
		}
	}
}

void deepsleep_routine()
{
	//Serial.printf("battery reading: %f\r\n", bat);
	if (!digitalRead(16))
	{
		charge_stopped = millis();
	}
	if (digitalRead(16)){
		if (bat < highvolt && millis() - charge_stopped >= charge_interval )
		{
			Serial.println("I should buzz then sleep");
			//wake_up_task_before_sleep(20000, 2);
			routine_low_battery_sleep(long_sleep_time);   
		}
	}
}

void routine_low_battery_sleep(int sleep_time)
{
	gps_standby();
	lora_sleep();
	
	Serial.printf("Setup ESP32 to sleep for every %i Seconds\r\n", sleep_time);
	issleep = true;
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, HIGH);
	esp_sleep_enable_timer_wakeup(sleep_time * uS);
	//buzzer(3);
	//Serial.flush();
	Serial.println("Enter Deep Sleep Mode");
	Serial.println("End Time : " + String(millis()));
	digitalWrite(GPIO_NUM_2, LOW);//後面需要切換MPU6050 sleep模式
	digitalWrite(GPIO_NUM_17, LOW);

	esp_wifi_stop();
	//esp_bluedroid_disable();
	//esp_bt_controller_disable();

	esp_deep_sleep_start();
	
}

void lora_task_bcode_wake_up(){
  	Serial.println("loratask b");
	//Serial.println("sending b msg");
	nsendloramsg(encode_cmsg('S'));
	//Serial.println("sent b msg");
	recordCounterB++;
	recordCounterS++;
	bmsging = true;
}

void wake_up_task_before_sleep(int time_interval, int attempts){
	Serial.println("inside wake_up_task_before_sleep");
	delay(300);					// this is needed to ensure firm lora connection

	int start_time = millis();

	while ((!sLongitude[1] || !sLatitude[1] ) && millis() - start_time <= GPS_GET_TIME){
	  	tinygps();
		//Serial.println(millis() - start_time);
	}

	Serial.println("Get GPS Time : " + String(millis() - start_time));
	showgpsinfo();

	if (!cmsgingb){
		for (int i = 0; i < attempts; i ++){
			Serial.printf("attempt is : %i\r\n", i + 1);
			start_time = millis();
			//Serial.printf("cmsging before function: %i", cmsging);
			lora_task_bcode_wake_up();
			//Serial.printf("cmsging after function: %i\r\n", cmsging);
			while ((recordCounterB > 0 || recordCounterS > 0) && millis() - start_time <= time_interval){
				lora_rountine();
			}
			if(recordCounterB == 0 && recordCounterS == 0){
				Serial.println("Sleep S CMSG Suc.");
				break;
			} 
		}
	}
}

