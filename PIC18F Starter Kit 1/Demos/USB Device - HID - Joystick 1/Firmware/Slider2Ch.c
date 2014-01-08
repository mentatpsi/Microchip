/******************************************************************************
Slider4Ch.c

The operation of the Slider on the demo board is controlled by 
the functions in this file.

serviceSlider() - this function determines if either of the channels
	associated with the slider are pressed.  If so, it calculates the 
	position of the touch on the slider on a scale of 0 to 100.

******************************************************************************/

/******************************************************************************

* File Name:       Slider.c
* Dependencies:    None
* Processor:       PIC24FJ128GB106
* Compiler:        C30 v3.02c
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

Author          Date    Comments
--------------------------------------------------------------------------------
BDB          26-Jun-2008 First release
SB			 22-Oct-2008
NK			 03-Nov-2008
*******************************************************************************/


#include "Slider2Ch.h"
#include "mtouch.h"

#define NUMROWS 0
#define NUMCOLS 0


unsigned int	curRawData	   	[NUM_CTMU_CH];	//Storage for CTMU channel values
unsigned int	tripValue   	[NUM_CTMU_CH];	//Storage for the trip point for each channel	
unsigned int	hystValue   	[NUM_CTMU_CH];	//Storage for the hysterisis value for each channel
unsigned int	pressedCount   	[NUM_CTMU_CH];	//Storage for count of pressed value for each channel
unsigned int	unpressedCount  [NUM_CTMU_CH];	//Storage for count of unpressed value for each channel
unsigned int	avg_delay 		[NUM_CTMU_CH];//Storage for count for average update for each channel		


unsigned int	immediateValue;			// current button value
unsigned int	bigValue;				// current button bigval

unsigned int	averageData [NUM_CTMU_CH];	// running average of CTMU channels	
unsigned int	smallAvg	[NUM_CTMU_CH];	// current button smallavg
unsigned int	actualValue	[NUM_CTMU_CH];	// actual raw A/D counts for each channel
unsigned int	tripValue   [NUM_CTMU_CH];	//Storage for the trip point for each channel

int	startupCount = 50;				// variable to 'discard' first N samples


//long 	pct_output_2Chl;
long 	pct_output_2Chl;
long	pct_sum_2Chl;		//n30jan2009 -- long
long 	pct_array_2Chl[AVG_INDEX_2CHDATA];
unsigned int tempVar=1;


void CapInit(void) 
{
	unsigned char	Index;					// Index of what button is being checked.
	
	// Load defaults for buttons
	for (Index=0; Index < NUM_CTMU_CH; Index++) 
	{			// Load Default GB/TRP
			// Normal Buttons
			curRawData[Index]		= 0;
			averageData[Index] 	= 0;
			tripValue[Index]=1024;
	}
}


//unsigned long int touchFlag=0;
/*....................................................................
.	ReadCTMU(int Index)
.
.		Capacitive service routine for ISR.  This function should be
.	called on each overflow of Timer which indicates a measurement
.	is ready to be taken.
.
.	It takes the average of multiple measurements, determines if 
.   the button under test is pressed or not (setting a flag accordingly)
.   The slow average the new reading appropriately and then scan 
.   the next button in sequence on the next ISR.
....................................................................*/
void ReadCTMU(int Index) 
{
	unsigned long total = 0;
	int chrd;
	Index--; 
	// Get the raw sensor reading.
	for(chrd=0; chrd< NUM_HF_READS; chrd++)
	{
	    immediateValue =  mTouchReadButton(Index+1);
		total = total + immediateValue;
	}
	
	immediateValue = total/NUM_HF_READS;
	//bigValue   = immediateValue  * 64; //16;	// BIGVAL is current measurement left shifted 4 bits
	curRawData[Index] = immediateValue;				// curRawData array holds the most recent BIGVAL values
	actualValue[Index] = immediateValue;
	
} //end ReadCTMU()


