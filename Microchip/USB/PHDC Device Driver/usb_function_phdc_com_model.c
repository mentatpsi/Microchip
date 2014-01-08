/************************************************************************
  File Information:
    FileName:       usb_function_phdc_com_model.c
    Dependencies:   See INCLUDES section
    Processor:      PIC18 or PIC24 USB Microcontrollers
    Hardware:       The code is natively intended to be used on the following
                    hardware platforms: PICDEM™ FS USB Demo Board,
                    PIC18F87J50 FS USB Plug-In Module, or
                    Explorer 16 + PIC24 USB PIM.  The firmware may be
                    modified for use on other USB platforms by editing the
                    HardwareProfile.h file.
    Complier:       Microchip C18 (for PIC18) or C30 (for PIC24)
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
    datatypes, etc. that are required for usage with the application software
	.

    This file is located in the "\<Install Directory\>\\Microchip\\USB\\PHDC
    Device Driver" directory.

  Description:
    Application file

    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the application software.

    This file is located in the "\<Install Directory\>\\Microchip\\USB\\PHDC
    Device Driver" directory.



********************************************************************/

/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  1.0    Initial Release
  2.0

********************************************************************/
/** I N C L U D E S **********************************************************/
#include "USB/usb_function_phdc.h"
#include "USB/usb.h"
#include "USB/usb_function_phdc_com_model.h"
#include "IEEE/ieee_11073_phd_types.h"
#include "IEEE/ieee_11073_nom_codes.h"
#include "HardwareProfile.h"
#include "phd_config.h"

#ifdef PHD_USE_RTCC_FOR_TIME_STAMP
    #include "rtcc.h"
#endif


/** V A R I A B L E S ********************************************************/
#if defined(__18CXX)
    #pragma udata
#endif
PHDC_APP_CB AppCB;
UINT8 PhdComState;
BOOL PhdAssociationRequestTimeoutStatus;
INT16 PhdAssociationRequestTimeout;
INT8 PhdAssociationRequestRetry;
BOOL PhdConfigurationTimeoutStatus;
INT16 PhdConfigurationTimeout;
BOOL PhdAssociationReleaseTimeoutStatus;
INT16 PhdAssociationReleaseTimeout;
BOOL PhdConfirmTimeoutStatus;
INT16 PhdConfirmTimeout;
BOOL PhdTimeStateAbsSynced;
UINT8 *pPhdAppBuffer;

/** DEFINITIONS ****************************************************/
#define ASSOCIATION_RESPONSE_REJECTED_PERMANENT_SIZE       48
#define RELEASE_REQUEST_SIZE                                6
#define RELEASE_RESPONSE_SIZE                               6
#define ABORT_SIZE                                          6

// Timeout
#define TIMEOUT_ENABLED 1
#define TIMEOUT_DISABLED 0
#define TIMEOUT_EXPIRED 0

/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void PHDAppDataRxHandler(BYTE* apdu);
static void PHDAssocRequestHandler(BYTE* apdu_val);
static void PHDAssocResponseHandler(BYTE* apdu_val);
static void PHDPrstApduHandler(BYTE* apdu_val);
static void PHDReleaseResponseHandler(BYTE* apdu_val);
static void PHDReleaseRequestHandler(BYTE* apdu_val);
static void PHDAbortRequestHandler(BYTE* apdu_val);
static void PHDSendReleaseRequestToManager(UINT16 releaseReason);
static void PHDSendAbortRequestToManager(UINT16 abortReason);
static void PHDSendRoerMessageToManager (UINT16 invoke_id, UINT16 error_value);
static void PHDDisableAllTimeout(void);
static void PHDMdsAttributesRequestHandler(BYTE* apdu_val);
static void PHDSetTime(BYTE* apdu_val);
static void UsbToPHDComCB(UINT8 USB_Event, void*);

/** CONSTANT DATA ********************************************************************************/

/* Association Response */
UINT8  ROM ASSOCIATION_RESPONSE_REJECTED_PERMANENT [ASSOCIATION_RESPONSE_REJECTED_PERMANENT_SIZE] = {
0xE3, 0x00,   //APDU CHOICE Type (AareApdu)
0x00, 0x2C,   //CHOICE.length = 44
0x00, 0x01,   //result = rejected-permanent
};


/*Release Request APDU */
UINT8  ROM RELEASE_REQUEST[RELEASE_REQUEST_SIZE] = {
0xE4, 0x00,                           /* APDU CHOICE Type (RlrqApdu) */
0x00, 0x02,                           /* CHOICE.length = 2 */
0x00, 0x00                            /* reason = normal */
};

/* Release Response APDU */
UINT8  ROM RELEASE_RESPONSE[RELEASE_RESPONSE_SIZE] = {
0xE5, 0x00,                           /* APDU CHOICE Type (RlrsApdu) */
0x00, 0x02,                           /* CHOICE.length = 2 */
0x00, 0x00                            /* reason = normal */
};


