/******************************************************************************

Demonstrations of the PIC24F Microcontroller

This file contains demonstrations that can be performed on the PIC24F
Starter Kit 1, showing the power and versatility of the PIC24F microcontroller.


RGB LED Control

The PIC24F Starter Kit contains an RGB LED that can produce nearly any color
by separately setting the red, green, and blue components of the color.  Each
color is controlled by a separate PWM on the PIC24F, which is configured using
Peripheral Pin Select to connect the proper pins to the appropriate PWM. The
pins are also configured to be open drain, enabling the complete range of
PWM control.

Select the color to modify by using the up and down touchpads.  Increase or
decrease the color intensity by using the right or left touchpads.  Exit the
demo by touching the center touchpad.

Saturation values are typically given in the range of 0x00-0xFF, so the PWM
timer and the sliders for each color are set to this range.  To interface with
the PWM module, we made two minor adjustments.  First, we cannot set the data
register OCxRS to 0, because that will generate a 50% duty cycle pulse at half
the desired frequency.  So the maximum saturation value we can set is 0xFE.
Alternately, the code can be modified to turn off the PWM and pull the output
low at the maximum saturation, and set the maximum saturation to 0xFF.
Second, to fully turn off the PWM when the saturation is 0, we have to set
OCxRS to 0x100 rather than the maximum saturation value; otherwise, we still
get a small pulse.


Real-time Graph

The PIC24F Starter Kit contains a 128x64 OLED graphical display and a
potentiometer connected to AN0.  The PIC24F can monitor the potentiometer
with a sample interval of 10ms while monitoring the five touchpads and
displaying the reading on a scrolling graph.

The update rate of the scrolling graph can be changed be using the touchpads.
The left touchpad decreases the update rate, and the right touchpad increases
the update rate.  The graphical display rate has no effect on the sample rate.
The demo exits when the up arrow touchpad is touched.


Real-time Capture and Graph

This demo is an expansion of the Real-time Graph demo.  In addition to the
functionality described above, the sampled data is also stored to a flash
drive under the file name of CAPTURE.CSV.  Sampling is not begun until a
flash drive is inserted.  It is terminated when the up arrow touchpad is
touched. NOTE: The capture will also terminate if the flash drive is
removed, but doing so may corrupt the flash drive, since the application
might be in the middle of a write operation.

Like the previous demo, the sample rate is constant, and is unaffected by the
graph update rate.  The data is stored on the flash drive, with a timestamp
of the number of 10ms intervals that have elapsed since the starter kit
began execution.  Note that while the graph is not guaranteed to display
every sample taken, the captured data file will contain every sample taken.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       Demos.c
* Dependencies:    None
* Processor:       PIC24FJ256GB106
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
KO          14-Feb-2008 First release

*******************************************************************************/
//DOM-IGNORE-END

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PIC24F Starter Kit.h"

//******************************************************************************
//******************************************************************************
// Constants
//******************************************************************************
//******************************************************************************

// To control the color, write the inverse of the saturation value to the PWM.
#define CONVERT_TO_COLOR(x)         (~x & 0xFF)

#define GRAPH_DATA_BUFFER_SIZE      (GRAPH_GRID_WIDTH + 1)

#define GRAPH_GRID_LEFT             (GRAPH_PANEL_LEFT + GOL_EMBOSS_SIZE)
#define GRAPH_GRID_RIGHT            (GRAPH_PANEL_RIGHT - GOL_EMBOSS_SIZE)
#define GRAPH_GRID_TOP              (GRAPH_PANEL_TOP + GOL_EMBOSS_SIZE)
#define GRAPH_GRID_BOTTOM           (GRAPH_PANEL_BOTTOM - GOL_EMBOSS_SIZE)

#define GRAPH_GRID_WIDTH            (GRAPH_GRID_RIGHT - GRAPH_GRID_LEFT)
#define GRAPH_GRID_HEIGHT           (GRAPH_GRID_BOTTOM - GRAPH_GRID_TOP)

#define GRAPH_PANEL_LEFT            0
#define GRAPH_PANEL_RIGHT           GetMaxX()
#define GRAPH_PANEL_TOP             LOCATION_MIN_Y
#define GRAPH_PANEL_BOTTOM          63 //(GetMaxY())

#define GRAPH_UPDATE_INTERVAL_INIT  (100 / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_MAX   (2000 / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_MIN   (MILLISECONDS_PER_TICK / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_STEP  (MILLISECONDS_PER_TICK / MILLISECONDS_PER_TICK)

#define GRID_SPACING_VERTICAL       (GRAPH_GRID_HEIGHT >> 3)    // ((GRAPH_GRID_BOTTOM - GRAPH_GRID_TOP) >> 3)
#define GRID_SPACING_HORIZONTAL     (GRAPH_GRID_WIDTH >> 3)     // ((GRAPH_GRID_RIGHT - GRAPH_GRID_LEFT) >> 3)

#define ID_DEMOS_RGB                ID_TOP_LEFT
#define ID_DEMOS_HOME               ID_TOP_RIGHT
#define ID_DEMOS_CAPTURE            ID_BOTTOM_LEFT
#define ID_DEMOS_GRAPH              ID_BOTTOM_RIGHT
#define ID_DEMOS_BAR                ID_THIRDROW

#define ID_GRAPH_FASTER             20
#define ID_GRAPH_HOME               21
#define ID_GRAPH_SLOWER             22
#define ID_MEDIA_REQUEST            23

#define ID_RGB_RED                  80
#define ID_RGB_RED_SLIDER           81
#define ID_RGB_GREEN                82
#define ID_RGB_GREEN_SLIDER         83
#define ID_RGB_BLUE                 84
#define ID_RGB_BLUE_SLIDER          85
#define ID_RGB_INTENSITY            86
#define ID_RGB_INTENSITY_SLIDER     87
#define ID_RGB_BUTTON_HOME          88
#define ID_RGB_BUTTON_NEXT          89
#define ID_RGB_BUTTON_PREVIOUS      90

#define INITIAL_BLUE                (SLIDER_COLOR_MAX/2)    // Initial blue saturation.  Do not set to 0.
#define INITIAL_GREEN               (SLIDER_COLOR_MAX/2)    // Initial green saturation.  Do not set to 0.
#define INITIAL_RED                 (SLIDER_COLOR_MAX/2)    // Initial red saturation.  Do not set to 0.

#define TEXT_HEIGHT                 GetTextHeight( (void *)&FONTDEFAULT )

#define SLIDER_PAGE_SIZE            16

#define LABEL_WIDTH                 40

#define PWM_CONFIGURATION_1         0x0007
#define PWM_CONFIGURATION_2         0x000C
#define PWM_OFF                     0x0000

#define RGB_TOP_START               LOCATION_MIN_Y

