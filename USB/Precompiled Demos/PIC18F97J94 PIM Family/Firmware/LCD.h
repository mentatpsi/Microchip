/********************************************************************
 FileName:      LCD.h
 Dependencies:  See INCLUDES section
 Processor:     PIC18F97J94 and PIC18F87J94
 Hardware:      This demo is natively intended to be used on Microchip USB demo
                boards supported by the MCHPFSUSB stack.  See release notes for
                support matrix.  This demo can be modified for use on other 
                hardware platforms.
 Complier:      Microchip C18 (for PIC18)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC® Microcontroller is intended and
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
  1.0   Initial release
  1.1   Adding PIC18F87J94 part support
  1.2   Updated for PIC18F97J94 PIM rev2
********************************************************************/

#ifndef LCD_H
#define LCD_H

#include "GenericTypeDefs.h"
#include "Compiler.h"

/** DEFINITIONS ****************************************************/

// Maximum number of banners to scroll through
#define LCDMAXBANR  3
// Maximum number of characters in a banner string
#define BANRSTRMAX	50
// Number of leading blank characters so that a string scrolls from off
// screen on teh right to the left.
#define SCRLOFFSET	6

// This union describes one column element for the scrolling banner
typedef union
{
	struct
	{
		BYTE R1 :1;//ROW
		BYTE R2 :1;
		BYTE R3 :1;
		BYTE R4 :1;
		BYTE R5 :1;
		BYTE R6 :1;
		BYTE R7 :1;
		BYTE :1;
	}Row;
	BYTE Col;
}BANRCOL;

// Struct for a single scrolling banner type 
typedef struct
{
	char String[BANRSTRMAX];
	BYTE CharStrtPt;
	BYTE ColStrtPt;
	BOOL ScrlCmplt;
}BANRSTR;	

/** Function Prototypes **********************************************/

void LCDInit(void);
void LCDSetAll(void);
void LCDClearAll(void);
void LCDMicrochip(void);
void LCDSetSpecialPixels(void);
void LCDClearSpecialPixels(void);
void LCDToggleSpecialPixels(void);
void Digit6(BYTE);
void Digit5(BYTE);
void Col2(BYTE);
void Digit4(BYTE);
void Digit3(BYTE);
void Col1(BYTE);
void Digit2(BYTE);
void Digit1(BYTE);
void AMPM(BYTE);
void UpdateBanner(BANRSTR* Banner);
void DisplayDate( DTVALUE* pCurrentDTV );
void DisplayTime( DTVALUE* pCurrentDTV );
void DisplaySplash( void );
BANRCOL GetBanrCol(unsigned char BanrChar, BYTE SubCol);


#define __97J94_PIM2
#if defined(__97J94_PIM2)
/** LCD Glass DEFINITIONS ********************************************/

//These are all of the redefinitions of the LCD pins for simplified use
                            // SE25             was SE26
                            // SE18             was VLCAP1
                            // SE26             was VLCAP2
                            // SE24             was SE15
                            // SE23             was SE20
                            // SE20             was SE19
                            // SE21             was SE27
                            // SE42             was SE18
                            // SE53             was SE49
                            // SE54             was SE50
                            // SE12             was SE11
                            // SE57             was SE51
                            // SE58             was SE52
                            // SE59             was SE60
                            // VLCAP2           was SE58
                            // VLCAP1           was SE59
#define S1 			LCDSE4bits.SE38         //  was LCDSE4bits.SE36
#define S2 			LCDSE4bits.SE37         //  was LCDSE7bits.SE57
#define S3 			LCDSE3bits.SE27         //  was LCDSE7bits.SE61
#define S4 			LCDSE2bits.SE22         //  was LCDSE4bits.SE35
#define S5 			LCDSE7bits.SE60         //  was LCDSE0bits.SE06

#define S6 			LCDSE7bits.SE61         //  was LCDSE7bits.SE62
#define S7 			LCDSE4bits.SE35         //  was LCDSE0bits.SE05
#define S8 			LCDSE4bits.SE39         //  was LCDSE4bits.SE37
#define S9			LCDSE4bits.SE34         //  was LCDSE0bits.SE04
#define S10			LCDSE4bits.SE33         //  was LCDSE0bits.SE03
#define S11			LCDSE4bits.SE32         //  was LCDSE0bits.SE02
#define S12			LCDSE0bits.SE04         //  was LCDSE0bits.SE00
#define S13			LCDSE0bits.SE03         //  was LCDSE6bits.SE48
#define S14			LCDSE0bits.SE02         //  was LCDSE5bits.SE47
#define S15			LCDSE7bits.SE63         //  was LCDSE5bits.SE46

#define S16			LCDSE6bits.SE49         //  was LCDSE6bits.SE53
#define S17			LCDSE6bits.SE51         //  was LCDSE6bits.SE54
#define S18			LCDSE5bits.SE41         //  was LCDSE7bits.SE56
#define S19			LCDSE5bits.SE40         //  was LCDSE2bits.SE22
#define S20			LCDSE1bits.SE08         //  was LCDSE4bits.SE32
#define S21			LCDSE1bits.SE11         //  was LCDSE0bits.SE07
#define S22			LCDSE0bits.SE07         //  was LCDSE7bits.SE63
#define S23			LCDSE7bits.SE62         //  was LCDSE0bits.SE01

#define S24			LCDSE6bits.SE52         //  was LCDSE6bits.SE55
#define S25			LCDSE5bits.SE44         //  was LCDSE5bits.SE40
#define S26			LCDSE6bits.SE50         //  was LCDSE2bits.SE21
#define S27			LCDSE2bits.SE19         //  was LCDSE4bits.SE39
#define S28			LCDSE6bits.SE55         //  was LCDSE4bits.SE38
#define S29			LCDSE0bits.SE06         //  was LCDSE4bits.SE34
#define S30			LCDSE0bits.SE05         //  was LCDSE4bits.SE33
#define S31			LCDSE0bits.SE01         //  was LCDSE3bits.SE25
#define S32			LCDSE0bits.SE00         //  was LCDSE3bits.SE24
#define S33			LCDSE6bits.SE48         //  was LCDSE2bits.SE23
#define S34			LCDSE5bits.SE47         //  was LCDSE5bits.SE43
#define S35			LCDSE5bits.SE46         //  was LCDSE5bits.SE42
#define S36			LCDSE5bits.SE45         //  was LCDSE5bits.SE41

