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

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "boot.h"
#include "Compiler.h"
#include "MDD File System/FSIO.h"
#include "flash_memory.h"
#include "oled.h"
#include <stdio.h>
#include <string.h>


//******************************************************************************
//******************************************************************************
// Global Data
//******************************************************************************
//******************************************************************************

// Buffer for reading image file data
BYTE            ReadBuffer[BL_READ_BUFFER_SIZE];

//******************************************************************************
//******************************************************************************
// Boot Loader Media Interface Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    BOOL BLMedia_InitializeTransport ( void )

  Description:
    This routine is called to initalize the media transport layer used to 
    access the boot medium.

  Precondition:
    None

  Parameters:
    None

  Returns:
    TRUE    - If able to initialize the boot medium's transport layer
    FALSE   - If not able to initialize the boot medium's transport layer

  Remarks:
    This routine can be implemented to inialize any desired boot medium.
***************************************************************************/

// Implemented as a macro 


/****************************************************************************
  Function:
    void BLMedia_DeinitializeTransport ( void )

  Description:
    This routine disables the media transport layer that was initialized by
    BLMedia_InitializeTransport.


  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    This routine must disable and clear any interrupts that were enabled
    by BLMedia_InitializeTransport.  Any stray pending interrupts (once the
    application boots) may cause the system to trigger an un-handled (or 
    incorrectly handled) interrupt.
***************************************************************************/

// Implemented as a macro 


/****************************************************************************
  Function:
    BOOL BLMedia_MediumAttached ( void )

  Description:
    This routine determines if the boot medium is currently attached to the
    system.

  Precondition:
    BLMedia_InitializeTransport must have been called and the medium muust
    be maintained as needed.

  Parameters:
    None

  Returns:
    TRUE    - If the boot medium is currently attached and accessible
    FALSE   - If the boot medium is not currently attached

  Remarks:
    None
***************************************************************************/

// Implemented as a macro 


/****************************************************************************
  Function:
    BOOL BLMedia_LoadFile (  char *file_name )

  Description:
    Accesses the boot image file, reads it over the transport, and programs
    it to Flash.

  Precondition:
    The boot medium and file system must have been initialized and
    maintained as needed.

  Parameters:
    file_name - Pointer to a null-terminated character string giving the 
                name of the application's image file.

  Returns:
    TRUE      - If a valid application image was read from the thumb drive
                and programmed into Flash at the APPLICATION_ADDRESS.
    FALSE     - If the application image was not valid or if unable to 
                program a valid application to Flash for any reason.

  Remarks:
    This routine calls the loader layer to translate and program the boot
    image file.
    
    This routine can be modified to account for differences in how the medium
    and file format must be processed.
  ***************************************************************************/
#define AsciiToHexByte(m,l) ( (AsciiToHexNibble(m) << 4 ) | AsciiToHexNibble(l) )
unsigned char AsciiToHexNibble(unsigned char data);
BYTE recordData[16];
BYTE recordDataOdd;
BOOL oddPacketDetected = FALSE;

/*
typedef enum
{
    START_CODE,
    BYTE_COUNT_B1,
    BYTE_COUNT_B2,
    ADDRESS_B1,
    ADDRESS_B2,
    ADDRESS_B3,
    ADDRESS_B4,
} RECORD_STATE;
*/
typedef enum
{
    RECORD_TYPE_DATA_RECORD = 0x00,
    RECORD_TYPE_EOF = 0x01,
    RECORD_TYPE_EXTENDED_ADDRESS = 0x04
} RECORD_TYPE;

