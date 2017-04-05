#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "SIM900.h"

/** 
 *  BlueTooth class. 
 *
 *  used to communicate with BlueTooth Module. 
 */
class BlueTooth
{
public:

    /** 
	 *  Power on BlueTooth module. 
	 *
     *  @retval true - success. 
     *  @retval false - failure.    
     *      
     */
    bool powerOn(void);
    
    /** 
	 *  Power off BlueTooth module. 
     *  
     *  @retval true - success.  
     *  @retval false - failure.   
	 *
     */
    bool powerOff(void);
    
    /** 
	 *  Get host device name of BlueTooth module.
     *	 
     *  @param  deviceName - buffer array to save device name.
     *  @retval 1 - success. 
     *  @retval <=0 - failure. 
     *      
     */
    int getHostDeviceName(char* deviceName);
    
    /** 
	 *  Scan for target device according to device name. 
	 *
     *  @param  deviceName - device which will be scanned for. 
	 *  @retval >0 - success.  
	 *  @retval <=0 - failure.
     *	@return targetDeviceID.
     *
     */
    int scanForTargetDevice(char* deviceName);
    
    /** 
	 *  Send pairing request to device according to deviceID. 
	 *
     *  @param  deviceID - device ID. 
	 *  @retval 0 - success. 
     *  @retval -1 - failure. 	 
     *
     */
    int sendPairingRequestToDevice(int deviceID);
    
    /** 
	 *  Accept other BlueTooth module's pairing request. 
	 *
     *  @retval >0 - success. 
     *  @retval <=0 - failure. 
	 *  @return targetDeviceID. 
	 *
     */
    int acceptPairing(void);
    
    /** 
	 *  UnPair with paired BlueTooth device.
     *	 
     *  @retval 1 - success. 
	 *  @retval <=0 - failure. 
     *
     */
    int unPair(void);
    
    /** 
	 *  Accept other BlueTooth device's connecting request. 
     * 
	 *  @retval 1 - success. 
	 *  @retval <=0 - failure. 
     *
     */
    int acceptConnect(void);
    
    /** 
	 *  Wait to handle other BlueTooth device's pairing or connecting  request. 
     *  
	 *  @retval 0 - success. 
	 *	@retval -1 - failure. 
     *
     */
    int loopHandle(void);
    
    /** 
	 *  Disconnect with connected BlueTooth device. 
     *
     *	@param  deviceID - device that will be disconnected. 
	 *  @retval 1 - success. 
     *  @retval <=0 - failure. 
     *
     */
    int disconnect(int deviceID);
    
    /** 
	 *  Connect with other BlueTooth device in SPP profile. 
     *  
	 *  @param  deviceID - device that will be connect in SPP profile.
     *  @retval 1 - success. 
     *  @retval <=0 - failure. 		
     * 
     */
    int connectInSPP(int deviceID);
    
    /** 
	 *  Receive data in SPP profile. 
	 *
     *  @param  data - buffer array to receive data from other BlueTooth device in SPP profile. 
     *  @retval 1 - success. 
	 *  @retval 0 - failure. 
 	 *
     */
    int recvInSPP(char* data);
    
    /** 
	 *  Send data in SPP profile. 
	 *
     *  @param  data - buffer array to send data to other BlueTooth device in SPP profile. 
	 *  @reval 1 - success. 
	 *  @retal <=0 - failure. 
     *
     */
    int sendInSPP(char* data);

private:
    
    /**
     *	BlueTooth power flag
     */
    bool bluetoothPower;
    
    /**
	 *  target device ID
     */
    int targetDeviceID;
};

#endif