/******************************************************************************
 * Function:
 *      void PHDAppInit(PHDC_APP_CB callback)
 *
 * Summary:
 *      This function is used to initialize the PHD stack.
 *
 * Description:
 *       This function initializes all the application related items.
 *       The input to the function is address of the callback function. This callback function
 *	 which will be called by PHD stack when there is a change in Agent's connection status.
 *
 * Conditions:
 *       None
 *
 * Parameters:
 *	PHDC_APP_CB callback - Pointer to application Call Back Function.
 *
 * Return:
 *	None
 *
 * Side Effects:
 *	None
 *
 * Remarks:
 *      None
 *
 *****************************************************************************/
void PHDAppInit(PHDC_APP_CB callback)
{
    PhdComState = PHD_COM_STATE_UNASSOCIATED;
	AppCB = callback;
	USBDevicePHDCInit(UsbToPHDComCB); //register the call back function
	PhdComState = PHD_COM_STATE_UNASSOCIATED;
	PHDDisableAllTimeout();
	PhdTimeStateAbsSynced = 0;
}

/******************************************************************************
 * Function:
 *      void PHDSendAppBufferPointer(UINT8 * pAppBuffer)
 *
 * Summary:
 *      This function is used to send measurement data to the PHD Manager.
 *
 * Description:
 *       This function passes the application buffer pointer to the PHD stack. The PHD stack
 *       uses this pointer send and receive data through the transport layer.
 *
 * Conditions:
 *
 *
 * Parameters:
 *	UINT8 *pAppBuffer - Pointer to Application Buffer.
 *
 * Return:
 *	None
 *
 * Side Effects:
 *	None
 *
 * Remarks:
 *      None
 *
 *****************************************************************************/
void PHDSendAppBufferPointer(UINT8 * pAppBuffer)
{
    pPhdAppBuffer = pAppBuffer;
}

/******************************************************************************
 * Function:        void UsbToPHDComCB(UINT8 USB_Event, void * val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine is called from the transport layer informing about
 *                  its events
 *
 * Note:
 *
 *****************************************************************************/
void UsbToPHDComCB(UINT8 USB_Event, void * val)
{
    UINT16 *size;
    APDU *ptr;
    UINT16 length, choice;//, count;
    #if defined(__18CXX)
        UINT16 *PTR;
    #elif defined(__C30__) || defined __XC16__
        //UINT16 *PTR;
    #elif defined(__PIC32MX__)
        UINT32 *PTR;
    #endif


	switch(USB_Event)
	{
		case USB_APP_SEND_COMPLETE:
		    if(PhdComState == PHD_COM_STATE_ASSOC_CFG_SENDING_CONFIG)
		    {
			    PhdComState = PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL;
			    PhdConfigurationTimeoutStatus = TIMEOUT_ENABLED;
			    PhdConfigurationTimeout = CONFIGURATION_TIMEOUT;
		    }
		break;

		case USB_APP_DATA_RECEIVED:
		    USBDevicePHDCReceiveData(PHDC_BULK_OUT_QOS,pPhdAppBuffer,0); //get ready to receive
			PHDAppDataRxHandler(pPhdAppBuffer);
		break;

		case USB_APP_GET_TRANSFER_SIZE:
		    size=(UINT16*)val;
		    ptr = (APDU*)pPhdAppBuffer;
		    length = BYTE_SWAP16(ptr->length);
		    choice = BYTE_SWAP16(ptr->choice);
		    *size = length + 4;
		    //PhdAppBufferOffset = 0;
		break;

		default:
		break;
	}//End of switch(USB_Event)
}

/******************************************************************************
 * Function:        void PHDAppDataRxHandler(BYTE* apdu)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles the received APDU
 *
 * Note:
 *
 *****************************************************************************/
