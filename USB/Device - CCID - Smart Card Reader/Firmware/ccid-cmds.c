/********************************************************************
 FileName:     	ccid-cmds.c
 Dependencies:	See INCLUDES section
 Processor:		Microchip USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board,
 				PIC18FXXJ50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM or PIC32 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) , C30 (for PIC24) or C32 (for PIC32) 
 Company:		Microchip Technology, Inc.

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

*********************************************************************/
#include "USB/usb.h"
#include "USB/usb_function_ccid.h"
#include "HardwareProfile.h"
#include "sc_config.h"
#include "./Smart Card/SClib.h"


/** VARIABLES ******************************************************/
#if defined(__18CXX)
    #pragma udata CCID
#endif  
   
USB_CCID_BUFFER  usbCcidApdu; 
USB_CCID_BUFFER* pUsbCcidApdu = &usbCcidApdu; 

#if defined(__18CXX)
    #pragma udata
#endif

UINT8  usbCcidOutPacketTrack=0;
UINT32_VAL usbCcidOutPacketLength;
INT32 usbCcidNoOfBytesToReceive;
UINT8 usbCcidProtocolNum; 
extern BYTE cgtETU;
extern BYTE maxSegmentLength; 

#if defined(__18CXX)
    #pragma udata
#endif

/** PRIVATE PROTOTYPES *********************************************/
void UsbCcidPcToRdrXfrBlock(void);
void UsbCcidCommandNotSupported(void);
void UsbCcidSendRdrToPcParameters (void); 

/******************************************************************************
    Function:
        void USB_CCID_SetParameters(void)

    Summary:
        This macro is used set the CCID parameters.

    Description:
        This macro is used set the CCID parameters as and when there is request from the host.

        Typical Usage:
        <code>
            USB_CCID_SetParameters();
        </code>

        This function is not used  for CCID devices that do not support PC_to_RDR_SetParameters Command.

  *****************************************************************************/
#define USB_CCID_SetParameters()    SC_DoPPS()

/******************************************************************************
    Function:
        void USB_CCID_ResetParameters(void)

    Summary:
        This macro is used Reset the CCID parameters.

    Description:
        This macro is used Reset the CCID parameters as and when there is request from the host.

        Typical Usage:
        <code>
            USB_CCID_ResetParameters();
        </code>

        This function is not used  for CCID devices that do not support PC_to_RDR_ResetParameters Command

 *****************************************************************************/
#define USB_CCID_ResetParameters()  SC_DoPPS()


/******************************************************************************
    Function:
        void USB_CCID_RestartClock(void)

    Summary:
        This macro is used to restart the Clock to the Smart Card

    Description:
        This macro is used to restart the Clock to the Smart Card as and when there is request from the host.

        Typical Usage:
        <code>
            USB_CCID_RestartClock();
        </code>

        This function is not used for CCID devices that do not support PC_to_RDR_IccClock  Command.

 *****************************************************************************/
#define USB_CCID_RestartClock()     SCdrv_EnableClock()


/******************************************************************************
    Function:
        void USB_CCID_StopClockWithPinHigh(void)
        void USB_CCID_StopClockWithPinLow (void)

    Summary:
        These macros are used to disbale the Clock to the Smart Card

    Description:
        These macros are used to disbale the Clock to the Smart Card as and when there is request from the host.

        Typical Usage:
        <code>
            USB_CCID_StopClockWithPinHigh();
        </code>

        These functions are not used for CCID devices that do not support PC_to_RDR_IccClock  Command.

 *****************************************************************************/
#define USB_CCID_StopClockWithPinHigh() SCdrv_DisableClock(); SCdrv_SetClockPinHigh()
#define USB_CCID_StopClockWithPinLow()  SCdrv_DisableClock(); SCdrv_SetClockPinLow()

BYTE ccid_clockstatus;


//CCID Smart Card IFD Slot Status Definition
union SLOT_STATUS
{
	struct {
		BYTE ICC_Status:2;	// 0 Card Present and Active, 1 Present and inactive, 2 Not Present, 3 Rsvd
		BYTE RFU:4;			// = 0
		BYTE CmdStatus:2;	// 0 Processed without error, 1 Failed (error code in error reg), 2 Time extension is requested
	};
	BYTE Val;
};


//////////////////////////////////////////////////////////////////
// bStatus - 0 Card Present and active,
//           1 Card Present and inactive,
//			 2 Not Present
//
BYTE GetCardStatus(void)
{
	if( !SC_CardPresent() )
		return 2;
	else if( SC_GetCardState() == SC_STATE_CARD_ACTIVE )
		return 0;
	else
		return 1;
}

//////////////////////////////////////////////////////////////////
void CCID_Init()
{
	//mInitAllSwitches();
    usbCcidProtocolNum = 0;
	//Start smart card stack
	SC_Initialize();

}

