/********************************************************************************
  File Information:
    FileName:       usb_function_phdc.c
    Dependencies:   See INCLUDES section
    Processor:      PIC18 or PIC24 USB Microcontrollers
    Hardware:       The code is natively intended to be used on the following
                    hardware platforms: PICDEM™ FS USB Demo Board,
                    PIC18F87J50 FS USB Plug-In Module, or
                    Explorer 16 + PIC24 USB PIM.  The firmware may be
                    modified for use on other USB platforms by editing the
                    HardwareProfile.h file.
    Complier:   Microchip C18 (for PIC18) or C30 (for PIC24)
    Company:        Microchip Technology, Inc.

    Software License Agreement:

    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
    software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.

    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
    
  Summary:
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the PHDC function
    driver. This file should be included in projects that use the PHDC
    \function driver.
    
    
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB\\PHDC
    Device Driver" directory.
  Description:
    USB PHDC Function Driver File
    
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the PHDC function
    driver. This file should be included in projects that use the PHDC
    \function driver.
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB\\PHDC
    Device Driver" directory.
    
    When including this file in a new project, this file can either be
    referenced from the directory in which it was installed or copied
    directly into the user application folder. If the first method is
    chosen to keep the file located in the folder in which it is installed
    then include paths need to be added so that the library and the
    application both know where to reference each others files. If the
    application folder is located in the same folder as the Microchip
    folder (like the current demo folders), then the following include
    paths need to be added to the application's project:
    
    ..\\..\\Include
    
    .
        
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application                                 
  ********************************************************************************/

/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  1.0   Initial release

********************************************************************/

/** I N C L U D E S **********************************************************/
#include "USB/usb.h"
#include "USB/usb_function_phdc.h"
#include "HardwareProfile.h"


#ifdef USB_USE_PHDC

/** V A R I A B L E S ********************************************************/
#if defined(__18CXX)
    #pragma udata

    //The phdc_data_rx[] and phdc_data_tx[] arrays are used as
    //USB packet buffers in this firmware.  Therefore, they must be located in
    //a USB module accessible portion of microcontroller RAM.
    #if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
        #pragma udata usbram2
    #elif defined(__18F2455) || defined(__18F2550) || defined(__18F4455) || defined(__18F4550)\
        || defined(__18F2458) || defined(__18F2553) || defined(__18F4458) || defined(__18F4553)\
        || defined(__18LF24K50) || defined(__18F24K50) || defined(__18LF25K50)\
        || defined(__18F25K50) || defined(__18LF45K50) || defined(__18F45K50)
        #pragma udata USB_VARIABLES=0x500
    #elif defined(__18F4450) || defined(__18F2450)
        #pragma udata USB_VARIABLES=0x480
    #else
        #pragma udata
    #endif
#endif

volatile FAR unsigned char phdc_data_rx[PHDC_DATA_OUT_EP_SIZE];
volatile FAR unsigned char phdc_data_tx[PHDC_DATA_IN_EP_SIZE];

#if defined(__18CXX)
    #pragma udata
#endif 

POINTER pPHDCSrc;            // Dedicated source pointer
POINTER pPHDCDst;            // Dedicated destination pointer


UINT16 TransferSize;
USB_PHDC_CB PhdAppCB;

//End point structures
PHDC_RX_ENDPOINT PhdcRXEP[PHDC_RX_ENDPOINTS];
PHDC_TX_ENDPOINT PhdcTXEP[PHDC_TX_ENDPOINTS];

UINT16 phdcEpDataBitmap;
extern BYTE_VAL *pDst;


/** P R I V A T E  P R O T O T Y P E S ***************************************/


/** D E C L A R A T I O N S **************************************************/
//#pragma code

/** C L A S S  S P E C I F I C  R E Q ****************************************/
/******************************************************************************
 	Function:
 		void USBDevicePHDCCheckRequest(void)
 
 	Description:
 		This routine checks the setup data packet to see if it
 		knows how to handle it
 		
 	PreCondition:
 		None

	Parameters:
		None
		
	Return Values:
		None
		
	Remarks:
		None
		 
  *****************************************************************************/
