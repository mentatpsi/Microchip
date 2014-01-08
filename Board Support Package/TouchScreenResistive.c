/*****************************************************************************
 *
 * Simple 4 wire resistive touch screen driver
 *
 *****************************************************************************
 * FileName:        TouchScreenResistive.c
 * Processor:       PIC24, PIC32, dsPIC, PIC24H
 * Compiler:       	MPLAB C30, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date        	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/19/11		Ported from TouchScreen driver.
 * 05/06/11     Added IDLE state and set start up state to IDLE.
 * 10/03/11     - Modified the calibration sequence.
 *              - Modified TouchDetectPosition() sampling to single A/D sampling.
 *              - Modified TouchDetectPosition() to return status of sampling.
 *              - Added Touch_ADCInit() to separate A/D initialization. This 
 *                will be useful when A/D is shared by multiple drivers.
 * 01/13/12     - Modified the calibration to store 8 data points.  
 *              - Modified the calculation of the x,y touch positions to fix
 *                overflow issue.
 * 05/16/12     Modified macros to be flexible with other PIC devices:
 *              - TRIS_XPOS, TRIS_YPOS, TRIS_XNEG, TRIS_YNEG,    
 *                LAT_XPOS, LAT_YPOS, LAT_XNEG, LAT_YNEG to 
 *                ResistiveTouchScreen_X(Y)Plus_Drive_High(), 
 *                ResistiveTouchScreen_X(Y)Plus_Drive_Low(), 
 *                ResistiveTouchScreen_X(Y)Plus_Config_As_Input(), 
 *                ResistiveTouchScreen_X(Y)Plus_Config_As_Output()  
 *                ResistiveTouchScreen_X(Y)Minus_Drive_High(), 
 *                ResistiveTouchScreen_X(Y)Minus_Drive_Low(), 
 *                ResistiveTouchScreen_X(Y)Minus_Config_As_Input(), 
 *                ResistiveTouchScreen_X(Y)Minus_Config_As_Output().
 *              - added two macros RESISTIVETOUCH_ANALOG and 
 *                RESISTIVETOUCH_DIGITAL to indicate pin mode.
 *
 *****************************************************************************/

#include "HardwareProfile.h"

#if defined (USE_TOUCHSCREEN_RESISTIVE)

#include "Graphics/Graphics.h"
#include "TimeDelay.h"
#include "TouchScreen.h"
#include "TouchScreenResistive.h"
#include "Compiler.h"

// Default Calibration Inset Value (percentage of vertical or horizontal resolution)
// Calibration Inset = ( CALIBRATIONINSET / 2 ) % , Range of 0–20% with 0.5% resolution
// Example with CALIBRATIONINSET == 20, the calibration points are measured
// 10% from the corners.
#ifndef CALIBRATIONINSET
    #define CALIBRATIONINSET   20       // range 0 <= CALIBRATIONINSET <= 40 
#endif

#define CAL_X_INSET    (((GetMaxX()+1)*(CALIBRATIONINSET>>1))/100)
#define CAL_Y_INSET    (((GetMaxY()+1)*(CALIBRATIONINSET>>1))/100)
#define SAMPLE_POINTS   4

//////////////////////// Resistive Touch Driver Version ////////////////////////////
// The first four bits is the calibration inset, next 8 bits is assigned the version 
// number and 0xF is assigned to this 4-wire resistive driver.
const WORD mchpTouchScreenVersion = 0xF110 | CALIBRATIONINSET;

//////////////////////// A/D Sampling Mode ///////////////////////
// first some error check
#if defined (RESISTIVETOUCH_MANUAL_SAMPLE_MODE) &&  defined (RESISTIVETOUCH_AUTO_SAMPLE_MODE)
    #error Cannot have two resistive touch modes enabled.
#endif
#ifndef RESISTIVETOUCH_MANUAL_SAMPLE_MODE
    // enable auto sampling mode
    #define RESISTIVETOUCH_AUTO_SAMPLE_MODE
    // else manual sampling mode is enabled 
#endif

//////////////////////// GUI Color Assignments ///////////////////////
// Set the colors used in the calibration screen, defined by 
// GraphicsConfig.h or gfxcolors.h 
#if (COLOR_DEPTH == 1)
    #define RESISTIVETOUCH_FOREGROUNDCOLOR BLACK	   
    #define RESISTIVETOUCH_BACKGROUNDCOLOR WHITE	   
#elif (COLOR_DEPTH == 4)
    #define RESISTIVETOUCH_FOREGROUNDCOLOR BLACK	   
    #define RESISTIVETOUCH_BACKGROUNDCOLOR WHITE	   
#elif (COLOR_DEPTH == 8) || (COLOR_DEPTH == 16) || (COLOR_DEPTH == 24) 
    #define RESISTIVETOUCH_FOREGROUNDCOLOR BRIGHTRED	   
    #define RESISTIVETOUCH_BACKGROUNDCOLOR WHITE	   