#define S3_PRESS	(!sw2)
#define S6_PRESS	(!sw3)
//////////////////////////////////////////////////////////////////
void ProcessCCID()
{
	static WORD CardPresentVerifyCounter = 0;
	static BOOL CardON = FALSE;
	BYTE i;
	BYTE ppsData[3]; 

	////// Detect Card Insertion and removal and transmit 	/////////////
	////// the status on interrupt endpoint 				/////////////

    if( !CardON && SC_CardPresent() )
    {
	    if( CardPresentVerifyCounter > 100 )
	    {
			if(!USBHandleBusy(usbCcidInterruptInHandle))
			{
		    	usbCcidBulkInEndpoint[0] = 0x50; //Msg Type
				usbCcidBulkInEndpoint[1] = 3;	//card present, change

		    	usbCcidInterruptInHandle = USBTxOnePacket(USB_EP_INT_IN, (BYTE*)&usbCcidBulkInEndpoint, 2);
		    	CardON = TRUE;
			}
		}
		else
			CardPresentVerifyCounter++;
	}
	else if( CardON && !SC_CardPresent() )
    {
	    SC_Shutdown();	//reset SC_Lib states and turn off hardware
		if(!USBHandleBusy(usbCcidInterruptInHandle))
		{
	    	usbCcidBulkInEndpoint[0] = 0x50; //Msg Type
			usbCcidBulkInEndpoint[1] = 2;	//card not present, change
	    	usbCcidInterruptInHandle = USBTxOnePacket(USB_EP_INT_IN, (BYTE*)&usbCcidBulkInEndpoint, 2);
	    	CardON = FALSE;
		}
	}
	else
		CardPresentVerifyCounter = 0;

	///// Process commands received on Bulk Endoint 		//////////////

    if(!USBHandleBusy(usbCcidBulkOutHandle))		//Check if the endpoint has received any data from the host.
    {
		union SLOT_STATUS card_st;
		BYTE ErrCode;
		static WORD TransactionCount=0;

		card_st.Val = 0;
		ErrCode     = 0; //clear error code
		
		
	    if(usbCcidOutPacketTrack == USB_CCID_BULK_OUT_FIRST_PACKET)
		{
            memset( pUsbCcidApdu->CCID_BulkOutBuffer, 0, sizeof(pUsbCcidApdu->CCID_BulkOutBuffer)) ;
            // copy the length from the CCID packet to a 32 Bit Variable.
	        usbCcidOutPacketLength.byte.LB = usbCcidBulkOutEndpoint[1];
	        usbCcidOutPacketLength.byte.HB = usbCcidBulkOutEndpoint[2];
	        usbCcidOutPacketLength.byte.UB = usbCcidBulkOutEndpoint[3];
	        usbCcidOutPacketLength.byte.MB = usbCcidBulkOutEndpoint[4];
	        usbCcidNoOfBytesToReceive = usbCcidOutPacketLength.Val + 10; // CCID command overhead is 10 bytes.
			
    	    for (i =0;i < USB_EP_SIZE;i++) // Copy received data from host to a temperary buffer
    	        pUsbCcidApdu->CCID_BulkOutBuffer[i] = usbCcidBulkOutEndpoint[i];

    	    if (usbCcidNoOfBytesToReceive > USB_EP_SIZE) // We still have more USB transactions to receive from host
    	    {
    	        usbCcidNoOfBytesToReceive = usbCcidNoOfBytesToReceive - USB_EP_SIZE;
    	        usbCcidOutPacketTrack = USB_CCID_BULK_OUT_SUBSEQUENT_PACKET;
    	        usbCcidBulkOutHandle = USBRxOnePacket(USB_EP_BULK_OUT,(BYTE*)&usbCcidBulkOutEndpoint,USB_EP_SIZE);
    	        return;
    	    }
    	    else // We have received everything from the host.
    	    {
        	    usbCcidNoOfBytesToReceive = 0;
        	    usbCcidOutPacketTrack = USB_CCID_BULK_OUT_FIRST_PACKET;
        	    TransactionCount =0;
        	}

        }
        else if (usbCcidOutPacketTrack == USB_CCID_BULK_OUT_SUBSEQUENT_PACKET)
        {
            // copy data to the APDU[];
            TransactionCount++;	
			if (usbCcidNoOfBytesToReceive > USB_EP_SIZE)
			{
            	for (i =0 ; i< USB_EP_SIZE; i++ )
                	pUsbCcidApdu->CCID_BulkOutBuffer[USB_EP_SIZE *  TransactionCount + i] = usbCcidBulkOutEndpoint[i];
			}
			else
			{
				for (i =0 ; i< usbCcidNoOfBytesToReceive; i++ )
                	pUsbCcidApdu->CCID_BulkOutBuffer[USB_EP_SIZE*TransactionCount+i] = usbCcidBulkOutEndpoint[i];

			}
    	    if (usbCcidNoOfBytesToReceive > USB_EP_SIZE) // We still have more bytes to receive from host
    	    {
    	        usbCcidNoOfBytesToReceive = usbCcidNoOfBytesToReceive - USB_EP_SIZE;
    	        usbCcidOutPacketTrack = USB_CCID_BULK_OUT_SUBSEQUENT_PACKET;
    	        usbCcidBulkOutHandle = USBRxOnePacket(USB_EP_BULK_OUT,(BYTE*)&usbCcidBulkOutEndpoint,USB_EP_SIZE);
                return;
    	    }
    	    else // We have received everything from the host.
    	    {
        	    usbCcidNoOfBytesToReceive = 0;
        	    TransactionCount =0;
        	    usbCcidOutPacketTrack = USB_CCID_BULK_OUT_FIRST_PACKET;
        	}

        }
		
        switch(pUsbCcidApdu->CCID_BulkOutBuffer[0])		//Data arrived, check what kind of command might be in the packet of data.
		{
			////////////////////////////////////////////////////////////////////////////////////
			case USB_CCID_PC_TO_RDR_ICC_POWER_ON:
			{
				if( !SC_CardPresent() ) //if Card is not present
				{
					//Card not present. send error reply
					card_st.Val 		= 0;
					card_st.ICC_Status	= GetCardStatus();
					card_st.CmdStatus	= 1; //Command Failed Code

					pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_DATA_BLOCK; 		//Msg Type
					pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  			//bStatus
					pUsbCcidApdu->CCID_BulkInBuffer[8] = USB_CCID_ICC_MUTE;					//bError ICC_MUTE
					pUsbCcidApdu->CCID_BulkInBuffer[9] = 0;
                    
                    USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );
				}
				else  //else if Card is Present
				{
					SC_Initialize();
					if( SC_PowerOnATR() ) //Get ATR
					{
//						SC_DoPPS();	//switch baud rate based on ATR setting	
                        if (SC_T0ProtocolType() && !SC_T1ProtocolType())
                            usbCcidProtocolNum = USB_CCID_T0_PROTOCOL; 	
                        else if (!SC_T0ProtocolType() && SC_T1ProtocolType())
                            usbCcidProtocolNum = USB_CCID_T1_PROTOCOL; 
                        			
						card_st.Val 		= 0;
						card_st.ICC_Status	= GetCardStatus();
						card_st.CmdStatus	= 0; //processed without error

					    // ATR success
						pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_DATA_BLOCK; 		//Msg Type

						pUsbCcidApdu->CCID_BulkInBuffer[1] = scATRLength;
						pUsbCcidApdu->CCID_BulkInBuffer[2] = 0;
						pUsbCcidApdu->CCID_BulkInBuffer[3] = usbCcidBulkInEndpoint[4] = 0;

						pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
						pUsbCcidApdu->CCID_BulkInBuffer[8] = 0;			//bError
						pUsbCcidApdu->CCID_BulkInBuffer[9] = 0;  //no extended APDU support

						for (i = 0; i < scATRLength; i++)
						    pUsbCcidApdu->CCID_BulkInBuffer[10 + i] = scCardATR[i];
						USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, scATRLength+10 );
				    }
			        else // ATR Failed
			        {
					    // Reply with Error response
					    card_st.ICC_Status	= 1;  // from CCID Rev 1.1 page 28.  Error codes for Hardware Error
					    card_st.CmdStatus	= 1;  // Command Failed
					    ErrCode     		= USB_CCID_HW_ERROR;  // Hardware Error

					    pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_DATA_BLOCK; 		//Msg Type
					    pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
					    pUsbCcidApdu->CCID_BulkInBuffer[8] = ErrCode;		//bError
					    pUsbCcidApdu->CCID_BulkInBuffer[9] = 0;
                        USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );
				    }// End of else ATR Failed
			    }// End of else - card is present
			}// End case PC_to_RDR_IccPowerOn:    
			break;

			////////////////////////////////////////////////////////////////////////////////////
			case USB_CCID_PC_TO_RDR_ICC_POWER_OFF:
			    if( pUsbCcidApdu->CCID_BulkOutBuffer[5] == 0 )	// must be slot 0 (for 1 slot reader)
			    {
				    //Do power off sequence if required
				    SC_Shutdown();
				    card_st.ICC_Status	= GetCardStatus();	// bStatus - 0 Card Present and active,
														        //           1 Card Present and inactive,
														        //			 2 Not Present
			    }
			    else //slot # out of range
			    {
				    card_st.ICC_Status	= 2;  // from CCID Rev 1.1 page 28.  Error codes for bad slot umber
				    card_st.CmdStatus	= 1;  // Command Failed
				    ErrCode     		= 5;  // bError Slot Does not exist
			    }

			    pUsbCcidApdu->CCID_BulkInBuffer[0] = 0x81; //Msg Type
			    pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
			    pUsbCcidApdu->CCID_BulkInBuffer[8] = ErrCode;		//bError
			    pUsbCcidApdu->CCID_BulkInBuffer[9] = (SC_GetCardState()==SC_STATE_CARD_ACTIVE)?0:1;  //Clock Status: 0 Running, 1 Stopped L, 2 Stopped H
                USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );
            break;

            ////////////////////////////////////////////////////////////////////////////////////
            case USB_CCID_PC_TO_RDR_GET_SLOT_STATUS:
                if( pUsbCcidApdu->CCID_BulkOutBuffer[5] == 0 )	// must be slot 0 (for 1 slot reader)
			    {
				    card_st.ICC_Status	= GetCardStatus();	// bStatus - 0 Card Present and active,
														    //           1 Card Present and inactive,
														    //			 2 Not Present
			    }
			    else //slot # out of range
			    {
				    card_st.ICC_Status	= 2;  // from CCID Rev 1.1 page 28.  Error codes for bad slot umber
				    card_st.CmdStatus	= 1;  // Command Failed
				    ErrCode     		= 5;  // bError Slot Does not exist
			    }

			    pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_SLOT_STATUS; //Msg Type
			    pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
			    pUsbCcidApdu->CCID_BulkInBuffer[8] = ErrCode;		//bError
			    pUsbCcidApdu->CCID_BulkInBuffer[9] = (SC_GetCardState()==SC_STATE_CARD_ACTIVE)?0:1;  //Clock Status: 0 Running, 1 Stopped L, 2 Stopped H
			    USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );
            break;

            ////////////////////////////////////////////////////////////////////////////////////
			case USB_CCID_PC_TO_RDR_XFR_BLOCK:
			    UsbCcidPcToRdrXfrBlock();
			break;

            ////////////////////////////////////////////////////////////////////////////////////
            
			case USB_CCID_PC_TO_RDR_GET_PARAMETERS:
			    UsbCcidSendRdrToPcParameters(); 
			break;
			////////////////////////////////////////////////////////////////////////////////////
			case USB_CCID_PC_TO_RDR_RESET_PARAMETERS :
			    UsbCcidSendRdrToPcParameters();
			break;
			////////////////////////////////////////////////////////////////////////////////////
			case USB_CCID_PC_TO_RDR_SET_PARAMETERS :
			    if  (SC_GetCardState() == SC_STATE_CARD_ACTIVE)
				{
    				if (SC_T0ProtocolType() && SC_T1ProtocolType())
    				{
    				    usbCcidProtocolNum = pUsbCcidApdu->CCID_BulkOutBuffer[7];  
    				}    
    				ppsData[0] = 0xFF; 
					if (usbCcidProtocolNum == USB_CCID_T0_PROTOCOL)   //if T=0 Protocol
					{
    					//pUsbCcidApdu->CCID_BulkOutBuffer[10]; //Read FI Index
    					//pUsbCcidApdu->CCID_BulkOutBuffer[11]; //bmTCCKST0    
    					//pUsbCcidApdu->CCID_BulkOutBuffer[12]; // Read Guard Time
    					//pUsbCcidApdu->CCID_BulkOutBuffer[13]; // Read WI
    					ppsData[1] = 0x00; 
    					
    	            }
    	            else if (usbCcidProtocolNum == USB_CCID_T1_PROTOCOL) 
    	            {
        	            ppsData[1] = 0x01;
        	            //pUsbCcidApdu->CCID_BulkOutBuffer[10]; //Read FI Index
    					//pUsbCcidApdu->CCID_BulkOutBuffer[11]; //bmTCCKST1    
    					//pUsbCcidApdu->CCID_BulkOutBuffer[12]; // Guard Time
    					//pUsbCcidApdu->CCID_BulkOutBuffer[13]; // bmWaitingIntegersT1
    					//pUsbCcidApdu->CCID_BulkOutBuffer[14]; // bClockStop
    					//pUsbCcidApdu->CCID_BulkOutBuffer[15]; //bIFSC
    					//pUsbCcidApdu->CCID_BulkOutBuffer[16]; //bNadValue
        	        } 
        	        ppsData[2] = ppsData[0] ^ ppsData[1];  
        	        if (SC_T0ProtocolType() && SC_T1ProtocolType()) 
        	        {  		    
    			        if (!SC_DoPPS(ppsData))
    			        {
        		            SC_Shutdown();
        			    }
        			}   
    				ccid_clockstatus = pUsbCcidApdu->CCID_BulkOutBuffer[14]; // Read Clock Stop Status
                }
                UsbCcidSendRdrToPcParameters();				
			break;
		
			#ifndef LOW_PIN_COUNT_USB_DEVELOPMENT_KIT
            ////////////////////////////////////////////////////////////////////////////////////
            case USB_CCID_PC_TO_RDR_ESCAPE:
                // This Command is not supoprted
                pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_ESCAPE;
				UsbCcidCommandNotSupported();
                
            break;
			#endif
			
            ////////////////////////////////////////////////////////////////////////////////////
            case USB_CCID_PC_TO_RDR_ICC_CLOCK:
                if( pUsbCcidApdu->CCID_BulkOutBuffer[5] == 0 )	// must be slot 0 (for 1 slot reader)
			    {
					//Do power off sequence if required
				    if (pUsbCcidApdu->CCID_BulkOutBuffer[7] == 0)
					{
				        USB_CCID_RestartClock();//Restart Clock
					}
				    else if (pUsbCcidApdu->CCID_BulkOutBuffer[7] == 1)
				    {
				        if (ccid_clockstatus == 1)
						{
				        	USB_CCID_StopClockWithPinLow(); // Stop with Clock signal Low
						}
				        else if (ccid_clockstatus == 2)
						{
				            USB_CCID_StopClockWithPinHigh();// Stop with Clock signal High
						}
				        else if (ccid_clockstatus == 3)
						{
    				        USB_CCID_StopClockWithPinLow(); // Stop with Clock either High or Low
						}
				     }
				     card_st.ICC_Status	= GetCardStatus();	// bStatus - 0 Card Present and active,
														        //           1 Card Present and inactive,
														        //			 2 Not Present
			    }
			    else //slot # out of range
			    {
				    card_st.ICC_Status	= 2;  // from CCID Rev 1.1 page 28.  Error codes for bad slot umber
				    card_st.CmdStatus	= 1;  // Command Failed
				    ErrCode     		= 5;  // bError Slot Does not exist
			    }
			    pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_SLOT_STATUS; //Msg Type
			    pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
			    pUsbCcidApdu->CCID_BulkInBuffer[8] = ErrCode;		//bError
			    pUsbCcidApdu->CCID_BulkInBuffer[9] = (SC_GetCardState()==SC_STATE_CARD_ACTIVE)?0:1;  //Clock Status: 0 Running, 1 Stopped L, 2 Stopped H
					
				USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );
            break;

            ////////////////////////////////////////////////////////////////////////////////////
			#ifndef LOW_PIN_COUNT_USB_DEVELOPMENT_KIT
            case USB_CCID_PC_TO_RDR_SECURE :
                // This Command is not supoprted
                pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_DATA_BLOCK;
                UsbCcidCommandNotSupported();
            break;

            ////////////////////////////////////////////////////////////////////////////////////
            case USB_CCID_PC_TO_RDR_MECHANICAL:
                // This Command is not supoprted
				pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_SLOT_STATUS;
				UsbCcidCommandNotSupported();
            ////////////////////////////////////////////////////////////////////////////////////
            case USB_CCID_PC_TO_RDR_ABORT:
                // This Command is not supoprted
				pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_SLOT_STATUS;
				UsbCcidCommandNotSupported();

            ////////////////////////////////////////////////////////////////////////////////////
            case USB_CCID_PC_TO_RDR_SET_DATA_RATE_AND_CLOCK_FREQUENCY:
                // This Command is not supoprted
				pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_DATA_RATE_AND_CLOCK_FREQUENCY;
				UsbCcidCommandNotSupported();

            ////////////////////////////////////////////////////////////////////////////////////
            case USB_CCID_PC_TO_RDR_T0APDU :
                // This Command is not supoprted
                pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_SLOT_STATUS;
                UsbCcidCommandNotSupported();
            break;
 			#endif
            default:
                pUsbCcidApdu->CCID_BulkInBuffer[0] =  USB_CCID_RDR_TO_PC_SLOT_STATUS;
				#ifndef LOW_PIN_COUNT_USB_DEVELOPMENT_KIT
					UsbCcidCommandNotSupported();  
				#else
					card_st.Val 		= 0;
					card_st.ICC_Status	= GetCardStatus();
					card_st.CmdStatus	= 1; //Command Failed Code
					pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
    				pUsbCcidApdu->CCID_BulkInBuffer[8] = USB_CCID_CMD_NOT_SUPPORTED; //bError, Command not supported
					USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );  
				#endif 
            break;
        }
        usbCcidBulkOutHandle = USBRxOnePacket(USB_EP_BULK_OUT,(BYTE*)&usbCcidBulkOutEndpoint,USB_EP_SIZE);
    }
}

