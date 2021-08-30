#include <Arduino.h>
#include "Wire.h"
#include "wifi_c.h"
#include "tof.h"
#include "gyro.h"
#include "checkbool.h"
#include "buzzer.h"
#include "bat.h"
#include "gps.h"
#include "data_tracer.h"
#include "lora.h"
#include "encode_msg.h"
//#include "ota.h"
#include "http.h"
#include "version.h"
#include "mqtt.h"
//#include "radar.h"
#include "led.h"
//#include <esp_task_wdt.h>
//#include <ESPmDNS.h>
#include "bluetooth.h"
#include "deepsleep.h"
#include "checki2c.h"
#include "timer.h"

//core
TaskHandle_t task1;

//task
TaskHandle_t rout_task;
void rout_taskcode(void *);
TaskHandle_t lora_task_a;
void lora_task_acode(void *);
TaskHandle_t lora_task_b;
void lora_task_bcode(void *);

//Queue record
bool alerted = false;
#define maxrecord 500 //up to 1000
#define maxbuffer 60
int recordCounter = 0;
char arecord[maxrecord][maxbuffer];
char bmsg[maxbuffer];

//Important Timer
unsigned long previousMillis = 0;
const long interval = 1000; //1 second

unsigned long previousLoraAMsgMillis = 0;
long LoraAMsginterval = 30000; //30 seconds

unsigned long previousLoraBMsgMillis = 0;
long LoraBMsginterval = 900000; //15 minutes

unsigned long previousWifiMillis = 0;
long Wifiinterval = 3600000; //1 hour

void init();

void setup()
{
    //watchdog
    // esp_task_wdt_init(60, true); //enable panic so ESP32 restarts
    // esp_task_wdt_add(NULL); //add current thread to WDT watch

    init();
    bat_init();

    Wire.begin();                            //i2c bus
    Serial2.begin(9600, SERIAL_8N1, 32, 33); //serial for gps
    lora.begin(9600);                        //serial for lora
    delay(2000);                             //2s delay for lora initialize (should use)
    buzzer_init();
    tof_init();
    gyro_init();

    //Networking initialization
    // wifi_init();
    //mqtt_init();
    //bt_init();
    njoinlora();

    //core (legacy function)
    //xTaskCreatePinnedToCore(task1code,"task1",10000,NULL,0,&task1,0);
    deepsleep_handler();

    //Assign routine task
    xTaskCreate(rout_taskcode, "rout_task", 5000, NULL, 2, &rout_task);
    xTaskCreate(lora_task_acode, "lora_task_a", 5000, NULL, 1, &lora_task_a);

    //tof xshut io (may not need)
    pinMode(23, OUTPUT);
    digitalWrite(23, HIGH);
}