#define X1C1 		LCDDATA4bits.S38C0      //  was LCDDATA4bits.S36C0
#define X2C1 		LCDDATA12bits.S38C1     //  was LCDDATA12bits.S36C1
#define X3C1 		LCDDATA20bits.S38C2     //  was LCDDATA20bits.S36C2
#define X4C1 		LCDDATA28bits.S38C3     //  was LCDDATA28bits.S36C3
#define X5C1 		LCDDATA36bits.S38C4     //  was LCDDATA36bits.S36C4
#define X6C1 		LCDDATA44bits.S38C5     //  was LCDDATA44bits.S36C5
#define X7C1 		LCDDATA52bits.S38C6     //  was LCDDATA52bits.S36C6
#define Dollar		LCDDATA60bits.S38C7     //  was LCDDATA60bits.S36C7

#define X1C2	    LCDDATA4bits.S37C0      //  was LCDDATA7bits.S57C0
#define X2C2	    LCDDATA12bits.S37C1     //  was LCDDATA15bits.S57C1
#define X3C2	    LCDDATA20bits.S37C2     //  was LCDDATA23bits.S57C2
#define X4C2	    LCDDATA28bits.S37C3     //  was LCDDATA31bits.S57C3
#define X5C2	    LCDDATA36bits.S37C4     //  was LCDDATA39bits.S57C4
#define X6C2	    LCDDATA44bits.S37C5     //  was LCDDATA47bits.S57C5
#define X7C2	    LCDDATA52bits.S37C6     //  was LCDDATA55bits.S57C6
#define WHEEL1	    LCDDATA60bits.S37C7     //  was LCDDATA63bits.S57C7

#define X1C3	    LCDDATA3bits.S27C0      //  was LCDDATA7bits.S61C0
#define X2C3	    LCDDATA11bits.S27C1     //  was LCDDATA15bits.S61C1
#define X3C3	    LCDDATA19bits.S27C2     //  was LCDDATA23bits.S61C2
#define X4C3	    LCDDATA27bits.S27C3     //  was LCDDATA31bits.S61C3
#define X5C3	    LCDDATA35bits.S27C4     //  was LCDDATA39bits.S61C4
#define X6C3	    LCDDATA43bits.S27C5     //  was LCDDATA47bits.S61C5
#define X7C3	    LCDDATA51bits.S27C6     //  was LCDDATA55bits.S61C6
#define WHEEL2	    LCDDATA59bits.S27C7     //  was LCDDATA63bits.S61C7

#define X1C4	    LCDDATA2bits.S22C0      //  was LCDDATA4bits.S35C0
#define X2C4	    LCDDATA10bits.S22C1     //  was LCDDATA12bits.S35C1
#define X3C4	    LCDDATA18bits.S22C2     //  was LCDDATA20bits.S35C2
#define X4C4	    LCDDATA26bits.S22C3     //  was LCDDATA28bits.S35C3
#define X5C4	    LCDDATA34bits.S22C4     //  was LCDDATA36bits.S35C4
#define X6C4	    LCDDATA42bits.S22C5     //  was LCDDATA44bits.S35C5
#define X7C4	    LCDDATA50bits.S22C6     //  was LCDDATA52bits.S35C6
#define WHEEL3	    LCDDATA58bits.S22C7     //  was LCDDATA60bits.S35C7

#define X1C5	    LCDDATA7bits.S60C0      //  was LCDDATA0bits.S06C0
#define X2C5	    LCDDATA15bits.S60C1     //  was LCDDATA8bits.S06C1
#define X3C5	    LCDDATA23bits.S60C2     //  was LCDDATA16bits.S06C2
#define X4C5	    LCDDATA31bits.S60C3     //  was LCDDATA24bits.S06C3
#define X5C5	    LCDDATA39bits.S60C4     //  was LCDDATA32bits.S06C4
#define X6C5	    LCDDATA47bits.S60C5     //  was LCDDATA40bits.S06C5
#define X7C5	    LCDDATA55bits.S60C6     //  was LCDDATA48bits.S06C6
#define WHEEL4	    LCDDATA63bits.S60C7     //  was LCDDATA56bits.S06C7

#define X1C6	    LCDDATA7bits.S61C0      //  was LCDDATA7bits.S62C0
#define X2C6	    LCDDATA15bits.S61C1     //  was LCDDATA15bits.S62C1
#define X3C6	    LCDDATA23bits.S61C2     //  was LCDDATA23bits.S62C2
#define X4C6	    LCDDATA31bits.S61C3     //  was LCDDATA31bits.S62C3
#define X5C6	    LCDDATA39bits.S61C4     //  was LCDDATA39bits.S62C4
#define X6C6	    LCDDATA47bits.S61C5     //  was LCDDATA47bits.S62C5
#define X7C6	    LCDDATA55bits.S61C6     //  was LCDDATA55bits.S62C6
#define WHEEL5	    LCDDATA63bits.S61C7     //  was LCDDATA63bits.S62C7

#define X1C7	    LCDDATA4bits.S35C0      //  was LCDDATA0bits.S05C0
#define X2C7	    LCDDATA12bits.S35C1     //  was LCDDATA8bits.S05C1
#define X3C7	    LCDDATA20bits.S35C2     //  was LCDDATA16bits.S05C2
#define X4C7	    LCDDATA28bits.S35C3     //  was LCDDATA24bits.S05C3
#define X5C7	    LCDDATA36bits.S35C4     //  was LCDDATA32bits.S05C4
#define X6C7	    LCDDATA44bits.S35C5     //  was LCDDATA40bits.S05C5
#define X7C7	    LCDDATA52bits.S35C6     //  was LCDDATA48bits.S05C6
#define WHEEL6	    LCDDATA60bits.S35C7     //  was LCDDATA56bits.S05C7

#define X1C8	    LCDDATA4bits.S39C0      //  was LCDDATA4bits.S37C0
#define X2C8	    LCDDATA12bits.S39C1     //  was LCDDATA12bits.S37C1
#define X3C8	    LCDDATA20bits.S39C2     //  was LCDDATA20bits.S37C2
#define X4C8	    LCDDATA28bits.S39C3     //  was LCDDATA28bits.S37C3
#define X5C8	    LCDDATA36bits.S39C4     //  was LCDDATA36bits.S37C4
#define X6C8	    LCDDATA44bits.S39C5     //  was LCDDATA44bits.S37C5
#define X7C8	    LCDDATA52bits.S39C6     //  was LCDDATA52bits.S37C6
#define WHEEL7	    LCDDATA60bits.S39C7     //  was LCDDATA60bits.S37C7

#define X1C9	    LCDDATA4bits.S34C0      //  was LCDDATA0bits.S04C0
#define X2C9	    LCDDATA12bits.S34C1     //  was LCDDATA8bits.S04C1
#define X3C9	    LCDDATA20bits.S34C2     //  was LCDDATA16bits.S04C2
#define X4C9	    LCDDATA28bits.S34C3     //  was LCDDATA24bits.S04C3
#define X5C9	    LCDDATA36bits.S34C4     //  was LCDDATA32bits.S04C4
#define X6C9	    LCDDATA44bits.S34C5     //  was LCDDATA40bits.S04C5
#define X7C9	    LCDDATA52bits.S34C6     //  was LCDDATA48bits.S04C6
#define WHEEL8	    LCDDATA60bits.S34C7     //  was LCDDATA56bits.S04C7

