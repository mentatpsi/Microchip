/********************************************************************
 FileName:      Weighing Scale app.c
 Dependencies:  See INCLUDES section
 Processor:		PIC18, PIC24, and PIC32 USB Microcontrollers
 Hardware:		This demo is natively intended to be used on Microchip USB demo
 				boards supported by the MCHPFSUSB stack.  See release notes for
 				support matrix.  This demo can be modified for use on other hardware
 				platforms.
 Complier:  	Microchip C18 (for PIC18), C30 (for PIC24), C32 (for PIC32)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the ï¿½Companyï¿½) for its PICï¿½ Microcontroller is intended and
 supplied to you, the Companyï¿½s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN ï¿½AS ISï¿½ CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Initial release
  2.0 

********************************************************************/

/** I N C L U D E S **********************************************************/

#include "usb_config.h"
#include "USB/usb_function_phdc.h"
#include "USB/usb.h"
#include "USB/usb_function_phdc_com_model.h"
#include "Weighing Scale app.h"
#include "phd_config.h"
#include "HardwareProfile.h"
#ifdef PHD_USE_RTCC_FOR_TIME_STAMP
    #include "rtcc.h"
#endif

/** Definitions ****************************************************/


/*****************************************************************************
 * Constant and Macro's
 *****************************************************************************/




/* Association Request. The agent sends the following message to the manager. This example assumes the agent is an extended
weighing scale. */

UINT8  ROM ASSOCIATION_REQUEST[ASSOCIATION_REQUEST_SIZE] = {
  
0xE2, 0x00,                         /* APDU CHOICE Type (AarqApdu) */
0x00, 0x32,                         /* CHOICE.length = 50 */
0x80, 0x00, 0x00, 0x00,             /* assoc-version = assoc-version1 */
0x00, 0x01, 0x00, 0x2A,             /* data-proto-list.count=1 | length=42*/
0x50, 0x79,                         /* data-proto-id = 20601 */
0x00, 0x26,                         /* data-proto-info length = 38 */
0x80, 0x00, 0x00, 0x00,             /* protocolVersion */
0xA0, 0x00,                         /* encoding rules = MDER or PER */
0x80, 0x00, 0x00, 0x00,             /* nomenclatureVersion */
0x00, 0x00, 0x00, 0x00,             /* functionalUnits | no test association capabilities */
0x00, 0x80, 0x00, 0x00,             /* systemType = sys-type-agent */
0x00, 0x08,                         /* system-id length = 8 and value
                                     (manufacturer- and device- specific) */
0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,

0x05, 0xDC,                         /* dev-config-id | standard configuration - Weighing Scale(1500) */
0x00, 0x01,                         /* data-req-mode-flags */
0x01, 0x00,                         /* data-req-init-agent-count,
                                      data-req-init-manager-count */
0x00, 0x00, 0x00, 0x00              /* Atribute list */
};



/* Configuration information exchange. The agent reports its configuration to the manager */