#endif

//////////////////////// Deprecated Macros ///////////////////////////
// the following macros are deprecated
#ifdef TRIS_XPOS
    #warning "TRIS_XPOS is deprecated please use ResistiveTouchScreen_XPlus_Config_As_Input() macro instead. Where ResistiveTouchScreen_XPlus_Config_As_Input() is defined to the required pin. Ex: #define ResistiveTouchScreen_XPlus_Config_As_Input() TRISCbits.TRISC4 = 1"
#endif
#ifdef TRIS_YPOS
    #warning "TRIS_YPOS is deprecated please use ResistiveTouchScreen_YPlus_Config_As_Input() macro instead. Where ResistiveTouchScreen_YPlus_Config_As_Input() is defined to the required pin. Ex: #define ResistiveTouchScreen_YPlus_Config_As_Input() TRISGbits.TRISG7 = 1"
#endif
#ifdef TRIS_XNEG
    #warning "TRIS_XNEG is deprecated please use ResistiveTouchScreen_XMinus_Config_As_Input() macro instead. Where ResistiveTouchScreen_XMinus_Config_As_Input() is defined to the required pin. Ex: #define ResistiveTouchScreen_XMinus_Config_As_Input() TRISAbits.TRISA2 = 1"
#endif
#ifdef TRIS_YNEG
    #warning "TRIS_YNEG is deprecated please use ResistiveTouchScreen_YMinus_Config_As_Input() macro instead. Where ResistiveTouchScreen_YMinus_Config_As_Input() is defined to the required pin. Ex: #define ResistiveTouchScreen_YMinus_Config_As_Input() TRISAbits.TRISA1 = 1"
#endif

#ifdef LAT_XPOS
    #warning "LAT_XPOS is deprecated please use ResistiveTouchScreen_XPlus_Drive_Low() or ResistiveTouchScreen_XPlus_Drive_High() macros instead. Where ResistiveTouchScreen_XPlus_Drive_Low() or ResistiveTouchScreen_XPlus_Drive_High() are defined to drive the required pin. Ex: #define ResistiveTouchScreen_XPlus_Drive_Low() LATCbits.LATC4 = 0"
#endif
#ifdef LAT_YPOS
    #warning "LAT_YPOS is deprecated please use ResistiveTouchScreen_YPlus_Drive_Low() or ResistiveTouchScreen_YPlus_Drive_High() macros instead. Where ResistiveTouchScreen_YPlus_Drive_Low() or ResistiveTouchScreen_YPlus_Drive_High() are defined to drive the required pin. Ex: #define ResistiveTouchScreen_YPlus_Drive_Low() LATGbits.LATG7 = 0"
#endif
#ifdef LAT_XNEG
    #warning "LAT_XNEG is deprecated please use ResistiveTouchScreen_XMinus_Drive_Low() or ResistiveTouchScreen_XMinus_Drive_High() macros instead. Where ResistiveTouchScreen_XMinus_Drive_Low() or ResistiveTouchScreen_XMinus_Drive_High() are defined to drive the required pin. Ex: #define ResistiveTouchScreen_XMinus_Drive_Low() LATAbits.LATA2 = 0"
#endif
#ifdef LAT_YNEG
    #warning "LAT_YNEG is deprecated please use ResistiveTouchScreen_YMinus_Drive_Low() or yMinusDrive() macros instead. Where ResistiveTouchScreen_YMinus_Drive_Low() or yMinusDrive() are defined to drive the required pin. Ex: #define ResistiveTouchScreen_YMinus_Drive_Low() LATAbits.LATA1 = 0"
#endif


#ifndef ResistiveTouchScreen_XPlus_Config_As_Input
    #define ResistiveTouchScreen_XPlus_Config_As_Input() (TRIS_XPOS = 1)
#endif
#ifndef ResistiveTouchScreen_YPlus_Config_As_Input
    #define ResistiveTouchScreen_YPlus_Config_As_Input() (TRIS_YPOS = 1)
#endif
#ifndef ResistiveTouchScreen_XMinus_Config_As_Input
    #define ResistiveTouchScreen_XMinus_Config_As_Input() (TRIS_XNEG = 1)
#endif
#ifndef ResistiveTouchScreen_YMinus_Config_As_Input
    #define ResistiveTouchScreen_YMinus_Config_As_Input() (TRIS_YNEG = 1)
#endif
#ifndef ResistiveTouchScreen_XPlus_Config_As_Output
    #define ResistiveTouchScreen_XPlus_Config_As_Output() (TRIS_XPOS = 0)
#endif
#ifndef ResistiveTouchScreen_YPlus_Config_As_Output
    #define ResistiveTouchScreen_YPlus_Config_As_Output() (TRIS_YPOS = 0)