/********************************************************************
 * Function:        void PcToRdrXfrBlock(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None

 *
 * Overview:        This function handles data transfer
 *                  between host and the Smart Card
 *
 * Note:
 *******************************************************************/
void UsbCcidPcToRdrXfrBlock(void)
{
    static SC_APDU_COMMAND 	CardCmd;
	static SC_APDU_RESPONSE CardResp;
	SC_T1_PROLOGUE_FIELD t1Pfield;
	UINT16 temp;
	union SLOT_STATUS card_st;
	BYTE ErrCode;

	card_st.Val = 0;
	ErrCode     = 0; //clear error code

    if( SC_GetCardState() == SC_STATE_CARD_ACTIVE )
    {
        CardCmd.CLA = pUsbCcidApdu->CCID_BulkOutBuffer[10];
        CardCmd.INS = pUsbCcidApdu->CCID_BulkOutBuffer[11];
		CardCmd.P1 	= pUsbCcidApdu->CCID_BulkOutBuffer[12];
		CardCmd.P2  = pUsbCcidApdu->CCID_BulkOutBuffer[13];

		if (pUsbCcidApdu->CCID_BulkOutBuffer[14] == 0) // if APDU P3 is zero
		{
			//case 1: no data to exchange with ICC, only header
		    CardCmd.LC = 0;
		    CardCmd.LE = 0;
		}
		else if ( pUsbCcidApdu->CCID_BulkOutBuffer[1] == 5) // if dwLength field is eaqual to Length of (CLA + INS + P1 + P2 + P3)
		{
			//case 2: data expected from ICC:
		    CardCmd.LE  = pUsbCcidApdu->CCID_BulkOutBuffer[14];
		    CardCmd.LC  = 0;
		}
		else if (pUsbCcidApdu->CCID_BulkOutBuffer[1] == (BYTE)(pUsbCcidApdu->CCID_BulkOutBuffer[14]+5)) // if dwLength field is eaqual to P3 + Length of (CLA + INS + P1 + P2 + P3)
		{
			// case 3: data are to be sent to the ICC:
		    CardCmd.LC  = pUsbCcidApdu->CCID_BulkOutBuffer[14];
		    CardCmd.LE  = 0;
		}
		else if ( pUsbCcidApdu->CCID_BulkOutBuffer[1] == 5+pUsbCcidApdu->CCID_BulkOutBuffer[14]+1) // if dwLength field is eaqual to P3 + Length of (CLA + INS + P1 + P2 + P3 + LE)
		{
		    // case 4: Data is transferred to the card and is also returned from the card as a result of the command.
			CardCmd.LC  = pUsbCcidApdu->CCID_BulkOutBuffer[14];
			CardCmd.LE  = pUsbCcidApdu->CCID_BulkOutBuffer[14+CardCmd.LC+1];
		}
		else
		{
	        //Bad dwLength
			card_st.CmdStatus	= 1; //Command Failed Code
			ErrCode 			= 1; //bad len
		}

		if(usbCcidProtocolNum == USB_CCID_T1_PROTOCOL)
		{
			t1Pfield.NAD = 0x00;t1Pfield.PCB = 0x00;

			if(CardCmd.LE && CardCmd.LC)
			{
				t1Pfield.LENGTH = (6 + CardCmd.LC);
			}
			else if(!CardCmd.LE && CardCmd.LC)
			{
				t1Pfield.LENGTH = (5 + CardCmd.LC);
			}
			else if(CardCmd.LE && !CardCmd.LC)
			{
				t1Pfield.LENGTH = 5;
			}
			else
				t1Pfield.LENGTH = 4;

			#ifdef SC_PROTO_T1
			if(( !SC_TransactT1(&t1Pfield,pUsbCcidApdu->CCID_BulkOutBuffer+10,&CardResp) ) && (!ErrCode))
			#endif
			{
				//error exec command
				card_st.ICC_Status	= 1;
				card_st.CmdStatus	= 1;   //Command Failed Code
				ErrCode 			= -5;  //hardware error
			}
		}
		else if (usbCcidProtocolNum == USB_CCID_T0_PROTOCOL)
		{
			if( !ErrCode && !SC_TransactT0(&CardCmd, &CardResp, pUsbCcidApdu->CCID_BulkOutBuffer+15) )
			{
				//error exec command
				card_st.ICC_Status	= 1;
				card_st.CmdStatus	= 1;   //Command Failed Code
				ErrCode 			= -5;  //hardware error
			}
		}
    }
    else // Card not present or not active
    {
	    card_st.ICC_Status	= GetCardStatus();
		card_st.CmdStatus	= 1; //Command Failed Code
		ErrCode 			= 0xFE;	 //bError ICC_MUTE
    }
    if( card_st.CmdStatus == 0 ) //no error
    {
		card_st.ICC_Status	= GetCardStatus();
		card_st.CmdStatus	= 0; //processed without error
		pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_DATA_BLOCK; 		//Msg Type
		pUsbCcidApdu->CCID_BulkInBuffer[1] = CardResp.RXDATALEN+2;	//response size + SW1 and SW2
		pUsbCcidApdu->CCID_BulkInBuffer[2] = (CardResp.RXDATALEN+2)>>8;	//response size + SW1 and SW2
		pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
		pUsbCcidApdu->CCID_BulkInBuffer[8] = 0;			//bError none
		pUsbCcidApdu->CCID_BulkInBuffer[9] = 0;  			//RFU
        for( temp = 0; temp < CardResp.RXDATALEN; temp++ )
        {
            if(usbCcidProtocolNum == USB_CCID_T1_PROTOCOL)
                pUsbCcidApdu->CCID_BulkInBuffer[10+temp] = pUsbCcidApdu->CCID_BulkOutBuffer[10+temp];
            else if (usbCcidProtocolNum == USB_CCID_T0_PROTOCOL)
			    pUsbCcidApdu->CCID_BulkInBuffer[10+temp] = pUsbCcidApdu->CCID_BulkOutBuffer[15+temp];
	    }
	    pUsbCcidApdu->CCID_BulkInBuffer[10+CardResp.RXDATALEN]   = CardResp.SW1;
		pUsbCcidApdu->CCID_BulkInBuffer[10+CardResp.RXDATALEN+1] = CardResp.SW2;
		USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10+CardResp.RXDATALEN+2);
    }
    else
    {
	    pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_DATA_BLOCK; 		//Msg Type
	    pUsbCcidApdu->CCID_BulkInBuffer[1] = 0x00; 
		pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
		pUsbCcidApdu->CCID_BulkInBuffer[8] = ErrCode;		//bError
		pUsbCcidApdu->CCID_BulkInBuffer[9] = 0x02; 
		//pUsbCcidApdu->CCID_BulkInBuffer[10] = scLastError;
        USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10);
    }
}  