#define X1C10	    LCDDATA4bits.S33C0      //  was LCDDATA0bits.S03C0
#define X2C10	    LCDDATA12bits.S33C1     //  was LCDDATA8bits.S03C1
#define X3C10	    LCDDATA20bits.S33C2     //  was LCDDATA16bits.S03C2
#define X4C10	    LCDDATA28bits.S33C3     //  was LCDDATA24bits.S03C3
#define X5C10	    LCDDATA36bits.S33C4     //  was LCDDATA32bits.S03C4
#define X6C10	    LCDDATA44bits.S33C5     //  was LCDDATA40bits.S03C5
#define X7C10	    LCDDATA52bits.S33C6     //  was LCDDATA48bits.S03C6
#define WHEEL9	    LCDDATA60bits.S33C7     //  was LCDDATA56bits.S03C7

#define X1C11	    LCDDATA4bits.S32C0      //  was LCDDATA0bits.S02C0
#define X2C11	    LCDDATA12bits.S32C1     //  was LCDDATA8bits.S02C1
#define X3C11	    LCDDATA20bits.S32C2     //  was LCDDATA16bits.S02C2
#define X4C11	    LCDDATA28bits.S32C3     //  was LCDDATA24bits.S02C3
#define X5C11	    LCDDATA36bits.S32C4     //  was LCDDATA32bits.S02C4
#define X6C11	    LCDDATA44bits.S32C5     //  was LCDDATA40bits.S02C5
#define X7C11	    LCDDATA52bits.S32C6     //  was LCDDATA48bits.S02C6
#define WHEEL10	    LCDDATA60bits.S32C7     //  was LCDDATA56bits.S02C7

#define X1C12	    LCDDATA0bits.S04C0      //  was LCDDATA0bits.S00C0
#define X2C12	    LCDDATA8bits.S04C1      //  was LCDDATA8bits.S00C1
#define X3C12	    LCDDATA16bits.S04C2     //  was LCDDATA16bits.S00C2
#define X4C12	    LCDDATA24bits.S04C3     //  was LCDDATA24bits.S00C3
#define X5C12	    LCDDATA32bits.S04C4     //  was LCDDATA32bits.S00C4
#define X6C12	    LCDDATA40bits.S04C5     //  was LCDDATA40bits.S00C5
#define X7C12	    LCDDATA48bits.S04C6     //  was LCDDATA48bits.S00C6
#define SUN 	    LCDDATA56bits.S04C7     //  was LCDDATA56bits.S00C7

#define X1C13	    LCDDATA0bits.S03C0      //  was LCDDATA6bits.S48C0
#define X2C13	    LCDDATA8bits.S03C1      //  was LCDDATA14bits.S48C1
#define X3C13	    LCDDATA16bits.S03C2     //  was LCDDATA22bits.S48C2
#define X4C13	    LCDDATA24bits.S03C3     //  was LCDDATA30bits.S48C3
#define X5C13	    LCDDATA32bits.S03C4     //  was LCDDATA38bits.S48C4
#define X6C13	    LCDDATA40bits.S03C5     //  was LCDDATA46bits.S48C5
#define X7C13	    LCDDATA48bits.S03C6     //  was LCDDATA54bits.S48C6
#define PAN 	    LCDDATA56bits.S03C7     //  was LCDDATA62bits.S48C7

#define X1C14	    LCDDATA0bits.S02C0      //  was LCDDATA5bits.S47C0
#define X2C14	    LCDDATA8bits.S02C1      //  was LCDDATA13bits.S47C1
#define X3C14	    LCDDATA16bits.S02C2     //  was LCDDATA21bits.S47C2
#define X4C14	    LCDDATA24bits.S02C3     //  was LCDDATA29bits.S47C3
#define X5C14	    LCDDATA32bits.S02C4     //  was LCDDATA37bits.S47C4
#define X6C14	    LCDDATA40bits.S02C5     //  was LCDDATA45bits.S47C5
#define X7C14	    LCDDATA48bits.S02C6     //  was LCDDATA53bits.S47C6
#define WIFI 	    LCDDATA56bits.S02C7     //  was LCDDATA61bits.S47C7

#define X1C15	    LCDDATA7bits.S63C0      //  was LCDDATA5bits.S46C0
#define X2C15	    LCDDATA15bits.S63C1     //  was LCDDATA13bits.S46C1
#define X3C15	    LCDDATA23bits.S63C2     //  was LCDDATA21bits.S46C2
#define X4C15	    LCDDATA31bits.S63C3     //  was LCDDATA29bits.S46C3
#define X5C15	    LCDDATA39bits.S63C4     //  was LCDDATA37bits.S46C4
#define X6C15	    LCDDATA47bits.S63C5     //  was LCDDATA45bits.S46C5
#define X7C15	    LCDDATA55bits.S63C6     //  was LCDDATA53bits.S46C6
#define ZIG 	    LCDDATA63bits.S63C7     //  was LCDDATA61bits.S46C7

#define X1C16	    LCDDATA6bits.S49C0      //  was LCDDATA6bits.S53C0
#define X2C16	    LCDDATA14bits.S49C1     //  was LCDDATA14bits.S53C1
#define X3C16	    LCDDATA22bits.S49C2     //  was LCDDATA22bits.S53C2
#define X4C16	    LCDDATA30bits.S49C3     //  was LCDDATA30bits.S53C3
#define X5C16	    LCDDATA38bits.S49C4     //  was LCDDATA38bits.S53C4
#define X6C16	    LCDDATA46bits.S49C5     //  was LCDDATA46bits.S53C5
#define X7C16	    LCDDATA54bits.S49C6     //  was LCDDATA54bits.S53C6
#define CLOUD 	    LCDDATA62bits.S49C7     //  was LCDDATA62bits.S53C7

#define X1C17	    LCDDATA6bits.S51C0      //  was LCDDATA6bits.S54C0
#define X2C17	    LCDDATA14bits.S51C1     //  was LCDDATA14bits.S54C1
#define X3C17	    LCDDATA22bits.S51C2     //  was LCDDATA22bits.S54C2
#define X4C17	    LCDDATA30bits.S51C3     //  was LCDDATA30bits.S54C3
#define X5C17	    LCDDATA38bits.S51C4     //  was LCDDATA38bits.S54C4
#define X6C17	    LCDDATA46bits.S51C5     //  was LCDDATA46bits.S54C5
#define X7C17	    LCDDATA54bits.S51C6     //  was LCDDATA54bits.S54C6
#define HEART 	    LCDDATA62bits.S51C7     //  was LCDDATA62bits.S54C7

