#include "bluetooth.h"

BluetoothSerial bt;
char buf[50] = "";
char cvalue[20] = "";
int inc = 0;
char *line = NULL;
char *param = NULL;

unsigned long quit_time = 60000UL;
unsigned long extent_time = 30000UL;
bool isbtoff = false;

void bt_init(){
    bt.begin("RM");
}

void bt_routine(){
if(!isbtoff){
    /*if (Serial.available()) { //send to bluetooth
        bt.write(Serial.read());
    }*/
    if(bt.available()) {
        buf[inc] = (char)bt.read();
        //Serial.printf("%d: %c\r\n",inc,buf[inc]);
        inc++;
    }
    
    line = strchr(buf,'\n');
    if(line != NULL){
        param = strstr(buf,"v=");
        if(param != NULL){
            strcpy(cvalue,param +1);
        }
        memset(buf,'\0',sizeof(buf));
        inc=0;
        line=NULL;
        param = NULL;
        bt.flush();
    }
}
}

void bt_quit(){
    if(millis()>=quit_time && !isbtoff){
        if(bt.hasClient()){
            quit_time = quit_time + extent_time;
        }
        else{
            bt.disconnect();
            bt.end();
            isbtoff = true;
        }
    }
}