void USBDevicePHDCCheckRequest(void)
{
    /*
     * If request recipient is not an interface then return
     */
    if(SetupPkt.Recipient != USB_SETUP_RECIPIENT_INTERFACE_BITFIELD) return;

    /*
     * If request type is not class-specific then return
     */
    if(SetupPkt.RequestType != USB_SETUP_TYPE_CLASS_BITFIELD) return;

    /*
     * Interface ID must match interface numbers associated with
     * PHDC class, else return
     */
    if(SetupPkt.bIntfID != PHDC_INTF_ID)return;
    
    switch(SetupPkt.bRequest)
    {
        //****** These commands are required ******//
        case GET_DATA_STATUS :
            if(SetupPkt.wValue == 0)
            {
                CtrlTrfData[0] = (UINT8 )phdcEpDataBitmap; 
                CtrlTrfData[1] = (UINT8) phdcEpDataBitmap >> 8;
                USBEP0SendRAMPtr((BYTE*)CtrlTrfData, 2, USB_EP0_NO_OPTIONS);
            }
            
        break; 
        default:
            break;
    }//end switch(SetupPkt.bRequest)

}//end USBDevicePHDCCheckRequest

/** U S E R  A P I ***********************************************************/

/**************************************************************************
  Function:
        void USBDevicePHDCInit(USB_PHDC_CB callback)
    
  Summary:
    This function initializes the PHDC function driver. This function should
    be called during the init process.
  Description:
    This function initializes the PHDC function driver. This function initializes
   the end point data structures. A call back function from the upper layer is also
	registered.
    
       
    Typical Usage:
    <code>
     void PHDCAppInit(PHDC_APP_CB callback)
	{
		AppCB = callback;
		USBDevicePHDCInit(UsbToPHDComCB); //register the call back function
	}
    </code>
  Conditions:
    None
  Remarks:
    None                                                                   
  **************************************************************************/
void USBDevicePHDCInit(USB_PHDC_CB callback)
{    
	PhdcRXEP[0].ep_num=PHDC_DATA_EP;
	PhdcRXEP[0].offset =0;
	PhdcRXEP[0].qos = PHDC_BULK_OUT_QOS;	
	PhdcRXEP[0].transfer_size =0;
	PhdcRXEP[0].transfer_size =0; 
	PhdcRXEP[0].size = PHDC_DATA_OUT_EP_SIZE;
	
	PhdcTXEP[0].ep_num=PHDC_DATA_EP;
	PhdcTXEP[0].offset =0;
	PhdcTXEP[0].qos = PHDC_BULK_IN_QOS;	
	PhdcTXEP[0].transfer_size =0;
	PhdcTXEP[0].bytes_to_send = 0; 
	PhdcTXEP[0].size =PHDC_DATA_IN_EP_SIZE;

    #if defined USE_PHDC_INTERRUPT_ENDPOINT
		PhdcTXEP[1].ep_num=PHDC_INT_EP;
		PhdcTXEP[1].offset =0;
		PhdcTXEP[1].qos = PHDC_INT_IN_QOS;	
		PhdcTXEP[1].transfer_size =0;
		PhdcTXEP[1].bytes_to_send = 0; 
		PhdcTXEP[1].size =PHDC_INT_IN_EP_SIZE;
	#endif 

	PhdAppCB = callback;
	phdcEpDataBitmap = 0;
	  
}//end PHDCInitEP

/**********************************************************************************
  Function:
        UINT8 USBDevicePHDCReceiveData(UINT8 qos, UINT8 *buffer, UINT16 len)
    
  Summary:
    USBDevicePHDCReceiveData copies a string of BYTEs received through USB PHDC Bulk OUT
    endpoint to a user's specified location. It is a non-blocking function.
    It does not wait for data if there is no data available. Instead it
    returns '0' to notify the caller that there is no data available.

  Description:
    USBDevicePHDCReceiveData copies a string of BYTEs received through USB PHDC Bulk OUT
    endpoint to a user's specified location. It is a non-blocking function.
    It does not wait for data if there is no data available. Instead it
    returns '0' to notify the caller that there is no data available.
    
    Typical Usage:
    <code>
     
    </code>
  Conditions:
    Value of input argument 'len' should be smaller than the maximum
    endpoint size responsible for receiving bulk data from USB host for PHDC
    class. Input argument 'buffer' should point to a buffer area that is
    bigger or equal to the size specified by 'len'.
  Input:
	qos - quality of service
    buffer -  Pointer to where received BYTEs are to be stored
    len -     The number of BYTEs expected.
                                                                                   
  **********************************************************************************/
