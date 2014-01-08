/******************************************************************************

 Slider4Ch (Header File)

Description:
    This file contains function prototypes that are used in Slider.c
*******************************************************************************/

/*******************************************************************************

* FileName:        Slider.h
* Dependencies:    See included files, below.
* Processor:       PIC24FJ256GB110
* Compiler:        C30 v3.01
* Company:         Microchip Technology, Inc.

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

Author          Date       Comments
--------------------------------------------------------------------------------
BDB         26-Jun-2008 Initial release

Change History:

*******************************************************************************/

#ifndef __SLIDER2CH_H     
#define __SLIDER2CH_H 

//#include	"CTMUcapsense.h"
#define USE_SLIDER_2CHNL

#ifdef	USE_SLIDER_2CHNL
	

  #define AVG_INDEX_2CHDATA	30		//Index for the total average sampled data for the 2 channel
  #define MAX_LEFT_CHANNEL_SCALE_OF_SLIDER	77	// 80 % of entire length of the slider copper pad area
  #define MIN_RIGHT_CHANNEL_SCALE_OF_SLIDER	(100-MAX_LEFT_CHANNEL_SCALE_OF_SLIDER)	// 80 % of entire length of the slider copper pad area

  #define TOLERANCE_LIMIT_MAX_UNTOUCH 	15 // Tolerance limit for untouch recoginition
  #define NUM_CTMU_CH 2
  #define SLDR2CHL	2


  //Function Prototypes //
  void CapInit(void);
  void updateCTMU(int Index); 
  void serviceSlider2Chnl(void);
  void ReadCTMU(int Index);
   


   	extern long pct_output_2Chl;  //n30jan2009 ==long pct
   	
   
	
// #define AVG_INDEX_2CHDATA	6		//Index for the total average sampled data for the 2 channel

#define 	NUM_AVG				20		// Defines frequency of average update
#define		TRIPVAL				0		// Initial Value for startup

#define 	DEBOUNCECOUNT		3		// Defines How Many consecutive reads for
										// press or unpressed
#define 	NUM_HF_READS 		64		//n28jan09:64->10	// number of reads in CTMU channel read routine
										// each read takes 12uS. 1ms timeslice max.

#define		KEYTRIPDIV			250
										// the divisors define the trip points for
#define		SLIDERTRIPDIV		250		// each type of cap sensor.  The trip point
										// is defined as: trip = average/divisor
										// A press is then defined as:
										//  if(current value < average-trip)

#endif//USE_SLIDER_2CHNL


//#ifndef	USE_SLIDER_2CHNL
//#define SLDR2CHL	0
//#endif

#endif   	//end of #ifndef __SLIDER2CH_H 

