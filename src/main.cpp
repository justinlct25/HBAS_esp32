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
#include <esp_task_wdt.h>
//#include <ESPmDNS.h>
#include "bluetooth.h"
#include "deepsleep.h"
#include "checki2c.h"
#include "timer.h"
#include "wifista_update.h"
#include "wifiap_server.h"
#include <ArduinoNvs.h>


//core
TaskHandle_t task1;

//task
TaskHandle_t rout_task;
void rout_taskcode(void *);
//TaskHandle_t lora_task_a;
void lora_task_acode();
//TaskHandle_t lora_task_b;
void lora_task_bcode();

//Queue record
bool alerted = false;
#define maxrecord 500 //up to 1000
#define maxbuffer 60
int recordCounter = 0;
int recordCounterB = 0;
int recordCounterS = 0;
char arecord[maxrecord][maxbuffer];
char bmsg[maxbuffer];

//Important Timer
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
const long interval = 1000; //1 second

unsigned long previousLoraAMsgMillis = 0;
// long LoraAMsginterval = 30000; //30 seconds
long LoraAMsginterval = 20000; //30 seconds

unsigned long previousLoraBMsgMillis = 0;
long LoraBMsginterval = 300000; //5 minutes
// long LoraBMsginterval = 60000; //

unsigned long previousLoraMsgMillis = 0;
long LoraMsginterval = 45000; //

void init();

void setup()
{
    esp_task_wdt_init(60, true); //enable panic so ESP32 restarts
    // esp_task_wdt_add(NULL); //add current thread to WDT watc
    init();
    bat_init();

    Wire.begin();                            //i2c bus
    Serial2.begin(9600, SERIAL_8N1, 32, 33); //serial for gps
    lora.begin(9600);                        //serial for lora
    delay(2000);                             //2s delay for lora initialize (should use)
    //lora_AT_init();
    
    buzzer_init();
    tof_init();
    gyro_init();

    //new
    //if(!issleep) gps_init();
    //gps_init();
    if(bootCount == 0 && bat > highvolt && issleep == false && !digitalRead(16)) 
    {
        gps_coolstart();//20-40s +
    }
    else 
    {
        gps_hotstart();//5s +
    }
    //gps_hotstart();
    
    //Networking initialization
    //wifi_init();
    //mqtt_init();
    //bt_init();
    //njoinlora();

    //core (legacy function)
    //xTaskCreatePinnedToCore(task1code,"task1",10000,NULL,0,&task1,0);
    // Serial.println("issleep2:");
	// Serial.println(issleep);

    deepsleep_handler();

    // get config from NVS
    getNVSConfig();
    delay(500);
    //Assign routine task 5K Byte overflow?
    xTaskCreate(rout_taskcode, "rout_task", 5000, NULL, 2, &rout_task);
    //xTaskCreate(lora_task_acode, "lora_task_a", 5000, NULL, 1, &lora_task_a);
}