#define SLIDER_COLOR_MAX            0xFE
#define SLIDER_COLOR_MIN            0x0000
#define SLIDER_HEIGHT               GetTextHeight( (void *)&FONTDEFAULT )

#define SLIDER_RED                  0
#define SLIDER_GREEN                1
#define SLIDER_BLUE                 2


#define TRIS_INPUT                  1
#define TRIS_OUTPUT                 0


//******************************************************************************
//******************************************************************************
// Local Function Prototypes
//******************************************************************************
//******************************************************************************

void        DrawGridHorizontals( void );
void        DrawGridVerticals( void );
void        WriteOneBuffer( FSFILE *fptr, BYTE *data, WORD size );


//******************************************************************************
//******************************************************************************
// Global Variables
//******************************************************************************
//******************************************************************************

FSFILE              *captureFile;
BYTE                graphData[GRAPH_DATA_BUFFER_SIZE];
WORD                graphDataHead;
WORD                graphDataTail;
BYTE                graphOffset;
WORD                graphUpdateRate;
volatile BYTE       logBufferReading;
BYTE                logBufferWriting;
volatile LOG_DATA   logData[NUM_LOG_BUFFERS];
extern BOOL         mediaPresent;
const char          mediaRequest[] = "Insert USB flash drive...";
volatile WORD       potADC;
extern BYTE         usbErrorCode;


/****************************************************************************
  Function:
    void  AdjustColorPWM( WORD sliderID )

  Description:
    This function should be called when one of the color saturation sliders
    changes.  This routine will determine the new PWM output for the changed
    slider and set the PWM accordingly.

  Precondition:
    ShowScreenRGB() must be called prior to calling this function.

  Parameters:
    WORD sliderID   - ID of the slider to change
    SLIDER *pSlider - Pointer to the slider being changed.

  Returns:
    None

  Remarks:
    The PWM's run from 0 to 255, to match the saturation value.  The sliders
    run from 0 to 254.  The value 255 is omitted, because the value written
    to the PWM for the indicated saturation is the inverse of the slider
    value, and 0 (the inverse of 255) cannotbe written to the PWM.

    When the saturation level is 0, the PWM is turned off, and the pins are
    set to inputs to allow the LED's input to reach ~3.3V.  When the
    saturation level is greater than 0, the PWM is turned on if it was
    previously off, and the converted value is written to the PWM.
  ***************************************************************************/

void  AdjustColorPWM( WORD sliderID )
{
    WORD    saturation;

    saturation  = SldGetPos( (SLIDER*)GOLFindObject( sliderID ) );

    if (sliderID == ID_RGB_RED_SLIDER) // Red slider changed
    {
        RGBSetRed( saturation );
    }

    else if (sliderID == ID_RGB_GREEN_SLIDER) // Green slider changed
    {
        RGBSetGreen( saturation );
    }

    else if (sliderID == ID_RGB_BLUE_SLIDER) // Blue slider changed
    {
        RGBSetBlue( saturation );
    }
}


