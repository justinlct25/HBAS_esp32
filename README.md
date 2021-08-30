<!--README.md by nathanhoyn date: 20210824-->
# truck - Handbrake Alert System
## Project compontents
**0. Basic configuration and networking**
  - Power supply
  - Battery checking
  - Deepsleep mode
  - WiFi, MQTT, Bluetooth

**1. Checking hardbrake stauts and Alert**
  - Internal Accelerometer and Gyroscope *MPU6050*
  - External Accelerometer and Gyroscope *MPU6050*
  - Time-of-fight distance senor *VL53L0X*
  - Buzzer and LED

**2. Get datetime and location information**
  - GPS module *L76-LB*

**3. Send message via LoRaWan**
  - LoRaWan Modem *RHF76-052*
---
## Component category
**1. GPIO**
  - Power supply
  - Charging signal
  - Module control
  - LED control
  - Deepsleep

**2. ADC**
  - Battery checking

**3. PWM**
  - Buzzer control

**4. UART**
  - GPS
  - LoRaWan
  - Bluetooth
  - Debug printing

**5. I2C**
  - MPU6050 Accelerometer
  - VL53L0X Time-of-fight distance senor
---
## Program structure
| Program file | Description |
| --- | --- |
| main.cpp | main program |
| bat.cpp / bat.h | get battery value |
| bluetooth.cpp / bluetooth.h | bluetooth communication |
| buzzer.cpp / bluzzer.h | buzzer configuration and implemantation |
| cbuzzer.cpp / cbluzzer.h | custom library for buzzer |
| checkbool.cpp / cbluzzer.h | calculatation for alert boolean |
| checki2c.cpp / checki2c.h | check whether i2c devices is working and restore i2c devices |
| cled.cpp / cled.h | custom library for LED |
| data_tracer.h / data_tracer.cpp | printing debug message |
| deepsleep.cpp / deepsleep.h | handle deepsleep function |
| encode_msg.cpp /encode_msg.h | encoding message for LoRa transmission |
| gps.cpp / gps.h | handle gps function |
| gyro.cpp / gyro.h | handle accelerometer and its calculatation |
| http.cpp / http.h | version checking for OTA(Over-the-air) update |
| led.cpp / led.h | LED configuration and implemantation |
| lora.cpp / lora.h | handle join LoRaWan, send LoRa message |
| mqtt.cpp / mqtt.h | handle mqtt configuration and pubish mqtt message for demo data |
| ota.cpp / ota.h | handle OTA(Over-the-air) update |
| radar.cpp / radar.h | (may not used) radar related |
| timeout.cpp / timeout.h | (may not used) provide timeout bool for infinity while loop |
| timer.cpp / timer.h | provide printing of uptime and timer counter |
| tof.cpp / tof.h | handle tof(Time-of-fight) distance sensor and its calculatation |
| version.cpp / version.h | (can be improved) handle version checking from http.cpp |
| wifi_c.cpp / wifi_c.h | handle wifi configuration and connection |

## Program flow
main program flow

![program flow](/assets/images/truck_program_flow.png)

## Program usage
* **bat.h**
  * Variables
    * __bat__  (float) - voltage of battery
    * __bat2__ (int) - 100 times voltage of battery for encoding message
    * __bat_counter__ (int) - voltage sampling counting
  * Function
    * __void bat_init()__ - initialize and attach the ADC pin
    * __void getbattery()__ - read voltage from ADC pin and store to bat, bat2
    * __void samplebattery()__ - get sample of voltage by "bat_sample_interval" (deflaut is 60, define in bat.h)
    * __void showbattery()__ - print battery sample and counting to serial monitor
* **bluetooth.h**
  * Variables
     * __buf[50]__ (char[])- input buffer with size 50
     * __cvalue[20]__ (char[])- store value from input buffer
     * __inc__ (int) - received character counter
     * __line__ (char *) - pointer of input buffer
     * __param__ (char *) - pointer of stored value
     * __quit_time__ (unsigned long) - lifetime of Bluetooth session since booting
     * __extent_time__ (unsigned long) - extented lifetime of Bluetooth session if there is client
     * __isbtoff__ (bool) - availability of Blutooth function
  * Function
     * __void bt_init()__ - initialize the Bluetooth session
     * __void bt_routine()__ - send(disabled), recevie and check input data from Bluetooth
     * __void bt_quit()__ - end session if the uptime of decive are excessed quit_time, extent lifetime for extent_time if session has client 
