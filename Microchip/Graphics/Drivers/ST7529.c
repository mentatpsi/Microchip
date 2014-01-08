/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Sitronix ST7529 controller driver
 *****************************************************************************
 * FileName:        ST7529.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/20/08	    ...
 * 04/12/11     Graphics Library Version 3.00 Support
 *****************************************************************************/
#include "HardwareProfile.h"

#if defined (GFX_USE_DISPLAY_CONTROLLER_ST7529) 

#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/ST7529.h"
#include "Graphics/Primitive.h"

#if defined (USE_GFX_PMP)
    #include "Graphics/gfxpmp.h"
#elif defined (USE_GFX_EPMP)
    #include "Graphics/gfxepmp.h"
#endif  

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;  

// Color
GFX_COLOR   _color;
#ifdef USE_TRANSPARENT_COLOR
GFX_COLOR   _colorTransparent;
SHORT       _colorTransparentEnable;
#endif

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void)
{
	// Initialize device
	DriverInterfaceInit();

    DelayMs(20);
    DisplayEnable();
	
	DisplaySetCommand();
    DeviceWrite(EXTIN);        //Ext = 0
    DeviceWrite(SLPOUT);       //Sleep Out
    DeviceWrite(OSCON);        //OSC On
    DeviceWrite(PWRCTRL);      //Power Control Set
	DisplaySetData();
    DeviceWrite(0x08);        //Booster Must Be On First
    DelayMs(2);
	DisplaySetCommand();
    DeviceWrite(0x20);         //Power Control Set
	DisplaySetData();
    DeviceWrite(0x0B);        //Booster, Regulator, Follower ON
	DisplaySetCommand();
    DeviceWrite(VOLCTRL);      //Electronic Control
	DisplaySetData();
    DeviceWrite(0x3f);        //Vop=18.0V
    DeviceWrite(0x04);
	DisplaySetCommand();
    DeviceWrite(DISCTRL);      //Display Control
	DisplaySetData();
    DeviceWrite(0x00);        //CL=X1
    DeviceWrite(0x27);        //Duty=160
    DeviceWrite(0x00);        //FR Inverse-Set Value
	DisplaySetCommand();
    DeviceWrite(DISNOR);       // Normal Display
    DeviceWrite(COMSCN);       //COM Scan Direction
	DisplaySetData();
    DeviceWrite(0x01);        // 0->79 159->80
	DisplaySetCommand();
    DeviceWrite(DATSDR);       //Data Scan Direction
	DisplaySetData();
    #if (DISP_ORIENTATION == 180)
    DeviceWrite(0x01);        //Row Reverse
    DeviceWrite(0x00);        //Derect 3 Pixels Arrangement
    #else
    DeviceWrite(0x02);        //Column Reverse
    DeviceWrite(0x01);        //Inverse 3 Pixels Arrangement
    #endif
    DeviceWrite(0x02);        //3Byte 3Pixel mode
	DisplaySetCommand();
    DeviceWrite(LASET);        //Line Address Set(lines from 16 to 144 are used)
	DisplaySetData();
    DeviceWrite(0x10);        //Start Line=16
    DeviceWrite(0x8f);        //End Line =144-1
	DisplaySetCommand();
    DeviceWrite(CASET);        //Column Address Set
	DisplaySetData();
    DeviceWrite(0x00);        //Start Column=0
    DeviceWrite(0x54);        //End Column =84 ((84+1)*3 == 255)
	DisplaySetCommand();
    DeviceWrite(EXTOUT);       //Ext = 1
    DeviceWrite(ANASET);       //Analog Circuit Set
	DisplaySetData();
    DeviceWrite(0x00);        //OSC Frequency =000 (Default)
    DeviceWrite(0x01);        //Booster Efficiency=01(Default)
    DeviceWrite(0x00);        //Bias=1/14
	DisplaySetCommand();
    DeviceWrite(SWINT);        //Software Initial
    DeviceWrite(EXTIN);        //Ext = 0
    DeviceWrite(DISON);        //Display On
    DisplayDisable();
	DisplaySetData();
    DelayMs(100);
}

/*********************************************************************
* Function: void ContrastSet(WORD contrast)
*
* PreCondition: none
*
* Input: contrast value (LSB 10-0 are valid, MSB 15-11 are not used)
*
* Output: none
*
* Side Effects: none
*
* Overview: sets contrast
*
* Note: none
*
********************************************************************/
void ContrastSet(WORD contrast)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(VOLCTRL); 
	DisplaySetData();               
    DeviceWrite((((WORD_VAL) contrast).v[0]));
    DeviceWrite((((WORD_VAL) contrast).v[1]));
    DisplayDisable();
}

/*********************************************************************
* Function: void ContrastUp(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: increases contrast
*
* Note: none
*
********************************************************************/
void ContrastUp(void)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(VOLUP);
    DisplayDisable();
	DisplaySetData();
}

/*********************************************************************
* Function: void ContrastDown(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: decreases contrast
*
* Note: none
*
********************************************************************/
void ContrastDown(void)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(VOLDOWN);  //Electronic Control
    DisplayDisable();
	DisplaySetData();
}