UINT8  ROM CONFIG_EVENT_REPORT[CONFIG_EVENT_REPORT_SIZE] = {
0xE7, 0x00,                           /* APDU CHOICE Type (PrstApdu) */
BREAK_WORD_INTO_BYTES(72),            /* CHOICE.length = 72 */
BREAK_WORD_INTO_BYTES(70),            /* OCTET STRING.length = 70 */
0x00, 0x02,                           /*  invoke-id = 0x1235 (start of DataApdu
                                          . MDER encoded.) */
0x01, 0x01,                           /*  CHOICE(Remote Operation Invoke |
                                          Confirmed Event Report) */
BREAK_WORD_INTO_BYTES(64),            /*  CHOICE.length = 64 */
0x00, 0x00,                           /*  obj-handle = 0 (MDS object) */
0xFF, 0xFF, 0xFF, 0xFF,               /*  event-time = 0xFFFFFFFF    */
0x0D, 0x1C,                           /*  event-type = MDC_NOTI_CONFIG */
BREAK_WORD_INTO_BYTES(54),            /*  event-info.length = 54 (start of ConfigReport) */
0x05, 0xDC,                           /*  config-report-id = 1500 Standard Weighing Scale */
0x00, 0x01,                           /*  config-obj-list.count = 1 Measurement objects will be announced */              
BREAK_WORD_INTO_BYTES(44),            /*  config-obj-list.length = 44 */

//Measurement Object 1 (44 Bytes) 
0x00, 0x06,                           /*  obj-class = MDC_MOC_VMO_METRIC_NU */
0x00, 0x01,                           /*  obj-handle = 1 (.. 1st Measurement is body weight) */
0x00, 0x04,                           /*  attributes.count = 4  */
0x00, 0x24,                           /*  attributes.length = 36  */

// Attribute 1 (8 Bytes) 
0x09, 0x2F,                           /*  attribute-id = MDC_ATTR_ID_TYPE */ 
0x00, 0x04,                           /*  attribute-value.length = 4      */
0x00, 0x02, 0xE1, 0x40,               /*  0xE1 0x40 MDC_PART_SCADA |
                                          MDC_MASS_BODY_ACTUAL */
//Attribute 2 (6 Bytes)                                          
0x0A, 0x46,                           /*  attribute-id = MDC_ATTR_METRIC_SPEC_SMALL      */
0x00, 0x02,                           /*  attribute-value.length = 2  */
0xF0, 0x40,                           /*  intermittent, stored data, upd & msmt
                                          aperiodic, agent init, measured */
//Attribute 3  (6 Bytes)                                      
0x09, 0x96,                           /*  attribute-id = MDC_ATTR_UNIT_CODE */
0x00, 0x02,                           /*  attribute-value.length = 2 */
0x06, 0xC3,                           /*  MDC_DIM_KILO_G             */

//Attribute 4 (16 bytes) 
0x0A, 0x55,                           /*  attribute-id = MDC_ATTR_ATTRIBUTE_VAL_MAP  */
0x00, 0x0C,                           /*  attribute-value.length = 12     */
0x00, 0x02,                           /*  AttrValMap.count = 2         */
0x00, 0x08,                           /*  AttrValMap.length = 8          */
0x0A, 0x56, 0x00, 0x04,               /*  MDC_ATTR_NU_VAL_OBS_SIMP |value length = 4   */
0x09, 0x90, 0x00, 0x08,               /*  MDC_ATTR_TIME_STAMP_ABS | value length = 8     */
};