/********************************************************************
 * Function:        void UsbCcidSendRdrToPcParameters (void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None 
 *
 * Overview:        This function sends parameters to CCID host 
 *******************************************************************/
 void UsbCcidSendRdrToPcParameters (void)
 {
     union SLOT_STATUS card_st;
     BYTE Tempvar; 
     if( SC_GetCardState() == SC_STATE_CARD_ACTIVE )
	 {
	    card_st.Val 		= 0;
		card_st.ICC_Status	= GetCardStatus();
		card_st.CmdStatus	= 0; //processed without error

		pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_PARAMETERS; 		//Msg Type RDR_to_PC_Parameters
		
	    pUsbCcidApdu->CCID_BulkInBuffer[7]  = card_st.Val;  //bStatus
		pUsbCcidApdu->CCID_BulkInBuffer[8]  = 0;  //bError none
		pUsbCcidApdu->CCID_BulkInBuffer[9]  = usbCcidProtocolNum;  //  Protocol Number (T0 or T1?)
		pUsbCcidApdu->CCID_BulkInBuffer[10] = 0x01;	    //Fi and Di codes
		
	    pUsbCcidApdu->CCID_BulkInBuffer[12] = cgtETU - 12;	//Extra gaurd time
	    
	    
		    
		pUsbCcidApdu->CCID_BulkInBuffer[14] = 0x00;		//Does not allow clock stopping as of now

		// Protocol Data Structure
		if (usbCcidProtocolNum == USB_CCID_T0_PROTOCOL)
		{
    		pUsbCcidApdu->CCID_BulkInBuffer[1] = 5; //protocol data struct length
    		pUsbCcidApdu->CCID_BulkInBuffer[11] = 0x00;		//Direct
    		
    		if(scTD1 & 0x40)
    		    pUsbCcidApdu->CCID_BulkInBuffer[13] = scTC2;
            else 
                pUsbCcidApdu->CCID_BulkInBuffer[13] = SC_WI;     

            USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 15 );
        }
        else if (usbCcidProtocolNum == USB_CCID_T1_PROTOCOL)
        {
            pUsbCcidApdu->CCID_BulkInBuffer[1] = 7; //protocol data struct length
            pUsbCcidApdu->CCID_BulkInBuffer[11] = 0x10;		//LRC, Direct
            
            if(scTD1 & 0x20)
		        pUsbCcidApdu->CCID_BulkInBuffer[13] = scTB2;	//WI for T0, wwt = 960 * WI * (Fi / f)
		    else 
		        Tempvar = (SC_BWI & 0x0F) << 4; 
		    pUsbCcidApdu->CCID_BulkInBuffer[13] = Tempvar | SC_CWI; 
		    pUsbCcidApdu->CCID_BulkInBuffer[15] = maxSegmentLength;	//bIFSD = Size of negotiated IFSC
		    pUsbCcidApdu->CCID_BulkInBuffer[16] = 0x00;		//bNadValue value used by CCID
		    
		    USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 17 );
		}                       
	}
	else
	{
		//Card not present or not active
		card_st.Val 		= 0;
		card_st.ICC_Status	= GetCardStatus();
		card_st.CmdStatus	= 1; //Command Failed Code

		pUsbCcidApdu->CCID_BulkInBuffer[0] = USB_CCID_RDR_TO_PC_PARAMETERS; 		//Msg Type RDR_to_PC_Parameters
		pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
		pUsbCcidApdu->CCID_BulkInBuffer[8] = 0xFE;			//bError ICC_MUTE
		pUsbCcidApdu->CCID_BulkInBuffer[9] = 0;

        USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );
	}
 }       


