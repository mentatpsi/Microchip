/*****************************************************************************
 *
 * Simple 4 wire touch screen driver
 *
 *****************************************************************************
 * FileName:        TouchScreen.c
 * Dependencies:    Beep.h, TouchScreen.h
 * Processor:       PIC24, PIC32, dsPIC, PIC24H
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 04/21/11	    ...
 * 10/03/11	    TouchDetectPosition() now returns SHORT for compatibility.
 *****************************************************************************/
#include "GenericTypeDefs.h"
#include "TouchScreenAR1020.h"
#include "SST25VF016.h"
#include "drv_spi.h"
#include "HardwareProfile.h"
#include "TouchScreen.h"
#include "Graphics/Graphics.h"
#include "TimeDelay.h"

#ifdef USE_TOUCHSCREEN_AR1020

//////////////////////// LOCAL PROTOTYPES ////////////////////////////
void    TouchGetCalPoints(WORD,WORD,WORD);

#define WAIT_UNTIL_FINISH(x)    while(!x)

extern BYTE CalibrateFlag;
//XCHAR               Data[8];
BYTE                CapButtonState = 0;
BYTE                ARData[5];

// Default Calibration Inset Value (percentage of vertical or horizontal resolution)
// Calibration Inset = ( CALIBRATIONINSET / 2 ) % , Range of 0–20% with 0.5% resolution
// Example with CALIBRATIONINSET == 20, the calibration points are measured
// 10% from the corners.
#define CALIBRATIONINSET   25       // range 0 <= CALIBRATIONINSET <= 40 

#define CAL_X_INSET    (((GetMaxX()+1)*(CALIBRATIONINSET>>1))/100)
#define CAL_Y_INSET    (((GetMaxY()+1)*(CALIBRATIONINSET>>1))/100)

#define TOUCH_DIAMETER	10
#define SAMPLE_POINTS   4

#define CALIBRATION_DELAY   300				// delay between calibration touch points


extern NVM_READ_FUNC           pCalDataRead;                  // function pointer to data read
extern NVM_WRITE_FUNC          pCalDataWrite;                // function pointer to data write
extern NVM_SECTORERASE_FUNC    pCalDataSectorErase;    // function pointer to data sector erase

volatile long xcor= -1;
volatile long ycor= -1;  

#define TOUCH_AR1020_REG_TOUCH_THRESHOLD        0x02
#define TOUCH_AR1020_REG_SENSITIVITY_FILTER     0x03
#define TOUCH_AR1020_REG_SAMPLING_FAST          0x04
#define TOUCH_AR1020_REG_SAMPLING_SLOW          0x05
#define TOUCH_AR1020_REG_ACCURACY_FILTER_FAST   0x06
#define TOUCH_AR1020_REG_ACCURACY_FILTER_SLOW   0x07
#define TOUCH_AR1020_REG_SPEED_THRESHOLD        0x08
#define TOUCH_AR1020_REG_SLEEP_DELAY            0x0A
#define TOUCH_AR1020_REG_PEN_UP_DELAY           0x0B
#define TOUCH_AR1020_REG_TOUCH_MODE             0x0C
#define TOUCH_AR1020_REG_TOUCH_OPTIONS          0x0D
#define TOUCH_AR1020_REG_COLIBRATION_INSET      0x0E
#define TOUCH_AR1020_REG_PEN_STATE_REPORT_DELAY 0x0F
#define TOUCH_AR1020_REG_TOUCH_REPORT_DELAY     0x11

typedef BYTE    TOUCH_AR1020_REG;

typedef struct
{
    BYTE                startRegOffset;
    BYTE                size;
    TOUCH_AR1020_REG    values[8];
}TOUCH_AR1020_REG_WRITE_DATA;