/* Get Attributes Response. The agent responds to the manager with its attributes. This example assumes the agent supports
AbsoluteTime, but does not support RelativeTime. Further, some optional fields are communicated as well. */
UINT8  ROM MDS_ATTRIBUTES[MDS_ATTRIBUTES_SIZE] = {
0xE7, 0x00,                      /* APDU CHOICE Type (PrstApdu) */
BREAK_WORD_INTO_BYTES(156),      /* CHOICE.length = 136 */
BREAK_WORD_INTO_BYTES(154),      /* OCTET STRING.length = 134 */
0x00, 0x05,                      /* invoke-id = 5 (mirrored from request)*/
0x02, 0x03,                      /* CHOICE (Remote Operation Response | Get)*/
BREAK_WORD_INTO_BYTES(148),      /* CHOICE.length = 128 */
0x00, 0x00,                      /* handle = 0 (MDS object)                */
0x00, 0x08,                      /* attribute-list.count = 7               */
BREAK_WORD_INTO_BYTES(142),      /* attribute-list.length = 122              */


//MDC_ATTR_SYS_TYPE_SPEC_LIST (Total Length = 12 Bytes)
0x0A, 0x5A,                      /* attribute id=MDC_ATTR_SYS_TYPE_SPEC_LIST */
0x00, 0x08,                      /* attribute-value.length = 8              */
0x00, 0x01,                      /* TypeVerList count = 1                   */
0x00, 0x04,                      /* TypeVerList length = 4                  */
0x10, 0x0F,                      /* type = MDC_DEV_SPEC_PROFILE_SCALE     */
0x00, 0x01,                      /* version=ver 1 of the specialization     */

//MDC_ATTR_ID_MODEL (Total Length = 30 Bytes)
0x09, 0x28,                      /* attribute-id = MDC_ATTR_ID_MODEL    */
0x00, 0x1A,                      /* attribute-value.length = 26        */
0x00, 0x0A,                      /* string length = 10 | "Microchip "  */ 
'M', 'i', 'c', 'r', 'o', 'c', 'h', 'i' ,'p', ' ',          
0x00, 0x0C,                      /* string length = 12 | "Weigh Scale "  */
'W', 'e', 'i', 'g', 'h', ' ', 'S', 'c', 'a', 'l', 'e', ' ', 

//MDC_ATTR_SYS_ID (Total Length = 14 Bytes)
0x09, 0x84,                      /* attribute-id = MDC_ATTR_SYS_ID        */
0x00, 0x0A,                      /* attribute-value.length = 10            */
0x00, 0x08, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                                 /* octet string length = 8 | EUI-64  */
                                 
//MDC_ATTR_DEV_CONFIG_ID (Total Length = 6 Bytes)                                 
0x0A, 0x44,                      /* attribute-id = MDC_ATTR_DEV_CONFIG_ID   */
0x00, 0x02,                      /* attribute-value.length = 2             */
0x05, 0xDC,                      /* dev-config-id = 1500 Weighing Scale */

//MDC_ATTR_ID_PROD_SPECN (Total Length = 22 Bytes)
0x09,  0x2D,                      /* attribute-id = MDC_ATTR_ID_PROD_SPECN */ 
0x00,  0x12,                      /* attribute-value.length = 18 */  
0x00,  0x01,                      /* ProductionSpec.count = 1 */ 
0x00,  0x0E,                      /* ProductionSpec.length = 14  */ 
0x00,  0x01,                      /* ProdSpecEntry.spec-type = 1 (serial-number) */ 
0x00,  0x00,                      /* ProdSpecEntry.component-id = 0 */ 
0x00,  0x08,                      /* string length = 8 */ 
0x44, 0x45, 0x31,  0x32, 0x34, 0x35, 0x36,  0x37, 
                                 /* prodSpecEntry.prod-spec = “DE124567” */ 
                                 
//MDC_ATTR_REG_CERT_DATA_LIST (Total Length = 26 Bytes) 
0x0A, 0x4B,  /* attribute-id = MDC_ATTR_REG_CERT_DATA_LIST */ 
0x00, 0x16,  /* arrtibute-value.length = 16 */ 
0x00, 0x02,  /* RegCertDatalist.count = 1 */ 
0x00, 0x12,  /* RegCertDatalist.length = 12 */ 
0x02, 0x01,  /* auth-body = auth-body-continua (2) | auth-body-struc-type = continua-version-struct(1) */ 
0x00, 0x08,  /* length of auth-body-data */ 
0x01, 0x05,  /* major-IG-version = 1 | minor-IG-version = 0 */
0x00, 0x01,  /* CertfiedDeviceClassList.count = 1; */
0x00, 0x02,  /* CerifiedDeviceClassList.Length = 2; */ 
0x20, 0x0F,  /* CertifiedDeviceClassEntry =  MDC_DEV_SPEC_PROFILE_SCALE - 4096 + v_i_TCode * 8192 = 0x200F 
                where,  MDC_DEV_SPEC_PROFILE_SCALE = 4111, v_i_TCode = 1 for USB Agent */   
0x02, 0x02,  /* auth-body = auth-body-continua (2) | auth-body-struc-type = continua-reg-struct(2) */ 
0x00, 0x02,  /* length of auth-body-data */ 
0x00, 0x00,  /* RegulationBitField = unregulated device (0) */ 

//MDC_ATTR_MDS_TIME_INFO (Total Length = 20 Bytes)
0x0A, 0x45, /* attribute-id = MDC_ATTR_MDS_TIME_INFO */ 
0x00, 0x10, /* arrtibute-value.length = 16 */
0xC0, 0x20, /* MdsTimeCapState = mds-time-capab-real-time-clock | mds-time-capab-set-clock | mds-time-capab-sync-abs-time | 
               mds-time-state-abs-time-synced | mds-time-mgr-set-time */ 
0x1F, 0x00, /* time_sync_protocol = MDC_TIME_SYNC_NONE */ 
0xFF, 0xFF, 0xFF, 0xFF, /* RelativeTime time_sync_accuracy */ 
0x00, 0x00, /* intu16 time_resolution_abs_time */ 
0x00, 0x00, /* intu16 time_resolution_rel_time */ 
0x00, 0x00, 0x00, 0x00, /* intu32 time_resolution_high_res_time */ 
            

//MDC_ATTR_TIME_ABS (Total Length = 12 Bytes) //if 
0x09, 0x87,                      /* attribute-id = MDC_ATTR_TIME_ABS */
0x00, 0x08,                      /* attribute-value.length = 8           */
0x20, 0x11, 0x11, 0x18,          /* Absolute-Time-Stamp=2008-05-06T08:00:0000*/
0x14, 0x39, 0x00, 0x00


};

