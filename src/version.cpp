#include "version.h"

char curfw[strlen(fw_version)+1]=fw_version;
const char s[2] = ".";
char cver[3][2];
int icver[3];

char netfw[10];
char nver[3][2];
int inver[3];

bool islateset = true;

void version_spilt()
{   
    char *token;
    int i=0;
    token = strtok(curfw, s);
    while( token != NULL ) 
   {
      sprintf(cver[i],"%s", token );
      icver[i]=cver[i][0]-'0';
      token = strtok(NULL, s);
      i++;
   }
   
   latestVersion.toCharArray(netfw,latestVersion.length()+1);
   int j=0;
    token = strtok(netfw, s);
    while( token != NULL ) 
   {
      sprintf(nver[j],"%s", token );
      inver[j]=nver[j][0]-'0';
      token = strtok(NULL, s);
      j++;
   }
   
}

void version_check()
{
   version_spilt();
   islateset = true;
   int nver = inver[0]*100+inver[1]*10+inver[2];
   int cver = icver[0]*100+icver[1]*10+icver[2];
   if(nver>cver){
      islateset = false;
   }
   else{
      islateset = true;
   }
}

void showversion()
{
   Serial.printf("current version: %d.%d.%d\r\n",icver[0],icver[1],icver[2]);
   Serial.printf("lateset version: %d.%d.%d\r\n",inver[0],inver[1],inver[2]);   
}