#define X1C18	    LCDDATA5bits.S41C0      //  was LCDDATA7bits.S56C0
#define X2C18	    LCDDATA13bits.S41C1     //  was LCDDATA15bits.S56C1
#define X3C18	    LCDDATA21bits.S41C2     //  was LCDDATA23bits.S56C2
#define X4C18	    LCDDATA29bits.S41C3     //  was LCDDATA31bits.S56C3
#define X5C18	    LCDDATA37bits.S41C4     //  was LCDDATA39bits.S56C4
#define X6C18	    LCDDATA45bits.S41C5     //  was LCDDATA47bits.S56C5
#define X7C18	    LCDDATA53bits.S41C6     //  was LCDDATA55bits.S56C6
#define STRIP 	    LCDDATA61bits.S41C7     //  was LCDDATA63bits.S56C7

#define X1C19	    LCDDATA5bits.S40C0      //  was LCDDATA2bits.S22C0
#define X2C19	    LCDDATA13bits.S40C1     //  was LCDDATA10bits.S22C1
#define X3C19	    LCDDATA21bits.S40C2     //  was LCDDATA18bits.S22C2
#define X4C19	    LCDDATA29bits.S40C3     //  was LCDDATA26bits.S22C3
#define X5C19	    LCDDATA37bits.S40C4     //  was LCDDATA34bits.S22C4
#define X6C19	    LCDDATA45bits.S40C5     //  was LCDDATA42bits.S22C5
#define X7C19	    LCDDATA53bits.S40C6     //  was LCDDATA50bits.S22C6
#define BAT1 	    LCDDATA61bits.S40C7     //  was LCDDATA58bits.S22C7

#define X1C20	    LCDDATA1bits.S08C0      //  was LCDDATA4bits.S32C0
#define X2C20	    LCDDATA9bits.S08C1      //  was LCDDATA12bits.S32C1
#define X3C20	    LCDDATA17bits.S08C2     //  was LCDDATA20bits.S32C2
#define X4C20	    LCDDATA25bits.S08C3     //  was LCDDATA28bits.S32C3
#define X5C20	    LCDDATA33bits.S08C4     //  was LCDDATA36bits.S32C4
#define X6C20	    LCDDATA41bits.S08C5     //  was LCDDATA44bits.S32C5
#define X7C20	    LCDDATA49bits.S08C6     //  was LCDDATA52bits.S32C6
#define BAT2 	    LCDDATA57bits.S08C7     //  was LCDDATA60bits.S32C7

#define X1C21	    LCDDATA1bits.S11C0      //  was LCDDATA0bits.S07C0
#define X2C21	    LCDDATA9bits.S11C1      //  was LCDDATA8bits.S07C1
#define X3C21	    LCDDATA17bits.S11C2     //  was LCDDATA16bits.S07C2
#define X4C21	    LCDDATA25bits.S11C3     //  was LCDDATA24bits.S07C3
#define X5C21	    LCDDATA33bits.S11C4     //  was LCDDATA32bits.S07C4
#define X6C21	    LCDDATA41bits.S11C5     //  was LCDDATA40bits.S07C5
#define X7C21	    LCDDATA49bits.S11C6     //  was LCDDATA48bits.S07C6
#define BAT3 	    LCDDATA57bits.S11C7     //  was LCDDATA56bits.S07C7

#define X1C22	    LCDDATA0bits.S07C0      //  was LCDDATA7bits.S63C0
#define X2C22	    LCDDATA8bits.S07C1      //  was LCDDATA15bits.S63C1
#define X3C22	    LCDDATA16bits.S07C2     //  was LCDDATA23bits.S63C2
#define X4C22	    LCDDATA24bits.S07C3     //  was LCDDATA31bits.S63C3
#define X5C22	    LCDDATA32bits.S07C4     //  was LCDDATA39bits.S63C4
#define X6C22	    LCDDATA40bits.S07C5     //  was LCDDATA47bits.S63C5
#define X7C22	    LCDDATA48bits.S07C6     //  was LCDDATA55bits.S63C6
#define BAT4 	    LCDDATA56bits.S07C7     //  was LCDDATA63bits.S63C7

#define X1C23	    LCDDATA7bits.S62C0      //  was LCDDATA0bits.S01C0
#define X2C23	    LCDDATA15bits.S62C1     //  was LCDDATA8bits.S01C1
#define X3C23	    LCDDATA23bits.S62C2     //  was LCDDATA16bits.S01C2
#define X4C23	    LCDDATA31bits.S62C3     //  was LCDDATA24bits.S01C3
#define X5C23	    LCDDATA39bits.S62C4     //  was LCDDATA32bits.S01C4
#define X6C23	    LCDDATA47bits.S62C5     //  was LCDDATA40bits.S01C5
#define X7C23	    LCDDATA55bits.S62C6     //  was LCDDATA48bits.S01C6
#define DROP 	    LCDDATA63bits.S62C7     //  was LCDDATA56bits.S01C7

#define X1C24	    LCDDATA6bits.S52C0      //  was LCDDATA6bits.S55C0
#define X2C24	    LCDDATA14bits.S52C1     //  was LCDDATA14bits.S55C1
#define X3C24	    LCDDATA22bits.S52C2     //  was LCDDATA22bits.S55C2
#define X4C24	    LCDDATA30bits.S52C3     //  was LCDDATA30bits.S55C3
#define X5C24	    LCDDATA38bits.S52C4     //  was LCDDATA38bits.S55C4
#define X6C24	    LCDDATA46bits.S52C5     //  was LCDDATA46bits.S55C5
#define X7C24	    LCDDATA54bits.S52C6     //  was LCDDATA54bits.S55C6
#define THERMO 	    LCDDATA62bits.S52C7     //  was LCDDATA62bits.S55C7

#define X1C25	    LCDDATA5bits.S44C0      //  was LCDDATA5bits.S40C0
#define X2C25	    LCDDATA13bits.S44C1     //  was LCDDATA13bits.S40C1
#define X3C25	    LCDDATA21bits.S44C2     //  was LCDDATA21bits.S40C2
#define X4C25	    LCDDATA29bits.S44C3     //  was LCDDATA29bits.S40C3
#define X5C25	    LCDDATA37bits.S44C4     //  was LCDDATA37bits.S40C4
#define X6C25	    LCDDATA45bits.S44C5     //  was LCDDATA45bits.S40C5
#define X7C25	    LCDDATA53bits.S44C6     //  was LCDDATA53bits.S40C6
#define DEGCEL 	    LCDDATA61bits.S44C7     //  was LCDDATA61bits.S40C7