* **buzzer.h**
  * Variables
    * __buz__ (CBUZ) - CBUZ object from cbuzzer.h
  * Function 
    * __void buz_operate()__ - set operatation mode for buzzer
    * __(legacy function)__ void buzzer_init() - initialize buzzer
    * __(legacy function)__ void buzzer(int x) - turn on buzzter by mode x (0-off, 1-light, 2-full-on)
* **cbuzzer.h**
  * Variables
    * __CBUZ__ (class) - object of buzzer
      + BUZ_PIN (int) - PWM pin of buzzer
      + BUZ_OPERATE (buz_operation) - operatation mode of buzzer
      + BUZ_CHANNEL (int) - PWM channel
      + BUZ_FREQ (int) - PWM frequency
      + BUZ_RESOLUTION (int) - PWM resolution
      + BUZ_STATE (int) - buzzer operating PWM
  * Function 
    * __CBUZ(int _pin_, buz_operation _operatation_, int _freq_, int _channel_, int _resolution_)__ - constructor of CBUZ odject
    * __CBUZ::setOperatation(buz_operation _operatation_)__ - set operatation for CBUZ object
    * __CBUZ::cbuz_routine()__ - implement buzzer action base on operatation of CBUZ object
* **checkbool.h**
  * Variables
    * __isperson__ (bool) - is there a person depends on tof sensor
    * __isbrake__ (bool) - is the handbrake pulled depends on accelerometer
    * __isalert__ (bool) - is the alert tirggered (isperson & isbrake)
    * __lim_distance__ (int) - limit distance to determine _isperson_
    * __lim_angle__ (int) - limit angle to determine _isbrake_
  * Function 
    * __void person()__ - update isperson depends on _distance_ from tof.h
    * __void checkrot2()__ - update isbrake and isalert depends on isperson and _stickrot_ from gyro.h
* **checki2c.h**
  * Variables
    * __istof__ (bool) - is tof sensor connected to I2C bus
    * __isintmpu__ (bool) - is internal accelerometer connected to I2C bus
    * __isextmpu__ (bool) - is external accelerometer connected to I2C bus
  * Function 
    * __void i2cdev_restore()__ - check the connection of I2C devices. if notconnected, reinitialize the corresponding device
    * __void showi2cstate()__ - print all I2C device boolean to serial monitor
    * __void showi2cdev()__ - print all found I2C device address from I2C bus to serial monitor
* **CLED.h**
  * Variables
    * __CLED__ (class) - object of LED
      + LED_PIN (int) - pin of LED
      + LED_OPERATE (led_operation) - operatation mode of LED 
      + LED_STATE (int) - state of LED operating
      + LED_INTERVAL (unsigned long) - timer interval for LED blinking
      + LED_CURRENT (unsigned long) - current time for LED blinking
      + LED_PERVIOUS (unsigned long) - pervious time for LED blinking
  * Functions
    * __CLED::CLED(int _pin_,led_operation _operatation_)__ - constructor of CLED object
    * __CLED::setOperatation(led_operation operatation)__ - set operatation for CLED object
    * __CLED::routine()__ - implement LED action base on operatation of CLED object
* **data_tracer.h**
  * Functions
    * __void showrecord()__ - print all _arecord_ from main.cpp to serial monitor
    * __void showallbool()__ - print useful boolean to serial monitor
    * __void BTshowstatus()__ - print all _arecord_ from main.cpp to Bluetooth
    * __void BTshowallbool()__ - print useful boolean for debugging to Bluetooth
    * __(legacy function)__ void showstatus() - show basic value with Hex encoded
    * __(legacy function)__ void BTshowstatus() - show basic value with Hex encoded to Bluetooth
