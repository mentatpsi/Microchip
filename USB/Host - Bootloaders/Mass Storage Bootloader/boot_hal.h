/*
********************************************************************************
                                                                                
Software License Agreement                                                      
                                                                                
Copyright © 2010 Microchip Technology Inc. and its licensors.  All         
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

/****************************************************************************
  Function:
    void EraseAllApplicationMemory(void)

  Description:
    This routine will erase the entire application memory space.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None
***************************************************************************/
void EraseAllApplicationMemory(void);

/****************************************************************************
  Function:
    BOOL ProgramHexRecord(RECORD_STRUCT* record, WORD extended_vector)

  Description:
    This routine will program and verify a hex file data record into program memory

  Precondition:
    The data record must contain data that is word aligned to the processor
    specified program word size.

  Parameters:
    record - pointer to the data record
    extended_vector - the value of the extended vector address.

  Returns:
    TRUE    - If the data was programmed and verified correctly
    FALSE   - If the data in the record was not formatted correctly or fails to 
              verify correctly.
***************************************************************************/
BOOL ProgramHexRecord(RECORD_STRUCT* record, WORD extended_vector);