#endif
#ifndef ResistiveTouchScreen_XMinus_Config_As_Output
    #define ResistiveTouchScreen_XMinus_Config_As_Output() (TRIS_XNEG = 0)
#endif
#ifndef ResistiveTouchScreen_YMinus_Config_As_Output
    #define ResistiveTouchScreen_YMinus_Config_As_Output() (TRIS_YNEG = 0)
#endif
#ifndef ResistiveTouchScreen_XMinus_Drive_Low
    #define ResistiveTouchScreen_XMinus_Drive_Low() (LAT_XNEG = 0)
#endif
#ifndef ResistiveTouchScreen_YMinus_Drive_Low
    #define ResistiveTouchScreen_YMinus_Drive_Low() (LAT_YNEG = 0)
#endif
#ifndef ResistiveTouchScreen_XPlus_Drive_High
    #define ResistiveTouchScreen_XPlus_Drive_High() (LAT_XPOS = 1)
#endif
#ifndef ResistiveTouchScreen_YPlus_Drive_High
    #define ResistiveTouchScreen_YPlus_Drive_High() (LAT_YPOS = 1)
#endif
#ifndef RESISTIVETOUCH_ANALOG
    #define RESISTIVETOUCH_ANALOG  0
#endif
#ifndef RESISTIVETOUCH_DIGITAL
    #define RESISTIVETOUCH_DIGITAL 1 
#endif

//////////////////////// LOCAL PROTOTYPES ////////////////////////////
void    TouchGetCalPoints(void);
void 	TouchStoreCalibration(void);
void 	TouchCheckForCalibration(void);
void 	TouchLoadCalibration(void);
void    TouchCalculateCalPoints(void);

#ifdef ENABLE_DEBUG_TOUCHSCREEN
void    TouchScreenResistiveTestXY(void);
#endif

extern NVM_READ_FUNC           pCalDataRead;                // function pointer to data read
extern NVM_WRITE_FUNC          pCalDataWrite;               // function pointer to data write
extern NVM_SECTORERASE_FUNC    pCalDataSectorErase;         // function pointer to data sector erase

//////////////////////// GLOBAL VARIABLES ////////////////////////////
#ifndef TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD
    // you may define the threshold with a value, define the new value in the 
    // HardwareProfile.h
    #define TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD     256	// between 0-0x03ff the lesser this value 
											                // the lighter the screen must be pressed
#endif
#define CALIBRATION_DELAY   300				                // delay between calibration touch points

// Current ADC values for X and Y channels
volatile SHORT  adcX = -1;
volatile SHORT  adcY = -1;
volatile SHORT  adcPot = 0;

// coefficient values
volatile long   _trA;
volatile long   _trB;
volatile long   _trC;
volatile long   _trD;

// copy of the stored or sampled raw points (this is the calibration data stored)
/*      xRawTouch[0] - x sample from upper left corner; 
        xRawTouch[1] - x sample from upper right corner
        xRawTouch[2] - x sample from lower right corner
        xRawTouch[3] - x sample from lower left corner
        yRawTouch[0] - y sample from upper left corner; 
        yRawTouch[1] - y sample from upper right corner
        yRawTouch[2] - y sample from lower right corner
        yRawTouch[3] - y sample from lower left corner
*/
volatile SHORT  xRawTouch[SAMPLE_POINTS] = {TOUCHCAL_ULX, TOUCHCAL_URX, TOUCHCAL_LRX, TOUCHCAL_LLX};   
volatile SHORT  yRawTouch[SAMPLE_POINTS] = {TOUCHCAL_ULY, TOUCHCAL_URY, TOUCHCAL_LRY, TOUCHCAL_LLY};   

// WARNING: Watch out when selecting the value of SCALE_FACTOR 
// since a big value will overflow the signed int type 
// and the multiplication will yield incorrect values.
#ifndef TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR
    // default scale factor is 256
    #define TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR 8
#endif

// use this scale factor to avoid working in floating point numbers
#define SCALE_FACTOR (1<<TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR)

typedef enum
{
	IDLE,
    SET_X,
    RUN_X,
    GET_X,
    RUN_CHECK_X,
    CHECK_X,
    SET_Y,
    RUN_Y,
    GET_Y,
    CHECK_Y,
    SET_VALUES,
    GET_POT,
    RUN_POT
} TOUCH_STATES;

volatile TOUCH_STATES state = IDLE;