/********************************************************************
 * Function:        void UsbCcidCommandNotSupported (void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None 
 *
 * Overview:        This function is called when the ccid command received 
 *					from the host is not suuported by the reader.    
 *******************************************************************/
#ifndef LOW_PIN_COUNT_USB_DEVELOPMENT_KIT
void UsbCcidCommandNotSupported(void)
{
	union SLOT_STATUS card_st;
	card_st.Val 		= 0;
	card_st.ICC_Status	= GetCardStatus();
	card_st.CmdStatus	= 1; //Command Failed Code
	pUsbCcidApdu->CCID_BulkInBuffer[7] = card_st.Val;  //bStatus
    pUsbCcidApdu->CCID_BulkInBuffer[8] = USB_CCID_CMD_NOT_SUPPORTED; //bError, Command not supported
	USBCCIDSendDataToHost(pUsbCcidApdu->CCID_BulkInBuffer, 10 );

}  
#endif 
/********************************************************************
 * Function:        void UsbCcidAbortRequestHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    If either the USBEP0SendRAMPtr() or USBEP0Receive()
 *                  functions are not called in this function then the
 *                  requesting the ABORT request will be STALLed
 *
 * Overview:        This function is called by the CCID function driver
 *                  in response to a ABORT Request.
 *
 * Note:            This function is called from the stack in
 *                  response of a EP0 packet.  The response to this
 *                  packet should be fast in order to clear EP0 for
 *                  any potential SETUP packets.  Do not call any
 *                  functions or run any algorithms that take a long time
 *                  to execute (>50uSec).  Have any data that would be
 *                  read using one of these commands pre-calculated
 *                  from the main line code and just use this function
 *                  to transfer the data.
 *******************************************************************/
 #if defined USB_CCID_SUPPORT_ABORT_REQUEST
 void UsbCcidAbortRequestHandler(void)
 {
     
 }
 #endif