void PHDAppDataRxHandler(BYTE* apdu)
{
	UINT16_VAL  apduChoiceLength, apduChoiceType;

	apduChoiceType.byte.HB = apdu[0];
	apduChoiceType.byte.LB = apdu[1];

	apduChoiceLength.byte.HB = apdu[2];
	apduChoiceLength.byte.LB = apdu[3];


	switch (apduChoiceType.Val)
	{
		case PHD_ASSOCIATION_REQUEST:
		    PHDAssocRequestHandler(apdu);
		break;

		case PHD_ASSOCIATION_RESPONSE:
		    PHDAssocResponseHandler(apdu);
		break;

		case PHD_RELEASE_REQUEST:
		    PHDReleaseRequestHandler(apdu);
		break;

		case PHD_RELEASE_RESPONSE:
		    PHDReleaseResponseHandler(apdu);
		break;

		case PHD_ABORT_REQUEST:
		    PHDAbortRequestHandler(apdu);
		break;

		case PHD_PRESET_APDU:
		    PHDPrstApduHandler(apdu);
		break;

		default:
		    if ((PhdComState==PHD_COM_STATE_ASSOCIATING)
		       ||(PhdComState == PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL)
		       ||(PhdComState==PHD_COM_STATE_ASSOC_OPERATING)
		       ||(PhdComState ==  PHD_COM_STATE_DISASSOCIATING))
            {
                PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
            }
        break;

	}// end of switch (apduChoiceType.Val)
}

 /******************************************************************************
 * Function:
 *      void PHDConnect(void)
 *
 * Summary:
 *      This function is used to connect to the PHD Manager.
 *
 * Description:
 *       This function initiates connection to the PHD Manager by sending an
 *   Association request to manager.  The Agent doesn't get connected to
 *	 the Manager immediately after calling this function. Upon receiving
 *	 the association request from an Agent, the PHD Manager responds with
 *	 an association response. The association response tells whether Manager
 *	 accepting the request or rejecting it. The Association response from
 *	 the Manager is handled by the PHD stack. The PHD stack calls a callback
 *	 function (void(* PHDC_APP_CB)(UINT8)) to the application with status of
 *	 the connection.
 *	 The Manager should respond to the Agent within the specified timeout of
 *	 ASSOCIATION_REQUEST_TIMEOUT. The Agent should send the Association request
 *	 once more if no response is received from Manager and ASSOCIATION_REQUEST_TIMEOUT
 *   is expired. This function starts a Timer for the Association Timeout request.
 *   The timeout is handled by the PHDTimeoutHandler() function.
 *
 * Conditions:
 *       The agent should be in PHD_INITIALIZED state.
 *
 * Parameters:
 *	None
 *
 * Return:
 *	None
 *
 * Side Effects:
 *	None
 *
 * Remarks:
 *      None
 *
 *****************************************************************************/
void PHDConnect(void)
{
	if(PhdComState == PHD_COM_STATE_UNASSOCIATED)
	{
		PhdComState= PHD_COM_STATE_ASSOCIATING;
		USBDevicePHDCSendData(PHDC_BULK_IN_QOS,(UINT8 *) ASSOCIATION_REQUEST,ASSOCIATION_REQUEST_SIZE,MEM_ROM);
		USBDevicePHDCReceiveData(PHDC_BULK_OUT_QOS,pPhdAppBuffer,0); //get ready to receive

		PhdAssociationRequestTimeoutStatus = TIMEOUT_ENABLED;
		PhdAssociationRequestTimeout = ASSOCIATION_REQUEST_TIMEOUT;
		PhdAssociationRequestRetry = ASSOCIATION_REQUEST_RETRY;
	}
}

/******************************************************************************
 * Function:
 *      void PHDDisConnect(void)
 *
 * Summary:
 *      This function is used to disconnect from the PHD Manager.
 *
 * Description:
 *       This function initiates disconnection of the Agent from the PHD Manager by sending an
 *	 Release request to manager.  The Agent doesn't get disconnected from the Manager
 *	 immediately after calling this function. The PHD Manager sends back a release response
 *	 to the Agent. The Agent responds back with an Abort Message and the Agent moves to DISCONNECTED
 *	 state. The PHD stack calls a callback function (void(* PHDC_APP_CB)(UINT8)) to the application
 *	 with status of the connection. This function disables all timeout.
 *
 * Conditions:
 *       The agent should be in PHD_CONNECTED state.
 *
 * Parameters:
 *	None
 *
 * Return:
 *	None
 *
 * Side Effects:
 *	None
 *
 * Remarks:
 *      None
 *
 *****************************************************************************/
void PHDDisConnect(void)
{
	if(PhdComState == PHD_COM_STATE_ASSOCIATING)
	{
	    //do nothing

	}
	else if((PhdComState==PHD_COM_STATE_DISASSOCIATING)||
			(PhdComState==PHD_COM_STATE_UNASSOCIATED))
	{
		//do nothing
	}
	else
	{
		PhdComState= PHD_COM_STATE_DISASSOCIATING;
		USBDevicePHDCSendData(PHDC_BULK_IN_QOS,(UINT8 *) RELEASE_REQUEST,RELEASE_REQUEST_SIZE,MEM_ROM);
	}
	PHDDisableAllTimeout();
}