/*********************************************************************
* Function: void TouchDetectPosition(void)
********************************************************************/
SHORT TouchDetectPosition(void)
{
    static SHORT    tempX, tempY;
    SHORT           temp;

    switch(state)
    {
        case IDLE:
		    adcX = -1;
			adcY = -1;
            #ifdef ADC_POT
			    adcPot = 0;
			#endif
			break;

        case SET_VALUES:
#ifdef RESISTIVETOUCH_MANUAL_SAMPLE_MODE
			TOUCH_ADC_START = 0;    // stop sampling
#endif
            if(!TOUCH_ADC_DONE)
                break;

            if((WORD) TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD < (WORD) ADC1BUF0)
            {
                adcX = -1;
                adcY = -1;
            }
            else
            {
                adcX = tempX;
                adcY = tempY;
            }
        // If the hardware supports an analog pot, if not skip it
#ifdef ADC_POT
            state = RUN_POT;

       case RUN_POT:
       		TOUCH_ADC_INPUT_SEL = ADC_POT;
            TOUCH_ADC_START = 1;    // run conversion
            state = GET_POT;
            break;

        case GET_POT:
#ifdef RESISTIVETOUCH_MANUAL_SAMPLE_MODE
			TOUCH_ADC_START = 0;    // stop sampling
#endif
            if(TOUCH_ADC_DONE == 0){
                break;
            }
            adcPot = ADC1BUF0;
#endif // #ifdef ADC_POT
            state = SET_X;
            return 1;               // touch screen acquisition is done 

        case SET_X:
			
       		TOUCH_ADC_INPUT_SEL = ADC_XPOS;

            ResistiveTouchScreen_XPlus_Config_As_Input();
            ResistiveTouchScreen_YPlus_Config_As_Input();
            ResistiveTouchScreen_XMinus_Config_As_Input();
            ResistiveTouchScreen_YMinus_Drive_Low();
            ResistiveTouchScreen_YMinus_Config_As_Output();

#ifdef ADPCFG_YPOS
            ADPCFG_YPOS = RESISTIVETOUCH_DIGITAL;        // set to digital pin
#endif
#ifdef ADPCFG_YPOS
            ADPCFG_XPOS = RESISTIVETOUCH_ANALOG;        // set to analog pin
#endif

            TOUCH_ADC_START = 1;    // run conversion
            state = CHECK_X;
            break;

        case CHECK_X:
        case CHECK_Y:
#ifdef RESISTIVETOUCH_MANUAL_SAMPLE_MODE
			TOUCH_ADC_START = 0;    // stop sampling
#endif
            if(TOUCH_ADC_DONE == 0){
                break;
            }

            if((WORD) TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD > (WORD) ADC1BUF0)
            {
	            if (state == CHECK_X)
	            {
                	ResistiveTouchScreen_YPlus_Drive_High();
                	ResistiveTouchScreen_YPlus_Config_As_Output();
                	tempX     = -1;
                	state     = RUN_X;
                } 
                else 
                {
	                ResistiveTouchScreen_XPlus_Drive_High();
                    ResistiveTouchScreen_XPlus_Config_As_Output();
        	        tempY     = -1;
            	    state     = RUN_Y;    
	            }   	
            }
            else
            {
                adcX = -1;
                adcY = -1;
#ifdef ADC_POT
            	    state = RUN_POT;
#else
            		state = SET_X;
                    return 1;       // touch screen acquisition is done 	
#endif	    
                break;
            }

        case RUN_X:
        case RUN_Y:
            TOUCH_ADC_START = 1;
            state = (state == RUN_X) ? GET_X : GET_Y;
            // no break needed here since the next state is either GET_X or GET_Y
            break;
            
        case GET_X:
        case GET_Y:
#ifdef RESISTIVETOUCH_MANUAL_SAMPLE_MODE
			TOUCH_ADC_START = 0;    // stop sampling
#endif
            if(!TOUCH_ADC_DONE)
                break;

            temp = ADC1BUF0;
            if (state == GET_X)
	        {
	            if(temp != tempX)
	            {
	                tempX = temp;
	                state = RUN_X;
	                break;
	            }
	        }
	        else
	        {
	            if(temp != tempY)
	            {
	                tempY = temp;
	                state = RUN_Y;
	                break;
	            }		        
		    }     

            if (state == GET_X) 
            	ResistiveTouchScreen_YPlus_Config_As_Input();
            else	
	            ResistiveTouchScreen_XPlus_Config_As_Input();
            TOUCH_ADC_START = 1;
            state = (state == GET_X) ? SET_Y : SET_VALUES;
            break;

        case SET_Y:
#ifdef RESISTIVETOUCH_MANUAL_SAMPLE_MODE
			TOUCH_ADC_START = 0;    // stop sampling
#endif
            if(!TOUCH_ADC_DONE)
                break;

            if((WORD)  TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD < (WORD) ADC1BUF0)
            {
                adcX = -1;
                adcY = -1;
		        #ifdef ADC_POT
            	    state = RUN_POT;
            	#else
                	state = SET_X;
                    return 1;       // touch screen acquisition is done 
                #endif	
                break;
            }
			
       		TOUCH_ADC_INPUT_SEL = ADC_YPOS;

            ResistiveTouchScreen_XPlus_Config_As_Input();
            ResistiveTouchScreen_YPlus_Config_As_Input();
            ResistiveTouchScreen_XMinus_Drive_Low();
            ResistiveTouchScreen_XMinus_Config_As_Output();
            ResistiveTouchScreen_YMinus_Config_As_Input();

#ifdef ADPCFG_YPOS
            ADPCFG_YPOS = RESISTIVETOUCH_ANALOG;        // set to analog pin
#endif
#ifdef ADPCFG_YPOS
            ADPCFG_XPOS = RESISTIVETOUCH_DIGITAL;        // set to digital pin
#endif


            TOUCH_ADC_START = 1;    // run conversion

            state = CHECK_Y;
            break;

        default:
            state = SET_X;
            return 1;               // touch screen acquisition is done 
    }

    return 0;                       // touch screen acquisition is not done
}