void updateCTMU(int Index)
{
	unsigned int temp;
	Index--;
	//Index = chnl;
	// On power-up, reach steady-state readings
	// During power-up, system must be initialized, set average ea. pass = raw data
	if (startupCount > 0) 
	{
		startupCount--;					// Decr. N # times to establish startup
		averageData[Index] = curRawData[Index];
										// During start up time, set Average each pass.
	}
	else
	{
		if(startupCount == 0)
		{
			startupCount = -1;
		}
		// 3. Is a keypad button pressed?
		if (curRawData[Index] < (averageData[Index] - tripValue[Index])) 
		{
			unpressedCount[Index] = 0;
			pressedCount[Index] = pressedCount[Index] + 1;
			
		}else
		{
			pressedCount[Index] = 0;
		}

		// 5. Is a button unpressed?
		if (curRawData[Index] > (averageData[Index] - tripValue[Index] + hystValue[Index])) 
		{		// Is bigValue above { average - (trip + hysteresis) } ? .. Yes
				// Button is up.
			unpressedCount[Index] = unpressedCount[Index] + 1;
			

		}else
		{
			unpressedCount[Index] = 0;
		}
 		// 6. Implement quick-release for a released button
		if (curRawData[Index]  > averageData[Index])
		{
			averageData[Index] = curRawData[Index];				// If curRawData is above Average, reset to high average.
		}
		// 7. Average in the new value
		// Always Average (all buttons)

		if(avg_delay[Index] < NUM_AVG)
		{
			avg_delay[Index]++;						// Counting 0..8 has effect of every 9th count cycling the next button.
		}
		else
		{
			avg_delay[Index] = 0;					// Counting 0..4 will average faster and also can use 0..4*m, m=0,1,2,3..
		}
		
		if(avg_delay[Index] == NUM_AVG) 
		{ 
			smallAvg[Index] = averageData[Index] / NUM_AVG; 	// SMALLAVG is the current average right shifted 4 bits
			// Average in raw value.
			averageData[Index] = averageData[Index] + ((curRawData[Index] / NUM_AVG) - smallAvg[Index]);		
		}

	}// end else
}










/*....................................................................
.	serviceSlider2Chnl( )
.
.	This function will read the Current and the Average data obtained	
.	when the finger is moved across the slider.	
.	It will then calculate the position of the touch in the slider
.	Based on the position, it will send the corresponding percentage
.	so that the respective LED's are displayed.
....................................................................*/
#ifdef	USE_SLIDER_2CHNL
#if 0
void serviceSlider2Chnl(void)

{
	int         avgHold;
	unsigned long int       heldAvgCh[2];
	//float     heldAvgCh[3];
	unsigned long int 		deltaCh[2];
	unsigned long int       delta;
	unsigned long int 		percent;
	unsigned long int       percent1;
	unsigned long int       percent2;
	unsigned long int       percent3;
    int k;
    if(1)
    {
		if(avgHold != 1)
        {
        	avgHold = 1;
            heldAvgCh[0] = (float)averageData[NUMROWS+NUMCOLS];
            heldAvgCh[1] = (float)averageData[NUMROWS+NUMCOLS+1];
        }
        deltaCh[0] = (float)averageData[NUMROWS+NUMCOLS] -(float)curRawData[NUMROWS+NUMCOLS];
        deltaCh[1] = (float)averageData[NUMROWS+NUMCOLS+1] -(float)curRawData[NUMROWS+NUMCOLS+1];

    }
    else
    {
        deltaCh[1] = (float)averageData[NUMROWS+NUMCOLS+1] -(float)curRawData[NUMROWS+NUMCOLS+1];
        deltaCh[0] = (float)averageData[NUMROWS+NUMCOLS] -(float)curRawData[NUMROWS+NUMCOLS];
        avgHold = 0;
    }
    percent1 = deltaCh[1]*100;
    percent2 = deltaCh[0]*100;
    percent3 = (deltaCh[1] + deltaCh[0]);

    if(percent3 <= 0)
    {
        percent1 = 0;
        percent2 = 0;

    }
    else
    {
        percent1 = percent1 / percent3; 
        percent2 = percent2 / percent3;
        percent1 = ((100 - percent2) + percent1)/2;
        if(percent1 < 0)
        {
             percent1 = 0;
        }
        if(percent1 > 100)
        {
              percent1 = 100;

        }

     }
     if(1)
     {
         pct_output_2Chl = (int)percent1;
         pct_sum_2Chl = pct_array_2Chl[0];                     // Establish sum initially with first
         for (k=1; k<6; k++) 
         {
         	pct_sum_2Chl += pct_array_2Chl[k];              // Calculate sum over iterations
            pct_array_2Chl[k-1] = pct_array_2Chl[k];  // Shift values
         }
         pct_array_2Chl[5] = pct_output_2Chl;      
         pct_output_2Chl = pct_sum_2Chl/6;
      }
}
#endif