/****************************************************************************
  Function:
    void BarHS2(SHORT xtop, SHORT ytop, SHORT xbot, SHORT ybot)

  Description:
    This function is a high-speed bar drawing routine for the capacitive
    touch demo.  It is designed for the SH1101A driver.

  Precondition:
    None

  Parameters:
    SHORT xtop  - Left X coordinate
    SHORT ytop  - Top Y coordinate
    SHORT xbot  - Right X coordinate
    SHORT ybot  - Bottom Y coordinate

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void BarHS2(SHORT xtop, SHORT ytop,SHORT xbot, SHORT ybot)
{
    BYTE pg, ptop, pbot, lAddr, hAddr, wd;
    BYTE masktop, maskbot, display;
    BYTE maskval[9]={0,1,3,7,15,31,63,127,255};
    BYTE aa;

	// adjust bar coordinates to fit in clipping region
    if(_clipRgn)
    {
        if(xtop<_clipLeft)
            xtop=_clipLeft;
        if(xbot>_clipRight)
            xbot=_clipRight;
        if(ytop<_clipTop)
            ytop=_clipTop;
        if(ybot>_clipBottom)
            ybot=_clipBottom;
    }

	xtop+=OFFSET;
	xbot+=OFFSET;

	lAddr=xtop&0x0F;
	hAddr=0x10|(xtop>>4);

	masktop=~maskval[ytop&0x07];
	maskbot=maskval[(ybot&0x07)+1];

	ptop=ytop>>3;
	pbot=ybot>>3;
	if((ybot&0x07)==7) pbot++;	//use full byte writing instead of read-modify-write
	pg=ptop;
	wd=ybot-ytop+1;

	DisplayEnable();
	
	if(masktop!=0xff||wd<8) //kick in if bar width less than 8 and skip full byte write part
	{
		if(wd<8) masktop=masktop&maskval[(ytop&0x07)+wd];
		DisplaySetCommand(); 
		DeviceWrite(0xB0+pg);
		DeviceWrite(lAddr);
		DeviceWrite(hAddr);
		DeviceWrite(0xE0);
		DisplaySetData();
		for(aa=0;aa<xbot-xtop+1;aa++)
		{
			display = DeviceRead();
			display = DeviceRead();
			display=display&(~masktop); //clear bits included in zone
			display=display|(masktop&_color); //set bits with patern
			DeviceWrite(display);
		}
		DisplaySetCommand(); 
		DeviceWrite(0xEE);
		pg++;
	}

	if(wd>7) for(;pg<pbot;pg++)
	{
		DisplaySetCommand(); 
		DeviceWrite(0xB0+pg);
		DeviceWrite(lAddr);
		DeviceWrite(hAddr);
		DisplaySetData(); 
		for(aa=0;aa<xbot-xtop+1;aa++)
		{
			DeviceWrite(_color);
		}
	}

	if(maskbot!=0xff&&ptop!=pbot)
	{

		DisplaySetCommand(); 
		DeviceWrite(0xB0+pg);
		DeviceWrite(lAddr);
		DeviceWrite(hAddr);
		DeviceWrite(0xE0);
		DisplaySetData(); 
		for(aa=0;aa<xbot-xtop+1;aa++)
		{
			display = DeviceRead();
			display = DeviceRead();
			display=display&(~maskbot);
			display=display|(maskbot&_color);
			DeviceWrite(display);
		}
		DisplaySetCommand(); 
		DeviceWrite(0xEE);
		DisplaySetData(); 
	}

	DisplayDisable(); 

}


/****************************************************************************
  Function:
    void BarHSP(SHORT xtop, SHORT ytop,SHORT xbot, SHORT ybot, SHORT pat, SHORT xo)

  Description:
    This function is a high-speed patterned bar drawing routine for the
    capacitive touch demo.  It is designed for the SH1101A driver.

  Precondition:
    None

  Parameters:
    SHORT xtop  - Left X coordinate
    SHORT ytop  - Top Y coordinate
    SHORT xbot  - Right X coordinate
    SHORT ybot  - Bottom Y coordinate
    SHORT pat   - Pattern byte
    SHORT xo    - Byte mask to be XOR'd with the pattern byte

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void BarHSP(SHORT xtop, SHORT ytop,SHORT xbot, SHORT ybot, SHORT pat, SHORT xo)
{
    BYTE pg, ptop, pbot, lAddr, hAddr, wd;
    BYTE masktop, maskbot, display;
    BYTE maskval[9]={0,1,3,7,15,31,63,127,255};
    BYTE aa;

	// adjust bar coordinates to fit in clipping region
    if(_clipRgn)
    {
        if(xtop<_clipLeft)
            xtop=_clipLeft;
        if(xbot>_clipRight)
            xbot=_clipRight;
        if(ytop<_clipTop)
            ytop=_clipTop;
        if(ybot>_clipBottom)
            ybot=_clipBottom;
    }

	xtop+=OFFSET;
	xbot+=OFFSET;

	lAddr=xtop&0x0F;
	hAddr=0x10|(xtop>>4);

	masktop=~maskval[ytop&0x07];
	maskbot=maskval[(ybot&0x07)+1];

	ptop=ytop>>3;
	pbot=ybot>>3;
	if((ybot&0x07)==7) pbot++;	//use full byte writing instead of read-modify-write
	pg=ptop;
	wd=ybot-ytop+1;

	DisplayEnable(); 

	if(masktop!=0xff||wd<8) //kick in if bar width less than 8 and skip full byte write part
	{

		if(wd<8) masktop=masktop&maskval[(ytop&0x07)+wd];
		DisplaySetCommand(); 
		DeviceWrite(0xB0+pg);
		DeviceWrite(lAddr);
		DeviceWrite(hAddr);
		DeviceWrite(0xE0);
		DisplaySetData(); 
		for(aa=0;aa<xbot-xtop+1;aa++)
		{
			display = DeviceRead();
			display = DeviceRead(); 
			display=display&(~masktop); //clear bits included in zone
			display=display|(masktop&pat); //set bits with patern
			DeviceWrite(display);
			pat^=xo;
		}
		DisplaySetCommand(); 
		DeviceWrite(0xEE);
		pg++;
	}

	if(wd>7) for(;pg<pbot;pg++)
	{
		DisplaySetCommand(); 
		DeviceWrite(0xB0+pg);
		DeviceWrite(lAddr);
		DeviceWrite(hAddr);
		DisplaySetData(); 
		for(aa=0;aa<xbot-xtop+1;aa++)
		{
			DeviceWrite(pat);
			pat^=xo;
		}
	}

	if(maskbot!=0xff&&ptop!=pbot)
	{
		DisplaySetCommand(); 
		DeviceWrite(0xB0+pg);
		DeviceWrite(lAddr);
		DeviceWrite(hAddr);
		DeviceWrite(0xE0);
		DisplaySetData(); 
		for(aa=0;aa<xbot-xtop+1;aa++)
		{
			display = DeviceRead();
			display = DeviceRead();
			display=display&(~maskbot);
			display=display|(maskbot&pat);
			DeviceWrite(display);
			pat^=xo;
		}
		DisplaySetCommand(); 
		DeviceWrite(0xEE);
		DisplaySetData(); 
	}

	DisplayDisable(); 

}


/****************************************************************************
  Function:
    void CaptureMonitorMedia( void )

  Description:
    This function calls the background tasks necessary to support USB Host
    operation.  Upon initial insertion of the media, it initializes the file
    system support and reads the _volume label. Upon removal of the media,
    the _volume label is marked invalid.  If we are capturing data and a
    buffer is full, that buffer is written to the flash drive.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void CaptureMonitorMedia( void )
{
    BYTE            mediaPresentNow;
    BYTE            mountTries;

    USBTasks();

    mediaPresentNow = USBHostMSDSCSIMediaDetect();
    if (mediaPresentNow != mediaPresent)
    {
        if (mediaPresentNow)
        {
            // Media has attached.
            mountTries = 10;
            while(!FSInit() && mountTries--);
            if (mountTries)
            {
                if (screenState == SCREEN_CAPTURE_MEDIA)
                {
                    // Open the capture file
                    if ((captureFile = FSfopen( "CAPTURE.CSV", "w" )) == NULL)
                    {
                        // Shut down the USB.
                        USBHostShutdown();

                        usbErrorCode = USB_APP_CANNOT_OPEN_FILE;
                        screenState = SCREEN_DISPLAY_FLASH_ERROR;
                    }
                    else
                    {
#if 0
{
char                b[55];
extern unsigned int rawCTMU[NUM_TOUCHPADS];       //raw A/D value
extern unsigned int average[NUM_TOUCHPADS];       //averaged A/D value
extern unsigned int trip   [NUM_TOUCHPADS];       //trip point for TOUCHPAD
extern unsigned int hyst   [NUM_TOUCHPADS];       //hysterisis for TOUCHPAD
extern DWORD        sumCTMUPressed[NUM_TOUCHPADS];
extern DWORD        sumCTMUReleased[NUM_TOUCHPADS];

    sprintf( b, "raw:  %04X %04X %04X %04X %04X \r\n", rawCTMU[0], rawCTMU[1], rawCTMU[2], rawCTMU[3], rawCTMU[4]  );
    FSfwrite( b, 1, 33, captureFile);
    sprintf( b, "avg:  %04X %04X %04X %04X %04X \r\n", average[0], average[1], average[2], average[3], average[4]  );
    FSfwrite( b, 1, 33, captureFile);
    sprintf( b, "trip: %04X %04X %04X %04X %04X \r\n", trip[0], trip[1], trip[2], trip[3], trip[4]  );
    FSfwrite( b, 1, 33, captureFile);
    sprintf( b, "hyst: %04X %04X %04X %04X %04X \r\n", hyst[0], hyst[1], hyst[2], hyst[3], hyst[4]  );
    FSfwrite( b, 1, 33, captureFile);
    sprintf( b, "sumP: %08lX %08lX %08lX %08lX %08lX \r\n", sumCTMUPressed[0], sumCTMUPressed[1], sumCTMUPressed[2], sumCTMUPressed[3], sumCTMUPressed[4]  );
    FSfwrite( b, 1, 53, captureFile);
    sprintf( b, "sumR: %08lX %08lX %08lX %08lX %08lX \r\n", sumCTMUReleased[0], sumCTMUReleased[1], sumCTMUReleased[2], sumCTMUReleased[3], sumCTMUReleased[4]  );
    FSfwrite( b, 1, 53, captureFile);
}
#endif
                            // Start the capture
                        mediaPresent = TRUE;
                        screenState = SCREEN_DISPLAY_CAPTURE;
                    }
                }
            }
        }
        else
        {
            // Media has detached.
            mediaPresent = FALSE;

            if (screenState == SCREEN_CAPTURE)
            {
                // Close the file
                while (logData[logBufferWriting].bufferFull)
                {
                    WriteOneBuffer( captureFile, (BYTE *)logData[logBufferWriting].buffer, logData[logBufferWriting].index );
                }
                if (logData[logBufferWriting].index)
                {
                    WriteOneBuffer( captureFile, (BYTE *)logData[logBufferWriting].buffer, logData[logBufferWriting].index );
                }
            }

            // Exit the demo
            if (captureFile)
            {
                FSfclose( captureFile );
                captureFile = NULL;
            }

            // Shut down the USB.
            USBHostShutdown();

            screenState = SCREEN_DISPLAY_MAIN;
        }
    }

    if (mediaPresent)
    {
        // See if we need to write a buffer to the USB drive.
        if (logData[logBufferWriting].bufferFull)
        {
            WriteOneBuffer( captureFile, (BYTE *)logData[logBufferWriting].buffer, logData[logBufferWriting].index );
        }
    }
}


/****************************************************************************
  Function:
    void DrawGraphLine( void )

  Description:
    This data draws the line graph of the current graph data.  The routine
    uses the current line type and color, so it can be used to erase a
    line by first calling SetColor( BLACK ).

  Precondition:
    The line color must be set prior to calling this routine.  The graph
    variables graphData, graphDataHead, and graphDataTail, must also be
    valid.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void DrawGraphLine( void )
{
    static SHORT    previousY;
    WORD            tempHead;
    SHORT           x;
    SHORT           y;

    tempHead = graphDataHead;

    for (x = 0; (x < GRAPH_GRID_WIDTH) && (tempHead != graphDataTail); x++)
    {
        if (x == 0)
        {
            PutPixel( GRAPH_GRID_LEFT + x, graphData[tempHead] );
        }
        else
        {
            if (graphData[tempHead] > previousY)
            {
                for (y = previousY + GRAPH_GRID_TOP; y <= graphData[tempHead] + GRAPH_GRID_TOP; y++)
                    PutPixel( GRAPH_GRID_LEFT + x, y );
            }
            else
            {
                for (y = previousY + GRAPH_GRID_TOP; y >= graphData[tempHead] + GRAPH_GRID_TOP; y--)
                    PutPixel( GRAPH_GRID_LEFT + x, y );
            }
        }

        previousY = graphData[tempHead];
        tempHead ++;
        if (tempHead == GRAPH_GRID_WIDTH+1)
        {
            tempHead = 0;
        }
    }
}


/****************************************************************************
  Function:
    void DrawGridHorizontals( void )

  Description:
    This routine draws the horizontal lines on the scrolling graph.  The
    routine uses the current line color, so it can be used to erase the
    current horizontal lines by first calling SetColor( BLACK ).

  Precondition:
    The line color must be set prior to calling this routine.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void DrawGridHorizontals( void )
{
    SHORT   y;

    SetLineType( DOTTED_LINE );
    for (y = GRAPH_GRID_TOP + GRID_SPACING_VERTICAL; y < GRAPH_GRID_BOTTOM; y += GRID_SPACING_VERTICAL)
    {
        Line( GRAPH_GRID_LEFT, y, GRAPH_GRID_RIGHT, y );
    }
}


/****************************************************************************
  Function:
    void DrawGridVerticals( void )

  Description:
    This routine draws the vertical lines on the scrolling graph.  Since the
    graph is scrolling, the verticals scroll to match.  The current scroll
    offset is given by the variable graphOffset.  The routine uses the
    current line color, so it can be used to erase the current vertical lines
    by first calling SetColor( BLACK ).

  Precondition:
    The line color and scroll offset (graphOffset) must be set prior to
    calling this routine.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void DrawGridVerticals( void )
{
    SHORT   x;

    SetLineType( DOTTED_LINE );
    for (x = GRAPH_GRID_LEFT + GRID_SPACING_HORIZONTAL - graphOffset; x < GRAPH_GRID_RIGHT; x += GRID_SPACING_HORIZONTAL)
    {
        Line( x, GRAPH_GRID_TOP, x, GRAPH_GRID_BOTTOM );
    }
}


/****************************************************************************
  Function:
    void GraphReadPotentiometer( void )

  Description:
    This function reads the potentiometer and stores the current value into
    a global variable for later display on the scrolling graph.  If we are
    capturing the data to a flash drive, it also adds a record containing
    the current tick value and the potentiometer reading to the current log
    buffer.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    This routine is called from the tick interrupt.
  ***************************************************************************/