/********************************************************************
 * Function:        void UsbCcidGetClockFrequenciesRequestHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    If either the USBEP0SendRAMPtr() or USBEP0Receive()
 *                  functions are not called in this function then the
 *                  requesting the GET_CLOCK_FREQUENCIES request will be STALLed
 *
 * Overview:        This function is called by the CCID function driver
 *                  in response to a GET_CLOCK_FREQUENCIES Request.
 *
 * Note:            This function is called from the stack in
 *                  response of a EP0 packet.  The response to this
 *                  packet should be fast in order to clear EP0 for
 *                  any potential SETUP packets.  Do not call any
 *                  functions or run any algorithms that take a long time
 *                  to execute (>50uSec).  Have any data that would be
 *                  read using one of these commands pre-calculated
 *                  from the main line code and just use this function
 *                  to transfer the data.
 *******************************************************************/
 #if defined USB_CCID_SUPPORT_GET_CLOCK_FREQUENCIES_REQUEST
 void UsbCcidGetClockFrequenciesRequestHandler(void)
 {
 }
 #endif


 /********************************************************************
 * Function:        void UsbCcidGetDataRatesRequestHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    If either the USBEP0SendRAMPtr() or USBEP0Receive()
 *                  functions are not called in this function then the
 *                  requesting the GET_DATA_RATES request will be STALLed
 *
 * Overview:        This function is called by the CCID function driver
 *                  in response to a GET_DATA_RATES Request.
 *
 * Note:            This function is called from the stack in
 *                  response of a EP0 packet.  The response to this
 *                  packet should be fast in order to clear EP0 for
 *                  any potential SETUP packets.  Do not call any
 *                  functions or run any algorithms that take a long time
 *                  to execute (>50uSec).  Have any data that would be
 *                  read using one of these commands pre-calculated
 *                  from the main line code and just use this function
 *                  to transfer the data.
 *******************************************************************/
 #if defined USB_CCID_SUPPORT_GET_DATA_RATES_REQUEST
 void UsbCcidGetDataRatesRequestHandler(void)
 {
 }
 #endif