UINT8 USBDevicePHDCReceiveData(UINT8 qos, UINT8* buffer, UINT16 len)
{	
 	PTR_PHDC_RX_ENDPOINT recv_endpoint;
	UINT8 index;
	UINT8 phdc_rx_len = 0;
    
	for(index=0; index< PHDC_RX_ENDPOINTS;index++)
	{
		if((PhdcRXEP[index].qos & qos) != 0)
            break;
	}

	if(index == PHDC_RX_ENDPOINTS)
	{
		return 0; //no endpoint supports the qos
	}
		
	recv_endpoint = &PhdcRXEP[index];

	recv_endpoint->app_buff = (UINT8*)buffer;
	recv_endpoint->qos = qos;
	recv_endpoint->len = len;

	recv_endpoint->PHDCDataOutHandle = USBRxOnePacket(recv_endpoint->ep_num,
										   (UINT8*) phdc_data_rx,
										   recv_endpoint->size);
   
    return phdc_rx_len;
    
}//end getsUSBUSART

/******************************************************************************
  Function:
	void USBDevicePHDCSendData(UINT8 qos, UINT8 *data, UINT8 Length)
		
  Summary:
    USBDevicePHDCSendData writes an array of data to the USB. 

  Description:
    USBDevicePHDCSendData writes an array of data to the USB.
    
    Typical Usage:
    <code>
       
    </code>
    
    The transfer mechanism for device-to-host(put) is more flexible than
    host-to-device(get). It can handle a string of data larger than the
    maximum size of bulk IN endpoint. A state machine is used to transfer a
    \long string of data over multiple USB transactions. USBDevicePHDCTxRXService(PTR_USTAT_STRUCT val)
    will be called on a transfer event to keep sending blocks of data to the host.

  Conditions:
    None

  Input:
	qos - Quality of service information
    *data - pointer to a RAM/ROM array of data to be transfered to the host
    length - the number of bytes to be transfered.
	memtype - Indicates whether the data array is in ROM or RAM
		
 *****************************************************************************/
void USBDevicePHDCSendData(UINT8 qos, UINT8 *data, UINT16 length,BOOL memtype)
{
    PTR_PHDC_TX_ENDPOINT tx_endpoint;
    UINT8 index;
    UINT8 i;

    for(index=0; index< PHDC_TX_ENDPOINTS;index++)
    {
        if((PhdcTXEP[index].qos & qos) != 0)
        break;
    }
    if(index == PHDC_TX_ENDPOINTS)
    {
        return; //no endpoint supports the qos
    }
    tx_endpoint = &PhdcTXEP[index];
		
    if(	(tx_endpoint->transfer_size ==0) && (tx_endpoint->offset ==0) ) //new packet
    {
	tx_endpoint->transfer_size = length;
	tx_endpoint->memtype= memtype;
	if(tx_endpoint->transfer_size > tx_endpoint->size)
	{	
        	tx_endpoint->bytes_to_send = tx_endpoint->size; //multiple send
	}
	else
	{
	    tx_endpoint->bytes_to_send = tx_endpoint->transfer_size; //only packet to send
	}		
    }
    else
    {
        return; //send in progress
    }
    USBMaskInterrupts();	
    tx_endpoint->app_buff = (UINT8*)data;
    i= tx_endpoint->bytes_to_send;
    pPHDCDst.bRam = (BYTE*)phdc_data_tx; // Set destination pointer
    if(tx_endpoint->memtype == MEM_ROM)            // Determine type of memory source
    {
        pPHDCSrc.bRom = (ROM UINT8 *) data;
        while(i)
        {
            *pPHDCDst.bRam = *pPHDCSrc.bRom;
             pPHDCDst.bRam++;
             pPHDCSrc.bRom++;
             i--;
        }//end while(byte_to_send)		
    }
    else // _RAM
    {
	pPHDCSrc.bRam = (UINT8*)data;
	while(i)
        {
            *pPHDCDst.bRam = *pPHDCSrc.bRam;
             pPHDCDst.bRam++;
             pPHDCSrc.bRam++;
             i--;
        }//end while(byte_to_send._word)	
    }//end if(phdc_mem_type...)
           
    tx_endpoint->PHDCDataInHandle = USBTxOnePacket(tx_endpoint->ep_num,
	(BYTE*)phdc_data_tx,tx_endpoint->bytes_to_send);
    USBDevicePHDCUpdateStatus(tx_endpoint->ep_num, 1); // update the endpoint status. '1' if endpoint has data. '0' otherwise. 
	USBUnmaskInterrupts();
}//end USBDevicePHDCSendData


