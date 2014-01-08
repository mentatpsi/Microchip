/******************************************************************************
  File Information:
          FileName:        usb_function_ccid.c
          Dependencies:    See INCLUDES section below
          Processor:       PIC18, PIC24, or PIC32
          Compiler:        C18, C30, or C32
          Company:         Microchip Technology, Inc.
    
          Software License Agreement
    
          The software supplied herewith by Microchip Technology Incorporated
          (the “Company”) for its PICmicro® Microcontroller is intended and
          supplied to you, the Company’s customer, for use solely and
          exclusively on Microchip PICmicro Microcontroller products. The
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
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for use of CCID class function
    drivers. This file should be included in projects that use CCID class
    \function drivers. 
    
    
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB\\CCID Device Driver" directory.
  
    Description:
    USB CCID Class Driver Header File
    
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for use of CCID class function
    drivers. This file should be included in projects that use CCID class
    \function drivers.
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB\\CCID
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
    
    .
    
    ..\\..\\Microchip\\Include
    
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application                               
  ******************************************************************************/

/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  2.7    Initial revision

  2.7a   No change

********************************************************************/

/** I N C L U D E S **********************************************************/
#include "USB/usb.h"
#include "USB/usb_function_ccid.h"
#if defined(USB_USE_CCID)

/** V A R I A B L E S ********************************************************/
BYTE usbCcidBulkInTrfState;
WORD usbCcidBulkInLen;
POINTER pCCIDDst;            // Dedicated destination pointer
POINTER pCCIDSrc;            // Dedicated source pointer


/** P R I V A T E  P R O T O T Y P E S ***************************************/
#if defined USB_CCID_SUPPORT_ABORT_REQUEST
    void USB_CCID_ABORT_REQUEST_HANDLER(void);
#endif 

#if defined USB_CCID_SUPPORT_GET_CLOCK_FREQUENCIES_REQUEST
    void USB_CCID_GET_CLOCK_FREQUENCIES_REQUEST_HANDLER(void);
#endif 

#if defined USB_CCID_SUPPORT_GET_DATA_RATES_REQUEST
    void USB_CCID_GET_DATA_RATES_REQUEST_HANDLER(void);
#endif 



/** D E C L A R A T I O N S **************************************************/

/** C L A S S  S P E C I F I C  R E Q ****************************************/
/******************************************************************************
 	Function:
 		void USBCheckCCIDRequest(void)
 
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
  void USBCheckCCIDRequest(void)    
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
       * Interface ID must match interface number associated with
       * CCID class, else return
       */
      if(SetupPkt.bIntfID != USB_CCID_INTERFACE_ID)
          return;
      switch (SetupPkt.bRequest)// checking for the request ID
      {
          #if defined (USB_CCID_SUPPORT_ABORT_REQUEST)
          case USB_CCID_ABORT:
              USB_CCID_ABORT_REQUEST_HANDLER();
              break;
          #endif 
          
          #if defined (USB_CCID_SUPPORT_GET_CLOCK_FREQUENCIES_REQUEST)
          case USB_CCID_GET_CLOCK_FREQUENCIES:
              USB_CCID_GET_CLOCK_FREQUENCIES_REQUEST_HANDLER();
              break;
          #endif 
          
          #if defined (USB_CCID_SUPPORT_GET_DATA_RATES_REQUEST)
          case USB_CCID_GET_DATA_RATES:
              USB_CCID_GET_DATA_RATES_REQUEST_HANDLER();
              break;
          #endif 
          default:
              break;
      }//end switch(SetupPkt.bRequest)
}//end USBCheckCCIDRequest


