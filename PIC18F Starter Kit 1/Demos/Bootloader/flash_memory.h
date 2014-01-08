/*
********************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright © 2008 Microchip Technology Inc. and its licensors.  All         
rights reserved.                                                                
                                                                                
Microchip licenses to you the right to: (1) install Software on a single        
computer and use the Software with Microchip 16-bit microcontrollers and        
16-bit digital signal controllers ("Microchip Product"); and (2) at your        
own discretion and risk, use, modify, copy and distribute the device            
driver files of the Software that are provided to you in Source Code;           
provided that such Device Drivers are only used with Microchip Products         
and that no open source or free software is incorporated into the Device        
Drivers without Microchip's prior written consent in each instance.             
                                                                                
You should refer to the license agreement accompanying this Software for        
additional information regarding your rights and obligations.                   
                                                                                
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY         
KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY              
WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A          
PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE             
LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY,               
CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY           
DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY         
INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR         
LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY,                 
SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY         
DEFENSE THEREOF), OR OTHER SIMILAR COSTS.                                       
                                                                                
********************************************************************************
*/

#ifndef FLASH_H
#define FLASH_H

#include "GenericTypeDefs.h"

#define PM_ROW_WRITE    0x4001 
#define PM_PAGE_ERASE   0x4042

/* Device Flash Memory Configuration */
#define PM_INSTR_SIZE 		4		//bytes per instruction 
#define PM_ROW_SIZE 		256  	//user flash row size in bytes 
#define PM_PAGE_SIZE 		2048 	//user flash page size in bytes




/*******************************************************************************
  Prototypes                                                                   
*******************************************************************************/

/*******************************************************************************
  Function:
    void WritePM(WORD length, DWORD_VAL address, BYTE * buffer)
    
  Description:
    
  PreCondition:

  Parameters:
                               
  Returns:

  Remarks: 
*******************************************************************************/
void WritePM(WORD length, DWORD_VAL address, BYTE * buffer);

/*******************************************************************************
  Function:
    void ReadPM(WORD length, DWORD_VAL address, BYTE * buffer)
    
  Description:
    
  PreCondition:

  Parameters:
                               
  Returns:

  Remarks: 
*******************************************************************************/
void ReadPM(WORD length, DWORD_VAL address, BYTE * buffer);

/*******************************************************************************
  Function:
    void ErasePM(WORD length, DWORD_VAL address)
    
  Description:
    
  PreCondition:

  Parameters:
                               
  Returns:

  Remarks: 
*******************************************************************************/
void ErasePM(WORD length, DWORD_VAL address);

#endif