/************************************************************************
  Function:
        void USBDevicePHDCTxRXService(PTR_USTAT_STRUCT val)
    
  Summary:
    USBDevicePHDCTxRXService handles device-to-host transaction(s) and host-to-device transaction(s).
	This function is automatically called when a transfer event occurs.Device should be in configured state.
  
Description:
    USBDevicePHDCTxRXService handles device-to-host transaction(s) and host-to-device transaction(s).
	This function is automatically called when a transfer event occurs.Device should be in configured state.
    
    Typical Usage:
    <code>
    BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER:
            Nop();
			USBDevicePHDCTxRXService((PTR_USTAT_STRUCT)pdata);
            break;
        default:
            break;
    }      
    return TRUE; 
}
    </code>
  Conditions:
    None
  Remarks:
    None                                                                 
  ************************************************************************/
void USBDevicePHDCTxRXService(USTAT_FIELDS* pdata)
{
	
    UINT8 i;
	PTR_PHDC_TX_ENDPOINT tx_endpoint;
	PTR_PHDC_RX_ENDPOINT recv_endpoint;
	UINT8 index;
	USTAT_FIELDS val;
	val = *pdata;

    if(USBHALGetLastDirection(val) == DIR_IN)
	{   
        /* Find which Endpoint needs to send data based on the qos value  */
        for(index = 0; index < PHDC_TX_ENDPOINTS; index++)
        {
            if(PhdcTXEP[index].ep_num == USBHALGetLastEndpoint(val))
            break;
        }
        
		tx_endpoint = &PhdcTXEP[index];
		
        USBDevicePHDCUpdateStatus(tx_endpoint->ep_num, 0);
        
		tx_endpoint->offset += USBHandleGetLength(tx_endpoint->PHDCDataInHandle);
		if((tx_endpoint->offset == tx_endpoint->transfer_size)&& 
				(USBHandleGetLength(tx_endpoint->PHDCDataInHandle) !=0)) //send zero lenght packet
		{
			if(tx_endpoint->transfer_size % tx_endpoint->size == 0)
			{
				tx_endpoint->PHDCDataInHandle = USBTxOnePacket(tx_endpoint->ep_num,NULL,0);
				USBDevicePHDCUpdateStatus(tx_endpoint->ep_num, 1); 
				return;	
			}
		}
        
        //data sent. Also ZLP
		if(tx_endpoint->offset >= tx_endpoint->transfer_size)
		{
			tx_endpoint->transfer_size = 0;
			tx_endpoint->offset = 0;
			PhdAppCB(USB_APP_SEND_COMPLETE,NULL);
			return;
		}

	    if(tx_endpoint->offset < tx_endpoint->transfer_size)
		{
			tx_endpoint->bytes_to_send = tx_endpoint->transfer_size - tx_endpoint->offset;
			if(tx_endpoint->bytes_to_send  > tx_endpoint->size)
			{
				tx_endpoint->bytes_to_send = tx_endpoint->size; //multiple send
			}
			
		}

		USBMaskInterrupts();
		
		i= tx_endpoint->bytes_to_send;
        pPHDCDst.bRam = (BYTE*)phdc_data_tx; // Set destination pointer		

        if(tx_endpoint->memtype == MEM_ROM)            // Determine type of memory source
        {
			pPHDCSrc.bRom = (ROM UINT8*)(tx_endpoint->app_buff + tx_endpoint->offset);
			
            while(i)
            {
                *pPHDCDst.bRam = *pPHDCSrc.bRom;
                pPHDCDst.bRam++;
                pPHDCSrc.bRom++;
                i--;
            }//end while(byte_to_send)
		
        }
        else // _RAM
        {
			pPHDCSrc.bRam = (UINT8*)(tx_endpoint->app_buff + tx_endpoint->offset);
            while(i)
            {
                *pPHDCDst.bRam = *pPHDCSrc.bRam;
                pPHDCDst.bRam++;
                pPHDCSrc.bRam++;
                i--;
            }//end while(byte_to_send._word)
		
        }//end if(phdc_mem_type...)
           
        tx_endpoint->PHDCDataInHandle = USBTxOnePacket(tx_endpoint->ep_num,
																(UINT8*)phdc_data_tx,
																tx_endpoint->bytes_to_send);
	    USBDevicePHDCUpdateStatus(tx_endpoint->ep_num, 1); 
		USBUnmaskInterrupts();		
	}
	
	else //DIR_OUT
	{
		
		for(index = 0; index < PHDC_RX_ENDPOINTS; index++)
        {
            if(PhdcRXEP[index].ep_num == USBHALGetLastEndpoint(val))
            break;
        }
		recv_endpoint = &PhdcRXEP[index];
		recv_endpoint->len = USBHandleGetLength(recv_endpoint->PHDCDataOutHandle);
		if(recv_endpoint->len!=0)
		{
			if(recv_endpoint->transfer_size == 0) //first packet
			{
				 recv_endpoint->offset =0;
				 memcpy(recv_endpoint->app_buff,(void *)phdc_data_rx,recv_endpoint->len );
			     PhdAppCB(USB_APP_GET_TRANSFER_SIZE,&TransferSize);
				 recv_endpoint->transfer_size = TransferSize;			
			}
			else if(recv_endpoint->transfer_size > PHDC_MAX_APDU_SIZE)
			{
    			// do not copy the data to appbuffer if transfer size is more than maximum APDU size specified by the user. 
    			recv_endpoint->offset = 0; 
            } 			
			else
			{
				/* copy the data starting from the previuos offset*/
				memcpy((void *)(recv_endpoint->app_buff + recv_endpoint->offset), (void *)phdc_data_rx,recv_endpoint->len );
			}
		}
		/* move the offset */
		recv_endpoint->offset +=recv_endpoint->len;

		if((recv_endpoint->offset >= recv_endpoint->transfer_size) ||
				((recv_endpoint->len % recv_endpoint->size!=0)))
		{
		    if(((recv_endpoint->transfer_size % recv_endpoint->size)==0)&& 
					(recv_endpoint->len!=0))
			{
				//zero length packet receive here
				USBRxOnePacket(recv_endpoint->ep_num,NULL,0);
		    }
			recv_endpoint->offset =0;	
			recv_endpoint->transfer_size =0;
			PhdAppCB(USB_APP_DATA_RECEIVED,NULL);/*inform the application*/
			USBUnmaskInterrupts();
			return;
		}
		 		
	
		USBRxOnePacket(recv_endpoint->ep_num,(BYTE *)phdc_data_rx,
								(recv_endpoint->transfer_size-recv_endpoint->offset));
                
	        									 
	}
	USBUnmaskInterrupts();
}

  
/******************************************************************************
 	Function:
 		void USBDevicePHDCUpdateStatus (WORD EndpointNo, BIT Status)
 
 	Description:
 		This routine handles the Get Data Status request received from the PHDC Host. 
 		
 	PreCondition:
 		None

	Parameters:
		None
		
	Return Values:
		None
		
	Remarks:
		None
		 
 *****************************************************************************/
void USBDevicePHDCUpdateStatus (WORD EndpointNo, BIT Status)
{
    if (Status == 1)
        phdcEpDataBitmap |= 1 << EndpointNo ;
    else 
        phdcEpDataBitmap &= ~(1 << EndpointNo);     
}    


#endif //USB_USE_PHDC

/** EOF usb_function_phdc.c ****************************************************************/
