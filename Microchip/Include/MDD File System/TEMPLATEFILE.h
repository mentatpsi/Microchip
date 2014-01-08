/******************************************************************************
 *
 *               Microchip Memory Disk Drive File System
 *
 ******************************************************************************
 * FileName:        TEMPLATEFILE.h
 * Dependencies:    GenericTypeDefs.h
 *					FSconfig.h
 *					FSDefs.h
 * Processor:       PIC18/PIC24/dsPIC30/dsPIC33
 * Compiler:        C18/C30
 * Company:         Microchip Technology, Inc.
 * Version:         1.2.4
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
*****************************************************************************/


/*************************************************************************/
/*  Note:  This file is included as a template of a header file for      */
/*         a new physical layer. It is designed to go with               */
/*         "FS Phys Interface Template.c"                               */
/*************************************************************************/


#include "GenericTypeDefs.h"
#include "FSconfig.h"
#include "FSDefs.h"

#define FALSE	0
#define TRUE	!FALSE

/****************************************************************/
/*                    YOUR CODE HERE                            */
/* Add any defines here                                         */
/****************************************************************/

#define INITIALIZATION_VALUE		0x55


/***************************************************************/
/*                      END OF YOUR CODE                       */
/***************************************************************/


BYTE MDD_TEMPLATE_InitIO(void);
BYTE MDD_TEMPLATE_MediaDetect(void);
MEDIA_INFORMATION * MDD_TEMPLATE_MediaInitialize(void);
BYTE MDD_TEMPLATE_SectorRead(DWORD sector_addr, BYTE* buffer);
BYTE MDD_TEMPLATE_SectorWrite(DWORD sector_addr, BYTE* buffer, BYTE allowWriteToZero);
extern BYTE gDataBuffer[];
extern BYTE gFATBuffer[];
extern DISK gDiskData;


/****************************************************************/
/*                    YOUR CODE HERE                            */
/* Add prototypes for any custom functions here                 */
/****************************************************************/


/***************************************************************/
/*                      END OF YOUR CODE                       */
/***************************************************************/


