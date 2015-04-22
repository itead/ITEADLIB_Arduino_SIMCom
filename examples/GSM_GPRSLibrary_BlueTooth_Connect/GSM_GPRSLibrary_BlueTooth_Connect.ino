#include "bluetooth.h"
#include <SoftwareSerial.h>
/*create a BlueTooth object*/ 
BlueTooth blue;
void setup()
{
    /*Serial connection.*/
    Serial.begin(9600);
    Serial.println("GSM Shield testing.");
     
    /*Start configuration of shield with baudrate.*/
    /*For http uses is raccomanded to use 4800 or slower.*/
    if (gsm.begin(9600))
    {
        Serial.println("\nstatus=READY");
    }
    else
        Serial.println("\nstatus=IDLE");
          
};

void loop()
{  
    if(blue.powerOff() == true)
        Serial.println("close ok");
    else
        Serial.println("close failed ");  
		
    if(blue.powerOn() == true)
       Serial.println("open ok ");
    else 
       Serial.println("open failed"); 
	   
    int bluetemp = 0;
    delay(2000);
    while((bluetemp=blue.acceptPairing()) == 0);   
    while(1);
};







