/********************************************************************
 FileName:     	phd_config.h
 Dependencies: 	None 
 Processor:		PIC18, PIC24 or PIC32 Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18),  C30 (for PIC24) or C32(for PIC32) 
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

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  0.03  1/16/2012    
 *******************************************************************/
#ifndef _PHDCFG_H
#define  _PHDCFG_H


/**** PHDC Related Configuration ***********************************/ 
#define PHDC_THERMOMETER_SPECIALIZATOIN
#define MAX_APDU_SIZE_THERMOMETER          255 //896 
#define PHDC_MAX_APDU_SIZE 255

#define PHD_APP_BUFFER_SIZE  256
 
#define SEND_QOS PHDC_BULK_IN_QOS

/* PHD Time Cordination Macro  *************************************/
#define PHD_USE_ABSOLUTE_TIME 
//#define PHD_USE_RELATIVE_TIME
//#define PHD_USE_TIME_SYNCHRONIZATION
//#define PHD_USE_SETTABLE_TIME
 

/* Request/Response Sizes */
#define ASSOCIATION_REQUEST_SIZE     54
#define CONFIG_EVENT_REPORT_SIZE     72
#define MDS_ATTRIBUTES_SIZE          160
#define MEASUREMENT_DATA_SIZE        44

#define PHD_MDS_ATTR_ABS_TIME_STAMP_POINTER 152
#define PHD_MDS_ATTR_ABS_SYNCED_POINTER    133

/**  PHD Agent Device Specilization Info **/ 
extern UINT8  ROM ASSOCIATION_REQUEST[];
extern UINT8  ROM CONFIG_EVENT_REPORT[];
extern UINT8  ROM MDS_ATTRIBUTES[];

/** PHD Timeout Settings ***/ 
#define ASSOCIATION_REQUEST_TIMEOUT   10000 //milli Seconds
#define ASSOCIATION_REQUEST_RETRY     3     //Times
#define CONFIGURATION_TIMEOUT         10000 //milli Seconds
#define ASSOCIATION_RELEASE_TIMOUT    3000  //milli Seconds
#define CONFIRM_TIMEOUT               3000  //milli Seconds


#endif //_PHDCFG_H