/* Agent-initiated measurement data transmission. The agent sends a spontaneous event report to the 
   manager with measurement observations */
UINT8  ROM MEASUREMENT_DATA[MEASUREMENT_DATA_SIZE] = {
0xE7, 0x00, /*APDU CHOICE Type (PrstApdu)*/
0x00, 0x2A,  /*CHOICE.length = 42*/
0x00, 0x28, /*OCTET STRING.length = 40*/
0x12, 0x36, /*invoke-id = 0x1236*/
0x01, 0x01, /*CHOICE(Remote Operation Invoke | Confirmed Event Report)*/
0x00, 0x22, /*CHOICE.length = 34*/
0x00, 0x00, /*obj-handle = 0 (MDS object)*/
0xFF, 0xFF, 0xFF, 0xFF, /*event-time = 0*/
0x0D, 0x1D, /*event-type = MDC_NOTI_SCAN_REPORT_FIXED*/
0x00, 0x18, /*event-info.length = 24*/
0xF0, 0x00, /*ScanReportInfoFixed.data-req-id = 0xF000*/
0x00, 0x00, /*ScanReportInfoFixed.scan-report-no = 0*/
0x00, 0x01, /*ScanReportInfoFixed.obs-scan-fixed.count = 1*/
0x00, 0x10, /*ScanReportInfoFixed.obs-scan-fixed.length = 16*/
0x00, 0x01, /*ScanReportInfoFixed.obs-scan-fixed.value[0].obj-handle = 1*/
0x00, 0x0C, /*ScanReportInfoFixed.obs-scan-fixed.value[0]. obs-val-data.length = 12*/
0xFF, 0x00, 0x02, 0xFA, /*Simple-Nu-Observed-Value = 76.2 (kg)*/
0x20, 0x11, 0x04, 0x06, /*Absolute-Time-Stamp = 2011-04-06T12:10:0000*/
0x12, 0x10, 0x00, 0x00,
};


/** V A R I A B L E S ********************************************************/

#if defined(__18CXX)
    #pragma udata USB_PHDC_BUFFER
#endif 
UINT8 PhdAppBuffer[PHD_APP_BUFFER_SIZE];

#if defined(__18CXX)
    #pragma udata
#endif

volatile BOOL buttonPressedsw2;
volatile BOOL buttonPressedsw3;
volatile BYTE buttonCountsw2;
volatile BYTE buttonCountsw3;
BYTE sw2ButtonPressCounter = 0; 
 