void GraphReadPotentiometer( void )
{
    AD1CON1             = 0x00E4;   // Off, Auto sample start, auto-convert
    AD1CON2             = 0;        // AVdd, AVss, int every conversion, MUXA only
    AD1CON3             = 0x1F05;   // 31 Tad auto-sample, Tad = 5*Tcy
    AD1CHS              = 0x0;      // MUXA uses AN0
    AD1CSSL             = 0;        // No scanned inputs
    AD1CON1bits.ADON    = 1;        // Turn on module

    while(!AD1CON1bits.DONE);       // Wait for conversion to complete

    potADC              = ADC1BUF0;
    AD1CON1bits.ADON    = 0;        // Turn off module

    if (screenState == SCREEN_CAPTURE)
    {
        sprintf( (char *)&(logData[logBufferReading].buffer[logData[logBufferReading].index]),
                    "%010ld,%05d\r\n", tick, potADC );
        logData[logBufferReading].index += 18;

        if ((logData[logBufferReading].index + 18) > MAX_LOG_BUFFER_SIZE)
        {
            logData[logBufferReading++].bufferFull = TRUE;
            if (logBufferReading >= NUM_LOG_BUFFERS)
            {
                logBufferReading = 0;
            }
        }
    }
}


/****************************************************************************
  Function:
    WORD ProcessMessageDemos( WORD translatedMsg, OBJ_HEADER* pObj,
                        GOL_MSG* pMsg )

  Description:
    This function processes the messages for the demo selection screen.  The
    global screenState is updated to proceed to the user's selection.

  Precondition:
    Call ShowScreenDemos() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None.
  ***************************************************************************/

WORD ProcessMessageDemos( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    buttonID;

    buttonID = GetObjID(pObj);

    switch (buttonID)
    {
        case ID_DEMOS_RGB:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                screenState = SCREEN_DISPLAY_RGB;
            }
            break;

        case ID_DEMOS_GRAPH:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                screenState = SCREEN_DISPLAY_GRAPH;
            }
            break;

        case ID_DEMOS_CAPTURE:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                screenState = SCREEN_DISPLAY_CAPTURE_MEDIA;
            }
            break;

        case ID_DEMOS_HOME:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;
        case ID_DEMOS_BAR:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                screenState = SCREEN_DISPLAY_BAR_DEMO;
            }
            break;

        default:
            Nop();
            break;
    }

    // Keep the current unimplemented button "pressed"
    SetState( (BUTTON*)pObj, BTN_PRESSED | BTN_DRAW );

    return 0;   // Do not call the default handler.
}


