/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        DemoSelection.h
 * Dependencies:    MainDemo.h
 * Processor:       PIC24/PIC32
 * Compiler:        C30/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok
 * Paolo Tamayo         03/10/08    ...
 ********************************************************************/
#ifndef DEMOSELECTION_H
    #define DEMOSELECTION_H

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
//// macroses for icons positions
    #define ICON_WIDTH          (74 + GOL_EMBOSS_SIZE)
    #define ICON_HEIGHT         (100)   //(72+GOL_EMBOSS_SIZE)
    #define ICON_COLUMNSNUM     3
    #define ICON_ROWSNUM        2
    #define ICON_XINDENT        ((GetMaxX() - ICON_COLUMNSNUM * ICON_WIDTH) / (ICON_COLUMNSNUM + 1))
    #define ICON_YINDENT        ((GetMaxY() - CTRLBTN_HEIGHT - (ICON_ROWSNUM * ICON_HEIGHT)) / (ICON_ROWSNUM + 1))
    #define IconTop(row)        (((row + 1) * ICON_YINDENT) + (row * ICON_HEIGHT))
    #define IconBottom(row)     ((row + 1) * (ICON_YINDENT + ICON_HEIGHT))
    #define IconLeft(column)    (((column + 1) * ICON_XINDENT) + (column * ICON_WIDTH))
    #define IconRight(column)   ((column + 1) * (ICON_XINDENT + ICON_WIDTH))

/************************************************************************
 Function: WORD CreateDemoSelection(void)
                                                                       
 Overview: Creates the menu of small demos each represented by an icon.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void    CreateDemoSelection(void);

/************************************************************************
 Function: WORD MsgDemoSelection(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: Message callback for the demo selection. Based on user selection
 		   A demo is enabled by switching the screen state. Icons used
 		   changes from colored version to monochrome version when pressed. 
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 
                                                                       
 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD    MsgDemoSelection(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: CheckDemoStatus()
                                                                       
 Overview: Checks the hardware components of a demo. If hardware is 
 		   not present demo icon will be disabled.
 		                                                                          
 Input: none                                                          
                                                                       
 Output: none
************************************************************************/
void    CheckDemoStatus(void);

/************************************************************************
 Function: void InitDemoSelectStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void    InitDemoSelectStyleScheme(GOL_SCHEME *pScheme);
#endif