BYTE AppState; 
BOOL connectionUnderProgress = FALSE,dataSendingUnderProgress = FALSE;
UINT16_VAL scanReportNo;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void PhdWeighingScaleUpdateMeasurements(void); 
static void PhdComToAppCb(UINT8);
WORD_VAL ReadPOT(void);

/******************************************************************************
 * Function:        void ApplicationInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine register all the application related items.
 *                  Also a call back function is sent to the PHD_COM layer.
                    This routine also passes a pointer to the Application buffer. 
 *
 * Note:            
 *
 *****************************************************************************/
void ApplicationInit()
{
    AppState = PHD_DISCONNECTED;
	PHDAppInit(PhdComToAppCb); //Register Call Back Function
	PHDSendAppBufferPointer(PhdAppBuffer);	// Pass the address of the application buffer to the lower layer. 
	sw2ButtonPressCounter =0; 
	scanReportNo.Val =0;  
}
/******************************************************************************
 * Function:        BYTE PHDGetAgentState(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Returns App state to the user. 
 *
 * Note:            
 *
 *****************************************************************************/
BYTE PHDGetAgentState(void)
{
    return AppState; 
}   
/******************************************************************************
 * Function:        void ApplicationTask(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine is called periodically from the main.
 *
 * Note:            
 *
 *****************************************************************************/
void ApplicationTask(void)
{
    
#if defined DEMO_BOARD_HAS_ONLY_ONE_PUSH_BUTTON
// Some of the demo boards has only one pushbutton. The demo works the following way for all the above demo boards. If the pushbutton is pressed for the first time
// then the Weig Scale Agent sends an Association request to the  Manager. If the push button is pressed for the second, third or fourth time the Agent sends some measured data 
// to the Manager. If the Pushbutton is pressed  for the Fifth time the Agent disconnects from the Manager and resets the bush button counter.   
    if (buttonPressedsw2)
    {
       if(dataSendingUnderProgress == FALSE)
       {
           sw2ButtonPressCounter++; 
	       if(AppState == PHD_DISCONNECTED)
		   {
		       PHDConnect();
		   }
		   else if(AppState == PHD_CONNECTED)
		   {
		       if (sw2ButtonPressCounter == 2)
		       {
    		         
    	           // if the push button is pressed for the second time then the agent sends Measured Data.
				   PhdWeighingScaleUpdateMeasurements(); 
		           PHDSendMeasuredData();
		       }
		       else if (sw2ButtonPressCounter == 3) 
		       {
    		       // if the push button is pressed for the Third time then the agent sends Measured Data.
    		       PhdWeighingScaleUpdateMeasurements(); 
    	           PHDSendMeasuredData();      
    	       }
    	       else if (sw2ButtonPressCounter == 4)
		       {
    		       // if the push button is pressed for the Fourth time then the agent sends Measured Data.
    		       PhdWeighingScaleUpdateMeasurements(); 
    	           PHDSendMeasuredData();      
    	       }	       
		       else if (sw2ButtonPressCounter == 5)
		       {	
    		       // if the push button is pressed for the Fifth time then the agent sends Measured Data.        
		   	       AppState = PHD_DISCONNECTING;
				   PHDDisConnect();
				   sw2ButtonPressCounter = 0; // reset Push Button Pressed Counter. 
			   }
		   }
		   dataSendingUnderProgress=TRUE;
		} 
	}
	else
	{
	    dataSendingUnderProgress=FALSE;
	}

#else //for all other demo boards that has atleast two push buttons
		if(buttonPressedsw2)
		{
			if(connectionUnderProgress == FALSE)
			{
    			connectionUnderProgress = TRUE;
				if(AppState == PHD_DISCONNECTED)
				{
					PHDConnect();
				}
				else if(AppState == PHD_CONNECTED)
				{
					AppState = PHD_DISCONNECTING;
					PHDDisConnect();		
				}
				connectionUnderProgress=TRUE;
			}
		}
		else
		{
			connectionUnderProgress=FALSE;
		}
		if(buttonPressedsw3)
		{
			if(dataSendingUnderProgress==FALSE)
			{
    			dataSendingUnderProgress=TRUE;
				if(AppState == PHD_CONNECTED)
				{
					PhdWeighingScaleUpdateMeasurements();
					PHDSendMeasuredData();
				}
				
			}
		}
		else
		{
			dataSendingUnderProgress=FALSE;
		}
 #endif 
}