/****************************************************************************
  Function:
    WORD ProcessMessageGraph( WORD translatedMsg, OBJ_HEADER* pObj,
                    GOL_MSG* pMsg )

  Description:
    This function processes the messages for the potentiometer graphing.
    The right touchpad increases the graph update rate, the left touchpad
    decreases the graph update rate, and the up touchpad exits the demo.

  Precondition:
    Call ShowScreenGraph() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None.
  ***************************************************************************/

WORD ProcessMessageGraph( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_GRAPH_SLOWER:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                if (graphUpdateRate < GRAPH_UPDATE_INTERVAL_MAX)
                {
                    graphUpdateRate += GRAPH_UPDATE_INTERVAL_STEP;
                }
            }
            break;

        case ID_GRAPH_FASTER:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                if (graphUpdateRate > GRAPH_UPDATE_INTERVAL_MIN)
                {
                    graphUpdateRate -= GRAPH_UPDATE_INTERVAL_STEP;
                }
            }
            break;

        case ID_GRAPH_HOME:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                if (captureFile)
                {
                    FSfclose( captureFile );
                    captureFile = NULL;
                }
                SetLineType( SOLID_LINE );

                // Shut down the USB, in case we were capturing.
                USBHostShutdown();

                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;

        default:
            break;
    }

    return 0;
}


/****************************************************************************
  Function:
    WORD ProcessMessageRGB( WORD translatedMsg, OBJ_HEADER* pObj,
                    GOL_MSG* pMsg )

  Description:
    This function processes the messages for the RGB LED =manipulation
    screen.  It allows the user to control the Red, Green, and Blue
    components of the light emitted by the LED.  The up and down touchpads
    select the component, and the right and left touchpads increase and
    decrease the saturation value. The center touchpad exits the demo.

  Precondition:
    Call ShowScreenRGB() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None.
  ***************************************************************************/

WORD ProcessMessageRGB( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_RGB_RED_SLIDER:
            SldMsgDefault( translatedMsg, (SLIDER *)pObj, pMsg);
            AdjustColorPWM( ID_RGB_RED_SLIDER );
            break;

        case ID_RGB_GREEN_SLIDER:
            SldMsgDefault( translatedMsg, (SLIDER *)pObj, pMsg);
            AdjustColorPWM( ID_RGB_GREEN_SLIDER );
            break;

        case ID_RGB_BLUE_SLIDER:
            SldMsgDefault( translatedMsg, (SLIDER *)pObj, pMsg);
            AdjustColorPWM( ID_RGB_BLUE_SLIDER );
            break;

        case ID_RGB_BUTTON_NEXT:
            // Set the next slider as active.
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                if (GetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED), BTN_PRESSED ))
                {
                    ClrState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED),   BTN_PRESSED );
                    SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_PRESSED );
                }
                else if (GetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_PRESSED ))
                {
                    ClrState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_PRESSED );
                    SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_BLUE),  BTN_PRESSED );
                }
                else
                {
                    ClrState( (OBJ_HEADER *)GOLFindObject(ID_RGB_BLUE),  BTN_PRESSED );
                    SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED),   BTN_PRESSED );
                }

                SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED),   BTN_DRAW );
                SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_DRAW );
                SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_BLUE),  BTN_DRAW );
            }
            break;

        case ID_RGB_BUTTON_PREVIOUS:
            // Set the previous slider as active.
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                if (GetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED), BTN_PRESSED ))
                {
                    ClrState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED),   BTN_PRESSED );
                    SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_BLUE),  BTN_PRESSED );
                }
                else if (GetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_PRESSED ))
                {
                    ClrState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_PRESSED );
                    SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED),   BTN_PRESSED );
                }
                else
                {
                    ClrState( (OBJ_HEADER *)GOLFindObject(ID_RGB_BLUE),  BTN_PRESSED );
                    SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_PRESSED );
                }

                SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED),   BTN_DRAW );
                SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_DRAW );
                SetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_BLUE),  BTN_DRAW );
            }
            break;

        case ID_RGB_BUTTON_HOME:
            if (translatedMsg == BTN_MSG_PRESSED)
            {
                RGBTurnOffLED();
                screenState = SCREEN_DISPLAY_MAIN;
            }
            break;
    }
    return 0;   // We do not need to call the default handler.
}


