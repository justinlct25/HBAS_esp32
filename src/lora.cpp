#include "lora.h"

//Lora
SoftwareSerial lora(14,13); //TX,RX

//lorawan send and receive
bool isjoin = false;

char cmsg[60];

char input[60];
char lvalue[60];
int inn = 0;
char *ln = NULL;
char *par=NULL;
bool isack = false;

bool joining = false;
bool umsging = false;
bool cmsging = false;
bool amsging = false;
bool amsgsuc = false;
bool bmsging = false;
//bool bmsgsuc = false;

RTC_DATA_ATTR int rssi = 0;
RTC_DATA_ATTR int snr = 0;

//new development
void manuallora(){
    if(Serial.available()>0){
        char a=(char)Serial.read();
        lora.print(a);
        Serial.print(a); //local echo
    }
    if(lora.available()>0){
        char b=(char)Serial.read();
        Serial.print(b);
    }
}

void lora_rountine(){
    /*if(Serial.available()>0){
        char a=(char)Serial.read();
        lora.print(a);
        Serial.print(a); //local echo
    }*/
    if(lora.available()>0){
        input[inn] = (char)lora.read();
        //Serial.printf("%d: %c\r\n",inn,input[inn]);
        inn++;
    }

    ln = strchr(input,'\n');
    if(ln != NULL){
        //for(int i=0;input[i]!='\0';i++) Serial.print(input[i]);
        
        Serial.print("lora receive msg");
        Serial.println(input);

        //join 
        if(strstr(input,"+JOIN: Network joined") !=NULL || strstr(input,"+JOIN: Joined already") !=NULL || strstr(input,"joined") !=NULL || strstr(input,"Joined") !=NULL || strstr(input,"already") !=NULL || strstr(input,"+JOIN: Network joine") !=NULL || strstr(input,"NetID") !=NULL  || strstr(input,"DevAddr") !=NULL ){
            isjoin = true;
        } else if(strstr(input,"+JOIN: Join failed") !=NULL || strstr(input,"failed")){
            isjoin = false;
        }
        if(strstr(input,"+JOIN: Done") !=NULL || strstr(input,"+JOIN: Joined already") !=NULL || strstr(input,"Joined") !=NULL || strstr(input,"already") !=NULL || LORABUSY){
            joining = false;
        }

        //umsg
        if(strstr(input,"+MSG: Done") !=NULL || LORABUSY){
            umsging = false;
            delay(1000);
        }

        //cmsg
        if(strstr(input,"+CMSG: ACK Received")!=NULL || strstr(input,"ACK Received")!=NULL || strstr(input,"Received")!=NULL){
            isack = true;
        }
        lora_msginghandle();
        if(strstr(input,"+CMSG: Done") !=NULL || LORABUSY){
            cmsging = false;
            isack = false;
            isjoin = true;
        }
        
        //Done
        if(strstr(input,"Done") !=NULL){
            joining = false;
            umsging = false;
            cmsging = false;
        }

        //RSSI & SNR 
        if(strstr(input, "RSSI") !=NULL){
            char *RSSInSNR = strstr(input, "RSSI");
            char *token = strtok(RSSInSNR, ", ");
            int count = 0;
            char *RSSIstr = "";
            char *SNRstr = "";
            while(token != NULL){
                if(count==1){
                    rssi = atoi(token);
                }else if(count==3){
                    snr = atoi(token);
                }
                count++;
                token = strtok(NULL, ", ");
            }
            Serial.println("rssi&snr: ");
            Serial.println(String(rssi));
            Serial.println(String(snr));
        }

        //Reflash
        if(strstr(input, "+CMSG: Length error") != NULL || strstr(input, "Length error") != NULL)
        {
            lora.print("AT+CMSG\r\n");
            delay(5);
            lora.print("AT+MSG\r\n");
            Serial.println("reflash lora");
        }

        memset(input,'\0',sizeof(input));
        inn=0;
        ln=NULL;
        //lora.flush();
    }


}

void njoinlora(){
    //lora.print("AT+JOIN\r\n");
    lora.print("AT+JOIN\r\n");
    //lora.flush();
    isjoin = true;
    joining = true;
}

void nsendloramsg(char *msg){
    lora_getpayload();//test use
    lora.printf("AT+CMSG=\"%s\"\r\n",msg);
    //lora.flush();
    //umsging = true;
    isack = false;
    cmsging = true;
}