/******************************************************************************
 * Function:        void PhdComToAppCb(UINT8 event)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine changes the application states based on the call back events
 *                   from the COM layer	
 *
 * Note:            
 *
 *****************************************************************************/
void PhdComToAppCb(UINT8 event)
{

	switch(event)
	{
		case PHD_CONNECTED:
		AppState = PHD_CONNECTED;
		break;
		
		case PHD_MEASUREMENT_SENT:
		AppState = PHD_CONNECTED;
		break;

		case PHD_DISCONNECTED:
		AppState = PHD_DISCONNECTED;
		break;
	}
	
}


/******************************************************************************
 * Function:        void UpdateWeighScaleMeasurements(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine updates the measured weigh scale values in to corresponding 
 *                  APDU locations	
 *
 * Note:            
 *
 *****************************************************************************/

void PhdWeighingScaleUpdateMeasurements(void)
{
    /* User can update the Measured value from the Weiging Scale*/
    /* Please go through IEEE_STD_11073_10417 (Weighing Scale device spec)*/
    UINT16_VAL Weight; 
    #if defined PHD_USE_RTCC_FOR_TIME_STAMP
        rtccTimeDate TimeStamp ; 
    #endif 
    POINTER Src;
    BYTE i; 
    
    /*************** Copy Measurement Date from Program Memory to RAM *******/ 
    Src.bRom = MEASUREMENT_DATA;
    for(i=0;i<MEASUREMENT_DATA_SIZE;i++)
	{
	    PhdAppBuffer[i] = *Src.bRom++; 	
	}
    
    /*********** Update Scan Report No **********************/ 
    PhdAppBuffer[24] = scanReportNo.byte.HB; //MSB 
	PhdAppBuffer[25] = scanReportNo.byte.LB; //LSB 
	scanReportNo.Val++; // Increment scanReportNo for the next transfer.
	
    /********* Update Weight data ******************/   
    Weight.Val = GetWeight();
    PhdAppBuffer[36]= Weight.byte.HB;  	//Measured analog voltage LSB
    PhdAppBuffer[37]= Weight.byte.LB;  	//Measured analog voltage MSB
    
    
    /************ Update Time Stamp **********************/ 
    #if defined PHD_USE_RTCC_FOR_TIME_STAMP
        RtccReadTimeDate(&TimeStamp);
        PhdAppBuffer[38] = 0x20;                    //Year 1
        PhdAppBuffer[39] = TimeStamp.f.year ;       //Year 2TimeStamp.f.year  
        PhdAppBuffer[40] = TimeStamp.f.mon;         // Month 
        PhdAppBuffer[41] = TimeStamp.f.mday;        // Day
        PhdAppBuffer[42] = TimeStamp.f.hour;        // Hour
        PhdAppBuffer[43] = TimeStamp.f.min;         // Minute
        PhdAppBuffer[44] = TimeStamp.f.sec;         // Seconds
        PhdAppBuffer[45] = 0x00;
    #else 
        /* Time Stamp  = 12:30:00, 20th July 2011*/ 
        PhdAppBuffer[38] = 0x20;           //Year 1
        PhdAppBuffer[39] = 0x11;           //Year 2
        PhdAppBuffer[40] = 0x11;           // Month 
        PhdAppBuffer[41] = 0x18;           // Day
        PhdAppBuffer[42] = 0x14;            // Hour
        PhdAppBuffer[43] = 0x39;           // Minute
        PhdAppBuffer[44] = 0x00;           // Seconds
        PhdAppBuffer[45] = 0x00;
    #endif     
}
/******************************************************************************
 * Function:        UINT16 GetWeight(void);
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine Gets the Weight simulated by Potentiometer.  	
 *
 * Note:            
 *
 *****************************************************************************/