/****************************************************************************
  Function:
    void RGBSetBlue( WORD saturation )

  Description:
    This function sets the blue RGB LED channel to the indicated saturation
    level.  If the saturation level is 0, then OxRS must be set to 0x100 to
    completely turn off that component of the color.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void RGBSetBlue( WORD saturation )
{
    if (saturation == 0)
    {
        OC3RS = 0x100;
    }
    else
    {
        OC3RS = CONVERT_TO_COLOR( saturation );
    }
}


/****************************************************************************
  Function:
    void RGBSetGreen( WORD saturation )

  Description:
    This function sets the green RGB LED channel to the indicated saturation
    level.  If the saturation level is 0, then OxRS must be set to 0x100 to
    completely turn off that component of the color.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void RGBSetGreen( WORD saturation )
{
    if (saturation == 0)
    {
        OC2RS = 0x100;
    }
    else
    {
        OC2RS = CONVERT_TO_COLOR( saturation );
    }
}


/****************************************************************************
  Function:
    void RGBSetRed( WORD saturation )

  Description:
    This function sets the red RGB LED channel to the indicated saturation
    level.  If the saturation level is 0, then OxRS must be set to 0x100 to
    completely turn off that component of the color.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void RGBSetRed( WORD saturation )
{
    if (saturation == 0)
    {
        OC1RS = 0x100;
    }
    else
    {
        OC1RS = CONVERT_TO_COLOR( saturation );
    }
}


/****************************************************************************
  Function:
    void RGBMapColorPins( void )

  Description:
    This function sets up the Peripheral Pin Select to configure the three
    color control PWM's to the correct pins as per the schematic.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void RGBMapColorPins( void )
{
    // Configure the LED Red Control, pins 31 (RP10) and 32 (RP17), for Output Compare 1 (function 18)
    RPOR5bits.RP10R  = 18;
    RPOR8bits.RP17R  = 18;

    // Configure the LED Green Control, pins 6 (RP19) and 8 (RP27), for Output Compare 2 (function 19)
    RPOR9bits.RP19R  = 19;
    RPOR13bits.RP27R = 19;

    // Configure the LED Blue Control, pins 4 (RP21) and 5 (RP26), for Output Compare 3 (function 20)
    RPOR10bits.RP21R = 20;
    RPOR13bits.RP26R = 20;

    RPOR4            = 0;   // AN8 and AN9 PPS, leave analog
}


/****************************************************************************
  Function:
    void RGBTurnOffLED( void )

  Description:
    This function turns off the LED by turning off the timers, turning off
    the PWM's, and setting the port pins to inputs.  It should be called at
    the completion of the RGB LED demo.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void RGBTurnOffLED( void )
{
    T2CON               = 0x0000;
    OC1CON1             = PWM_OFF;
    OC2CON1             = PWM_OFF;
    OC3CON1             = PWM_OFF;

    TRISFbits.TRISF4    = TRIS_INPUT;
    TRISFbits.TRISF5    = TRIS_INPUT;
    TRISGbits.TRISG8    = TRIS_INPUT;
    TRISGbits.TRISG9    = TRIS_INPUT;
    TRISGbits.TRISG6    = TRIS_INPUT;
    TRISGbits.TRISG7    = TRIS_INPUT;
}


/****************************************************************************
  Function:
    void RGBTurnOnLED( void )

  Description:
    This function turns on the LED by turning on the timers, turning on
    the PWM's, and setting the port pins to outputs.  It should be called at
    the beginning of the RGB LED demo.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void RGBTurnOnLED( WORD initialRed, WORD initialGreen, WORD initialBlue )
{
    // Initialize the timer for the PWM's.  We'll select a period value that
    // makes it simple to align the saturation values.
    T2CON                           = 0x0030;
    PR2                             = 0x00FF;

    // Initialize the PWM's
    RGBSetRed( initialRed );
    OC1R                            = 0;
    OC1CON2                         = PWM_CONFIGURATION_2;
    OC1CON1                         = PWM_CONFIGURATION_1;
    RGBSetGreen( initialGreen );
    OC2R                            = 0;
    OC2CON2                         = PWM_CONFIGURATION_2;
    OC2CON1                         = PWM_CONFIGURATION_1;
    RGBSetBlue( initialBlue );
    OC3R                            = 0;
    OC3CON2                         = PWM_CONFIGURATION_2;
    OC3CON1                         = PWM_CONFIGURATION_1;

    // Configure the PWM pins for output.
    TRISFbits.TRISF4                = TRIS_OUTPUT;   // Red
    TRISFbits.TRISF5                = TRIS_OUTPUT;
    TRISGbits.TRISG8                = TRIS_OUTPUT;   // Green
    TRISGbits.TRISG9                = TRIS_OUTPUT;
    TRISGbits.TRISG6                = TRIS_OUTPUT;   // Blue
    TRISGbits.TRISG7                = TRIS_OUTPUT;

    // Enable open drain.
    ODCF                            = 0x0030;       // Red
    ODCG                            = 0x03C0;       // Green and Blue

    // Turn on the timer.
    T2CON                           |= 0x8000;
}

/****************************************************************************
  Function:
    WORD ProcessMessageBarDemo( WORD translatedMsg, OBJ_HEADER* pObj,
                    GOL_MSG* pMsg )

  Description:
    This function processes the messages for the RGB LED =manipulation
    screen.  It allows the user to control the Red, Green, and Blue
    components of the light emitted by the LED.  The up and down touchpads
    select the component, and the right and left touchpads increase and
    decrease the saturation value. The center touchpad exits the demo.

  Precondition:
    Call ShowScreenRGB() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    None.
  ***************************************************************************/

WORD ProcessMessageBarDemo( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD    controlID;

    controlID = GetObjID(pObj);

    switch (controlID)
    {

    }

    return 0;   // We do not need to call the default handler.
}


