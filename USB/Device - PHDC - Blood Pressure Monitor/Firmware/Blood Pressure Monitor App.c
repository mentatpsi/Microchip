/********************************************************************
 FileName:      Blood Pressure Monitor App.c
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
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
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
  2.0   

********************************************************************/

/** I N C L U D E S **********************************************************/

#include "usb_config.h"
#include "USB/usb_function_phdc.h"
#include "USB/usb.h"
#include "USB/usb_function_phdc_com_model.h"
#include "Blood Pressure Monitor App.h"
#include "phd_config.h"
#include "HardwareProfile.h"
#ifdef PHD_USE_RTCC_FOR_TIME_STAMP
    #include "rtcc.h"
#endif

/** Definitions ****************************************************/


/*****************************************************************************
 * Constant and Macro's
 *****************************************************************************/

/* Association Request. The agent sends the following message to the manager. This example assumes the agent is a Standard 
BP Monitor. */

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

0x02, 0xBC,                         /* dev-config-id | standard configuration - Blood Pressure Monitor*/
0x00, 0x01,                         /* data-req-mode-flags */
0x01, 0x00,                         /* data-req-init-agent-count,
                                      data-req-init-manager-count */
0x00, 0x00, 0x00, 0x00              /* Atribute list */
};



/* Configuration information exchange. The agent reports its configuration to the manager */

