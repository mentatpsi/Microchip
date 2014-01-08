/*********************************************************************
 * Microchip Graphic Library Demo Application
 * This is the header file for the Game demo.
 *********************************************************************
 * FileName:        Games.h
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
 * KO          			2/14/08		First release
 * Anton Alkhimenok		3/10/08     Ported for ESC demo.
 ********************************************************************/
#ifndef _GAMES_H
    #define _GAMES_H

/************************************************************************
 Function: void DrawSnake(DWORD tick)
                                                                       
 Overview: This function draws the snake for every determined interval.
           If the tick count does not exceed the interval from the last
           update, it will do nothing.
 		                                                                          
 Input: tick - the counter for the elapsed tick time.
                                                                       
 Output: none
************************************************************************/
void    DrawSnake(DWORD tick);

/*******************************************************************************
Function:       WORD ProcessMessageSnake( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

Precondition:   Call ShowScreenSnake() prior to using this function to display
                the correct screen.

Overview:       This function processes the messages for the Snake game, to
                change the snake's direction.  We will be nice to the player -
                if he selects the direction opposite to what he is traveling,
                we will ignore the message rather than instantly tie the snake
                into a knot.

Input:          WORD translatedMsg  The translated control-level message
                OBJ_HEADER* pObj    Object to which the message applies
                GOL_MSG* pMsg       The original system message information

Output:         None

Return Values:  0   Do not call the default message handler.
                1   Call the default message handler.

Side Effects:   None

Remarks:        None
*******************************************************************************/
WORD    ProcessMessageSnake(WORD translatedMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: void ShowScreenScore(void)
                                                                       
 Overview: This function shows the score after the game is terminated.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void    ShowScreenScore(void);

/************************************************************************
 Function: void ShowScreenSnake(void)
                                                                       
 Overview: This function creates the game screen.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void    ShowScreenSnake(void);

/************************************************************************
 Function: void InitGamesStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void    InitGamesStyleScheme(GOL_SCHEME *pScheme);
#endif