const TOUCH_AR1020_REG_WRITE_DATA regInit[] =
{
    {   TOUCH_AR1020_REG_TOUCH_THRESHOLD,       0x07,   { 0xC5, 0xC0, 0x04, 0x10, 0x02, 0x08, 0x04 }    },
    {   TOUCH_AR1020_REG_SLEEP_DELAY,           0x06,   { 0x64, 0x03, 0x51, 0x00, 0x19, 0xC8 }          },
    {   TOUCH_AR1020_REG_TOUCH_REPORT_DELAY,    0x01,   { 0x00 }                                        }
};

#define TOUCH_AR1020_REG_INIT_SIZE          (sizeof(regInit) / sizeof(TOUCH_AR1020_REG_WRITE_DATA))

#define TOUCH_AR1020_CMD_HEADER             0x55

#define TOUCH_AR1020_CMD_ENABLE_TOUCH       0x12
#define TOUCH_AR1020_CMD_DISABLE_TOUCH      0x13
#define TOUCH_AR1020_CMD_CALIBRATE_MODE     0x14
#define TOUCH_AR1020_CMD_REG_READ           0x20
#define TOUCH_AR1020_CMD_REG_WRITE          0x21
#define TOUCH_AR1020_CMD_REG_START_ADDR     0x22
#define TOUCH_AR1020_CMD_REG_WRITE_EEPROM   0x23
#define TOUCH_AR1020_CMD_EEPROM_READ        0x28
#define TOUCH_AR1020_CMD_EEPROM_WRITE       0x29
#define TOUCH_AR1020_CMD_EEPROM_WRITE_REG   0x2B

#define TOUCHAR1020_RESP_SUCCESS            0x00
#define TOUCHAR1020_RESP_BAD_CMD            0x01
#define TOUCHAR1020_RESP_BAD_HDR            0x03
#define TOUCHAR1020_RESP_CMD_TO             0x04
#define TOUCHAR1020_RESP_CAL_CANCEL         0xFC
#define TOUCHAR1020_RESP_INVALID            0xFF

typedef union
{
    struct
    {
        BYTE pen:       1;
        BYTE reserved:  6;
        BYTE startBit:  1;
        BYTE lowX;
        BYTE highX;
        BYTE lowY;
        BYTE highY;
    };
    BYTE packet[5];
}AR1020_TOUCH_REPORT_PACKET;


BYTE TouchAR1020SendCommandAndGetResponse(BYTE command, BYTE *data, BYTE dataSize);
void TouchAR1020RegisterWrite(WORD regOffset, TOUCH_AR1020_REG *regData, BYTE numReg);
void TouchAR1020SendCommand(BYTE command, BYTE *data, BYTE dataSize);
BYTE TouchAR1020GetResponceData(BYTE command, BYTE cmdDataSize, BYTE *data);
BYTE TouchAR1020GetResponceStatus(BYTE command, BYTE dataSize);
static void SendByte(BYTE data);
static BYTE GetByte(void);

//////////////////////// Resistive Touch Driver Version ////////////////////////////
// The first three digit is version number and 0xF is assigned to this
// 4-wire resistive driver.
const WORD mchpTouchScreenVersion = 0xE100;


static BYTE detectPosition;
static DRV_SPI_INIT_DATA spiInitData;
/*********************************************************************
* Function: AR1020 ISR
* PreCondition: none
* Input: none
* Output: Returns 1 if touch sampling is done.
*         Returns 0 if the touch sampling is not successful.  
* Side Effects: none
* Overview: AR1020 ISR
* Note: none
********************************************************************/
SHORT TouchDetectPosition(void)            //Routine for touch messages including cap and resistive
{
    static long xc,yc;
    AR1020_TOUCH_REPORT_PACKET touchReport;
    BYTE i;

    if(!SPILock(spiInitData.channel))
        return 0;

    if((TouchScreenPacketReady()) && (detectPosition))
    {
        DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);
        
        TouchScreenChipEnable();

        for(i = 0; i < sizeof(AR1020_TOUCH_REPORT_PACKET); i++)
        {
            touchReport.packet[i] = GetByte();
        }

        xcor = -1;
        ycor = -1;

        if(touchReport.startBit)
        {
            if(touchReport.pen)
            {
                xc = (long)touchReport.highX;
                xc <<= 7;
                xc |= (long)touchReport.lowX;
                xc *= GetMaxX();
                xc >>= 11;
                xc++;
                xc >>= 1;

                yc = (long)touchReport.highY;
                yc <<= 7;
                yc |= (long)touchReport.lowY;
                yc *= GetMaxY();
                yc >>= 11;
                yc++;
                yc >>= 1;

                xcor = xc;
                ycor = yc;
            }
        }

        TouchScreenChipDisable();
    }

    SPIUnLock(spiInitData.channel);
    return 1;    
}