/****************************************************************************
  Function:
    void ShowScreenCapture( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the capture demonstration.  It draws the initial grid, and initializes
    the variables needed for the demo.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenCapture( void )
{
    strcpy( pageInformation.title, "Pot. Capture" );
    InitializeScreen();

    // Create the hidden controls to receive touchpad messages.

    BtnCreate(          ID_GRAPH_FASTER,
                        GetMaxX()-8, 0, GetMaxX()-6, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_GRAPH_SLOWER,
                        GetMaxX()-6, 0, GetMaxX()-4, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_GRAPH_HOME,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    GOLPanelDraw( GRAPH_PANEL_LEFT, GRAPH_PANEL_TOP, GRAPH_PANEL_RIGHT, GRAPH_PANEL_BOTTOM, 0,
        BLACK, NULL, NULL, NULL, GOL_EMBOSS_SIZE);

    GOLPanelDrawTsk();          // draw panel for graph

    // Draw the initial grid.

    SetColor( WHITE );
    SetLineType( DOTTED_LINE );

    previousTick    = tick;
    graphOffset     = 0;
    graphDataHead   = 0;
    graphDataTail   = 0;
    graphUpdateRate = GRAPH_UPDATE_INTERVAL_INIT;

    DrawGridVerticals();
    DrawGridHorizontals();
}


/****************************************************************************
  Function:
    void ShowScreenCaptureMedia( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen that requests capture media insertion.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenCaptureMedia( void )
{
    strcpy( pageInformation.title, "Pot. Capture" );
    InitializeScreen();

    // Create the hidden controls to receive touchpad messages.

    BtnCreate(          ID_GRAPH_FASTER,
                        GetMaxX()-8, 0, GetMaxX()-6, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_GRAPH_SLOWER,
                        GetMaxX()-6, 0, GetMaxX()-4, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_GRAPH_HOME,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    StCreate(           ID_MEDIA_REQUEST,                       // ID
                        0,                              // left
                        20,                             // top
                        GetMaxX(),                      // right
                        20+GetTextHeight( (void *)&FONTDEFAULT ),  // bottom
                        ST_DRAW,
                        (char *)mediaRequest,
                        NULL );

    mediaPresent = FALSE;
    // Initialize USB Embedded Host mode.
    USBInitialize( 0 );
}


/****************************************************************************
  Function:
    void ShowScreenDemos( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen to select which demo to run.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenDemos( void )
{
    strcpy( pageInformation.title, "Demonstrations" );

    strcpy( pageInformation.buttonTextTopLeft,      "RGB LED" );
    strcpy( pageInformation.buttonTextBottomLeft,   "Capture" );
    strcpy( pageInformation.buttonTextTopRight,     "Home" );
    strcpy( pageInformation.buttonTextBottomRight,  "Graph" );
    strcpy( pageInformation.buttonTextThirdrow,     "Cap Touch" );

    pageInformation.buttonBitmapTopLeft        = NULL;
    pageInformation.buttonBitmapBottomLeft     = NULL;
    pageInformation.buttonBitmapTopRight       = NULL;
    pageInformation.buttonBitmapBottomRight    = NULL;
    pageInformation.buttonBitmapThirdrow       = NULL;

    pageInformation.instructions[0] = 0;

    ShowScreenGeneric();
}


/****************************************************************************
  Function:
    void ShowScreenGraph( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the graphing demonstration.  It draws the initial grid, and initializes
    the variables needed for the demo.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenGraph( void )
{
    strcpy( pageInformation.title, "Potentiometer Graph" );
    InitializeScreen();

    // Create the hidden controls to receive touchpad messages.

    BtnCreate(          ID_GRAPH_FASTER,
                        GetMaxX()-8, 0, GetMaxX()-6, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_GRAPH_SLOWER,
                        GetMaxX()-6, 0, GetMaxX()-4, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(          ID_GRAPH_HOME,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    GOLPanelDraw( GRAPH_PANEL_LEFT, GRAPH_PANEL_TOP, GRAPH_PANEL_RIGHT, GRAPH_PANEL_BOTTOM, 0,
        BLACK, NULL, NULL, NULL, GOL_EMBOSS_SIZE);

    GOLPanelDrawTsk();          // draw panel for graph

    // Draw the initial grid.

    SetColor( WHITE );
    SetLineType( DOTTED_LINE );

    previousTick    = tick;
    graphOffset     = 0;
    graphDataHead   = 0;
    graphDataTail   = 0;
    graphUpdateRate = GRAPH_UPDATE_INTERVAL_INIT;

    DrawGridVerticals();
    DrawGridHorizontals();
}


/****************************************************************************
  Function:
    void ShowScreenRGB( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen for the RGB LED demo.  This screen allows users to separately
    adjust the red, green and blue components of the LED, controlling the
    color of the emitted light.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    Buttons for the Home and Next functions are hidden.

    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenRGB( void )
{
    strcpy( pageInformation.title, "RGB LED" );
    InitializeScreen();

    // Create the visible screen controls

    BtnCreate(          ID_RGB_RED,
                        0, RGB_TOP_START+(SLIDER_HEIGHT*0), LABEL_WIDTH-2, RGB_TOP_START+(SLIDER_HEIGHT*1)-3, 3,
                        BTN_DRAW|BTN_PRESSED,
                        NULL,
                        "Red",
                        NULL );
    SldCreate(          ID_RGB_RED_SLIDER,
                        LABEL_WIDTH,RGB_TOP_START+(SLIDER_HEIGHT*0),GetMaxX()-2,RGB_TOP_START+(SLIDER_HEIGHT*1)-1,          // dimension
                        SLD_DRAW,
                        SLIDER_COLOR_MAX,           // range
                        SLIDER_PAGE_SIZE,           // page
                        INITIAL_RED,                // pos
                        NULL );

    BtnCreate(          ID_RGB_GREEN,
                        0, RGB_TOP_START+(SLIDER_HEIGHT*1), LABEL_WIDTH-2, RGB_TOP_START+(SLIDER_HEIGHT*2)-3, 3,
                        BTN_DRAW,
                        NULL,
                        "Green",
                        NULL );
    SldCreate(          ID_RGB_GREEN_SLIDER,
                        LABEL_WIDTH,RGB_TOP_START+(SLIDER_HEIGHT*1),GetMaxX()-2,RGB_TOP_START+(SLIDER_HEIGHT*2)-1,          // dimension
                        SLD_DRAW,
                        SLIDER_COLOR_MAX,           // range
                        SLIDER_PAGE_SIZE,           // page
                        INITIAL_GREEN,              // pos
                        NULL );

    BtnCreate(          ID_RGB_BLUE,
                        0, RGB_TOP_START+(SLIDER_HEIGHT*2), LABEL_WIDTH-2, RGB_TOP_START+(SLIDER_HEIGHT*3)-3, 3,
                        SLD_DRAW,
                        NULL,
                        "Blue",
                        NULL );
    SldCreate(          ID_RGB_BLUE_SLIDER,
                        LABEL_WIDTH,RGB_TOP_START+(SLIDER_HEIGHT*2),GetMaxX()-2,RGB_TOP_START+(SLIDER_HEIGHT*3)-1,          // dimension
                        SLD_DRAW,
                        SLIDER_COLOR_MAX,           // range
                        SLIDER_PAGE_SIZE,           // page
                        INITIAL_BLUE,               // pos
                        NULL );

    // Create the hidden controls to receive touchpad messages.

    BtnCreate(
                        ID_RGB_BUTTON_HOME,
                        GetMaxX()-6, 0, GetMaxX()-4, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(
                        ID_RGB_BUTTON_NEXT,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    BtnCreate(
                        ID_RGB_BUTTON_PREVIOUS,
                        GetMaxX()-2, 0, GetMaxX()-0, 2, 3,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );

    RGBTurnOnLED( INITIAL_RED, INITIAL_GREEN, INITIAL_BLUE );
}

/****************************************************************************
  Function:
    void ShowScreenBarDemo( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen for the capacitive touch demo. The ADC values read on the
    capacitive touchpads are displayed as solid bars and changed in realtime.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    Buttons for the Home and Next functions are hidden.

    The global list of graphics objects is destroyed and replaced. Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenBarDemo( void )
{
    extern unsigned int                trip   [NUM_TOUCHPADS];       //trip point for TOUCHPAD
    extern unsigned int                hyst   [NUM_TOUCHPADS];
    extern unsigned int                average[NUM_TOUCHPADS];
    BYTE aa,bb;

    strcpy( pageInformation.title, "Cap Touch" );
    InitializeScreen();

    SetColor(WHITE);

    aa=((average[0]-trip[0])>>9);
    bb=((average[0]-trip[0]+hyst[0])>>9);
    aa=GetMaxY()-(aa*3)/2;
    bb=GetMaxY()-(bb*3)/2;

    BarHSP(0,bb,GetMaxX(),GetMaxY(),0x0A,0xAA);
    BarHSP(0,aa,GetMaxX(),GetMaxY(),0xAA,0xFF);

    BarHS2(0,bb,GetMaxX(),bb);
    BarHS2(0,aa,GetMaxX(),aa);

}

void DrawBarDemo ( void )
{
BYTE bval,aa,bb,cc,dd,ee;
extern unsigned int                rawCTMU[NUM_TOUCHPADS];
extern unsigned int                trip   [NUM_TOUCHPADS];       //trip point for TOUCHPAD
extern unsigned int                hyst   [NUM_TOUCHPADS];
extern unsigned int                average[NUM_TOUCHPADS];
extern WORD                   buttons;

    if(tick!=previousTick)
    {

        for(aa=0;aa<NUM_TOUCHPADS;aa++)
            {

                bval=rawCTMU[aa]>>9;
                bval=(bval*3)/2;

                bb=0xFF;
                if(buttons&(1<<aa)) bb=0xAA;

                dd=(GetMaxX()-((NUM_TOUCHPADS-1)*10))/2+(aa)*10;
                cc=((average[aa]-trip[aa])>>9);
                ee=((average[aa]-trip[aa]+hyst[aa])>>9);
                cc=GetMaxY()-(cc*3)/2;
                ee=GetMaxY()-(ee*3)/2;

                SetColor(BLACK);
                BarHS2(dd-5,16,dd-3,ee+1);
                BarHS2(dd+3,16,dd+5,ee+1);

                BarHSP(dd-5,ee,dd-3,cc+1,0x0A,0xAA);
                BarHSP(dd+3,ee,dd+5,cc+1,0x0A,0xAA);

                BarHSP(dd-5,cc,dd-3,GetMaxY(),0xAA,0xFF);
                BarHSP(dd+3,cc,dd+5,GetMaxY(),0xAA,0xFF);

                SetColor(BLACK);
                BarHS2(dd-2,16,dd+2,GetMaxY()-bval);
                BarHSP(dd-2,GetMaxY()-bval+1,dd+2,GetMaxY(),bb,0x00);

                if((buttons&0x11)==0x11) //check for button 1&5 pressed at the same time
                {
                    screenState=SCREEN_DISPLAY_DEMOS;
                    return;
                }

            }


        previousTick=tick;
    }

}

/****************************************************************************
  Function:
    void TranslateMessageGraph( GOL_MSG *pMsg )

  Description:
    If the message is valid, this function translates the raw touchpad message
    pMsg->param1 to the proper message for the appropriate control.

  Precondition:
    ShowScreenGraph() must be the last screen display routine called before
    this function executes.

  Parameters:
    GOL_MSG* pMsg       - The original system message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageGraph( GOL_MSG *pMsg )
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_TOUCH_PAD))
    {
        if (pMsg->param2 == SCAN_RIGHT_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_FASTER;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_SLOWER;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_CR_PRESSED)
        {
            pMsg->param1 = ID_GRAPH_HOME;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
    }
}


/****************************************************************************
  Function:
    void TranslateMessageRGB( GOL_MSG *pMsg )

  Description:
    If the message is valid, this function translates the raw touchpad message
    pMsg->param1 to the proper message for the appropriate control.

  Precondition:
    ShowScreenGraph() must be the last screen display routine called before
    this function executes.

  Parameters:
    GOL_MSG* pMsg       - The original system message information

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageRGB( GOL_MSG *pMsg )
{
    WORD    currentSlideControl;

    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_TOUCH_PAD))
    {
        if (GetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_RED), BTN_PRESSED ))
        {
            currentSlideControl = ID_RGB_RED_SLIDER;
        }
        else if (GetState( (OBJ_HEADER *)GOLFindObject(ID_RGB_GREEN), BTN_PRESSED ))
        {
            currentSlideControl = ID_RGB_GREEN_SLIDER;
        }
        else
        {
            currentSlideControl = ID_RGB_BLUE_SLIDER;
        }

        if (pMsg->param2 == SCAN_RIGHT_PRESSED)
        {
            pMsg->param1 = currentSlideControl;
            pMsg->param2 = SCAN_UP_PRESSED;
        }
        else if (pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            pMsg->param1 = currentSlideControl;
            pMsg->param2 = SCAN_DOWN_PRESSED;
        }
        else if (pMsg->param2 == SCAN_UP_PRESSED)
        {
            pMsg->param1 = ID_RGB_BUTTON_PREVIOUS;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_DOWN_PRESSED)
        {
            pMsg->param1 = ID_RGB_BUTTON_NEXT;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else if (pMsg->param2 == SCAN_CR_PRESSED)
        {
            pMsg->param1 = ID_RGB_BUTTON_HOME;
            //pMsg->param2 = SCAN_CR_PRESSED;
        }
    }
}


/****************************************************************************
  Function:
    void UpdateGraph( void )

  Description:
    This routine updates the display of the scrolling graph.  The update rate
    is controlled by the variable graphUpdateRate, which the user can
    increase or decrease.  First, the old graph lines and vertical are
    erased.  The graph data is then updated with the new potentiometer
    reading.  Then the graph line and the graph horizontal and vertical lines
    are redrawn.  The horizontal lines are not affected by the scrolling,
    but if we do not redraw them, there will gaps where the previous graph
    line crossed the horizontal lines.

  Precondition:
    The graph information (graphData, graphDataHead, and graphDataTail) must
    be valid.

  Parameters:
    None

  Returns:
    None

  Remarks:
    Since normally a graph's 0 point is on the bottom, but our (0,0) is the
    upper left corner, we need to flip the graph.  This way, it matches the
    graph produced by Excel.
  ***************************************************************************/