/*********************************************************************
* Function: void Touch_ADCInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Initializes the A/D channel used for the touch detection.
*
* Note: none
*
********************************************************************/
void Touch_ADCInit(void)
{
#if defined (RESISTIVETOUCH_AUTO_SAMPLE_MODE)
    // Initialize ADC for auto sampling mode    
    AD1CON1 = 0;            // reset
    AD1CON2 = 0;            // AVdd, AVss, int every conversion, MUXA only
    AD1CON3 = 0x1FFF;       // 31 Tad auto-sample, Tad = 256*Tcy
    AD1CON1 = 0x80E0;       // Turn on A/D module, use auto-convert
#elif defined (RESISTIVETOUCH_MANUAL_SAMPLE_MODE)
    // Initialize ADC for manual sampling mode
    AD1CON1 = 0;            // reset
    AD1CON2 = 0;            // AVdd, AVss, int every conversion, MUXA only
    AD1CON3 = 0x0101;       // 1 Tad auto-sample, Tad = 2*Tcy
    AD1CON1 = 0x8000;       // Turn on A/D module, use manual convert
#else
    #error Resistive Touch sampling mode is not set.
#endif

}

/*********************************************************************
* Function: void TouchHardwareInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Initializes touch screen module.
*
* Note: none
*
********************************************************************/
void TouchHardwareInit(void *initValues)
{
    Touch_ADCInit();
    
    // set the used D/A port to be analog
	#ifdef ADPCFG_XPOS
    	ADPCFG_XPOS = RESISTIVETOUCH_ANALOG;
	#endif
	#ifdef ADPCFG_YPOS
    	ADPCFG_YPOS = RESISTIVETOUCH_ANALOG;
    #endif   
	#ifdef ADC_POT
    	ADC_POT_PCFG = RESISTIVETOUCH_ANALOG;
    #endif	
    
    AD1CSSL = 0;            // No scanned inputs
	
	state = SET_X;          // set the state of the statemachine to start the sampling

}

/*********************************************************************
* Function: SHORT TouchGetX()
*
* PreCondition: none
*
* Input: none
*
* Output: x coordinate
*
* Side Effects: none
*
* Overview: returns x coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetX(void)
{
    long    result;

    result = TouchGetRawX();

    if(result >= 0)
    {
        result = (long)((((long)_trC*result) + _trD)>>TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR);

		#ifdef TOUCHSCREEN_RESISTIVE_FLIP_X
			result = GetMaxX() - result;
		#endif	
	}
    return ((SHORT)result);
}

/*********************************************************************
* Function: SHORT TouchGetRawX()
*
* PreCondition: none
*
* Input: none
*
* Output: x coordinate
*
* Side Effects: none
*
* Overview: returns x coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetRawX(void)
{
    #ifdef TOUCHSCREEN_RESISTIVE_SWAP_XY
    return adcY;
    #else
    return adcX;
    #endif
}
/*********************************************************************
* Function: SHORT TouchGetY()
*
* PreCondition: none
*
* Input: none
*
* Output: y coordinate
*
* Side Effects: none
*
* Overview: returns y coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetY(void)
{

    long    result;

    result = TouchGetRawY();
    
    if(result >= 0)
    {
        result = (long)((((long)_trA*result) + (long)_trB)>>TOUCHSCREEN_RESISTIVE_CALIBRATION_SCALE_FACTOR);

		#ifdef TOUCHSCREEN_RESISTIVE_FLIP_Y
			result = GetMaxY() - result;
		#endif	
	}
    return ((SHORT)result);
}

/*********************************************************************
* Function: SHORT TouchGetRawY()
*
* PreCondition: none
*
* Input: none
*
* Output: y coordinate
*
* Side Effects: none
*
* Overview: returns y coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetRawY(void)
{
    #ifdef TOUCHSCREEN_RESISTIVE_SWAP_XY
    return adcX;
    #else
    return adcY;
    #endif
}

/*********************************************************************
* Function: void TouchStoreCalibration(void)
*
* PreCondition: Non-volatile memory initialization function must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: stores calibration parameters into non-volatile memory
*
* Note: none
*
********************************************************************/
void TouchStoreCalibration(void)
{
 	if (pCalDataWrite != NULL)
	{
		// the upper left X sample address is used since it is the first one
		// and this assumes that all stored values are located in one 
		// sector
		if (pCalDataSectorErase != NULL)
			pCalDataSectorErase(ADDRESS_RESISTIVE_TOUCH_ULX);

    	pCalDataWrite(xRawTouch[0], ADDRESS_RESISTIVE_TOUCH_ULX);
    	pCalDataWrite(yRawTouch[0], ADDRESS_RESISTIVE_TOUCH_ULY);

    	pCalDataWrite(xRawTouch[1], ADDRESS_RESISTIVE_TOUCH_URX);
    	pCalDataWrite(yRawTouch[1], ADDRESS_RESISTIVE_TOUCH_URY);

    	pCalDataWrite(xRawTouch[3], ADDRESS_RESISTIVE_TOUCH_LLX);
    	pCalDataWrite(yRawTouch[3], ADDRESS_RESISTIVE_TOUCH_LLY);

    	pCalDataWrite(xRawTouch[2], ADDRESS_RESISTIVE_TOUCH_LRX);
    	pCalDataWrite(yRawTouch[2], ADDRESS_RESISTIVE_TOUCH_LRY);

    	pCalDataWrite(mchpTouchScreenVersion, ADDRESS_RESISTIVE_TOUCH_VERSION);
   	
 	}

}