/*********************************************************************
 * TouchAR1020GetTouchReport
 *********************************************************************/
/*********************************************************************
* Function: void TouchInit(void)
* PreCondition: none
* Input: none
* Output: none
* Side Effects: none
* Overview: sets ADC 
* Note: none
********************************************************************/
void TouchHardwareInit(void *initValues)
{
    TouchScreenConfigPacketReady();

    DRV_SPI_Initialize(((DRV_SPI_INIT_DATA *)initValues)->channel, (DRV_SPI_INIT_DATA *)initValues);
    
    memcpy(&spiInitData, initValues, sizeof(DRV_SPI_INIT_DATA));

    // Set IOs directions for AR1020 SPI

    TouchScreenChipDisable();
    TouchScreenChipDirection();

    
    while(TouchAR1020SendCommandAndGetResponse(TOUCH_AR1020_CMD_ENABLE_TOUCH, NULL, 0) != TOUCHAR1020_RESP_SUCCESS);          //Enable Touch Messages
    detectPosition = 1;

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
    return (SHORT)xcor;
}
/*********************************************************************
* Function: SHORT TouchGetRawX()
*
* PreCondition: none
*
* Input: none
*
* Output: x raw value
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
    return (SHORT)xcor;
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
    return (SHORT)ycor;
}

/*********************************************************************
* Function: SHORT TouchGetRawY()
*
* PreCondition: none
*
* Input: none
*
* Output: raw y value
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
    return (SHORT)ycor;
}
/*********************************************************************
* Function: void TouchStoreCalibration(void)
*
* PreCondition: EEPROMInit() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: stores calibration parameters into EEPROM
*
* Note: none
*
********************************************************************/
void TouchStoreCalibration(void)
{
 	if (pCalDataWrite != NULL)
	{
		// the coefficient A address is used since it is the first one
		// and this assumes that all stored values are located in one 
		// sector
		if (pCalDataSectorErase != NULL)
			pCalDataSectorErase(ADDRESS_RESISTIVE_TOUCH_VERSION);


    	pCalDataWrite(mchpTouchScreenVersion, ADDRESS_RESISTIVE_TOUCH_VERSION);
    	
 	}
}