void loop()
{
    //Handle lora transmission status
    lora_rountine();

    //legacy function
    //lora alert msg
    // if(isalert){
    //     //tmsg=encode_fmsg("A");
    //     //strcpy(arecord[recordCounter],tmsg.c_str());
    //     strcpy(arecord[recordCounter],encode_cmsg('A'));
    //     recordCounter++;
    //     previousTimeoutMillis=millis();
    //     while(!isbrake && timeout(previousTimeoutMillis));
    //     alerted=true;
    //   if(!isjoin){
    //     //Serial.println("Not in LoRaWan. Rejoining LoRaWan...");
    //     njoinlora();
    //     //LoraAMsginterval = 30000;
    //   }
    //   else{
    //     Serial.println("Lora alert msg sent");
    //     //sendloracmsg(tmsg);
    //     nsendloracmsg(arecord[recordCounter-1]);
    //     if(isretry){
    //       Serial.println("Retry in 30s");
    //       alerted=true;
    //       //LoraAMsginterval = 30000;
    //     }
    //     else{
    //       Serial.println("ack received");
    //       recordCounter--;//
    //       if(recordCounter == 0){
    //         alerted=false;
    //         //LoraAMsginterval = 300000;
    //       }
    //       else{
    //         LoraAMsginterval = 30000;
    //       }
    //     }
    //   }
    // }
    //
    // if(alerted){
    //   unsigned long currentLoraAMSGMillis = millis();
    //   if(currentLoraAMSGMillis - previousLoraAMsgMillis >= LoraAMsginterval){
    //     Serial.println("LORA AMSG LOOP");
    //     if(!isjoin){
    //       Serial.println("Not in LoRaWan. Rejoining LoRaWan...");
    //       njoinlora();
    //       LoraAMsginterval = 30000;
    //     }
    //     if(recordCounter>0){
    //       Serial.println("Get stored alert msg and send");
    //       //String tmsg=arecord[recordCounter-1];
    //       Serial.printf("Resend msg %d\r\n",recordCounter-1);
    //       //sendloracmsg(tmsg);
    //       sendloracmsg(arecord[recordCounter-1]);
    //       if(isretry){
    //         Serial.println("Retry in 30s");
    //         LoraAMsginterval = 30000;
    //         Serial.printf("%d records are not sent\r\n",recordCounter);
    //       }
    //       else{
    //         Serial.println("ack received");
    //         Serial.printf("msg %d sent\r\n",recordCounter-1);
    //         recordCounter--;
    //       }
    //     }
    //     else{
    //       LoraAMsginterval = 300000;
    //       alerted=false;
    //     }
    //     previousLoraAMsgMillis = currentLoraAMSGMillis;
    //   }
    // }
    //
    //lora routine battery msg
    // unsigned long currentLoraBMSGMillis = millis();
    // if(currentLoraBMSGMillis - previousLoraBMsgMillis >= LoraBMsginterval){
    //   Serial.println("LORA BMSG LOOP");
    //   //tmsg=encode_fmsg("B");
    //   strcpy(bmsg,encode_cmsg('B'));
    //   if(!isjoin){
    //     //Serial.println("Not in LoRaWan. Rejoining LoRaWan...");
    //     njoinlora();
    //   }
    //   else{
    //     Serial.println("routine LoRa battery msg sent");
    //     nsendloracmsg(bmsg);
    //     bmsging = true;
    //   }
    //   previousLoraBMsgMillis = currentLoraBMSGMillis;
    // }
}

void init()
{
    //very important
    pinMode(17, OUTPUT);    //All module power supply
    digitalWrite(17, HIGH); //pull up
    pinMode(16, INPUT);     //battery charging signal
    Serial.begin(115200);
    //very important
}

void rout_taskcode(void *parameter)
{
    for (;;)
    {
        if (millis() - previousMillis >= interval)
        {
            previousMillis = millis();
            //Necessary checking
            i2cdev_restore();
            getdistance();
            gyro_update();
            calrot3();
            person();
            checkrot2();
            samplebattery();
            tinygps();

            //Debug printing
            //Serial.print("\f");  //new page for some serial monitor
            //Serial.printf(">>>>>> run on core %d  stack: %d\r\n",xPortGetCoreID(),uxTaskGetStackHighWaterMark(NULL));
            Serial.printf("-------------uptime: %s-------------\r\n", getuptime());
            //showgpsinfo();
            showbattery();
            //Serial.printf("wifi status: %d , RSSI: %d\r\n",wifi_stat(),wifi_strength());
            //Serial.printf("io16: %d\r\n",digitalRead(16));
            //showstatus();
            showgyro();
            showtof();
            showallbool();
            showrecord();
            //showversion();
            //showi2cstate();
            //showi2cdev();
            //utctime();
            Serial.printf("amsg timer: %s\r\n", gettimer(previousLoraAMsgMillis));
            Serial.printf("bmsg timer: %s\r\n", gettimer(previousLoraBMsgMillis));
            showlora();

            //mqtt monitor
            //mqttpub();

            //bluetooth terminal
            //bt.printf("      ----------uptime: %s----------      \r\n",getuptime());
            //BTshowstatus();
            //BTshowrecord();
            //BTshowallbool();

            //LED & Buzzer operate
            led_operate(); //LED
            buz_operate(); //buzzer

            //deep sleep
            deepsleep_routine();

        } //end 1s rountine

        //Bluetootooth routine
        //bt_routine();
        //bt_quit();

        //LED & Buzzer routine
        buz.cbuz_routine();
        blueled.cled_routine();
        yellowled.cled_routine();

        //Quene Amsg
        if (isalert && !alerted)
        {
            strcpy(arecord[recordCounter], encode_cmsg('A'));
            recordCounter++;
            alerted = true;
        }
        if (!isalert && alerted)
        {
            alerted = false;
        }

        //lora task a
        if (millis() - previousLoraAMsgMillis >= LoraAMsginterval)
        {
            xTaskCreate(lora_task_acode, "lora_task_a", 5000, NULL, 2, &lora_task_a);
            previousLoraAMsgMillis = millis();
        }

        //lora task b
        if (millis() - previousLoraBMsgMillis >= LoraBMsginterval)
        {
            xTaskCreate(lora_task_bcode, "lora_task_b", 5000, NULL, 2, &lora_task_b);
            previousLoraBMsgMillis = millis();
        }

        //check amsg stauts
        if ((amsging && !cmsging) && amsgsuc)
        {
            memset(arecord[recordCounter], '\0', sizeof(arecord[recordCounter])); //may not need
            recordCounter--;
            amsging = false;
            amsgsuc = false;
        }
        else if ((amsging && !cmsging) && !amsgsuc)
        {
            amsging = false;
        }

        //check bmsg status
        if ((bmsging && !umsging))
        {
            bmsging = false;
        }
    }
}

