#include "bluetooth.h"
bool BlueTooth::powerOn(void)
{
	bool ret_val = false;
    gsm.SimpleWriteln("AT+BTPOWER=1");
	gsm.WaitResp(1000, 500, "OK");
    if(gsm.IsStringReceived("OK"))
        ret_val = true;
    else if(gsm.IsStringReceived("ERROR"))
        ret_val = false;
		  
    return ret_val;
}

bool BlueTooth::powerOff(void)
{
	bool ret_val = false;
    gsm.SimpleWriteln("AT+BTPOWER=0");
	gsm.WaitResp(1000, 500, "OK");
    if(gsm.IsStringReceived("OK"))
        ret_val = true;
    else if(gsm.IsStringReceived("ERROR"))
        ret_val = false;
	  
    return ret_val;
}

int BlueTooth::getHostDeviceName(char* deviceName)
{
    char *s,*p;
    int i = 0;
	int ret_val = -1;
	gsm.SimpleWriteln("AT+BTHOST?");
    gsm.WaitResp(1000, 500, "OK");
	if (gsm.IsStringReceived("OK"))
		ret_val = 1;
	else
		return 0;
	if(gsm.IsStringReceived("+BTHOST:")) 
	{
		s = strchr((char *)gsm.comm_buf,':');
	
    if(NULL == (s = strchr((char *)gsm.comm_buf,':')))
    {
        return -1;
    }
    p = s + 9;/*+BTHOST: SIM800,33:7d:77:18:62:60*/
    while(*(p) != ',')
	{
        deviceName[i++] = *p;
        p++;
    }
    deviceName[i] = '\0';
	}
	
    return i;
}

int BlueTooth::scanForTargetDevice(char* deviceName)
{
	char *s;
	gsm.SimpleWriteln("AT+BTSCAN=1,20");
	/*gsm.RxInit(5000, 1500);*/
	if (RX_FINISHED_STR_RECV == gsm.WaitResp(10000, 10000, "+BTSCAN:"))
	{
		if(NULL == (s = strstr((char *)gsm.comm_buf,deviceName))) 
		{
			return 0;
		}
		else
		    targetDeviceID = atoi(s-3);
	}

	return targetDeviceID;
}

int BlueTooth::sendPairingRequestToDevice(int deviceID)
{
    if(0 == deviceID)                          
        return -1;

    gsm.SimpleWrite("AT+BTPAIR=0,");
	gsm.SimpleWrite(deviceID);
	gsm.SimpleWrite("\r\n");
	if (RX_FINISHED_STR_RECV == gsm.WaitResp(2000, 1000, "+BTPAIRING")) {
		gsm.SimpleWriteln("AT+BTPAIR=1,1");

	}
    return 0;
}

int BlueTooth::unPair(void)
{
	int ret_val = -1;
    if(0 == targetDeviceID)
        return -1;
	gsm.SimpleWrite("AT+BTUNPAIR=0");
	/*gsm.SimpleWrite(targetDeviceID);*/
	gsm.SimpleWrite("\r\n");
	gsm.WaitResp(1000, 500, "OK");
    if(gsm.IsStringReceived("OK"))
          ret_val = 1;
	else
		  ret_val = 0;

    return ret_val;
}