/******************************************************************************
 * Function:
 *      void PHDSendMeasuredData(void)
 *
 * Summary:
 *      This function is used to send measurement data to the PHD Manager.
 *
 * Description:
 *       This function sends measurement data to manager. Before calling this function
 *       the caller should fill the Application buffer with the data to send. The Agent
 *	 expects a Confirmation from the Manager for the data sent. This confirmation should
 *	 arrive at the Agent within a specified time of CONFIRM_TIMEOUT. The function starts
 *	 a Timer to see if the Confirmation from the Manager arrives within specified time.
 *	 The timeout is handled by the PHDTimeoutHandler() function.
 *
 * Conditions:
 *       Before calling this function the caller should fill the Application buffer with the data to send.
 *
 * Parameters:
 *	None
 *
 * Return:
 *	None
 *
 * Side Effects:
 *	None
 *
 * Remarks:
 *      None
 *
 *****************************************************************************/
void PHDSendMeasuredData(void)
{
	USBDevicePHDCSendData(SEND_QOS,pPhdAppBuffer,(UINT16)MEASUREMENT_DATA_SIZE,MEM_RAM);
	USBDevicePHDCReceiveData(PHDC_BULK_OUT_QOS,pPhdAppBuffer,0); //get ready to receive

	PhdConfirmTimeoutStatus = TIMEOUT_ENABLED;
	PhdConfirmTimeout = CONFIRM_TIMEOUT;
}

/******************************************************************************
 * Function:        static void PHDAssocRequestHandler(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles association response
 *
 * Note:
 *
 *****************************************************************************/
static void PHDAssocRequestHandler(BYTE* apdu_val)
{
    if (PhdComState == PHD_COM_STATE_ASSOCIATING)
    {
        PhdComState = PHD_COM_STATE_UNASSOCIATED;
		USBDevicePHDCSendData(PHDC_BULK_IN_QOS,(UINT8 *) ASSOCIATION_RESPONSE_REJECTED_PERMANENT, ASSOCIATION_RESPONSE_REJECTED_PERMANENT_SIZE, MEM_ROM);
		USBDevicePHDCReceiveData(PHDC_BULK_OUT_QOS,pPhdAppBuffer,0); //get ready to receive
    }
    else if ((PhdComState == PHD_COM_STATE_ASSOC_OPERATING) || (PhdComState == PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL) || (PhdComState ==  PHD_COM_STATE_DISASSOCIATING))
    {
        PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
    }
}

/******************************************************************************
 * Function:        static void PHDAssocResponseHandler(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles association response
 *
 * Note:
 *
 *****************************************************************************/
static void PHDAssocResponseHandler(BYTE* apdu_val)
{
	UINT16_VAL apduChoiceLength, apduChoiceType,result;
    BYTE* assoc_response_res;

    PhdAssociationRequestTimeoutStatus = TIMEOUT_DISABLED;

    apduChoiceType.byte.HB = apdu_val[0];
	apduChoiceType.byte.LB = apdu_val[1];

	apduChoiceLength.byte.HB = apdu_val[2];
	apduChoiceLength.byte.LB = apdu_val[3];

	if ((PhdComState == PHD_COM_STATE_ASSOC_OPERATING) || (PhdComState == PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL)|| (PhdComState ==  PHD_COM_STATE_DISASSOCIATING))
    {
        PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
        return;
    }

    if((PhdComState== PHD_COM_STATE_ASSOCIATING) && (apduChoiceType.Val == AARE_CHOSEN))
	{
		assoc_response_res = &apdu_val[4];
		result.byte.HB  = apdu_val[4];
		result.byte.LB  = apdu_val[5];
		if(result.Val == ACCEPTED_UNKNOWN_CONFIG)
		{
			PhdComState=PHD_COM_STATE_ASSOC_CFG_SENDING_CONFIG;
			USBDevicePHDCSendData(PHDC_BULK_IN_QOS,(UINT8 *)CONFIG_EVENT_REPORT,CONFIG_EVENT_REPORT_SIZE,MEM_ROM);
		}
		else
		{
			PhdComState=PHD_COM_STATE_ASSOC_OPERATING;
			AppCB(PHD_CONNECTED);
		}
	}
}

/******************************************************************************
 * Function:        static void PHDPrstApduHandler(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles prst APDU
 *
 * Note:
 *
 *****************************************************************************/