BOOL BLMedia_LoadFile (  char *file_name )
{
    FSFILE         *fp;             // File pointer
    size_t          nBuffer;        // Number of bytes in the read buffer
    size_t          iBuffer;        // Index into the read buffer
    unsigned int    nRemaining;     // Number of bytes remaining to decode
    unsigned int    Result;         // Result code from "GetFlashBlock" operation

    WORD_VAL        byteCountASCII;
    DWORD_VAL       addressASCII;
    DWORD_VAL       extendedAddressASCII;
    WORD_VAL        recordTypeASCII;
    WORD_VAL        checksumASCII;
    WORD_VAL        dataByteASCII;
    DWORD_VAL       totalAddress;

    BYTE_VAL        byteCount;
    WORD_VAL        address;
    WORD_VAL        extendedAddress;
    BYTE_VAL        recordType;
    BYTE_VAL        checksum;
    BYTE_VAL        dataByte;

    BYTE            recordDataCounter;
    BYTE            byteEvenVsOdd;

    WORD            byteCountCopy;
    WORD*           pData;
    BYTE            i;

	BYTE			mode;

	mode = 0x72;	//"r"

    // Attempt to open the file
    if ( (fp=FSfopen((const char*)file_name, (const char*)&mode)) == NULL )
    {
        BLIO_ReportBootStatus(BL_FS_FILE_ERR, "BL: Media Error - Unable to open file\r\n" );
        return FALSE;
    }
    else
    {
        for(totalAddress.Val=PROGRAM_FLASH_BASE;totalAddress.Val<(PROGRAM_FLASH_END);totalAddress.Val+=FLASH_BLOCK_SIZE)
        {
			TBLPTRL = totalAddress.byte.LB;
			TBLPTRH = totalAddress.byte.HB;
			TBLPTRU = totalAddress.byte.UB;

			EECON1 = 0b00010100;
		
			INTCONbits.GIE = 0;	//Make certain interrupts disabled for unlock process.
			_asm
			MOVLW 0x55
			MOVWF EECON2, 0
			MOVLW 0xAA
			MOVWF EECON2, 0
			BSF EECON1, 1, 0
			_endasm
		
		    //Good practice now to clear the WREN bit, as further protection against any
			//	 future accidental activation of self write/erase operations.
			EECON1bits.WREN = 0;
        }

        // Read the file and program it to Flash
        iBuffer     = 0;
        nRemaining  = 0;
        nBuffer     = 0;

        while(1)
        {
            if(nRemaining == 0)
            {
                nRemaining = FSfread(&ReadBuffer[0], 1, BL_READ_BUFFER_SIZE, fp );
		        if(nRemaining == 0)
                {   
                    //unable to read data from the file
                    FSfclose( fp );
                    return FALSE;
                }
                iBuffer = 0;
            }

            switch(nBuffer)
            {
                case 0: //start code
                    if(ReadBuffer[iBuffer] != ':')
                    {
                        //ignore line feeds and line returns
                        if((ReadBuffer[iBuffer] != 0x0D) || (ReadBuffer[iBuffer] != 0x0A))
                        {
                            iBuffer++; 
                            nRemaining--;
                            //end of line of the last line
                            continue;
                        }
                        else
                        {
                            FSfclose( fp );
                            return FALSE;
                        }
                    }    
                    iBuffer++;        
                    break;
                case 1: //byte count byte 1
                    byteCountASCII.v[1] = ReadBuffer[iBuffer++];
                    break;
                case 2: //byte count byte 2
                    byteCountASCII.v[0] = ReadBuffer[iBuffer++];
                    byteCount.Val = AsciiToHexByte(byteCountASCII.v[1],byteCountASCII.v[0]);
                    byteCountCopy = byteCount.Val;
                    byteEvenVsOdd = 0;
                    recordDataCounter = 0;
                    break;
                case 3: //address byte 1
                    addressASCII.v[3] = ReadBuffer[iBuffer++];
                    break;
                case 4: //address byte 2
                    addressASCII.v[2] = ReadBuffer[iBuffer++];
                    break;
                case 5: //address byte 3
                    addressASCII.v[1] = ReadBuffer[iBuffer++];
                    break;
                case 6: //address byte 4
                    addressASCII.v[0] = ReadBuffer[iBuffer++];
                    address.v[0] = AsciiToHexByte(addressASCII.v[1],addressASCII.v[0]);
                    address.v[1] = AsciiToHexByte(addressASCII.v[3],addressASCII.v[2]);
                    break;
                case 7: //record type byte 1
                    recordTypeASCII.v[1] = ReadBuffer[iBuffer++];
                    break;
                case 8: //record type byte 2
                    recordTypeASCII.v[0] = ReadBuffer[iBuffer++];
                    recordType.Val = AsciiToHexByte(recordTypeASCII.v[1],recordTypeASCII.v[0]);
                    break;
                case 9: //data
                    if(byteCountCopy != 0)
                    {
                        if(byteEvenVsOdd == 0)
                        {
                            dataByteASCII.v[1] = ReadBuffer[iBuffer++];
                            byteEvenVsOdd = 1;
                        }
                        else
                        {
                            dataByteASCII.v[0] = ReadBuffer[iBuffer++];
                            dataByte.Val = AsciiToHexByte(dataByteASCII.v[1],dataByteASCII.v[0]);
                            recordData[recordDataCounter++] = dataByte.Val;
                            byteCountCopy--;
                            byteEvenVsOdd = 0;
                        }
                        break;
                    }
                    else
                    {
                        nBuffer = 10;
                    }
                case 10: //checksum byte 1
                    checksumASCII.v[1] = ReadBuffer[iBuffer++];
                    break;
                case 11: //checksum byte 2
                    checksumASCII.v[0] = ReadBuffer[iBuffer++];
                    checksum.Val = AsciiToHexByte(checksumASCII.v[1],checksumASCII.v[0]);

                    switch(recordType.Val)
                    {
                        case RECORD_TYPE_DATA_RECORD:
                            //check the address here
                            totalAddress.word.HW = extendedAddress.Val;
                            totalAddress.word.LW = address.Val;

                            if(totalAddress.Val < PROGRAM_FLASH_BASE)
                            {
                                //invalid address - below base - don't program the requested address
                                break;
                            }

                            if(totalAddress.Val >= PROGRAM_FLASH_END)
                            {
                                break;
                            }
			        
                            {
                                unsigned int i;

                                i = 0;

                                if(totalAddress.Val & 0x01)
                                {
                                    totalAddress.Val--;
    
    								TBLPTRL = totalAddress.byte.LB;
    								TBLPTRH = totalAddress.byte.HB;
    								TBLPTRU = totalAddress.byte.UB;
    
                					_asm
                					tblrd
       								tblwtpostinc
    								_endasm
    								TABLAT = recordData[i++];
    								_asm
    								tblwt					//Do not increment TBLPTR on the second write.  See datasheet.
    								_endasm					
    								
    								EECON1 = 0b00100100;	//Word programming mode
    								INTCONbits.GIE = 0;		//Make certain interrupts disabled for unlock process.
    								_asm
    								MOVLW 0x55
    								MOVWF EECON2, 0
    								MOVLW 0xAA
    								MOVWF EECON2, 0
    								BSF EECON1, 1, 0		//Initiates write operation (halts CPU execution until complete)
    								_endasm		
    
    							    //Good practice now to clear the WREN bit, as further protection against any
    								//	 future accidental activation of self write/erase operations.
    								EECON1bits.WREN = 0;
    
    								totalAddress.Val = totalAddress.Val + 2;
    
                                    byteCount.Val--;
                                }
    
    							while(byteCount.Val > 1)
    					        {
    								TBLPTRL = totalAddress.byte.LB;
    								TBLPTRH = totalAddress.byte.HB;
    								TBLPTRU = totalAddress.byte.UB;
    
    								TABLAT = recordData[i++];
                					_asm
       								tblwtpostinc
    								_endasm
    
    								TABLAT = recordData[i++];
    								_asm
    								tblwt					//Do not increment TBLPTR on the second write.  See datasheet.
    								_endasm					
    								
    								EECON1 = 0b00100100;	//Word programming mode
    								INTCONbits.GIE = 0;		//Make certain interrupts disabled for unlock process.
    								_asm
    								MOVLW 0x55
    								MOVWF EECON2, 0
    								MOVLW 0xAA
    								MOVWF EECON2, 0
    								BSF EECON1, 1, 0		//Initiates write operation (halts CPU execution until complete)
    								_endasm		
    
    							    //Good practice now to clear the WREN bit, as further protection against any
    								//	 future accidental activation of self write/erase operations.
    								EECON1bits.WREN = 0;
    
    								totalAddress.Val += 2;
    
                                    byteCount.Val -= 2;
                                }       
    
                                if(byteCount.Val == 1)
                                {
    								TBLPTRL = totalAddress.byte.LB;
    								TBLPTRH = totalAddress.byte.HB;
    								TBLPTRU = totalAddress.byte.UB;
    
    								TABLAT = recordData[i++];
                					_asm
       								tblwtpostinc
    								_endasm
    
                					_asm
                					tblrd
    								tblwt					//Do not increment TBLPTR on the second write.  See datasheet.
    								_endasm					
    								
    								EECON1 = 0b00100100;	//Word programming mode
    								INTCONbits.GIE = 0;		//Make certain interrupts disabled for unlock process.
    								_asm
    								MOVLW 0x55
    								MOVWF EECON2, 0
    								MOVLW 0xAA
    								MOVWF EECON2, 0
    								BSF EECON1, 1, 0		//Initiates write operation (halts CPU execution until complete)
    								_endasm		
    
    							    //Good practice now to clear the WREN bit, as further protection against any
    								//	 future accidental activation of self write/erase operations.
    								EECON1bits.WREN = 0;
    
    								totalAddress.Val += 2;
    
                                    byteCount.Val--;
                                }   
                            }       

                            break;
                        case RECORD_TYPE_EOF:
                            FSfclose( fp );
                            return TRUE;
                            break;
                        case RECORD_TYPE_EXTENDED_ADDRESS:
                            extendedAddress.v[1] = recordData[0];
                            extendedAddress.v[0] = recordData[1];
                            break;
                    }
                    break;
                default:
                    FSfclose( fp );
                    return FALSE;
            }

            if(nBuffer != 9)
            {
                nBuffer++;
            }

            if(nBuffer == 12)
            {
                nBuffer = 0;
            }

            nRemaining--;
        }

        FSfclose( fp );
    }
    return TRUE;

} // BLMedia_LoadFile

/*
*******************************************************************************
EOF
*******************************************************************************
*/