/*********************************************************************
* Function: void TouchLoadCalibration(void)
*
* PreCondition: Non-volatile memory initialization function must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: loads calibration parameters from non-volatile memory
*
* Note: none
*
********************************************************************/
void TouchLoadCalibration(void)
{

	if (pCalDataRead != NULL)
	{

    	xRawTouch[0] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_ULX);
    	yRawTouch[0] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_ULY);

    	xRawTouch[1] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_URX);
    	yRawTouch[1] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_URY);

    	xRawTouch[3] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_LLX);
    	yRawTouch[3] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_LLY);

    	xRawTouch[2] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_LRX);
    	yRawTouch[2] = pCalDataRead(ADDRESS_RESISTIVE_TOUCH_LRY);

	}

    TouchCalculateCalPoints();
    
}

/*********************************************************************
* Function: void TouchGetCalPoints(void)
*
* PreCondition: InitGraph() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: gets values for 3 touches
*
* Note: none
*
********************************************************************/
//void TouchCalculateCalPoints(WORD *xRawTouch, WORD *yRawTouch, WORD *xPoint, WORD *yPoint)
void TouchCalculateCalPoints(void)
{
	long trA, trB, trC, trD;                    // variables for the coefficients 
	long trAhold, trBhold, trChold, trDhold;
    long test1, test2;                          // temp variables (must be signed type)

    SHORT    xPoint[SAMPLE_POINTS], yPoint[SAMPLE_POINTS];

    yPoint[0] = yPoint[1] = CAL_Y_INSET; 
	yPoint[2] = yPoint[3] = (GetMaxY() - CAL_Y_INSET);
	xPoint[0] = xPoint[3] = CAL_X_INSET; 
	xPoint[1] = xPoint[2] = (GetMaxX() - CAL_X_INSET);
    
	// calculate points transfer functiona
	// based on two simultaneous equations solve for the 
	// constants
	
	// use sample points 1 and 4
	// Dy1 = aTy1 + b; Dy4 = aTy4 + b 
	// Dx1 = cTx1 + d; Dy4 = aTy4 + b

	test1 = (long)yPoint[0] - (long)yPoint[3];
	test2 = (long)yRawTouch[0] - (long)yRawTouch[3];
	
	trA = ((long)((long)test1 * SCALE_FACTOR) / test2);
	trB = ((long)((long)yPoint[0] * SCALE_FACTOR) - (trA * (long)yRawTouch[0]));
    
	test1 = (long)xPoint[0] - (long)xPoint[2];
	test2 = (long)xRawTouch[0] - (long)xRawTouch[2];

	trC = ((long)((long)test1 * SCALE_FACTOR) / test2);
	trD = ((long)((long)xPoint[0] * SCALE_FACTOR) - (trC * (long)xRawTouch[0]));

	trAhold = trA; 
	trBhold = trB; 
	trChold = trC; 
	trDhold = trD; 

	// use sample points 2 and 3
	// Dy2 = aTy2 + b; Dy3 = aTy3 + b 
	// Dx2 = cTx2 + d; Dy3 = aTy3 + b

	test1 = (long)yPoint[1] - (long)yPoint[2];
	test2 = (long)yRawTouch[1] - (long)yRawTouch[2];

	trA = ((long)(test1 * SCALE_FACTOR) / test2);
	trB = ((long)((long)yPoint[1] * SCALE_FACTOR) - (trA * (long)yRawTouch[1]));

	test1 = (long)xPoint[1] - (long)xPoint[3];
	test2 = (long)xRawTouch[1] - (long)xRawTouch[3];

	trC = ((long)((long)test1 * SCALE_FACTOR) / test2);
	trD = ((long)((long)xPoint[1] * SCALE_FACTOR) - (trC * (long)xRawTouch[1]));

	// get the average and use the average
	_trA = (trA + trAhold) >> 1;
	_trB = (trB + trBhold) >> 1;
	_trC = (trC + trChold) >> 1;
	_trD = (trD + trDhold) >> 1;

}