UINT8  ROM CONFIG_EVENT_REPORT[CONFIG_EVENT_REPORT_SIZE] = {
    
0xE7, 0x00,                 /* APDU CHOICE Type (PrstApdu)  */
//0x00, 0x94,                 /* CHOICE.length = 148  */ 
BREAK_WORD_INTO_BYTES(132),  /* CHOICE.length = 132  */ 
//0x00, 0x92,                 /* OCTET STRING.length = 146  */ 
BREAK_WORD_INTO_BYTES(130), /* OCTET STRING.length = 130  */
0x00, 0x53,                 /* invoke-id (differentiates this message from any other outstanding)  */
0x01, 0x01,                 /* CHOICE(Remote Operation Invoke | Confirmed Event Report) */
//0x00, 0x8C,                 /* CHOICE.length = 140 */ 
BREAK_WORD_INTO_BYTES(124), /* CHOICE.length = 124 */
0x00, 0x00,                 /* obj-handle = 0 (MDS object) */ 
0xFF, 0xFF, 0xFF, 0xFF,     /* event-time */ 
0x0D, 0x1C,                 /* event-type = MDC_NOTI_CONFIG */ 
//0x00, 0x82,                 /* event-info.length = 130 (start of ConfigReport) */
BREAK_WORD_INTO_BYTES(114), /* event-info.length = 114 (start of ConfigReport) */
0x02, 0xBC,                 /* config-report-id (Dev-Configuration-Id value)  */
0x00, 0x02,                 /* config-obj-list.count = 2 Measurement objects will be “announced” */
//0x00, 0x7C,                 /* config-obj-list.length = 124 */
BREAK_WORD_INTO_BYTES(108), /* config-obj-list.length = 108 */

0x00, 0x06,                 /*  obj-class = MDC_MOC_VMO_METRIC_NU  */  //(8 Bytes)
0x00, 0x01,                 /*  obj-handle = 1  (Æ 1st Measurement is systolic, diastolic, MAP) */ 
0x00, 0x06,                 /*  attributes.count = 7  */ 
//0x00, 0x40,                 /*  attributes.length = 64  */
BREAK_WORD_INTO_BYTES(56),  /*  attributes.length = 56  */
 
0x09, 0x2F,                 /*  attribute-id = MDC_ATTR_ID_TYPE */  //(8 Bytes)
0x00, 0x04,                 /*  attribute-value.length = 4  */ 
0x00, 0x02, 0x4A, 0x04,     /*  MDC_PART_SCADA | MDC_PRESS_BLD_NONINV  */
 
0x0A, 0x46,                 /*  attribute-id = MDC_ATTR_METRIC_SPEC_SMALL  */ //(6 Bytes)
0x00, 0x02,                 /*  attribute-value.length = 2  */ 
0xF0, 0x40,                 /*  intermittent, stored data, upd & msmt aperiodic, agent init, measured  */ 
 
0x0A, 0x73,                 /*  attribute-id = MDC_ATTR_METRIC_STRUCT_SMALL  */ //(6 bytes)
0x00, 0x02,                 /*  attribute-value.length = 2  */ 
0x03, 0x03,                 /*  {ms-struct-compound-fix, 3}  */ 

0x0A, 0x76,                 /*  attribute-id = MDC_ATTR_ID_PHYSIO_LIST  */ //(14 bytes)
0x00, 0x0A,                 /*  attribute-value.length = 10  */  
0x00, 0x03,                 /*  MetricIdList.count = 3  */  
0x00, 0x06,                 /*  MetricIdList.length = 6  */  
0x4A, 0x05,                 /*  {MDC_PRESS_BLD_NONINV_SYS,  */ 
0x4A, 0x06,                 /*  MDC_PRESS_BLD_NONINV_DIA,  */ 
0x4A, 0x07,                 /*  MDC_PRESS_BLD_NONINV_MEAN} */ 

0x09, 0x96,                 /*  attribute-id = MDC_ATTR_UNIT_CODE  */ //(6 Bytes)
0x00, 0x02,                 /*  attribute-value.length = 2  */ 
0x0F, 0x20,                 /*  MDC_DIM_MMHG  */ 

//0x09, 0x4A,                 /*  attribute-id = MDC_ATTR_NU_ACCUR_MSMT  */  //(8 Bytes) 
//0x00, 0x04,                 /*  attribute-value.length = 4  */ 
//0xFA, 0x1E, 0x84, 0x80,     /*  accuracy = 2.0  */ 

0x0A, 0x55,                 /*  attribute-id = MDC_ATTR_ATTRIBUTE_VAL_MAP */ //(16 Bytes) 
0x00, 0x0C,                 /*  attribute-value.length = 12  */ 
0x00, 0x02,                 /*  AttrValMap.count = 2  */ 
0x00, 0x08,                 /*  AttrValMap.length = 8  */ 
0x0A, 0x75, 0x00, 0x0A,     /*  MDC_ATTR_NU_CMPD_VAL_OBS_BASIC | value length = 10  */
0x09, 0x90, 0x00, 0x08,     /*  MDC_ATTR_TIME_STAMP_ABS | value length = 8  */ 

0x00, 0x06,                 /* obj-class = MDC_MOC_VMO_METRIC_NU  */ //(8 Bytes)
0x00, 0x02,                 /* obj-handle = 2  (Æ 2nd Measurement is pulse rate)  */ 
0x00, 0x04,                 /* attributes.count = 5  */
//0x00, 0x2C,                 /* attributes.length = 44  */
BREAK_WORD_INTO_BYTES(36),  /* attributes.length = 36  */

0x09, 0x2F,                 /* attribute-id = MDC_ATTR_ID_TYPE  */ //(8 Bytes)
0x00, 0x04,                 /* attribute-value.length = 4  */
0x00, 0x02, 0x48, 0x2A,     /* MDC_PART_SCADA | MDC_PULS_RATE_NON_INV  */

0x0A, 0x46,                 /* attribute-id = MDC_ATTR_METRIC_SPEC_SMALL  */ //(6 Bytes)
0x00, 0x02,                 /* attribute-value.length = 2  */
0xF0, 0x40,                 /* intermittent, stored data, upd & msmt aperiodic, agent init, measured  */ //(6 Bytes)

0x09, 0x96,                 /* attribute-id = MDC_ATTR_UNIT_CODE  */ //(6 Bytes)
0x00, 0x02,                 /* attribute-value.length = 2 */
0x0A, 0xA0,                 /* MDC_DIM_BEAT_PER_MIN */

//0x09, 0x4A,                 /* attribute-id = MDC_ATTR_NU_ACCUR_MSMT */  //(8 Bytes)
//0x00, 0x04,                 /* attribute-value.length = 4 */
//0xFA, 0x0F, 0x42, 0x40,     /* accuracy = 1.0 */

0x0A, 0x55,                 /* attribute-id = MDC_ATTR_ATTRIBUTE_VAL_MAP */ //(16 Bytes)
0x00, 0x0C,                 /* attribute-value.length = 12 */
0x00, 0x02,                 /* AttrValMap.count = 2 */
0x00, 0x08,                 /* AttrValMap.length = 8 */
0x0A, 0x4C, 0x00, 0x02,     /* MDC_ATTR_NU_VAL_OBS_BASIC | value length = 2 */
0x09, 0x90, 0x00, 0x08,     /* MDC_ATTR_TIME_STAMP_ABS | value length = 8 */

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
0x10, 0x07,                      /* type = MDC_DEV_SPEC_PROFILE_BP     */
0x00, 0x01,                      /* version=ver 1 of the specialization     */

//MDC_ATTR_ID_MODEL (Total Length = 30 Bytes)
0x09, 0x28,                      /* attribute-id = MDC_ATTR_ID_MODEL    */
0x00, 0x1A,                      /* attribute-value.length = 26        */
0x00, 0x0A,                      /* string length = 10 | "Microchip "  */ 
'M', 'i', 'c', 'r', 'o', 'c', 'h', 'i' ,'p', ' ',          
0x00, 0x0C,                      /* string length = 12 | "BP Monitor  "  */
'B', 'P', ' ', 'M', 'o', 'n', 'i', 't', 'o', 'r', ' ', ' ', 

//MDC_ATTR_SYS_ID (Total Length = 14 Bytes)
0x09, 0x84,                      /* attribute-id = MDC_ATTR_SYS_ID        */
0x00, 0x0A,                      /* attribute-value.length = 10            */
0x00, 0x08, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
                                 /* octet string length = 8 | EUI-64  */
                                 
//MDC_ATTR_DEV_CONFIG_ID (Total Length = 6 Bytes)                                 
0x0A, 0x44,                      /* attribute-id = MDC_ATTR_DEV_CONFIG_ID   */
0x00, 0x02,                      /* attribute-value.length = 2             */
0x02, 0xBC,                      /* dev-config-id = 700 */

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
0x20, 0x07,  /* CertifiedDeviceClassEntry = MDC_DEV_SPEC_PROFILE_BP - 4096 + v_i_TCode * 8192 = 0x2007 
                where, MDC_DEV_SPEC_PROFILE_BP = 4113, v_i_TCode = 1 for USB Agent */   
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
0xE7, 0x00,             /* APDU CHOICE Type (PrstApdu) */
0x00, 0x3E,             /* CHOICE.length = 62 */ 
0x00, 0x3C,             /* OCTET STRING.length = 60 */
0x00, 0x54,             /* invoke-id  */
0x01, 0x01,             /* CHOICE(Remote Operation Invoke | Confirmed Event Report) */
0x00, 0x36,             /* CHOICE.length = 5 */
0x00, 0x00,             /* obj-handle = 0 (MDS object) */
0xFF, 0xFF, 0xFF, 0xFF, /* event-time (set to 0xFFFFFFFF if RelativeTime is not supported) */

0x0D, 0x1D,             /* event-type = MDC_NOTI_SCAN_REPORT_FIXED */
0x00, 0x2C,             /* event-info.length = 44 */
0xF0, 0x00,             /* ScanReportInfoFixed.data-req-id = 0xF000 */
0x00, 0x00,             /* ScanReportInfoFixed.scan-report-no = 0 */
0x00, 0x02,             /* ScanReportInfoFixed.obs-scan-fixed.count = 2 */
0x00, 0x24,             /* ScanReportInfoFixed.obs-scan-fixed.length = 36 */
0x00, 0x01,             /* ScanReportInfoFixed.obs-scan-fixed.value[0].obj-handle = 1 */
0x00, 0x12,             /* ScanReportInfoFixed.obs-scan-fixed.value[0]. obs-val-data.length = 18 */
0x00, 0x03,             /* Compound Object count (3 entries) */
0x00, 0x06,             /* Compound Object length (6 bytes) */
0x00, 0x78,             /* Systolic = 120   */
0x00, 0x50,             /* Diastolic = 80    */
0x00, 0x64,             /* MAP = 100   */
0x20, 0x07, 0x12, 0x06, /* Absolute-Time-Stamp = 2007-12-06T12:10:0000   */
0x12, 0x10, 0x00, 0x00,   
0x00, 0x02,             /* ScanReportInfoFixed.obs-scan-fixed.value[1].obj-handle = 2  */
0x00, 0x0A,             /* ScanReportInfoFixed.obs-scan-fixed.value[1]. obs-val-data.length = 10   */
0xF2, 0x58,             /* Basic-Nu-Observed-Value = 60.0 (BPM)   */
0x20, 0x07, 0x12, 0x06,  /* Absolute-Time-Stamp = 2007-12-06T12:10:0000 */
0x12, 0x10, 0x00, 0x00 

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
void PhdBpMonitorUpdateMeasurements(void);
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
#if defined DEMO_BOARD_HAS_ONLY_ONE_PUSH_BUTTON //(see hardware profile file for the specifc hardware that your are using)
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
				   PhdBpMonitorUpdateMeasurements(); 
		           PHDSendMeasuredData();
		       }
		       else if (sw2ButtonPressCounter == 3) 
		       {
    		       // if the push button is pressed for the Third time then the agent sends Measured Data.
    		       PhdBpMonitorUpdateMeasurements(); 
    	           PHDSendMeasuredData();      
    	       }
    	       else if (sw2ButtonPressCounter == 4)
		       {
    		       // if the push button is pressed for the Fourth time then the agent sends Measured Data.
    		       PhdBpMonitorUpdateMeasurements(); 
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
    				PhdBpMonitorUpdateMeasurements();
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
 * Function:        void PhdBpMonitorUpdateMeasurements(void)
 *
 * PreCondition:    None
 *
 * Input:           pointer to the APDU Buffer where measurement data needs to be updated. 
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine updates the measured BP Monitor values in to corresponding 
 *                  APDU locations	
 *
 * Note:            
 *
 *****************************************************************************/

void PhdBpMonitorUpdateMeasurements(void)
{
    /* User can update the Measured value from the BP Monitor*/
    /* Please go through IEEE_STD_11073_10407 (BP Monitor device spec)*/
    PHD_BLOOD_PRESSURE_DATA BloodPressure;  
    #if defined PHD_USE_RTCC_FOR_TIME_STAMP
        rtccTimeDate TimeStamp; 
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
	
	
    /********* Update Blood Pressure Data  ******************/   
    BloodPressure = GetBloodPressure();
    
    //BP Systolic
    PhdAppBuffer[38]= BloodPressure.systolic.byte.HB;   // MSB
    PhdAppBuffer[39]= BloodPressure.systolic.byte.LB;   // LSB
    
    //BP Diastolic
    PhdAppBuffer[40]= BloodPressure.diastolic.byte.HB;  //MSB
    PhdAppBuffer[41]= BloodPressure.diastolic.byte.LB;; //LSB
    
    //BP Mean
    PhdAppBuffer[42]= BloodPressure.mean.byte.HB; //MSB
    PhdAppBuffer[43]= BloodPressure.mean.byte.LB; // LSB
   
    /********* Update Pulse Rate Data  ******************/   
    PhdAppBuffer[56]= BloodPressure.pulseRate.byte.HB; //MSB  	
    PhdAppBuffer[57]= BloodPressure.pulseRate.byte.LB; //LSB 
    
    /************ Update Time Stamp **********************/ 
    #if defined PHD_USE_RTCC_FOR_TIME_STAMP
        RtccReadTimeDate(&TimeStamp);
        PhdAppBuffer[44] = PhdAppBuffer[58] = 0x20;                 //Year 1
        PhdAppBuffer[45] = PhdAppBuffer[59] = TimeStamp.f.year ;   //Year 2TimeStamp.f.year  
        PhdAppBuffer[46] = PhdAppBuffer[60] = TimeStamp.f.mon;     // Month 
        PhdAppBuffer[47] = PhdAppBuffer[61] = TimeStamp.f.mday;    // Day
        PhdAppBuffer[48] = PhdAppBuffer[62] = TimeStamp.f.hour;    // Hour
        PhdAppBuffer[49] = PhdAppBuffer[63] = TimeStamp.f.min;     // Minute
        PhdAppBuffer[50] = PhdAppBuffer[64] = TimeStamp.f.sec;     // Seconds
        PhdAppBuffer[51] = PhdAppBuffer[65] = 0x00;
    #else 
        /* Time Stamp  = 12:30:00, 20th July 2011*/ 
        PhdAppBuffer[44] = PhdAppBuffer[58] = 0x20;           //Year 1
        PhdAppBuffer[45] = PhdAppBuffer[59] = 0x11;           //Year 2
        PhdAppBuffer[46] = PhdAppBuffer[60] = 0x11;           // Month 
        PhdAppBuffer[47] = PhdAppBuffer[61] = 0x18;           // Day
        PhdAppBuffer[48] = PhdAppBuffer[62] = 0x14;            // Hour
        PhdAppBuffer[49] = PhdAppBuffer[63] = 0x39;           // Minute
        PhdAppBuffer[50] = PhdAppBuffer[64] = 0x00;           // Seconds
        PhdAppBuffer[51] = PhdAppBuffer[65] = 0x00;
    #endif     
}
/******************************************************************************
 * Function:        UINT8 GetBloodPressure(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine Gets the blood Pressure Level simulated by Potentiometer.  	
 *
 * Note:            
 *
 *****************************************************************************/
PHD_BLOOD_PRESSURE_DATA GetBloodPressure(void)
{
  PHD_BLOOD_PRESSURE_DATA bloodPressure;  
  #if defined PHD_USE_POT_FOR_TEMP_SIMULATION
    WORD_VAL w;  
    mInitPOT();
    w = ReadPOT();	//Use ADC to read the I/O pin voltage.  See the relevant HardwareProfile - xxxxx.h file for the I/O pin that it will measure.
					//Some demo boards, like the PIC18F87J50 FS USB Plug-In Module board, do not have a potentiometer (when used stand alone).
					//This function call will still measure the analog voltage on the I/O pin however.  To make the demo more interesting, it
					//is suggested that an external adjustable analog voltage should be applied to this pin. 
	w.Val = 100 + (w.Val>>3); 
	bloodPressure.systolic.Val = w.Val;  
  #else 
    bloodPressure.systolic.Val = 120; 
  #endif 
    bloodPressure.diastolic.Val = 80; 
    bloodPressure.mean.Val = 100; 
    bloodPressure.pulseRate.Val = 72;   
    return bloodPressure;  
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