void lora_task_acode(void *parameter)
{
    Serial.println("loratask a");
    if (!isjoin)
    {
        njoinlora();
    }
    else
    {
        //if(!joining && !cmsging && !umsging){
        if (recordCounter > 0)
        {
            Serial.printf("Send Amsg %d\r\n", recordCounter - 1);
            nsendloracmsg(arecord[recordCounter - 1]);
            amsging = true;
        }
        // else{ //replace amsg with bmsg if no arecord
        // Serial.println("routine LoRa battery msg sent");
        // strcpy(bmsg,encode_cmsg('B'));
        // nsendloramsg(bmsg);
        // bmsging = true;
        // } //
        //}
        //else{
        //Serial.println("LoRa module not free");
        //}
    }

    vTaskDelete(NULL);
}

void lora_task_bcode(void *parameter)
{
    Serial.println("loratask b");
    if (!isjoin)
    {
        njoinlora();
    }
    else
    {
        //if(!joining && !cmsging && !umsging){
        Serial.println("routine LoRa battery msg sent");
        strcpy(bmsg, encode_cmsg('B'));
        nsendloramsg(bmsg);
        bmsging = true;
    }
    //else{
    //Serial.println("LoRa module not free");
    //}
    //}

    vTaskDelete(NULL);
}

void task1code(void *parameter)
{ //legacy function
    for (;;)
    {
        //routine checking
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;
            //Serial.print("\f");
            i2cdev_restore();
            getdistance();
            gyro_update();
            calrot3();
            person();
            checkrot2();
            samplebattery();
            tinygps();

            //debug section
            //Serial.printf(">>>>>> run on core %d  stack: %d\r\n",xPortGetCoreID(),uxTaskGetStackHighWaterMark(NULL));
            Serial.printf("----------------uptime: %s----------------\r\n", getuptime());
            showgpsinfo();
            showbattery();
            //Serial.printf("wifi status: %d , RSSI: %d\r\n",wifi_stat(),wifi_strength());
            //Serial.printf("io16: %d\r\n",digitalRead(16));
            //showstatus();
            //showgyro();
            //showtof();
            //showallbool();
            //showrecord();
            //showversion();
            //showi2cstate();
            //showi2cdev();
            //utctime();
            //showlora();

            //mqtt monitor
            //mqttpub();

            //bluetooth terminal
            // bt.printf("      ----------uptime: %s----------      \r\n",getuptime());
            // BTshowstatus();
            // BTshowrecord();
            // BTshowallbool();

            led_operate(); //LED
            buz_operate(); //buzzer

            //deep sleep
            deepsleep_routine();

        } //end 1 second

        /*//wifi connection and ota
  unsigned long currentWifiMillis = millis();
  if(currentWifiMillis - previousWifiMillis >= Wifiinterval){
    previousWifiMillis = currentWifiMillis;
    Serial.println("Check wifi status");
    if(!wifi_stat()){
      wifi_reconnect();
      Serial.println("Reconnecting to wifi");
      Serial.printf("wifi status: %d , RSSI: %d\r\n",wifi_stat(),wifi_strength());
    }
    else{ 
      Serial.printf("latest version: %s\r\n",http_version_check());
      version_check();
      if(!islateset){
        Serial.println("got lateset version");
        if(!isstartup){
          ota_update();
          isstartup = true;
        }
        else{
          ota_status();
        }
      }
    }
  }*/
        //bt_routine();
        //bt_quit();

        //buz.cbuz_routine();
        blueled.cled_routine();
        yellowled.cled_routine();
    }
}