#define X1C26	    LCDDATA6bits.S50C0      //  was LCDDATA2bits.S21C0
#define X2C26	    LCDDATA14bits.S50C1     //  was LCDDATA10bits.S21C1
#define X3C26	    LCDDATA22bits.S50C2     //  was LCDDATA18bits.S21C2
#define X4C26	    LCDDATA30bits.S50C3     //  was LCDDATA26bits.S21C3
#define X5C26	    LCDDATA38bits.S50C4     //  was LCDDATA34bits.S21C4
#define X6C26	    LCDDATA46bits.S50C5     //  was LCDDATA42bits.S21C5
#define X7C26	    LCDDATA54bits.S50C6     //  was LCDDATA50bits.S21C6
#define DEGFAR 	    LCDDATA62bits.S50C7     //  was LCDDATA58bits.S21C7

#define X1C27	    LCDDATA2bits.S19C0      //  was LCDDATA4bits.S39C0
#define X2C27	    LCDDATA10bits.S19C1     //  was LCDDATA12bits.S39C1
#define X3C27	    LCDDATA18bits.S19C2     //  was LCDDATA20bits.S39C2
#define X4C27	    LCDDATA26bits.S19C3     //  was LCDDATA28bits.S39C3
#define X5C27	    LCDDATA34bits.S19C4     //  was LCDDATA36bits.S39C4
#define X6C27	    LCDDATA42bits.S19C5     //  was LCDDATA44bits.S39C5
#define X7C27	    LCDDATA50bits.S19C6     //  was LCDDATA52bits.S39C6
#define MICROCHIP   LCDDATA58bits.S19C7     //  was LCDDATA60bits.S39C7

#define X1C28	    LCDDATA6bits.S55C0      //  was LCDDATA4bits.S38C0
#define X2C28	    LCDDATA14bits.S55C1     //  was LCDDATA12bits.S38C1
#define X3C28	    LCDDATA22bits.S55C2     //  was LCDDATA20bits.S38C2
#define X4C28	    LCDDATA30bits.S55C3     //  was LCDDATA28bits.S38C3
#define X5C28	    LCDDATA38bits.S55C4     //  was LCDDATA36bits.S38C4
#define X6C28	    LCDDATA46bits.S55C5     //  was LCDDATA44bits.S38C5
#define X7C28	    LCDDATA54bits.S55C6     //  was LCDDATA52bits.S38C6
#define CENT        LCDDATA62bits.S55C7     //  was LCDDATA60bits.S38C7

#define X1C29	    LCDDATA0bits.S06C0      //  was LCDDATA4bits.S34C0
#define X2C29	    LCDDATA8bits.S06C1      //  was LCDDATA12bits.S34C1
#define X3C29	    LCDDATA16bits.S06C2     //  was LCDDATA20bits.S34C2
#define X4C29	    LCDDATA24bits.S06C3     //  was LCDDATA28bits.S34C3
#define X5C29	    LCDDATA32bits.S06C4     //  was LCDDATA36bits.S34C4
#define X6C29	    LCDDATA40bits.S06C5     //  was LCDDATA44bits.S34C5
#define X7C29	    LCDDATA48bits.S06C6     //  was LCDDATA52bits.S34C6
#define RMB         LCDDATA56bits.S06C7     //  was LCDDATA60bits.S34C7

#define X1C30	    LCDDATA0bits.S05C0      //  was LCDDATA4bits.S33C0
#define X2C30	    LCDDATA8bits.S05C1      //  was LCDDATA12bits.S33C1
#define X3C30	    LCDDATA16bits.S05C2     //  was LCDDATA20bits.S33C2
#define X4C30	    LCDDATA24bits.S05C3     //  was LCDDATA28bits.S33C3
#define X5C30	    LCDDATA32bits.S05C4     //  was LCDDATA36bits.S33C4
#define X6C30	    LCDDATA40bits.S05C5     //  was LCDDATA44bits.S33C5
#define X7C30	    LCDDATA48bits.S05C6     //  was LCDDATA52bits.S33C6
#define X1C37       LCDDATA56bits.S05C7     //  was LCDDATA60bits.S33C7

#define X1C31	    LCDDATA0bits.S01C0      //  was LCDDATA3bits.S25C0
#define X2C31	    LCDDATA8bits.S01C1      //  was LCDDATA11bits.S25C1
#define X3C31	    LCDDATA16bits.S01C2     //  was LCDDATA19bits.S25C2
#define X4C31	    LCDDATA24bits.S01C3     //  was LCDDATA27bits.S25C3
#define X5C31	    LCDDATA32bits.S01C4     //  was LCDDATA35bits.S25C4
#define X6C31	    LCDDATA40bits.S01C5     //  was LCDDATA43bits.S25C5
#define X7C31	    LCDDATA48bits.S01C6     //  was LCDDATA51bits.S25C6
#define X2C37       LCDDATA56bits.S01C7     //  was LCDDATA59bits.S25C7

#define X1C32	    LCDDATA0bits.S00C0      //  was LCDDATA3bits.S24C0
#define X2C32	    LCDDATA8bits.S00C1      //  was LCDDATA11bits.S24C1
#define X3C32	    LCDDATA16bits.S00C2     //  was LCDDATA19bits.S24C2
#define X4C32	    LCDDATA24bits.S00C3     //  was LCDDATA27bits.S24C3
#define X5C32	    LCDDATA32bits.S00C4     //  was LCDDATA35bits.S24C4
#define X6C32	    LCDDATA40bits.S00C5     //  was LCDDATA43bits.S24C5
#define X7C32	    LCDDATA48bits.S00C6     //  was LCDDATA51bits.S24C6
#define X3C37       LCDDATA56bits.S00C7     //  was LCDDATA59bits.S24C7

#define X1C33	    LCDDATA6bits.S48C0      //  was LCDDATA2bits.S23C0
#define X2C33	    LCDDATA14bits.S48C1     //  was LCDDATA10bits.S23C1
#define X3C33	    LCDDATA22bits.S48C2     //  was LCDDATA18bits.S23C2
#define X4C33	    LCDDATA30bits.S48C3     //  was LCDDATA26bits.S23C3
#define X5C33	    LCDDATA38bits.S48C4     //  was LCDDATA34bits.S23C4
#define X6C33	    LCDDATA46bits.S48C5     //  was LCDDATA42bits.S23C5
#define X7C33	    LCDDATA54bits.S48C6     //  was LCDDATA50bits.S23C6
#define X4C37       LCDDATA62bits.S48C7     //  was LCDDATA58bits.S23C7