static void PHDPrstApduHandler(BYTE* apdu_val)
{
    UINT16_VAL data,result,invoke_id, handle,apduChoiceLength, eventType, actionType;

	apduChoiceLength.byte.HB = apdu_val[PHD_APDU_LENGTH_FIELD];
	apduChoiceLength.byte.LB = apdu_val[PHD_APDU_LENGTH_FIELD + 1];

	data.byte.HB = apdu_val[8];
	data.byte.LB = apdu_val[9];

	invoke_id.byte.HB = apdu_val[6];
    invoke_id.byte.LB = apdu_val[7];

    if (apduChoiceLength.Val > PHDC_MAX_APDU_SIZE)
	{
        PHDSendRoerMessageToManager(invoke_id.Val, PROTOCOL_VIOLATION);
    }
	if (PhdComState ==  PHD_COM_STATE_DISASSOCIATING)
    {
        if ((data.Val == ROER_CHOSEN) || (data.Val = RORJ_CHOSEN))
        {
            PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
        }
    }
    else if (PhdComState==PHD_COM_STATE_ASSOCIATING)
    {
        PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
    }
    else if ((PhdComState == PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL)|| (PhdComState==PHD_COM_STATE_ASSOC_CFG_SENDING_CONFIG))
    {
        if(data.Val == RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)//response for CONFIG_EVENT (sent by Manager)
		{
			result.byte.HB = apdu_val[24];
			result.byte.LB = apdu_val[25];
			if(result.Val == ACCEPTED_CONFIG)
    		{
			    PhdComState=PHD_COM_STATE_ASSOC_OPERATING; //change the state if Accepted config
				AppCB(PHD_CONNECTED);
				PhdConfigurationTimeoutStatus = TIMEOUT_DISABLED;
			}
			else if (result.Val == UNSUPPORTED_CONFIG)
    		{
				PhdComState = PHD_COM_STATE_DISASSOCIATING;
			    PHDSendReleaseRequestToManager(RELEASE_REQUEST_REASON_NO_MORE_CONFIGURATIONS);
			}
		}
		else if(data.Val == ROIV_CMIP_GET_CHOSEN) //Get command from Manager
		{
    		handle.byte.HB = apdu_val[12];
    		handle.byte.LB = apdu_val[13];
    		if(handle.Val == 0)
    		{
		       PHDMdsAttributesRequestHandler(apdu_val);
            }
            else
            {
                //Send ROER error.
                PHDSendRoerMessageToManager( invoke_id.Val, NO_SUCH_OBJECT_INSTANCE);
            }
		}
		else if  ((data.Val == ROIV_CMIP_EVENT_REPORT_CHOSEN)
		        ||(data.Val == ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
		        ||(data.Val == ROIV_CMIP_SET_CHOSEN)
		        ||(data.Val == ROIV_CMIP_CONFIRMED_SET_CHOSEN)
		        ||(data.Val == ROIV_CMIP_ACTION_CHOSEN))
		        //||(data.Val == ROIV_CMIP_CONFIRMED_ACTION_CHOSEN))
	    {
    	    //respond with ROER error.
    	    PHDSendRoerMessageToManager( invoke_id.Val, NO_SUCH_OBJECT_INSTANCE);
    	}
		else
		{
    		PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
        }
    }
	else if (PhdComState==PHD_COM_STATE_ASSOC_OPERATING)
    {
        if(data.Val == ROIV_CMIP_GET_CHOSEN) //Get command from Manager
		{
    		handle.byte.HB = apdu_val[12];
    		handle.byte.LB = apdu_val[13];
    		if(handle.Val == 0)
    		{
		       PHDMdsAttributesRequestHandler(apdu_val);
            }
            else
            {
                //Send ROER error.
                PHDSendRoerMessageToManager( invoke_id.Val, NO_SUCH_OBJECT_INSTANCE);
            }
		}
		else if (data.Val == RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
		{
    		eventType.byte.HB = apdu_val[18];
    		eventType.byte.LB = apdu_val[19];

    		if (eventType.Val == MDC_NOTI_SCAN_REPORT_FIXED)
    		{
                //We have received the response for the measurement that we sent.
                PhdConfirmTimeoutStatus = TIMEOUT_DISABLED;
                AppCB(PHD_MEASUREMENT_SENT); 
            }
		}
		else if (data.Val == ROIV_CMIP_CONFIRMED_ACTION_CHOSEN)
		{
    		actionType.byte.HB = apdu_val[14];
    		actionType.byte.LB = apdu_val[15];
    		if (actionType.Val == MDC_ACT_SET_TIME)
    		{
        		PHDSetTime(apdu_val);
        		PhdTimeStateAbsSynced = 1;
        		pPhdAppBuffer[0] = 0xE7; pPhdAppBuffer[1] = 0x00;  /* APDU CHOICE Type (AbortApdu) */
                pPhdAppBuffer[2] = 0x00; pPhdAppBuffer[3] = 0x0E;  /* CHOICE.length = 14 */
                pPhdAppBuffer[4] = 0x00; pPhdAppBuffer[5] = 0x0C;  /* Octect String Length */
                pPhdAppBuffer[6]= (UINT8)(invoke_id.Val>>8); pPhdAppBuffer[7]= (UINT8)invoke_id.Val;
                pPhdAppBuffer[8] = 0x02; pPhdAppBuffer[9] = 0x07;  /*rors-cmip-confirmed-action */
                pPhdAppBuffer[10] = 0x00; pPhdAppBuffer[11] = 0x06;
                pPhdAppBuffer[12] = 0x00; pPhdAppBuffer[13] = 0x00;
                pPhdAppBuffer[14] = 0x0C; pPhdAppBuffer[15] = 0x17;
                pPhdAppBuffer[16] = 0x00; pPhdAppBuffer[17] = 0x00;
                USBDevicePHDCSendData(PHDC_BULK_IN_QOS,pPhdAppBuffer,18,MEM_RAM);
            }
            else
            {
    		    //respond with ROER error.
    		    PHDSendRoerMessageToManager( invoke_id.Val, NO_SUCH_ACTION);
    		}
        }
        else if (data.Val == ROIV_CMIP_SET_CHOSEN)
        {
            //respond with ROER error.
    		PHDSendRoerMessageToManager( invoke_id.Val, NO_SUCH_ACTION);
        }
        else if (data.Val == RORS_CMIP_GET_CHOSEN)
        {
            PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
        }
    }
}

/******************************************************************************
 * Function:        static void PHDReleaseResponseHandler(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles Release response
 *
 * Note:
 *
 *****************************************************************************/
static void PHDReleaseResponseHandler(BYTE* apdu_val)
{
    if ((PhdComState==PHD_COM_STATE_ASSOCIATING)
        || (PhdComState == PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL)
        || (PhdComState ==  PHD_COM_STATE_DISASSOCIATING)
        || (PhdComState == PHD_COM_STATE_ASSOC_OPERATING))
	{
    	PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
    }
}

/******************************************************************************
 * Function:        static void PHDReleaseRequestHandler(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles Release request from manager
 *
 * Note:
 *
 *****************************************************************************/
static void PHDReleaseRequestHandler(BYTE* apdu_val)
{
	if ((PhdComState==PHD_COM_STATE_ASSOCIATING))
	{
    	PHDSendAbortRequestToManager(ABORT_REASON_UNDEFINED);
    }
    else if (PhdComState ==  PHD_COM_STATE_DISASSOCIATING)
    {
        USBDevicePHDCSendData(PHDC_BULK_IN_QOS, (UINT8 *)RELEASE_RESPONSE,RELEASE_RESPONSE_SIZE,MEM_ROM);
    }
	else if ((PhdComState==PHD_COM_STATE_ASSOC_OPERATING)
	         || (PhdComState==PHD_COM_STATE_ASSOC_CFG_SENDING_CONFIG)
	         || (PhdComState == PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL))
	{
		PhdComState=PHD_COM_STATE_UNASSOCIATED;
		USBDevicePHDCSendData(PHDC_BULK_IN_QOS, (UINT8 *)RELEASE_RESPONSE,RELEASE_RESPONSE_SIZE,MEM_ROM);
		AppCB(PHD_DISCONNECTED);
	}
	PhdAssociationReleaseTimeoutStatus = TIMEOUT_DISABLED;
}

/******************************************************************************
 * Function:        static void PHDAbortRequestHandler(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles Abort request from manager
 *
 * Note:
 *
 *****************************************************************************/
static void PHDAbortRequestHandler(BYTE* apdu_val)
{
		PhdComState=PHD_COM_STATE_UNASSOCIATED;
		AppCB(PHD_DISCONNECTED);
		PHDDisableAllTimeout();
}

/******************************************************************************
 * Function:        static void PHDSendAbortRequestToManager(UINT16 abortReason)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine Sends abort request to Manager.
 *
 * Note:
 *
 *****************************************************************************/
static void PHDSendAbortRequestToManager(UINT16 abortReason)
{
   pPhdAppBuffer[0] = 0xE6;  /* APDU CHOICE Type (AbortApdu) */
   pPhdAppBuffer[1] = 0x00;
   pPhdAppBuffer[2] = 0x00;  /* CHOICE.length = 2 */
   pPhdAppBuffer[3] = 0x02;
   pPhdAppBuffer[4] = (UINT8)abortReason>>8;  /* reason */
   pPhdAppBuffer[5] = (UINT8)abortReason;

   PhdComState = PHD_COM_STATE_UNASSOCIATED;
   USBDevicePHDCSendData(PHDC_BULK_IN_QOS,(UINT8 *) pPhdAppBuffer,ABORT_SIZE,MEM_RAM);
   AppCB(PHD_DISCONNECTED);
   PHDDisableAllTimeout();

}

/******************************************************************************
 * Function:        void PHDSendReleaseRequestToManager(UINT16 releaseReason)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine sends Release request to Manager
 *
 * Note:
 *
 *****************************************************************************/
void PHDSendReleaseRequestToManager(UINT16 releaseReason)
{
   pPhdAppBuffer[0] = 0xE4;  /* APDU CHOICE Type (RlrqApdu) */
   pPhdAppBuffer[1] = 0x00;
   pPhdAppBuffer[2] = 0x00;  /* CHOICE.length = 2 */
   pPhdAppBuffer[3] = 0x02;
   pPhdAppBuffer[4] = (UINT8)releaseReason>>8;  /* reason */
   pPhdAppBuffer[5] = (UINT8)releaseReason;

   PhdComState= PHD_COM_STATE_DISASSOCIATING;
   USBDevicePHDCSendData(PHDC_BULK_IN_QOS,(UINT8 *) pPhdAppBuffer,RELEASE_REQUEST_SIZE,MEM_RAM);

   PhdAssociationReleaseTimeoutStatus = TIMEOUT_ENABLED;
   PhdAssociationReleaseTimeout = ASSOCIATION_RELEASE_TIMOUT;
}

/******************************************************************************
 * Function:        void PHDSendRoerMessageToManager (UINT16 invoke_id, UINT16 error_value)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles Abort request from manager
 *
 * Note:
 *
 *****************************************************************************/
void PHDSendRoerMessageToManager (UINT16 invoke_id, UINT16 error_value)
{
    pPhdAppBuffer[0] = 0xE7;  pPhdAppBuffer[1] = 0x00;
    pPhdAppBuffer[2] = 0x00;  pPhdAppBuffer[3] = 0x0C;
    pPhdAppBuffer[4] = 0x00;  pPhdAppBuffer[5] = 0x0A;
    pPhdAppBuffer[6]= (UINT8)(invoke_id>>8); pPhdAppBuffer[7]= (UINT8)invoke_id;
    pPhdAppBuffer[8] = 0x03;  pPhdAppBuffer[9] = 0x00;
    pPhdAppBuffer[10] = 0x00;  pPhdAppBuffer[11] = 0x04;
    pPhdAppBuffer[12] = (UINT8)(error_value>>8);  pPhdAppBuffer[13] = (UINT8)error_value;
    pPhdAppBuffer[14] = 0x00;  pPhdAppBuffer[15] = 0x00;
    USBDevicePHDCSendData(PHDC_BULK_IN_QOS,pPhdAppBuffer,16,MEM_RAM);
}

/******************************************************************************
 * Function:
 *      void PHDTimeoutHandler(void)
 *
 * Summary:
 *      This function is used to handle all timeout.
 *
 * Description:
 *       This function handles all timers. This function should be called once in every milli Second.
 *
 * Conditions:
 *       None
 *
 * Parameters:
 *	None
 *
 * Return:
 *	None
 *
 * Side Effects:
 *	None
 *
 * Remarks:
 *      If USB is used at the Transport layer then the USB SOF handler can call this function.
 *
 *****************************************************************************/
 void PHDTimeoutHandler(void)
 {
    if ((PhdAssociationRequestTimeoutStatus == TIMEOUT_ENABLED) && (PhdComState == PHD_COM_STATE_ASSOCIATING))
    {
        PhdAssociationRequestTimeout--;
        if (PhdAssociationRequestTimeout <= TIMEOUT_EXPIRED)
        {
            if (PhdAssociationRequestRetry > 0)
            {
                PhdAssociationRequestRetry--;
                PhdComState= PHD_COM_STATE_ASSOCIATING;
		        USBDevicePHDCSendData(PHDC_BULK_IN_QOS,(UINT8 *) ASSOCIATION_REQUEST,ASSOCIATION_REQUEST_SIZE,MEM_ROM);
		        USBDevicePHDCReceiveData(PHDC_BULK_OUT_QOS,pPhdAppBuffer,0); //get ready to receive
		        PhdAssociationRequestTimeoutStatus = TIMEOUT_ENABLED;
		        PhdAssociationRequestTimeout = ASSOCIATION_REQUEST_TIMEOUT;
		    }
		    else
		    {
    		    PhdAssociationRequestTimeoutStatus = TIMEOUT_DISABLED;
    		    PHDSendAbortRequestToManager(ABORT_REASON_RESPONSE_TIMEOUT);
    		}
        }
    }
    if ((PhdConfigurationTimeoutStatus == TIMEOUT_ENABLED) && (PhdComState == PHD_COM_STATE_ASSOC_CFG_WAITING_APPROVAL))
    {
        PhdConfigurationTimeout--;
        if (PhdConfigurationTimeout <= TIMEOUT_EXPIRED)
        {
            PhdConfigurationTimeoutStatus = TIMEOUT_DISABLED;
            PHDSendAbortRequestToManager(ABORT_REASON_RESPONSE_TIMEOUT);
        }
    }

    if (PhdAssociationReleaseTimeoutStatus == TIMEOUT_ENABLED)
    {
        PhdAssociationReleaseTimeout--;
        if (PhdAssociationReleaseTimeout <= TIMEOUT_EXPIRED)
        {
            PhdAssociationReleaseTimeoutStatus = TIMEOUT_DISABLED;
            PHDSendAbortRequestToManager(ABORT_REASON_RESPONSE_TIMEOUT);
        }
    }
    if (PhdConfirmTimeoutStatus == TIMEOUT_ENABLED)
    {
         PhdConfirmTimeout--;
         if (PhdConfirmTimeout <= TIMEOUT_EXPIRED)
         {
             PhdConfirmTimeoutStatus = TIMEOUT_DISABLED;
             PHDSendAbortRequestToManager(ABORT_REASON_RESPONSE_TIMEOUT);
         }
    }
 }

/******************************************************************************
 * Function:         static void PHDDisableAllTimeout(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Disables all timeout
 *
 * Note:
 *
 *****************************************************************************/
 static void PHDDisableAllTimeout(void)
 {
	PhdAssociationRequestTimeoutStatus = TIMEOUT_DISABLED;
	PhdConfigurationTimeoutStatus = TIMEOUT_DISABLED;
	PhdAssociationReleaseTimeoutStatus = TIMEOUT_DISABLED;
	PhdConfirmTimeoutStatus = TIMEOUT_DISABLED;
 }


 /******************************************************************************
 * Function:         static void PHDMdsAttributesRequestHandler(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Disables all timeout
 *
 * Note:
 *
 *****************************************************************************/
 static void PHDMdsAttributesRequestHandler(BYTE* apdu_val)
 {
     UINT16_VAL count,invoke_id;
	 UINT16 i;
	 POINTER Src;
	 #ifdef PHD_USE_RTCC_FOR_TIME_STAMP
	    rtccTimeDate TimeAndDate;
     #endif
     invoke_id.byte.HB = apdu_val[6];
     invoke_id.byte.LB = apdu_val[7];

     count.byte.HB =  apdu_val[14];
     count.byte.LB =  apdu_val[15];

	 if(count.Val == 0)
	 {
	     Src.bRom = MDS_ATTRIBUTES;//reply with all MDS attributes
		 for(i=0;i<MDS_ATTRIBUTES_SIZE;i++)
		 {
		     pPhdAppBuffer[i] = *Src.bRom++;
		 }
		 pPhdAppBuffer[6]= (UINT8)((invoke_id.Val>>8)&0xff);
		 pPhdAppBuffer[7]= (UINT8)(invoke_id.Val&0xff);
		 #ifdef PHD_USE_RTCC_FOR_TIME_STAMP
		    RtccReadTimeDate(&TimeAndDate);        //Rtcc_read_TimeDate will have latest time
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER]     = 0x20;
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER + 1] = TimeAndDate.f.year;
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER + 2] = TimeAndDate.f.mon;
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER + 3] = TimeAndDate.f.mday;
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER + 4] = TimeAndDate.f.hour;
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER + 5] = TimeAndDate.f.min;
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER + 6] = TimeAndDate.f.sec;
		    pPhdAppBuffer[PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER + 7] = 0x00;
		 #endif
		 if (PhdTimeStateAbsSynced == 1)
		 {
		   pPhdAppBuffer[PHD_MDS_ATTR_ABS_SYNCED_POINTER] = 0xA0;
		 }
		 else
		 {
		   pPhdAppBuffer[PHD_MDS_ATTR_ABS_SYNCED_POINTER] = 0x20;
		 }
		 USBDevicePHDCSendData(PHDC_BULK_IN_QOS,pPhdAppBuffer,MDS_ATTRIBUTES_SIZE,MEM_RAM);
	 }
	 else
	 {
        // The Manager requests for a specific MDS attribute.
        //respond with ROER error.
        PHDSendRoerMessageToManager( invoke_id.Val, NOT_ALLOWED_BY_OBJECT);
     }
 }

 /******************************************************************************
 * Function:        void PHDSetTime(BYTE* apdu_val)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Disables all timeout
 *
 * Note:
 *
 *****************************************************************************/
 void PHDSetTime(BYTE* apdu_val)
 {
     #ifdef PHD_USE_RTCC_FOR_TIME_STAMP
        rtccTimeDate RtccTimeDate ;
        RtccTimeDate.f.year =  apdu_val[19];     //set year
        RtccTimeDate.f.mon  =  apdu_val[20];     //Se month
        RtccTimeDate.f.mday =  apdu_val[21];     //Set day
        RtccTimeDate.f.hour =  apdu_val[22];     //Set Hour
        RtccTimeDate.f.min  =  apdu_val[23];     //Set minute
        RtccTimeDate.f.sec  =  apdu_val[24];    //Set second
        RtccWriteTimeDate(&RtccTimeDate,1);      //write into registers
     #endif
  }

 /** EOF phdc_com_main.c *************************************************/