UINT16 GetWeight(void)
{
  UINT16 weight;
  
  #if defined PHD_USE_POT_FOR_TEMP_SIMULATION 
    WORD_VAL w;   
    mInitPOT();
    w = ReadPOT();	//Use ADC to read the I/O pin voltage.  See the relevant HardwareProfile - xxxxx.h file for the I/O pin that it will measure.
					//Some demo boards, like the PIC18F87J50 FS USB Plug-In Module board, do not have a potentiometer (when used stand alone).
					//This function call will still measure the analog voltage on the I/O pin however.  To make the demo more interesting, it
					//is suggested that an external adjustable analog voltage should be applied to this pin.  
	weight = w.Val;  
  #else 
    weight = 0x280; //64.0 Kg
  #endif   
    return weight;  
}    

/******************************************************************************
 * Function:        static void PhdTimeoutHandlerApp(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles Time Out  	
 *
 * Note:            
 *
 *****************************************************************************/
void PhdTimeoutHandlerApp(void)
{
    PHDTimeoutHandler();
}  

/******************************************************************************
 * Function:        WORD_VAL ReadPOT(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          WORD_VAL - the 10-bit right justified POT value
 *
 * Side Effects:    ADC buffer value updated
 *
 * Overview:        This function reads the POT and leaves the value in the 
 *                  ADC buffer register
 *
 * Note:            None
 *****************************************************************************/
 #if defined PHD_USE_POT_FOR_TEMP_SIMULATION 
 WORD_VAL ReadPOT(void)
 {
    WORD_VAL w;

    #if defined(__18CXX)
        mInitPOT();

        ADCON0bits.GO = 1;              // Start AD conversion
        while(ADCON0bits.NOT_DONE);     // Wait for conversion

        w.v[0] = ADRESL;
        w.v[1] = ADRESH;

    #elif defined(__C30__) || defined(__C32__) || defined __XC16__
        #if defined(PIC24FJ256GB110_PIM) || \
            defined(PIC24FJ256DA210_DEV_BOARD) || \
            defined(PIC24FJ256GB210_PIM)
            AD1CHS = 0x5;           //MUXA uses AN5

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

        #elif defined(DSPIC33EP512MU810_PIM) || defined (PIC24EP512GU810_PIM)
            
            // Routine to read the Explorer 16 potentiometer.
              // Get an ADC sample
            AD1CHS0bits.CH0SA = 5;
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete
        
        #elif defined (DSPIC33E_USB_STARTER_KIT)
        
            w.Val = 0;
        
        #elif defined(PIC24FJ64GB004_PIM)
            AD1CHS = 0x7;           //MUXA uses AN7

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

        #elif defined(PIC24F_STARTER_KIT)
            AD1CHS = 0x0;           //MUXA uses AN0

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

        #elif defined(PIC32MX460F512L_PIM) || defined(PIC32_USB_STARTER_KIT) || defined(PIC32MX795F512L_PIM)
            AD1PCFG = 0xFFFB; // PORTB = Digital; RB2 = analog
            AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling ...
            // and starts converting
            AD1CHS = 0x00020000; // Connect RB2/AN2 as CH0 input ..
            // in this example RB2/AN2 is the input
            AD1CSSL = 0;
            AD1CON3 = 0x0002; // Manual Sample, Tad = internal 6 TPB
            AD1CON2 = 0;
            AD1CON1SET = 0x8000; // turn ADC ON

            AD1CON1SET = 0x0002; // start sampling ...
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1CLR = 0x0002; // start Converting
            while (!(AD1CON1 & 0x0001));// conversion done?
        #else
            #error
        #endif

        w.Val = ADC1BUF0;

    #endif

    return w;
}//end ReadPOT
#endif