#define X1C34	    LCDDATA5bits.S47C0      //  was LCDDATA5bits.S43C0
#define X2C34	    LCDDATA13bits.S47C1     //  was LCDDATA13bits.S43C1
#define X3C34	    LCDDATA21bits.S47C2     //  was LCDDATA21bits.S43C2
#define X4C34	    LCDDATA29bits.S47C3     //  was LCDDATA29bits.S43C3
#define X5C34	    LCDDATA37bits.S47C4     //  was LCDDATA37bits.S43C4
#define X6C34	    LCDDATA45bits.S47C5     //  was LCDDATA45bits.S43C5
#define X7C34	    LCDDATA53bits.S47C6     //  was LCDDATA53bits.S43C6
#define X5C37       LCDDATA61bits.S47C7     //  was LCDDATA61bits.S43C7

#define X1C35	    LCDDATA5bits.S46C0      //  was LCDDATA5bits.S42C0
#define X2C35	    LCDDATA13bits.S46C1     //  was LCDDATA13bits.S42C1
#define X3C35	    LCDDATA21bits.S46C2     //  was LCDDATA21bits.S42C2
#define X4C35	    LCDDATA29bits.S46C3     //  was LCDDATA29bits.S42C3
#define X5C35	    LCDDATA37bits.S46C4     //  was LCDDATA37bits.S42C4
#define X6C35	    LCDDATA45bits.S46C5     //  was LCDDATA45bits.S42C5
#define X7C35	    LCDDATA53bits.S46C6     //  was LCDDATA53bits.S42C6
#define X6C37       LCDDATA61bits.S46C7     //  was LCDDATA61bits.S42C7

#define X1C36	    LCDDATA5bits.S45C0      //  was LCDDATA5bits.S41C0
#define X2C36	    LCDDATA13bits.S45C1     //  was LCDDATA13bits.S41C1
#define X3C36	    LCDDATA21bits.S45C2     //  was LCDDATA21bits.S41C2
#define X4C36	    LCDDATA29bits.S45C3     //  was LCDDATA29bits.S41C3
#define X5C36	    LCDDATA37bits.S45C4     //  was LCDDATA37bits.S41C4
#define X6C36	    LCDDATA45bits.S45C5     //  was LCDDATA45bits.S41C5
#define X7C36	    LCDDATA53bits.S45C6     //  was LCDDATA53bits.S41C6
#define X7C37       LCDDATA61bits.S45C7     //  was LCDDATA61bits.S41C7

#else // not defined(__97J94_PIM2)
/** LCD Glass DEFINITIONS ********************************************/

//These are all of the redefinitions of the LCD pins for simplified use
#define S1 			LCDSE4bits.SE36
#define S2 			LCDSE7bits.SE57
#define S3 			LCDSE7bits.SE61
#define S4 			LCDSE4bits.SE35
#define S5 			LCDSE0bits.SE06

#define S6 			LCDSE7bits.SE62
#define S7 			LCDSE0bits.SE05
#define S8 			LCDSE4bits.SE37
#define S9			LCDSE0bits.SE04
#define S10			LCDSE0bits.SE03
#define S11			LCDSE0bits.SE02
#define S12			LCDSE0bits.SE00
#define S13			LCDSE6bits.SE48
#define S14			LCDSE5bits.SE47
#define S15			LCDSE5bits.SE46

#define S16			LCDSE6bits.SE53
#define S17			LCDSE6bits.SE54
#define S18			LCDSE7bits.SE56
#define S19			LCDSE2bits.SE22
#define S20			LCDSE4bits.SE32
#define S21			LCDSE0bits.SE07
#define S22			LCDSE7bits.SE63
#define S23			LCDSE0bits.SE01

#define S24			LCDSE6bits.SE55
#define S25			LCDSE5bits.SE40
#define S26			LCDSE2bits.SE21
#define S27			LCDSE4bits.SE39
#define S28			LCDSE4bits.SE38
#define S29			LCDSE4bits.SE34
#define S30			LCDSE4bits.SE33
#define S31			LCDSE3bits.SE25
#define S32			LCDSE3bits.SE24
#define S33			LCDSE2bits.SE23
#define S34			LCDSE5bits.SE43
#define S35			LCDSE5bits.SE42
#define S36			LCDSE5bits.SE41

#define X1C1 		LCDDATA4bits.S36C0
#define X2C1 		LCDDATA12bits.S36C1
#define X3C1 		LCDDATA20bits.S36C2
#define X4C1 		LCDDATA28bits.S36C3
#define X5C1 		LCDDATA36bits.S36C4
#define X6C1 		LCDDATA44bits.S36C5
#define X7C1 		LCDDATA52bits.S36C6
#define Dollar		LCDDATA60bits.S36C7

#define X1C2		LCDDATA7bits.S57C0
#define X2C2		LCDDATA15bits.S57C1
#define X3C2		LCDDATA23bits.S57C2
#define X4C2		LCDDATA31bits.S57C3
#define X5C2		LCDDATA39bits.S57C4
#define X6C2		LCDDATA47bits.S57C5
#define X7C2		LCDDATA55bits.S57C6
#define WHEEL1	 	LCDDATA63bits.S57C7

#define X1C3		LCDDATA7bits.S61C0
#define X2C3		LCDDATA15bits.S61C1
#define X3C3		LCDDATA23bits.S61C2
#define X4C3		LCDDATA31bits.S61C3
#define X5C3		LCDDATA39bits.S61C4
#define X6C3		LCDDATA47bits.S61C5
#define X7C3		LCDDATA55bits.S61C6
#define WHEEL2		LCDDATA63bits.S61C7

#define X1C4		LCDDATA4bits.S35C0
#define X2C4		LCDDATA12bits.S35C1
#define X3C4		LCDDATA20bits.S35C2
#define X4C4		LCDDATA28bits.S35C3
#define X5C4		LCDDATA36bits.S35C4
#define X6C4		LCDDATA44bits.S35C5
#define X7C4		LCDDATA52bits.S35C6
#define WHEEL3		LCDDATA60bits.S35C7

#define X1C5		LCDDATA0bits.S06C0
#define X2C5		LCDDATA8bits.S06C1
#define X3C5		LCDDATA16bits.S06C2
#define X4C5		LCDDATA24bits.S06C3
#define X5C5		LCDDATA32bits.S06C4
#define X6C5		LCDDATA40bits.S06C5
#define X7C5		LCDDATA48bits.S06C6
#define WHEEL4		LCDDATA56bits.S06C7

#define X1C6		LCDDATA7bits.S62C0
#define X2C6		LCDDATA15bits.S62C1
#define X3C6		LCDDATA23bits.S62C2
#define X4C6		LCDDATA31bits.S62C3
#define X5C6		LCDDATA39bits.S62C4
#define X6C6		LCDDATA47bits.S62C5
#define X7C6		LCDDATA55bits.S62C6
#define WHEEL5		LCDDATA63bits.S62C7

