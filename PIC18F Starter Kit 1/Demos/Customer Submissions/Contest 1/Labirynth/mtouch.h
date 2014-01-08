/********************************************************************
  File Information:
    FileName:     	soft_start.h
    Dependencies:   See INCLUDES section
    Processor:      PIC18F46J50
    Hardware:       PIC18F Starter Kit
    Complier:  	    Microchip C18 (for PIC18)
    Company:        Microchip Technology, Inc.
    
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

  File Description:
    
    Change History:
     Rev   Date         Description
     1.0                Initial release

********************************************************************/
#ifndef MTOUCH_H
#define MTOUCH_H

/******** Include files **********************/
#include "Compiler.h"
/*********************************************/

void mTouchInit(void);
void mTouchCalibrate(void);
unsigned int mTouchReadButton(unsigned char buttonID);


#endif