int BlueTooth::acceptPairing(void)
{   
    char *s = NULL;
	if (RX_FINISHED_STR_RECV == gsm.WaitResp(2000, 1000, "+BTPAIRING"))	
	{
		gsm.SimpleWriteln("AT+BTPAIR=1,1");
		Serial.println("+++pair ok+++");/*it indicates device pair ok*/
		if (RX_FINISHED_STR_RECV == gsm.WaitResp(10000, 10000, "+BTPAIR")) 
		{
		   if(NULL == (s = strstr((char *)gsm.comm_buf,":"))) 
		   {
			  return 0;
		   }
		   else
	          targetDeviceID = atoi(s+1);
	    }
	   return targetDeviceID;
	}
	else
	{
	   Serial.println("zlnonono");	/*it indicates that there is no device try to pair with SIM800*/
       return 0;
	}	   	
}
int BlueTooth::acceptConnect(void)
{
	int ret_val = -1;
	gsm.SimpleWriteln("AT+BTACPT=1");
	gsm.WaitResp(1000, 500, "OK");
	
	if(gsm.IsStringReceived("OK"))
        ret_val = 1;
    else 
        ret_val = 0;
	
    return ret_val;
}
int BlueTooth::disconnect(int deviceID)
{
	int ret_val = -1;
    if(0 == targetDeviceID)
        return -1;
	gsm.SimpleWrite("AT+BTDISCONN=");
	gsm.SimpleWrite(targetDeviceID);
	gsm.SimpleWrite("\r\n");
	gsm.WaitResp(1000, 500, "OK");
    if(gsm.IsStringReceived("OK"))
        ret_val = 1;
	else
		ret_val = 0;

    return ret_val;
}
/*
int BlueTooth::loopHandle(void)
{
    char gprsBuffer[100];
    cleanBuffer(gprsBuffer,100);
    while(1) {
        if(serialSIM800.available())
	    {
           break;
        }
        delay(1000);
    }
    readBuffer(gprsBuffer,100,DEFAULT_TIMEOUT);
    
    if(NULL != strstr(gprsBuffer,"+BTPAIRING:"))
	{
        if(0 != acceptPairing())
		{
            return -1;
            ERROR("\r\nERROR:bluetoothAcceptPairing\r\n");
        }
    }
	if((NULL != strstr(gprsBuffer,"+BTCONNECTING:")) && (NULL != strstr(gprsBuffer,"SPP")))
	{
        if(0 != acceptConnect())
		{
            return -1;
            ERROR("\r\nERROR:bluetoothAcceptConnecting\r\n");
        }
    }
    return 0;
}
*/

/*int BlueTooth::getDeviceId(void)
{  
    return 1;
}*/

int BlueTooth::connectInSPP(int deviceID) /*Serial Port Profile*/
{
	int ret_val = 1;
    if(0 == targetDeviceID)
        return -1;
	
	/*gsm.SimpleWriteln("AT+BTGETPROF);*/
	gsm.SimpleWrite("AT+BTCONNECT=");
	gsm.SimpleWrite(targetDeviceID);
	gsm.SimpleWrite(",4\r\n");
	Serial.println("***connetc SPP***"); /*it means connect to SPP*/ 
	if (RX_FINISHED_STR_RECV == gsm.WaitResp(10000, 500, "OK")) 
	{
	    return 1;
	}
    /*gsm.SetCommLineStatus(CLS_FREE); */ 
	return ret_val;	
}

int BlueTooth::recvInSPP(char* data)
{   
    char * p = NULL;
    int count = 0;
    if (RX_FINISHED_STR_RECV == gsm.WaitResp(1000, 500, "+BTSPPDATA:")) 
	{
		p = strchr((char *)gsm.comm_buf,':');
		if(NULL != p)
		{
            p += 5; 
            int i = 0;
            while(*(p++) != 0x0D)
		    {
                data[i++] = *p;
            }
		    data[i]='+';		
		    return 1;
	   }
	   else   
		    return 0;
	}
	else 
	   return 0;
}

int BlueTooth::sendInSPP(char* data)
{
	int ret_val = -1;
	char end[2];
    end[0]=0x1a;
    end[1]='\0';
	gsm.SimpleWriteln("AT+BTSPPSEND");
	
	if (RX_FINISHED_STR_RECV == gsm.WaitResp(1000, 500, ">")) 
	{
		gsm.SimpleWrite(data);
		gsm.SimpleWriteln(end);
		if (RX_FINISHED_STR_RECV == gsm.WaitResp(2000, 2000, "SEND OK"))
		ret_val = 1;	
	}
	else
	    ret_val = 0;
		
	return ret_val;	
}
