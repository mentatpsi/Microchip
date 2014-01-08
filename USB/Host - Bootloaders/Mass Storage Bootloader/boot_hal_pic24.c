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

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "boot.h"

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
void EraseAllApplicationMemory(void)
{
    DWORD_VAL totalAddress;

    for(totalAddress.Val=PROGRAM_FLASH_BASE;(totalAddress.Val/1)<((unsigned long)PROGRAM_FLASH_BASE+(unsigned long)PROGRAM_FLASH_LENGTH);totalAddress.Val+=(FLASH_BLOCK_SIZE))
    {
        NVMCON = 0x4042;				//Erase page on next WR
    
        TBLPAG = totalAddress.word.HW;
    	__builtin_tblwtl(totalAddress.word.LW, 0xFFFF);
            
        asm("DISI #16");					//Disable interrupts for next few instructions for unlock sequence
        __builtin_write_NVM();
        while(NVMCONbits.WR == 1){}
    }
}

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
BOOL ProgramHexRecord(RECORD_STRUCT* record, WORD extended_vector)
{
    WORD*           pData;
    DWORD_VAL       totalAddress;

    //Generic loop index.  Needs to be a WORD since a record could be 255 bytes
    //  and in this case written 4 bytes at a time.  Thus the counts for that loop
    //  would be 252 and 256.  Since 256 can't be represented in a byte, a loop
    //  counter of a byte could only count up to 252 bytes.
    WORD            i;

    totalAddress.word.HW = extended_vector;
    totalAddress.word.LW = record->LoadOffset;
    totalAddress.Val = totalAddress.Val / 2;

    pData = (WORD*)&record->data[0];

    NVMCON = 0x4003;		//Perform WORD write next time WR gets set = 1.

    TBLPAG = totalAddress.word.HW;

    if((record->RecordLength%4)!=0)
    {
        //not word aligned data
        BLIO_ReportBootStatus(LOADER_ALIGNMENT, "BL: Data record was not 32-bit word aligned.\r\n" );
        return FALSE;
    }

    //Program the data
    for(i=0;i<record->RecordLength;i+=4)
    {                 
	    TBLPAG = totalAddress.word.HW;

                //Write the low word to the latch
	    __builtin_tblwtl(totalAddress.word.LW, *pData++);	
        //Write the high word to the latch (8 bits of 
        //  data + 8 bits of "phantom data")	
	    __builtin_tblwth(totalAddress.word.LW, *pData++);	
        
        totalAddress.Val+=2;

        //Disable interrupts for next few instructions for unlock sequence
    	asm("DISI #16");					
    	__builtin_write_NVM();
        while(NVMCONbits.WR == 1){}
    }
    //Good practice to clear WREN bit anytime we are not
    //  expecting to do erase/write operations, further 
    //  reducing probability of accidental activation.
	NVMCONbits.WREN = 0;		

    //verify that the contents were programmed correctly
    pData = (WORD*)&record->data[0];
    totalAddress.Val-=(record->RecordLength/2);

    for(i=0;i<record->RecordLength;i+=4)
    {       
        TBLPAG = totalAddress.word.HW;
             
	    if(__builtin_tblrdl(totalAddress.word.LW) != *pData++)
        {
            //data in flash doesn't match expected value,
            //  close file and bail.
            BLIO_ReportBootStatus(LOADER_FLASH_VERIFY_ERR, "BL: Data in flash doesn't match expected value.\r\n" );
            return FALSE;
        }

	    if(__builtin_tblrdh(totalAddress.word.LW) != *pData++)
        {
            //data in flash doesn't match expected value,
            //  close file and bail.
            BLIO_ReportBootStatus(LOADER_FLASH_VERIFY_ERR, "BL: Data in flash doesn't match expected value.\r\n" );
            return FALSE;
        }
        
        totalAddress.Val+=2;
    }

    //Everything programmed correctly
    return TRUE;
}
