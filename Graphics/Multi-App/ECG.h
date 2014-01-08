/*********************************************************************
 * Microchip Graphic Library Demo Application
 * This is the header file for the ECG demo.
 *********************************************************************
 * FileName:        ECG.h
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
 * Anton Alkhimenok		03/10/08    ...
 ********************************************************************/
#ifndef ECG_H
    #define ECG_H

/************************************************************************
 Function: void CreateECG(void)
                                                                       
 Overview: Creates the ECG demo screen.
  		                                         
 Input: none
                                                                       
 Output: none
************************************************************************/
void    CreateECG(void);

/************************************************************************
 Function: void PanelECG(void)
                                                                       
 Overview: Draws the box for the ECG data.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
WORD    PanelECG(void);

/************************************************************************
 Function: void GraphECG(void)
                                                                       
 Overview: Draws the ECG data or graph.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void    GraphECG(void);

/************************************************************************
 Function: void GetECGSamples(WORD number)
                                                                       
 Overview: Gets ECG sample data from FLASH (can be any source) into 
 		   temporary buffer. 
 		                                                                          
 Input: number - specifies the sample number to be retrieved.
                                                                       
 Output: Returns zero if samples number in the buffer is less 
 		 than defined by parameter
************************************************************************/
WORD    GetECGSamples(WORD number);

/************************************************************************
 Function: WORD MsgECG(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the original GOL message.
                                                                       
 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD    MsgECG(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: void UpdateECG(WORD tick)
                                                                       
 Overview: Fill the circular buffer with sample data.
  		                                             
 Input: tick - specifies the current tick time. Used to check if the 
 			   pre-defined delay between sample display is reached.
                                                                       
 Output: none
 ************************************************************************/
void    UpdateECG(DWORD tick);

/************************************************************************
 Function: void InitECGStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void    InitECGStyleScheme(GOL_SCHEME *pScheme);
#endif