/**************************************************************************
  Function:
        void USBCCIDInitEP(void)
    
  Summary:
    This function initializes the CCID function driver. This function should
    be called after the SET_CONFIGURATION command.
  Description:
    This function initializes the CCID function driver. This function sets
    the default line coding (baud rate, bit parity, number of data bits,
    and format). This function also enables the endpoints and prepares for
    the first transfer from the host.
    
    This function should be called after the SET_CONFIGURATION command.
    This is most simply done by calling this function from the
    USBCBInitEP() function.
    
    Typical Usage:
    <code>
        void USBCBInitEP(void)
        {
            USBCCIDInitEP();
        }
    </code>
  Conditions:
    None
  Remarks:
    None                                                                   
  **************************************************************************/
 void USBCCIDInitEP(void)
 {
   
    usbCcidBulkInTrfState = USB_CCID_BULK_IN_READY;
    usbCcidBulkInLen =0;
    
    /*
     * Do not have to init Cnt of IN pipes here.
     * Reason:  Number of BYTEs to send to the host
     *          varies from one transaction to
     *          another. Cnt should equal the exact
     *          number of BYTEs to transmit for
     *          a given IN transaction.
     *          This number of BYTEs will only
     *          be known right before the data is
     *          sent.
     */
     
    USBEnableEndpoint(USB_EP_INT_IN,USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    USBEnableEndpoint(USB_EP_BULK_IN,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    usbCcidBulkInHandle = 0;
	usbCcidInterruptInHandle = 0;
    usbCcidBulkOutHandle = USBRxOnePacket(USB_EP_BULK_OUT,(BYTE*)&usbCcidBulkOutEndpoint,USB_EP_SIZE);
    
}//end CCIDInitEP


/************************************************************************
  Function:
        void USBCCIDBulkInService(void)
    
  Summary:
    USBCCIDBulkInService handles device-to-host transaction(s). This function
    should be called once per Main Program loop after the device reaches
    the configured state.
  Description:
    USBCCIDBulkInService handles device-to-host transaction(s). This function
    should be called once per Main Program loop after the device reaches
    the configured state.
    
    Typical Usage:
    <code>
    void main(void)
    {
        USBDeviceInit();
        while(1)
        {
            USBDeviceTasks();
            if((USBGetDeviceState() \< CONFIGURED_STATE) ||
               (USBIsDeviceSuspended() == TRUE))
            {
                //Either the device is not configured or we are suspended
                //  so we don't want to do execute any application code
                continue;   //go back to the top of the while loop
            }
            else
            {
                //Run application code.
                UserApplication();

				//Keep trying to send data to the PC as required
                USBCCIDBulkInService();
            }
        }
    }
    </code>
  Conditions:
    None
  Remarks:
    None                                                                 
  ************************************************************************/
 
void USBCCIDBulkInService(void)
{
    WORD byte_to_send;
    BYTE i;

    USBMaskInterrupts();
    if(USBHandleBusy(usbCcidBulkInHandle)) 
    {
        USBUnmaskInterrupts();
        return;
    }


    if(usbCcidBulkInTrfState == USB_CCID_BULK_IN_COMPLETING)
        usbCcidBulkInTrfState = USB_CCID_BULK_IN_READY;
    
    /*
     * If USB_CCID_BULK_IN_READY state, nothing to do, just return.
     */
    if(usbCcidBulkInTrfState == USB_CCID_BULK_IN_READY)
    {
        USBUnmaskInterrupts();
        return;
    }
    
    /*
     * If USB_CCID_BULK_IN_BUSY_ZLP state, send zero length packet
     */
    if(usbCcidBulkInTrfState == USB_CCID_BULK_IN_BUSY_ZLP)
    {
        usbCcidBulkInHandle = USBTxOnePacket(USB_EP_BULK_IN,NULL,0);
        usbCcidBulkInTrfState = USB_CCID_BULK_IN_COMPLETING;
    }
    else if(usbCcidBulkInTrfState == USB_CCID_BULK_IN_BUSY) 
    {
        /*
         * First, have to figure out how many byte of data to send.
         */
    	if(usbCcidBulkInLen > sizeof(usbCcidBulkInEndpoint)) 
    	    byte_to_send = sizeof(usbCcidBulkInEndpoint);
    	else
    	    byte_to_send = usbCcidBulkInLen;
            
        /*
         * Subtract the number of bytes just about to be sent from the total.
         */
    	usbCcidBulkInLen = usbCcidBulkInLen - byte_to_send;
    	  
        pCCIDDst.bRam = (BYTE*)usbCcidBulkInEndpoint; // Set destination pointer
        
        i = byte_to_send;
       
        while(i)
        {
            *pCCIDDst.bRam = *pCCIDSrc.bRam;
            pCCIDDst.bRam++;
            pCCIDSrc.bRam++;
            i--;
        }//end while(byte_to_send._word)
 
        
        /*
         * Lastly, determine if a zero length packet state is necessary.
         * See explanation in USB Specification 2.0: Section 5.8.3
         */
        if(usbCcidBulkInLen == 0)
        {
            if(byte_to_send == USB_EP_SIZE)
                usbCcidBulkInTrfState = USB_CCID_BULK_IN_BUSY_ZLP;
            else
                usbCcidBulkInTrfState = USB_CCID_BULK_IN_COMPLETING;
        }//end if(usbCcidBulkInLen...)
        usbCcidBulkInHandle = USBTxOnePacket(USB_EP_BULK_IN,(BYTE*)usbCcidBulkInEndpoint,byte_to_send);

    }//end if(cdc_tx_sate == USB_CCID_BULK_IN_BUSY)
    USBUnmaskInterrupts();
}//end USBCCIDBulkInService


/******************************************************************************
  Function:
	void USBCCIDSendDataToHost(BYTE *data, WORD length)
		
  Summary:
    USBCCIDSendDataToHost writes an array of data to the USB. Use this version, is
    capable of transfering 0x00 (what is typically a NULL character in any of
    the string transfer functions).

  Description:
    USBCCIDSendDataToHost writes an array of data to the USB. Use this version, is
    capable of transfering 0x00 (what is typically a NULL character in any of
    the string transfer functions).
    
    
    The transfer mechanism for device-to-host(put) is more flexible than
    host-to-device(get). It can handle a string of data larger than the
    maximum size of bulk IN endpoint. A state machine is used to transfer a
    \long string of data over multiple USB transactions. USBCCIDBulkInService()
    must be called periodically to keep sending blocks of data to the host.

  Conditions:
    

  Input:
    BYTE *data - pointer to a RAM array of data to be transfered to the host
    WORD length - the number of bytes to be transfered 
		
 *****************************************************************************/
void USBCCIDSendDataToHost(BYTE *pData, WORD len)
{

    mUSBCCIDBulkInRam((BYTE*)pData, len);

}


#endif //def USB_USE_CCID

/** EOF usb_function_ccid.c *************************************************************/