void UpdateGraph( void )
{
    if ((tick - previousTick ) > graphUpdateRate)
    {
        previousTick = tick;

        // Remove the old graph and vertical grid lines
        SetColor( BLACK );
        DrawGraphLine();
        DrawGridVerticals();

        // Get the latest potentiometer reading.
        graphData[graphDataTail++] = GRAPH_GRID_HEIGHT - (potADC * (GRAPH_GRID_HEIGHT) / 0x400);
        if (graphDataTail == GRAPH_DATA_BUFFER_SIZE)
        {
            graphDataTail = 0;
        }

        // When the graph is full, update the head pointer to wrap and the display offset
        if (graphDataHead == graphDataTail)
        {
            graphDataHead ++;
            if (graphDataHead == GRAPH_DATA_BUFFER_SIZE)
            {
                graphDataHead = 0;
            }

            graphOffset ++;
            if (graphOffset >= GRID_SPACING_HORIZONTAL)
            {
                graphOffset = 0;
            }
        }

        // Draw the new graph
        SetColor( WHITE );
        DrawGraphLine();

        // Draw the updated grid
        SetColor( WHITE );
        DrawGridVerticals();
        DrawGridHorizontals();
    }
}


/****************************************************************************
  Function:
    void WriteOneBuffer( FSFILE *fptr, BYTE *data, WORD size )

  Description:
    This function writes one log buffer to the indicated file.  It then
    advances the pointer to the current buffer to write.

  Precondition:
    None

  Parameters:
    FSFILE *fptr    - File pointer of the open data file
    BYTE *data      - Data to write to the file
    WORD size       - Amount of data to write

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void WriteOneBuffer( FSFILE *fptr, BYTE *data, WORD size )
{
    if (FSfwrite( data, 1, size, fptr) != size)
    {
        // Error.  Someone probably pulled the flash drive during a write.
        usbErrorCode = USBHostDeviceStatus(1);

        // Shut down the USB.
        USBHostShutdown();

        screenState = SCREEN_DISPLAY_FLASH_ERROR;
    }
    logData[logBufferWriting].bufferFull    = FALSE;
    logData[logBufferWriting].index         = 0;

    logBufferWriting++;
    if (logBufferWriting >= NUM_LOG_BUFFERS)
    {
        logBufferWriting = 0;
    }
}