* **deepsleep.h**
  * Variables
    * __bootCount__ (RTC_DATA_ATTR int) - boot count from awaking from deepsleep mode (RTC variable)
    * __issleep__ (RTC_DATA_ATTR bool) - is the devices already enter deepsleep mode before (RTC variable)
  * Functions
    * __void print_wakeup_reason()__ - print wake up reason to serial monitor
    * __void deepsleep_handler()__ - check whether device need to enter deepsleep mode on startup
    * __void deepsleep_routine()__ - check whether device need to enter deepsleep mode on routine
* **encode_msg.h**
  * Variables
    * __testmsg[60]__ (char[]) - encoding message buffer
  * Functions
    * __char *encode_cmsg(char msgtype)__ - encode message with message type _msgtype_ for LoRa transmission, return the pointer to _testmsg[60]_
    * __(legacy function)__ String encode_fmsg(String msgtype) - encode message with message type _msgtype_ for LoRa transmission, return the String _encode_msg_
* **gps.h**
  * Variables
    * __tgps__ (TinyGPSPlus) - class object from library TinyGPSPlus
    * __sday, smonth, syear__ (String) - information of day, month and year
    * __shour, sminute, ssecond, scentisecond__ (String) - infomation of hour, minute, second, scentisecond
    * __location__ (String) - printing infromation of latitude and longitude
    * __datetime__ (String) - printing information of date and time
    * __ptime__ (int) - information of time in seconds
    * __pdate__ (int) - information of date in 8-digit integer
  * Functions
    * __void tinygps()__ - update date, time, location information via gps module
    * __void showgpsinfo()__ - print date, time and location to serial monitor
    * __time_t utctime()__ - get utc timestamp in second base on datatime information, return utc timestamp in char[] format
* **gyro.h**
  * Variables
    * a1 (MPU6050) - class object from library MPU6050
    * a2 (MPU6050) - class object from library MPU6050
    * __ax, ay, az__ (int16_t) - acceleration for internal MPU6050 in xyz axis
    * __ax2, ay2, az2__ (int16_t) - acceleration for external MPU6050 in xyz axis
    * __x, y, z__ (double) - degree for internal MPU6050 in  xyz axis
    * __x2, y2, z2__ (double) - degree for external MPU6050 in xyz axis
    * __minVal__ (int) - magic number  265 to calculate degree
    * __maxVal__ (int) - magic number  402 to calculate degree
    * __globx__ (int) - value of y (0 - 360)
    * __offsetx__ (int) - mapped value of _globx_ (0 - 180, 0 - -180)
    * __stibx__ (int) -  mapped value of _x2_ (0 - 180, 0 - -180)
    * __stickrot__ (int) - included angle of _offsetx_ and _stibx_
  * Functions
    * __void gyro_init()__ - initialization of a1 and a2
    * __void gyro_update()__ - get acceleration data of xyz axis for both a1 and a2
    * __void calrot3()__ - calculate globx, offsetx, stibx and stickrot to determine handbrake status
    * __void showgyro()__ - print gyro debug information to serial monitor
* **http.h**
  * Variables
    * __URL__ (const char *) - url for version checking server
    * __latestVersion__ (String) - latest version get from http response from server
    * __payload__ (String) - http response buffer
    * __httpResponseCode__ (int) - http response code
  * Functions
    * __String http_version_check()__ - send http requset to server to get the response of version number and store to latestVersion, return latest Version
* **led.h**
  * Variables
    * __blueled__ (CLED) - CLED object from cled.h for blue LED
    * __yellowled__ (CLED) - CLED object from cled.h for yellow LED
  * Functions
    * __void led_operate()__ - set operatation mode for LED