void nsendloracmsg(char *msg){
    lora_getpayload();//test use
    lora.printf("AT+CMSG=\"%s\"\r\n",msg);
    //lora.flush();
    isack = false;
    cmsging = true;
}

void lora_getpayload()
{
    lora.printf("AT+LW=LEN\r\n");
    delay(5);
}
// void checklorarssi(){
//     lora.printf("AT+TEST=RSSI");
// }

void showlora(){
  Serial.println("--------------lora bool-------------------");
  Serial.printf("%-10s: %d %-10s: %d %-10s: %d\r\n","isjoin",isjoin,"isack",isack,"bmsging",bmsging);
  Serial.printf("%-10s: %d %-10s: %d %-10s: %d\r\n","joining",joining,"cmsging",cmsging,"umsging",umsging);
  Serial.printf("%-10s: %d %-10s: %d\r\n","amsging",amsging,"amsgsuc",amsgsuc);
  Serial.println("------------------------------------------");

}

void lora_msginghandle(){
    //handle Amsg
    if( (amsging && cmsging) && isack ){
        amsgsuc = true;
    }else{
        amsgsuc = false;
    }

}

//legacy development
//bool isretry = false;  //legacy variable
//bool isend = false;    //legacy variable
//String tmsg = "";      //legacy variable
//String feedback = "";  //legacy variable

// void joinlora(){   //legacy function
//   previousTimeoutMillis=millis();
//   isend=false;
//   Serial.println("Joining LoRaWan...");
//   lora.print("AT+JOIN\r\n");
//   lora.flush();
//   while(!isend && timeout(previousTimeoutMillis) ){
//       while(lora.available()>0){
//           char a=char(lora.read());
//           feedback+=a;
//           Serial.print(a);        
//         }
//         if(feedback.indexOf("already")>0 || feedback.indexOf("joined")>0 || feedback.indexOf("failed")>0 || feedback.indexOf("busy")>0 || feedback.indexOf("ms")>0 || feedback.indexOf("ERROR")>0){
//             isend=true;
//         }
//     }
//     //Serial.println(feedback);
//     if(feedback.indexOf("joined")>0 || feedback.indexOf("already")>0){
//         isjoin=true;
//     }
//     if(feedback.indexOf("failed")>0 || feedback.indexOf("busy")>0 || feedback.indexOf("ms")>0){
//         isjoin=false;
//     }
//     feedback="";
// }

// void sendloramsg(String msg){  //legacy function
//   previousTimeoutMillis=millis();
//   isend = false;
//   tmsg="AT+MSG=\"" + msg + "\"\r\n";
//   lora.print(tmsg);
//   while( !isend && timeout(previousTimeoutMillis) ){
//       while(lora.available()>0){
//           char a=char(lora.read());
//           feedback+=a;
//           Serial.print(a);
//         }
//         if(feedback.indexOf("Done")>0 || feedback.indexOf("busy")>0 || feedback.indexOf("ms")>0 || feedback.indexOf("ERROR")>0){
//             isend=true;
//         }
//     }
//     //Serial.println(feedback);
//     feedback="";
// }

// void sendloracmsg(String msg){  //legacy function
//   previousTimeoutMillis=millis();
//   isend = false;
//   tmsg="AT+CMSG=\""+ msg + "\"\r\n";
//   lora.print(tmsg);
//   while( !isend && timeout(previousTimeoutMillis) ){
//       while(lora.available()>0){
//           char a=char(lora.read());
//           feedback+=a;
//           Serial.print(a);
//         }
//         if(feedback.indexOf("Done")>0 || feedback.indexOf("busy")>0 || feedback.indexOf("ms")>0 || feedback.indexOf("ERROR")>0){
//             isend=true;
//         }
//     }
//     //Serial.println(feedback);
//     //if(feedback.indexOf("busy")>0 || feedback.indexOf("ms")>0 || feedback.indexOf("ERROR")>0){isretryb=true;}
//     if(feedback.indexOf("Received")>0){
//         isretry=false;
//     }
//     else{
//         isretry=true;
//     }
//     feedback=""; 
// }

// void sendloracmsg(char *msg){  //legacy function
//   lora.printf("AT+CMSG=\"%s\"\r\n",msg);
//     while(lora.available()>0){
//         char a=char(lora.read());
//         Serial.print(a);
//     }
//     Serial.println("");
//     Serial.println("----------------------");
//     lora.flush();
// }