void loop()
{
    //watchdog
    // esp_task_wdt_init(30, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watc
    esp_task_wdt_reset();

    //Handle lora transmission status
    lora_rountine();

    tinygps();
    // wifista_update();
    // deepsleep_routine();

        //Quene Amsg
        if (isalert && !alerted)
        {
            strcpy(arecord[recordCounter], encode_cmsg('A'));
            if(recordCounter < 2) recordCounter++;
            alerted = true;
        }
        if (!isalert && alerted)
        {
            alerted = false;
        }

        //lora task a
        if (millis() - previousLoraAMsgMillis >= LoraAMsginterval)
        {
            previousLoraAMsgMillis = millis();
            //xTaskCreate(lora_task_acode, "lora_task_a", 5000, NULL, 2, &lora_task_a);
            lora_task_acode();
        }

        //lora task b
        if (millis() - previousLoraBMsgMillis >= LoraBMsginterval)
        {
            previousLoraBMsgMillis = millis();
            recordCounterB = 1;//start send b
            //xTaskCreate(lora_task_bcode, "lora_task_b", 5000, NULL, 2, &lora_task_b);
            lora_task_bcode();
            previousLoraMsgMillis = millis();
        }

        //repeat b
        if (millis() - previousLoraMsgMillis >= LoraMsginterval)
        {
            previousLoraMsgMillis = millis();
            lora_task_bcode();
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
        //B發送中 && cmsgb沒有發送中 && B發送成功
        if ((bmsging && !cmsgingb) && bmsgsuc)
        {
            bmsging = false;
            bmsgsuc = false;
        }//B發送中 && cmsgb沒有發送中 && B發送不成功
        else if ((bmsging && !cmsgingb) && !bmsgsuc)
        {
            bmsging = false;
        }

        //current WDT status
        // if (millis() - previousMillis1 >= interval)
        // {
        //     previousMillis1 = millis();
        //     Serial.print("loop WDT STATUS : ");
        //     Serial.println(esp_task_wdt_status(NULL));
        // }
}

void init()
{
    //very important
    pinMode(17, OUTPUT);    //All module power supply
    digitalWrite(17, HIGH); //pull up
    pinMode(16, INPUT_PULLUP);     //battery charging signal

    //pinMode(39, ANALOG); //bat STDBY

    //open MPU6050/Radar Power
    pinMode(GPIO_NUM_2, OUTPUT);
    digitalWrite(GPIO_NUM_2, HIGH);
    // //Red LED PIN INIT
    pinMode(GPIO_NUM_19, OUTPUT);
    digitalWrite(GPIO_NUM_19, HIGH);

    // //tof xshut io (may not need)
    pinMode(23, OUTPUT);
    digitalWrite(23, HIGH);

    
    Serial.begin(115200);
}

void rout_taskcode(void *parameter)
{
    for (;;)
    {
        esp_task_wdt_add(NULL); //add current thread to WDT watc
        esp_task_wdt_reset();
        if (millis() - previousMillis >= interval)
        {
        //     Serial.print("NVS.getString(latitude)");
        //     Serial.println(NVS.getString("latitude"));
        //     Serial.print("NVS.getBlobSize(latitude)");
        //     Serial.println(NVS.getBlobSize("latitude"));

        //             unsigned short usStackHighWaterMark;
        // Serial.print(xPortGetFreeHeapSize());

            previousMillis = millis();
            //Necessary checking
            i2cdev_restore();
            getdistance();
            gyro_update();
            calrot3();
            person();
            checkrot2();
            samplebattery();

            // tinygps();

            //Debug printing
            //Serial.print("\f");  //new page for some serial monitor
            //Serial.printf(">>>>>> run on core %d  stack: %d\r\n",xPortGetCoreID(),uxTaskGetStackHighWaterMark(NULL));
            Serial.printf("---------uptime: %s----bootCount: %i---------\r\n", getuptime(),bootCount);
            showgpsinfo();
            showbattery();
            //Serial.printf("wifi status: %d , RSSI: %d\r\n",wifi_stat(),wifi_strength());
            //Serial.printf("io16: %d\r\n",digitalRead(16));
            //showstatus();
            showgyro();
            showtof();
            Serial.print("BAT CHANGE : ");
            Serial.println(digitalRead(16));
            // Serial.print("BAT FULL : ");
            // Serial.println(analogRead(39));

            // Serial.print("rout_taskcode WDT STATUS : ");
            // Serial.println(esp_task_wdt_status(NULL));
            
            //showallbool();
            //showrecord();
            //showversion();
            //showi2cstate();
            //showi2cdev();
            // //utctime();
            // Serial.printf("amsg timer: %s\r\n", gettimer(previousLoraAMsgMillis));
            // Serial.printf("bmsg timer: %s\r\n", gettimer(previousLoraBMsgMillis));
            // showlora();

            // // Serial.printf("before update");

            //if(bat >= highvolt) wifiAPServer_routine();
            if(bat >= highvolt && bootCount == 1) 
            {
                wifiAPServer_routine();
                webSocketMeasureInfo();
                webSocketLoggerInfo();
            }

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

            // send websocket info
            // webSocketMeasureInfo();
            // webSocketLoggerInfo();


            // Serial.println(wifi_ssid);
            // Serial.println(wifi_password);
            // Serial.println(update_server_url);
            // Serial.println(version_url);
            // Serial.println(bin_url);

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
    }
}

void lora_task_acode()
{
    Serial.println("loratask a");
    if (!isjoin)
    {
        //Serial.println("joining lora...");
        njoinlora();
        //Serial.println("joined lora...");
    }
    else
    {
        //if(!joining && !cmsging && !umsging){
        if (recordCounter > 0)
        {
            //Serial.printf("Send Amsg %d\r\n", recordCounter - 1);
            //Serial.println("sending a msg");
            nsendloracmsg(arecord[recordCounter - 1]);
            amsging = true;
            //Serial.println("sent a msg");
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

    //vTaskDelete(NULL);
}

void lora_task_bcode()
{
    Serial.println("loratask b");
    if (!isjoin)
    {
        //Serial.println("joining lora...");
        njoinlora();
        //Serial.println("joined lora...");
    }
    else
    {
        //if(!joining && !cmsging && !umsging){
        //Serial.println("routine LoRa battery msg sent");
        if(recordCounterB > 0)
        {
            strcpy(bmsg, encode_cmsg('B'));
            // nsendloramsg(bmsg);
            //Serial.println("sending b msg");
            nsendloramsg(bmsg);
            bmsging = true;
        }
        //isjoin = false; // will turn true if +CMSG: Done is received (for checking if isjoin actually keeping)
        //Serial.println("sent b msg");
    }
    //else{
    //Serial.println("LoRa module not free");
    //}
    //}

    //vTaskDelete(NULL);
}