/*********************************************************************
* Function: void TouchLoadCalibration(void)
*
* PreCondition: EEPROMInit() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: loads calibration parameters from EEPROM
*
* Note: none
*
********************************************************************/
void TouchLoadCalibration(void)
{
    // Calibration data is loaded by the controller
}
/*********************************************************************
* Function: void TouchGetCalPoints(void)
* PreCondition: InitGraph() must be called before
* Input: none
* Output: none
* Side Effects: none
* Overview: gets values for 3 touches
* Note: none
********************************************************************/
void TouchCalHWGetPoints(void)
{
    BYTE i;
    BYTE type;

    detectPosition = 0;

    while(TouchAR1020SendCommandAndGetResponse(TOUCH_AR1020_CMD_DISABLE_TOUCH, NULL, 0) != TOUCHAR1020_RESP_SUCCESS);          //Disable Touch Messages

    DelayMs(100);                                                 //asked for in spec
    
    for(i = 0; i < TOUCH_AR1020_REG_INIT_SIZE; i++)
        TouchAR1020RegisterWrite(regInit[i].startRegOffset, (TOUCH_AR1020_REG *)regInit[i].values, regInit[i].size);

    
    type = 0x04;

    while(TouchAR1020SendCommandAndGetResponse(TOUCH_AR1020_CMD_CALIBRATE_MODE, (BYTE *)&type, 1) != TOUCHAR1020_RESP_SUCCESS);          //Enable Touch Messages


    XCHAR  	calStr1[] = {'P','r','e','s','s',' ','&',' ','R','e','l','e','a','s','e',0};
    XCHAR   calStr2[] = {'o','n',' ','t','h','e',' ','f','i','l','l','e','d',0};
    XCHAR   calStr3[] = {'c','i','r','c','l','e',0};
    SHORT   counter;
    WORD    dx[SAMPLE_POINTS], dy[SAMPLE_POINTS];
    WORD    textHeight, msgY;



	SetFont((void *) &FONTDEFAULT);
    SetColor(BRIGHTRED);

    textHeight = GetTextHeight((void *) &FONTDEFAULT);

    while
    (
        !OutTextXY
            (
                (GetMaxX() - GetTextWidth((XCHAR *)calStr1, (void *) &FONTDEFAULT)) >> 1,
                ((GetMaxY() >> 1) - textHeight),
                (XCHAR *)calStr1
            )
    );

    while
    (
        !OutTextXY
            (
                (GetMaxX() - GetTextWidth((XCHAR *)calStr2, (void *) &FONTDEFAULT)) >> 1,
                (GetMaxY() >> 1),
                (XCHAR *)calStr2
            )
    );

    while
    (
        !OutTextXY
            (
                (GetMaxX() - GetTextWidth((XCHAR *)calStr3, (void *) &FONTDEFAULT)) >> 1,
                ((GetMaxY() >> 1) + textHeight),
                (XCHAR *)calStr3
            )
    );


    // calculate center points (locate them at 15% away from the corners)
	// draw the four touch points

    dy[0] = dy[1] = CAL_Y_INSET; 
	dy[2] = dy[3] = (GetMaxY() - CAL_Y_INSET);
	dx[0] = dx[3] = CAL_X_INSET; 
	dx[1] = dx[2] = (GetMaxX() - CAL_X_INSET);


    msgY = ((GetMaxY() >> 1) - textHeight);
	
    for(counter = 0; counter < SAMPLE_POINTS; counter++)
    {
        TouchGetCalPoints(dx[counter], dy[counter], TOUCH_DIAMETER);                            //Get Cal points from upper left
    }

    DelayMs(100);                                                 //asked for in spec


    while(TouchAR1020SendCommandAndGetResponse(TOUCH_AR1020_CMD_ENABLE_TOUCH, NULL, 0) != TOUCHAR1020_RESP_SUCCESS);          //Enable Touch Messages
    
    detectPosition = 1;

}

/*********************************************************************
* Function: void TouchGetCalPoints(void)
* PreCondition: InitGraph() must be called before
* Input: none
* Output: none
* Side Effects: none
* Overview: gets values for 3 touches
* Note: none
********************************************************************/
void TouchGetCalPoints(WORD xpos, WORD ypos, WORD radius)
{
    // draw the new filled circle (new calibration point)
    SetColor(BRIGHTRED);
    while(!(Circle(xpos, ypos, radius)));
    while(!(FillCircle(xpos, ypos, radius-3)));

    // show points left message

    TouchScreenChipEnable();            //Issue Calibrate Mode Command
    
    while(TouchAR1020GetResponceStatus(TOUCH_AR1020_CMD_CALIBRATE_MODE, 0) != TOUCHAR1020_RESP_SUCCESS)
        ;

    TouchScreenChipDisable();

    // start the touch delay 
    DelayMs(CALIBRATION_DELAY<<1);

    SetColor(WHITE);
    while(!(Circle(xpos, ypos, radius)));
    while(!(FillCircle(xpos, ypos, radius-3)));
}

static void SendByte(BYTE data)
{
    SPIPut(spiInitData.channel, data);
    Delay10us(6);       //Asked for in AR1020 Spec
    SPIGet(spiInitData.channel);
}