#if 1
void serviceSlider2Chnl(void)
{
	unsigned long int deltaCh[2];
	unsigned long int Channel_L;
	unsigned long int Channel_R;
	unsigned long int Channel_sum;

	float Percent_Display;
	float Percent_R;
	float Percent_L;
	int k;
	
	deltaCh[1] = (unsigned long int)averageData[NUMROWS+NUMCOLS+1] -(unsigned long int)curRawData[NUMROWS+NUMCOLS+1];		//n30jan2009 --> removed if
	deltaCh[0] = (unsigned long int)averageData[NUMROWS+NUMCOLS] -(unsigned long int)curRawData[NUMROWS+NUMCOLS];
	

	/***********************************n29jan2009*************************************/
	Channel_L = deltaCh[1];
	Channel_R = deltaCh[0];
	Channel_sum = (Channel_L + Channel_R);
	
	if(Channel_sum <= 0)		// to avoid divide by zero factor
	{
		Percent_L =0;
		Percent_R =0;
		Percent_Display =0;
	}
	else 
	{
		/* To check the Right Sensor values( channel 1) */
		if (Channel_R == 0)  Percent_R = 0;
		else
		{		
			Percent_R = ((float)Channel_R)/(Channel_sum);				
			Percent_R = 100 * Percent_R;
		}
		/* To check the Left Sensor values( channel 0) */
		if (Channel_L == 0)  Percent_L = 0; 
		else
		{
			Percent_L = ((float)Channel_L/Channel_sum);				
			Percent_L = 100* Percent_L;
		}
						
		if ((Percent_R >= 0) && (Percent_R <= TOLERANCE_LIMIT_MAX_UNTOUCH) ) // Tolerance for interference in RIGHT channel while measuring LEFT
		{
			if(Percent_L <= MAX_LEFT_CHANNEL_SCALE_OF_SLIDER)	Percent_Display = (  MAX_LEFT_CHANNEL_SCALE_OF_SLIDER - Percent_L); // sliding from MAX area of LEFT channel towards mid	
			else												Percent_Display = (  100 - Percent_L);
		}
		else if( (Percent_L >= 0) && (Percent_L <= TOLERANCE_LIMIT_MAX_UNTOUCH) ) // Tolerance for interference in LEFT channel while measuring RIGHT
		{
			if(Percent_R >= MAX_LEFT_CHANNEL_SCALE_OF_SLIDER)
			{
				Percent_Display = (  Percent_R - MAX_LEFT_CHANNEL_SCALE_OF_SLIDER); // sliding from MAX area of LEFT channel towards mid	
				Percent_Display += MAX_LEFT_CHANNEL_SCALE_OF_SLIDER;
			}
			else												Percent_Display = Percent_R;
		}
		else if(Percent_L !=0 && Percent_R !=0) 
		{	
			if(Percent_L < Percent_R)
				Percent_Display = Percent_R ;
			else if (Percent_L > Percent_R)	
				Percent_Display = ( 100 - Percent_L);
			else if (Percent_L == Percent_R)
				Percent_Display = (Percent_R + (MAX_LEFT_CHANNEL_SCALE_OF_SLIDER - Percent_L))/2 ;								

			if( (Percent_Display >= MAX_LEFT_CHANNEL_SCALE_OF_SLIDER) )		Percent_Display = Percent_Display - (100-MAX_LEFT_CHANNEL_SCALE_OF_SLIDER);	
			if( (Percent_Display <= MIN_RIGHT_CHANNEL_SCALE_OF_SLIDER) )	Percent_Display = Percent_Display + MIN_RIGHT_CHANNEL_SCALE_OF_SLIDER;	

		}	
	}

		pct_output_2Chl = (int)Percent_Display;
		pct_sum_2Chl = pct_array_2Chl[0];				// Establish sum initially with first
		for (k=1; k<AVG_INDEX_2CHDATA; k++) 
		{
			pct_sum_2Chl += pct_array_2Chl[k];			// Calculate sum over iterations
			pct_array_2Chl[k-1] = pct_array_2Chl[k];	// Shift values
		}
		pct_array_2Chl[AVG_INDEX_2CHDATA-1] = pct_output_2Chl;	
		
		pct_output_2Chl = pct_sum_2Chl/AVG_INDEX_2CHDATA;
			

}
#endif 
#endif	