#define X1C7		LCDDATA0bits.S05C0
#define X2C7		LCDDATA8bits.S05C1
#define X3C7		LCDDATA16bits.S05C2
#define X4C7		LCDDATA24bits.S05C3
#define X5C7		LCDDATA32bits.S05C4
#define X6C7		LCDDATA40bits.S05C5
#define X7C7		LCDDATA48bits.S05C6
#define WHEEL6		LCDDATA56bits.S05C7

#define X1C8		LCDDATA4bits.S37C0
#define X2C8		LCDDATA12bits.S37C1
#define X3C8		LCDDATA20bits.S37C2
#define X4C8		LCDDATA28bits.S37C3
#define X5C8		LCDDATA36bits.S37C4
#define X6C8		LCDDATA44bits.S37C5
#define X7C8		LCDDATA52bits.S37C6
#define WHEEL7		LCDDATA60bits.S37C7

#define X1C9		LCDDATA0bits.S04C0
#define X2C9		LCDDATA8bits.S04C1
#define X3C9		LCDDATA16bits.S04C2
#define X4C9		LCDDATA24bits.S04C3
#define X5C9		LCDDATA32bits.S04C4
#define X6C9		LCDDATA40bits.S04C5
#define X7C9		LCDDATA48bits.S04C6
#define WHEEL8		LCDDATA56bits.S04C7

#define X1C10		LCDDATA0bits.S03C0
#define X2C10		LCDDATA8bits.S03C1
#define X3C10		LCDDATA16bits.S03C2
#define X4C10		LCDDATA24bits.S03C3
#define X5C10		LCDDATA32bits.S03C4
#define X6C10		LCDDATA40bits.S03C5
#define X7C10		LCDDATA48bits.S03C6
#define WHEEL9		LCDDATA56bits.S03C7

#define X1C11		LCDDATA0bits.S02C0
#define X2C11		LCDDATA8bits.S02C1
#define X3C11		LCDDATA16bits.S02C2
#define X4C11		LCDDATA24bits.S02C3
#define X5C11		LCDDATA32bits.S02C4
#define X6C11		LCDDATA40bits.S02C5
#define X7C11		LCDDATA48bits.S02C6
#define WHEEL10		LCDDATA56bits.S02C7

#define X1C12		LCDDATA0bits.S00C0
#define X2C12		LCDDATA8bits.S00C1
#define X3C12		LCDDATA16bits.S00C2
#define X4C12		LCDDATA24bits.S00C3
#define X5C12		LCDDATA32bits.S00C4
#define X6C12		LCDDATA40bits.S00C5
#define X7C12		LCDDATA48bits.S00C6
#define SUN 		LCDDATA56bits.S00C7

#define X1C13		LCDDATA6bits.S48C0
#define X2C13		LCDDATA14bits.S48C1
#define X3C13		LCDDATA22bits.S48C2
#define X4C13		LCDDATA30bits.S48C3
#define X5C13		LCDDATA38bits.S48C4
#define X6C13		LCDDATA46bits.S48C5
#define X7C13		LCDDATA54bits.S48C6
#define PAN 		LCDDATA62bits.S48C7

#define X1C14		LCDDATA5bits.S47C0
#define X2C14		LCDDATA13bits.S47C1
#define X3C14		LCDDATA21bits.S47C2
#define X4C14		LCDDATA29bits.S47C3
#define X5C14		LCDDATA37bits.S47C4
#define X6C14		LCDDATA45bits.S47C5
#define X7C14		LCDDATA53bits.S47C6
#define WIFI 		LCDDATA61bits.S47C7

#define X1C15		LCDDATA5bits.S46C0
#define X2C15		LCDDATA13bits.S46C1
#define X3C15		LCDDATA21bits.S46C2
#define X4C15		LCDDATA29bits.S46C3
#define X5C15		LCDDATA37bits.S46C4
#define X6C15		LCDDATA45bits.S46C5
#define X7C15		LCDDATA53bits.S46C6
#define ZIG 		LCDDATA61bits.S46C7

#define X1C16		LCDDATA6bits.S53C0
#define X2C16		LCDDATA14bits.S53C1
#define X3C16		LCDDATA22bits.S53C2
#define X4C16		LCDDATA30bits.S53C3
#define X5C16		LCDDATA38bits.S53C4
#define X6C16		LCDDATA46bits.S53C5
#define X7C16		LCDDATA54bits.S53C6
#define CLOUD 		LCDDATA62bits.S53C7

#define X1C17		LCDDATA6bits.S54C0
#define X2C17		LCDDATA14bits.S54C1
#define X3C17		LCDDATA22bits.S54C2
#define X4C17		LCDDATA30bits.S54C3
#define X5C17		LCDDATA38bits.S54C4
#define X6C17		LCDDATA46bits.S54C5
#define X7C17		LCDDATA54bits.S54C6
#define HEART 		LCDDATA62bits.S54C7

#define X1C18		LCDDATA7bits.S56C0
#define X2C18		LCDDATA15bits.S56C1
#define X3C18		LCDDATA23bits.S56C2
#define X4C18		LCDDATA31bits.S56C3
#define X5C18		LCDDATA39bits.S56C4
#define X6C18		LCDDATA47bits.S56C5
#define X7C18		LCDDATA55bits.S56C6
#define STRIP 		LCDDATA63bits.S56C7

#define X1C19		LCDDATA2bits.S22C0
#define X2C19		LCDDATA10bits.S22C1
#define X3C19		LCDDATA18bits.S22C2
#define X4C19		LCDDATA26bits.S22C3
#define X5C19		LCDDATA34bits.S22C4
#define X6C19		LCDDATA42bits.S22C5
#define X7C19		LCDDATA50bits.S22C6
#define BAT1 		LCDDATA58bits.S22C7

#define X1C20		LCDDATA4bits.S32C0
#define X2C20		LCDDATA12bits.S32C1
#define X3C20		LCDDATA20bits.S32C2
#define X4C20		LCDDATA28bits.S32C3
#define X5C20		LCDDATA36bits.S32C4
#define X6C20		LCDDATA44bits.S32C5
#define X7C20		LCDDATA52bits.S32C6
#define BAT2 		LCDDATA60bits.S32C7

#define X1C21		LCDDATA0bits.S07C0
#define X2C21		LCDDATA8bits.S07C1
#define X3C21		LCDDATA16bits.S07C2
#define X4C21		LCDDATA24bits.S07C3
#define X5C21		LCDDATA32bits.S07C4
#define X6C21		LCDDATA40bits.S07C5
#define X7C21		LCDDATA48bits.S07C6
#define BAT3 		LCDDATA56bits.S07C7