static BYTE GetByte(void)
{
    SPIPut(spiInitData.channel, 0);
    Delay10us(6);       //Asked for in AR1020 Spec
    return SPIGet(spiInitData.channel);
}
WORD TouchAR1020GetRegisterStartAddress(void)
{
    BYTE responce;
    BYTE startAddress;
    BYTE size = 0;

    while(!SPILock(spiInitData.channel))
        ;

    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    TouchScreenChipEnable();

    TouchAR1020SendCommand(TOUCH_AR1020_CMD_REG_START_ADDR, NULL, 0);

    responce = TouchAR1020GetResponceData(TOUCH_AR1020_CMD_REG_START_ADDR, size, &startAddress);

    TouchScreenChipDisable();

    SPIUnLock(spiInitData.channel);

    if(responce != TOUCHAR1020_RESP_SUCCESS)
        return 0xFFFF;

    return (WORD)startAddress;
        
}

void TouchAR1020SendCommand(BYTE command, BYTE *data, BYTE dataSize)
{
    SendByte(TOUCH_AR1020_CMD_HEADER);  
    SendByte(1 + dataSize);  
    SendByte(command); 

    while(dataSize)
    {
        SendByte(*data);
        *data++;
        dataSize--;
    }
}

BYTE TouchAR1020GetResponceData(BYTE command, BYTE cmdDataSize, BYTE *data)
{
    BYTE responce;

    while(!TouchScreenPacketReady())
        ;

    if(GetByte() != TOUCH_AR1020_CMD_HEADER)
        return TOUCHAR1020_RESP_INVALID;

    cmdDataSize = GetByte();

    if(!cmdDataSize)
        return TOUCHAR1020_RESP_INVALID;

    responce = GetByte();

    if(GetByte() != command)
        return TOUCHAR1020_RESP_INVALID;

    cmdDataSize -= 2;

    while(cmdDataSize)
    {
        if(data != NULL)
        {
            *data = GetByte();
            data++;
        }else
        {
            GetByte();
        }

        cmdDataSize--;
    }

    return responce;
}
BYTE TouchAR1020GetResponceStatus(BYTE command, BYTE dataSize)
{
    return TouchAR1020GetResponceData(command, dataSize, NULL);
}

BYTE TouchAR1020SendCommandAndGetResponse(BYTE command, BYTE *data, BYTE dataSize)
{
    BYTE responce;

    while(!SPILock(spiInitData.channel))
        ;

    DRV_SPI_Initialize(spiInitData.channel, (DRV_SPI_INIT_DATA *)&spiInitData);

    TouchScreenChipEnable();

    TouchAR1020SendCommand(command, data, dataSize);

    responce = TouchAR1020GetResponceStatus(command, dataSize);

    TouchScreenChipDisable();

    SPIUnLock(spiInitData.channel);

    return responce;

}

void TouchAR1020RegisterWrite(WORD regOffset, TOUCH_AR1020_REG *regData, BYTE numReg)
{
    BYTE regPacket[12];
    WORD startAddress;

    startAddress = TouchAR1020GetRegisterStartAddress();

    while(numReg)
    {
        BYTE i;

         BYTE regDataSize = 8;

        if(regDataSize > numReg)
            regDataSize = numReg;

        startAddress += regOffset;

        regPacket[0] = (BYTE)(startAddress >> 8);
        regPacket[1] = (BYTE)(startAddress);
        regPacket[2] = regDataSize;
        
        for(i = 0; i < regDataSize; i++)
        {
            regPacket[i + 3] = *regData;
            *regData++;
        }

        if(TouchAR1020SendCommandAndGetResponse(TOUCH_AR1020_CMD_REG_WRITE, regPacket, regDataSize + 3) == TOUCHAR1020_RESP_SUCCESS)
        {
            numReg -= regDataSize;
            regOffset += regDataSize;
        }
    }
}
#endif