* **lora.h**
  * Variables
    * input[60]__ (char[]) - receive buffer for lora input
    * __inn__ (int) - received character counter
    * __ln__ (char *) - pointer of input buffer
    * __isjoin__ (bool) - is joined LoRaWan network
    * __isack__ (bool) - is ack is received during transmitting CMSG
    * __joining__ (bool) - is AT+JOIN command for LoRa modem processing
    * __umsging__ (bool) - is AT+MSG command for LoRa modem processing
    * __cmsging__ (bool) - is AT+CMSG command for LoRa modem processing
    * __amsging__ (bool) - is sending alert message
    * __amsgsuc__ (bool) - is transmission of alert message sucessful
    * __bmsging__ (bool) - is sending battery message
  * Functions
    * __void manuallora()__ - manual communication for LoRa module
    * __void lora_rountine()__ - send(disabled), recevie and check input data from LoRa modlue, determine all boolean of LoRa module status
    * __void lora_msginghandle()__ - determine amsgsuc, called in _lora_routine()_
    * __void njoinlora()__ - send join command to LoRa module
    * __void nsendloramsg(char* msg)__ - send uncomfirmed message _msg_ to LoRaWan via LoRa module
    * __void nsendloracmsg(char* msg)__ - send comfirmed message _msg_ to LoRaWan via LoRa module
    * __void showlora__ - print all lora boolean to serial monitor
* **mqtt.h**
  * Variables
    * __mqtt_server__ (const char*) - MQTT broker location
    * __mqtt_port__ (uint16_t) - MQTT port
  * Functions
    * __void mqtt_init()__ - initialize and configure MQTT server and port
    * __void mqttpub()__ - reconnect to MQTT broker and Pubish MQTT message 
* **ota.h**
  * Variables
    * __otastatus__ (static HttpsOTAStatus_t) - emun for ota status
    * __url__ (static const char) - url for ota server
    * __server_certificate__ (static const char) - certificate for ota server (if any)
    * __isstartup__ (bool) - is ota process running
  * Functions
    * __void HttpEvent(HttpEvent_t *event)__ - print http response of ota process
    * __void ota_update()__ - start ota process
    * __void ota_status()__ - check whether ota is sucessful or failed
* **radar.h**
  * Variables
    __radar_out__ (uint8_t) - radar signal output
    __radar_tx___ (uint8_t) - radar range configuration pin
    __radar_rx___ (uint8_t) - radar timeout configuration pin
  * Functions
    * __void init_radar()__ - initialization and configuration of radar
    * __bool radar_state()__ - check radar signal, return radar status
* **timeout.h**
  * Variables
    * __previousTimeoutMillis__ (unsigned long) - pervious time for timeout
    * __Timeoutinterval__ (const long) - timeout interval for timeout (deflaut 15 seconds)
  * Functions
    * __bool timeout(unsigned long perviousMillis)__ return true after 15 seconds since the time _perviousMillis_
    * __bool timeout(unsigned long perviousMillis,long customTimeoutinterval)__ return true after custom interval _customTimeoutinterval_ since the time _perviousMillis_
* **timer.h**
  * Variables
    * __cuptime[cuptimebuffer]__ (char[]) - buffer for uptime
    * __timer[ctimerbuffer]__ (char[]) - buffer for timer
  * Functions
    * __char *getuptime(unsigned long pervMillis)__ - print timer in format with time _pervMillis_, return the pointer formated timer _timer[ctimerbuffer]_
* **tof.h**
  * Variables
    * __tof__ (VL53L0X) - class object from library VL53L0x
    * __raw_distance__ (uint16_t) - raw distance data form tof sensor in unit mm
    * __distance__ (int16_t) - distance data form tof sensor in unit cm (range form 0~818), distance = -1 if tof sensor is not working
  * Functions
    * __void tof_init()__ - initialization of tof sensor
    * __void getdistance()__ - update raw_distance and distance from tof sensor, distance = -1 if tof sensor is not working
    * __void showtof()__ - print raw_distance and distance to serial monitor
* **version.h**
  * Variables
  * Functions
  * 
  __redesign is needed__
* **wifi_c.h**
  * Variables
    * __ssid__ (const char *) - ssid of wifi access point
    * __password__ (const char *) - password of wifi access point
  * Functions
    * __void wifi_init()__ - initialize and configure wifi
    * __bool wifi_stat()__ - return wifi connection state
    * __void wifi_reconnect()__ - reconnect to wifi
    * __int wifi_strength()__ - return RSSI