/*********************************************************************
* Function: void DisplayOn(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: turns on display
*
* Note: none
*
********************************************************************/
void DisplayOn(void)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(DISON);
    DisplayDisable();
	DisplaySetData();
}

/*********************************************************************
* Function: void DisplayOn(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: turns off display
*
* Note: none
*
********************************************************************/
void DisplayOff(void)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(DISOFF);
    DisplayDisable();
	DisplaySetData();
}

/*********************************************************************
* Function: void SleepIn(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: enters sleep mode
*
* Note: none
*
********************************************************************/
void SleepIn(void)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(SLPIN);
    DisplayDisable();
	DisplaySetData();
}

/*********************************************************************
* Function: void WakeUp(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: wakes up from sleep
*
* Note: none
*
********************************************************************/
void WakeUp(void)
{
    DisplayEnable();
	DisplaySetCommand();
    DeviceWrite(SLPOUT);
    DisplayDisable();
	DisplaySetData();
}

#ifdef USE_TRANSPARENT_COLOR
/*********************************************************************
* Function:  void TransparentColorEnable(GFX_COLOR color)
*
* Overview: Sets current transparent color.
*
* PreCondition: none
*
* Input: color - Color value chosen.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void TransparentColorEnable(GFX_COLOR color)
{
    _colorTransparent = color;    
    _colorTransparentEnable = TRANSPARENT_COLOR_ENABLE;

}
#endif

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: pixel position
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel with current color at given position
*
* Note: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y)
{
    BYTE    columnPixel[3];         // 3 Pixels in each column
    if(_clipRgn)
    {
        if(x < _clipLeft)
            return;
        if(x > _clipRight)
            return;
        if(y < _clipTop)
            return;
        if(y > _clipBottom)
            return;
    }

    DisplayEnable();

    // Set Row and Column Address
	DisplaySetCommand();
    DeviceWrite(LASET);
	DisplaySetData();
    DeviceWrite(y + 16);
    DeviceWrite(0x8f);
	DisplaySetCommand();
    DeviceWrite(CASET);
	DisplaySetData();
    DeviceWrite(x / 3);
    DeviceWrite(0x54);

    // Read Column
	DisplaySetCommand();
    DeviceWrite(RMWIN);
	DisplaySetData();
    columnPixel[0] = DeviceRead();    // Dummy reading
    columnPixel[0] = DeviceRead();    // Start reading cycle for pixel 0
    columnPixel[1] = DeviceRead();    // Start reading cycle for pixel 1
    columnPixel[2] = DeviceRead();    // Start reading cycle for pixel 2

    // Modify pixel
    columnPixel[x % 3] = _color;

    // Write Column
    DeviceWrite(columnPixel[0]);
    DeviceWrite(columnPixel[1]);
    DeviceWrite(columnPixel[2]);

	DisplaySetCommand();
    DeviceWrite(RMWOUT);
    DisplayDisable();
	DisplaySetData();
}

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: pixel position
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: returns pixel at given position
*
* Note: none
*
********************************************************************/
GFX_COLOR GetPixel(SHORT x, SHORT y)
{
    BYTE    columnPixel[3];         // 3 Pixels in each column
    DisplayEnable();

    // Set Row and Column Address
	DisplaySetCommand();
    DeviceWrite(LASET);
	DisplaySetData();
    DeviceWrite(y + 16);
    DeviceWrite(0x8f);
	DisplaySetCommand();
    DeviceWrite(CASET);
	DisplaySetData();
    DeviceWrite(x / 3);
    DeviceWrite(0x54);

    // Read Column
	DisplaySetCommand();
    DeviceWrite(RMWIN);
	DisplaySetData();
    columnPixel[0] = DeviceRead();    // Dummy
    columnPixel[0] = DeviceRead();    // Start reading cycle for pixel 0
    columnPixel[1] = DeviceRead();    // Start reading cycle for pixel 1
    columnPixel[2] = DeviceRead();    // Start reading cycle for pixel 2
	DisplaySetCommand();
    DeviceWrite(RMWOUT);
    DisplayDisable();
	DisplaySetData();

    return (columnPixel[x % 3]);
}

/*********************************************************************
* Function: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
WORD IsDeviceBusy(void)
{  
    return (0);
}

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color 
*
* Note: none
*
********************************************************************/
void ClearDevice(void)
{
    WORD    counter;

    DisplayEnable();

    // Whole screen
	DisplaySetCommand();
    DeviceWrite(LASET);    //Line Address Set(lines from 16 to 144 are used)
	DisplaySetData();
    DeviceWrite(0x10);    //Start Line=16
    DeviceWrite(0x8f);    //End Line =144-1
	DisplaySetCommand();
    DeviceWrite(CASET);    //Column Address Set
	DisplaySetData();
    DeviceWrite(0x00);    //Start Column=0
    DeviceWrite(0x54);    //End Column =84 ((84+1)*3 == 255)
	DisplaySetCommand();
    DeviceWrite(RAMWR);
	DisplaySetData();
    for(counter = 0; counter < (WORD) (GetMaxX() + 1) * (GetMaxY() + 1); counter++)
    {
        DeviceWrite(_color);
    }

    DisplayDisable();
}

#endif //(GFX_USE_DISPLAY_CONTROLLER_ST7529) 