/*********************************************************************
* Function: void TouchGetCalPoints(void)
*
* PreCondition: InitGraph() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: gets values for 3 touches
*
* Note: none
*
********************************************************************/
void TouchCalHWGetPoints(void)
{
	#define TOUCH_DIAMETER	10
	#define SAMPLE_POINTS   4

    XCHAR   calStr1[] = {'o','n',' ','t','h','e',' ','f','i','l','l','e','d',0};
    XCHAR   calStr2[] = {'c','i','r','c','l','e',0};
    XCHAR  	calTouchPress[] = {'P','r','e','s','s',' ','&',' ','R','e','l','e','a','s','e',0};

    XCHAR   calRetryPress[] = {'R','e','t','r','y',0};
    XCHAR   *pMsgPointer;
    SHORT   counter;

    WORD    dx[SAMPLE_POINTS], dy[SAMPLE_POINTS];
    WORD    textHeight, msgX, msgY;
    SHORT   tempX, tempY;

	SetFont((void *) &FONTDEFAULT);
    SetColor(RESISTIVETOUCH_FOREGROUNDCOLOR);

    textHeight = GetTextHeight((void *) &FONTDEFAULT);

    while
    (
        !OutTextXY
            (
                (GetMaxX() - GetTextWidth((XCHAR *)calStr1, (void *) &FONTDEFAULT)) >> 1,
                (GetMaxY() >> 1),
                (XCHAR *)calStr1
            )
    );

    while
    (
        !OutTextXY
            (
                (GetMaxX() - GetTextWidth((XCHAR *)calStr2, (void *) &FONTDEFAULT)) >> 1,
                ((GetMaxY() >> 1) + textHeight),
                (XCHAR *)calStr2
            )
    );

    // calculate center points (locate them at 15% away from the corners)
	// draw the four touch points

    dy[0] = dy[1] = CAL_Y_INSET; 
	dy[2] = dy[3] = (GetMaxY() - CAL_Y_INSET);
	dx[0] = dx[3] = CAL_X_INSET; 
	dx[1] = dx[2] = (GetMaxX() - CAL_X_INSET);


    msgY = ((GetMaxY() >> 1) - textHeight);
    pMsgPointer = calTouchPress;
	
    // get the four samples or calibration points
    for(counter = 0; counter < SAMPLE_POINTS;)
    {
    
		// redraw the filled circle to unfilled (previous calibration point)
        if (counter > 0)
        {
        	SetColor(RESISTIVETOUCH_BACKGROUNDCOLOR);
	    	while(!(FillCircle(dx[counter-1], dy[counter-1], TOUCH_DIAMETER-3)));
	    } 

		// draw the new filled circle (new calibration point)
        SetColor(RESISTIVETOUCH_FOREGROUNDCOLOR);
	    while(!(Circle(dx[counter], dy[counter], TOUCH_DIAMETER)));
	    while(!(FillCircle(dx[counter], dy[counter], TOUCH_DIAMETER-3)));

		// show points left message
        msgX = (GetMaxX() - GetTextWidth((XCHAR *)pMsgPointer, (void *) &FONTDEFAULT)) >> 1;
		TouchShowMessage(pMsgPointer, RESISTIVETOUCH_FOREGROUNDCOLOR, msgX, msgY);

        // Wait for press
        do {} 
    	    while((TouchGetRawX() == -1) && (TouchGetRawY() == -1));

        tempX = TouchGetRawX();
        tempY = TouchGetRawY();

        // wait for release
        do {}
        	while((TouchGetRawX() != -1) && (TouchGetRawY() != -1));

        // check if the touch was detected properly
        if ((tempX == -1) || (tempY == -1))
        {   
            // cannot proceed retry the touch, display RETRY PRESS message 

            // remove the previous string
	    	TouchShowMessage(pMsgPointer, RESISTIVETOUCH_BACKGROUNDCOLOR, msgX, msgY);
            pMsgPointer = calRetryPress;
            // show the retry message
    		msgX = (GetMaxX() - GetTextWidth((XCHAR *)pMsgPointer, (void *) &FONTDEFAULT)) >> 1;
	    	TouchShowMessage(pMsgPointer, RESISTIVETOUCH_FOREGROUNDCOLOR, msgX, msgY);
        }
        else
        {    

            // remove the previous string
	    	TouchShowMessage(pMsgPointer, RESISTIVETOUCH_BACKGROUNDCOLOR, msgX, msgY);
            pMsgPointer = calTouchPress;



    		#ifdef TOUCHSCREEN_RESISTIVE_FLIP_Y
            yRawTouch[3 - counter] = tempY; //TouchGetRawY();
    		#else
            yRawTouch[counter] = tempY; //ouchGetRawY();
            #endif
    
            #ifdef TOUCHSCREEN_RESISTIVE_FLIP_X
            xRawTouch[3 - counter] = tempX; //TouchGetRawX();
    		#else
            xRawTouch[counter] = tempX; //TouchGetRawX();
            #endif

            counter++;

        }

        // Wait for release
        do {} 
        	while((TouchGetRawX() != -1) && (TouchGetRawY() != -1));

        DelayMs(CALIBRATION_DELAY);
      
    }

    TouchCalculateCalPoints();
 
    #ifdef ENABLE_DEBUG_TOUCHSCREEN
        TouchScreenResistiveTestXY();
    #endif
}