#define X1C22		LCDDATA7bits.S63C0
#define X2C22		LCDDATA15bits.S63C1
#define X3C22		LCDDATA23bits.S63C2
#define X4C22		LCDDATA31bits.S63C3
#define X5C22		LCDDATA39bits.S63C4
#define X6C22		LCDDATA47bits.S63C5
#define X7C22		LCDDATA55bits.S63C6
#define BAT4 		LCDDATA63bits.S63C7

#define X1C23		LCDDATA0bits.S01C0
#define X2C23		LCDDATA8bits.S01C1
#define X3C23		LCDDATA16bits.S01C2
#define X4C23		LCDDATA24bits.S01C3
#define X5C23		LCDDATA32bits.S01C4
#define X6C23		LCDDATA40bits.S01C5
#define X7C23		LCDDATA48bits.S01C6
#define DROP 		LCDDATA56bits.S01C7

#define X1C24		LCDDATA6bits.S55C0
#define X2C24		LCDDATA14bits.S55C1
#define X3C24		LCDDATA22bits.S55C2
#define X4C24		LCDDATA30bits.S55C3
#define X5C24		LCDDATA38bits.S55C4
#define X6C24		LCDDATA46bits.S55C5
#define X7C24		LCDDATA54bits.S55C6
#define THERMO 		LCDDATA62bits.S55C7

#define X1C25		LCDDATA5bits.S40C0
#define X2C25		LCDDATA13bits.S40C1
#define X3C25		LCDDATA21bits.S40C2
#define X4C25		LCDDATA29bits.S40C3
#define X5C25		LCDDATA37bits.S40C4
#define X6C25		LCDDATA45bits.S40C5
#define X7C25		LCDDATA53bits.S40C6
#define DEGCEL 		LCDDATA61bits.S40C7

#define X1C26		LCDDATA2bits.S21C0
#define X2C26		LCDDATA10bits.S21C1
#define X3C26		LCDDATA18bits.S21C2
#define X4C26		LCDDATA26bits.S21C3
#define X5C26		LCDDATA34bits.S21C4
#define X6C26		LCDDATA42bits.S21C5
#define X7C26		LCDDATA50bits.S21C6
#define DEGFAR 		LCDDATA58bits.S21C7

#define X1C27		LCDDATA4bits.S39C0
#define X2C27		LCDDATA12bits.S39C1
#define X3C27		LCDDATA20bits.S39C2
#define X4C27		LCDDATA28bits.S39C3
#define X5C27		LCDDATA36bits.S39C4
#define X6C27		LCDDATA44bits.S39C5
#define X7C27		LCDDATA52bits.S39C6
#define MICROCHIP 	LCDDATA60bits.S39C7

#define X1C28		LCDDATA4bits.S38C0
#define X2C28		LCDDATA12bits.S38C1
#define X3C28		LCDDATA20bits.S38C2
#define X4C28		LCDDATA28bits.S38C3
#define X5C28		LCDDATA36bits.S38C4
#define X6C28		LCDDATA44bits.S38C5
#define X7C28		LCDDATA52bits.S38C6
#define CENT     	LCDDATA60bits.S38C7

#define X1C29		LCDDATA4bits.S34C0
#define X2C29		LCDDATA12bits.S34C1
#define X3C29		LCDDATA20bits.S34C2
#define X4C29		LCDDATA28bits.S34C3
#define X5C29		LCDDATA36bits.S34C4
#define X6C29		LCDDATA44bits.S34C5
#define X7C29		LCDDATA52bits.S34C6
#define RMB     	LCDDATA60bits.S34C7

#define X1C30		LCDDATA4bits.S33C0
#define X2C30		LCDDATA12bits.S33C1
#define X3C30		LCDDATA20bits.S33C2
#define X4C30		LCDDATA28bits.S33C3
#define X5C30		LCDDATA36bits.S33C4
#define X6C30		LCDDATA44bits.S33C5
#define X7C30		LCDDATA52bits.S33C6
#define X1C37     	LCDDATA60bits.S33C7

#define X1C31		LCDDATA3bits.S25C0
#define X2C31		LCDDATA11bits.S25C1
#define X3C31		LCDDATA19bits.S25C2
#define X4C31		LCDDATA27bits.S25C3
#define X5C31		LCDDATA35bits.S25C4
#define X6C31		LCDDATA43bits.S25C5
#define X7C31		LCDDATA51bits.S25C6
#define X2C37     	LCDDATA59bits.S25C7

#define X1C32		LCDDATA3bits.S24C0
#define X2C32		LCDDATA11bits.S24C1
#define X3C32		LCDDATA19bits.S24C2
#define X4C32		LCDDATA27bits.S24C3
#define X5C32		LCDDATA35bits.S24C4
#define X6C32		LCDDATA43bits.S24C5
#define X7C32		LCDDATA51bits.S24C6
#define X3C37    	LCDDATA59bits.S24C7

#define X1C33		LCDDATA2bits.S23C0
#define X2C33		LCDDATA10bits.S23C1
#define X3C33		LCDDATA18bits.S23C2
#define X4C33		LCDDATA26bits.S23C3
#define X5C33		LCDDATA34bits.S23C4
#define X6C33		LCDDATA42bits.S23C5
#define X7C33		LCDDATA50bits.S23C6
#define X4C37    	LCDDATA58bits.S23C7

#define X1C34		LCDDATA5bits.S43C0
#define X2C34		LCDDATA13bits.S43C1
#define X3C34		LCDDATA21bits.S43C2
#define X4C34		LCDDATA29bits.S43C3
#define X5C34		LCDDATA37bits.S43C4
#define X6C34		LCDDATA45bits.S43C5
#define X7C34		LCDDATA53bits.S43C6
#define X5C37    	LCDDATA61bits.S43C7

#define X1C35		LCDDATA5bits.S42C0
#define X2C35		LCDDATA13bits.S42C1
#define X3C35		LCDDATA21bits.S42C2
#define X4C35		LCDDATA29bits.S42C3
#define X5C35		LCDDATA37bits.S42C4
#define X6C35		LCDDATA45bits.S42C5
#define X7C35		LCDDATA53bits.S42C6
#define X6C37    	LCDDATA61bits.S42C7

#define X1C36		LCDDATA5bits.S41C0
#define X2C36		LCDDATA13bits.S41C1
#define X3C36		LCDDATA21bits.S41C2
#define X4C36		LCDDATA29bits.S41C3
#define X5C36		LCDDATA37bits.S41C4
#define X6C36		LCDDATA45bits.S41C5
#define X7C36		LCDDATA53bits.S41C6
#define X7C37    	LCDDATA61bits.S41C7

#endif // not defined(__97J94_PIM2)


#endif //EOF