/*********************************************************************
* Function: void TouchScreenResistiveTestXY(void)
*
* PreCondition: TouchHardwareInit has been called
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: prints raw x,y calibrated x,y and calibration factors to screen
*
* Note: modify pre-processor macro to include/exclude this test code
*       a common place to call this from is at the end of TouchCalHWGetPoints()
*
********************************************************************/
#ifdef ENABLE_DEBUG_TOUCHSCREEN
#include <stdio.h>
void TouchScreenResistiveTestXY(void)
{
    #define BUFFCHARLEN 60
    char buffChar[BUFFCHARLEN];
    WORD buffCharW[BUFFCHARLEN];
    unsigned char i;
    SHORT tempXX, tempYY,tempXX2,tempYY2, calXX, calYY;
    tempXX = tempYY = -1;
    tempXX2 = tempYY2 = 0;

    // store the last calibration
    TouchStoreCalibration();

    while(1)
    {
       
        // use this to always show the values even if not pressing the screen
//        tempXX = TouchGetRawX(); 
//        tempYY = TouchGetRawY();
        
        calXX = TouchGetX();
        calYY = TouchGetY();
        
        if((tempXX != tempXX2)||(tempYY != tempYY2))
        {
            SetColor(RESISTIVETOUCH_BACKGROUNDCOLOR);
            ClearDevice();
            SetColor(RESISTIVETOUCH_FOREGROUNDCOLOR);
            sprintf(buffChar,"raw_x=%d, raw_y=%d",(WORD)tempXX, (WORD)tempYY);
			
            #ifdef USE_MULTIBYTECHAR
              for(i = 0; i < BUFFCHARLEN;i++)
              {
                buffCharW[i] = buffChar[i];
              }
              while(!OutTextXY(0,0,(XCHAR*)buffCharW));
            #else
              while(!OutTextXY(0,0,(XCHAR*)buffChar));
            #endif
            
            sprintf(buffChar,"cal_x=%d, cal_y=%d",(WORD)calXX, (WORD)calYY);
            #ifdef USE_MULTIBYTECHAR
              for(i = 0; i < BUFFCHARLEN;i++)
              {
                buffCharW[i] = buffChar[i];
              }
              while(!OutTextXY(0,40,(XCHAR*)buffCharW));
            #else
              while(!OutTextXY(0,40,(XCHAR*)buffChar));
            #endif
            
            sprintf(buffChar,"_tr:A=%d,B=%d",(WORD)_trA,(WORD)_trB);
            #ifdef USE_MULTIBYTECHAR
              for(i = 0; i < BUFFCHARLEN;i++)
              {
                buffCharW[i] = buffChar[i];
              }
              while(!OutTextXY(0,80,(XCHAR*)buffCharW));
            #else
              while(!OutTextXY(0,80,(XCHAR*)buffChar));
            #endif

            sprintf(buffChar,"_tr:C=%d,D=%d",(WORD)_trC,(WORD)_trD);
            #ifdef USE_MULTIBYTECHAR
              for(i = 0; i < BUFFCHARLEN;i++)
              {
                buffCharW[i] = buffChar[i];
              }
              while(!OutTextXY(0,100,(XCHAR*)buffCharW));
            #else
              while(!OutTextXY(0,100,(XCHAR*)buffChar));
            #endif


        }
        
        tempXX2=tempXX;
        tempYY2=tempYY;

        do{tempXX = TouchGetRawX(); tempYY = TouchGetRawY();}
    	while((tempXX == -1) && (tempYY == -1));   
    }
}
#endif //#ifdef ENABLE_DEBUG_TOUCHSCREEN


#endif // #if defined (USE_TOUCHSCREEN_RESISTIVE)

