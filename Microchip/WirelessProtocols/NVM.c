/********************************************************************
* FileName:		NVM.c
* Dependencies: NVM.h
* Processor:	PIC18, PIC24, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright © 2007-2010 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy and distribute 
* Software only when embedded on a Microchip microcontroller or digital 
* signal controller and used with a Microchip radio frequency transceiver, 
* which are integrated into your product or third party product (pursuant 
* to the terms in the accompanying license agreement).  
*
* You should refer to the license agreement accompanying this Software for 
* additional information regarding your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
* KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
* WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
* PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
* LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
* CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
* DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
* ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
* LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
* TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
* NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*
*********************************************************************
* File Description:
*
*   This file access MAC EEPROM through SPI interface
*
* Change History:
*  Rev   Date         Author    Description
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#include "SystemProfile.h"

#if defined(ENABLE_NVM)

    #include "WirelessProtocols/NVM.h"
    #include "WirelessProtocols/MSPI.h"
    #include "WirelessProtocols/Console.h"
    #include "ConfigApp.h"
    
    extern void MacroNop(void);
            
    #if defined(USE_EXTERNAL_EEPROM) || defined(USE_DATA_EEPROM)
    
        WORD        nvmMyPANID;
        WORD        nvmCurrentChannel;
        WORD        nvmConnMode;
        WORD        nvmConnectionTable;
        WORD        nvmOutFrameCounter;
      
        #if defined(PROTOCOL_MIWI)
            WORD        nvmMyShortAddress;
            WORD        nvmMyParent;
            
            #ifdef NWK_ROLE_COORDINATOR
                WORD    nvmRoutingTable;
                WORD    nvmKnownCoordinators;
                WORD    nvmRole;
            #endif
        #endif
        
        #if defined(PROTOCOL_MIWI_PRO)
            WORD        nvmMyShortAddress;
            WORD        nvmMyParent;
            
            #ifdef NWK_ROLE_COORDINATOR
                WORD    nvmRoutingTable;
                WORD    nvmNeighborRoutingTable;
                WORD    nvmFamilyTree;
                WORD    nvmRole;
            #endif
        #endif
        
    #else
        
        ROM BYTE filler[ERASE_BLOCK_SIZE] = {0x00};
        
        ROM WORD_VAL            nvmMyPANID;
        ROM BYTE                nvmCurrentChannel;
        ROM BYTE                nvmConnMode;
        ROM CONNECTION_ENTRY    nvmConnectionTable[CONNECTION_SIZE];
        ROM DWORD_VAL           nvmOutFrameCounter;               
        
        #if defined(PROTOCOL_MIWI)
            
            ROM WORD_VAL        nvmMyShortAddress;
            ROM BYTE            nvmMyParent;
            
            #if defined(NWK_ROLE_COORDINATOR)
                ROM BYTE        nvmRoutingTable[8];
                ROM BYTE        nvmKnownCoordinators;
                ROM BYTE        nvmRole;
            #endif

        #endif
        
        #if defined(PROTOCOL_MIWI_PRO)
            
            ROM WORD_VAL        nvmMyShortAddress;
            ROM BYTE            nvmMyParent;
            
            #if defined(NWK_ROLE_COORDINATOR)
                ROM BYTE        nvmRoutingTable[NUM_COORDINATOR/8];
                ROM BYTE        nvmNeighborRoutingTable[NUM_COORDINATOR][NUM_COORDINATOR/8];
                ROM BYTE        nvmFamilyTree[NUM_COORDINATOR];
                ROM BYTE        nvmRole;
            #endif
            
        #endif
        
        ROM BYTE filler2[ERASE_BLOCK_SIZE] = {0x00};
    #endif
    
    #if defined(USE_EXTERNAL_EEPROM)
        #if defined(EEPROM_SHARE_SPI)
        
            #define EESPIPut( x )       SPIPut( x )
            #define EESPIGet()          SPIGet()
            
        #else
            void SPIPut2(BYTE);
            BYTE SPIGet2(void);
            
            #define EESPIPut( x )       SPIPut2( x )
            #define EESPIGet()          SPIGet2()
  
        #endif
    #endif
    
    
    #if defined(USE_EXTERNAL_EEPROM)
        
        /*********************************************************************
        * Function:         void NVMRead(BYTE *dest, BYTE addr, BYTE count)
        *
        * PreCondition:     SPI port has been initialized
        *
        * Input:		    dest -  pointer to the buffer to hold the read data
        *                   addr -  starting address for the read
        *                   count - total number of bytes to be read
        *
        * Output:		    none
        *
        * Side Effects:	    none
        *
        * Overview:		    This function will read count byte of data starting
        *                   from address addr, and store the read data to the
        *                   buffer starting from *dest.
        *
        * Note:			    None
        ********************************************************************/        
    	void NVMRead(BYTE *dest, WORD addr, WORD count)
        {
            #if defined(__18CXX)
                BYTE oldGIEH = INTCONbits.GIEH;
                
                INTCONbits.GIEH = 0;
            #else
                BYTE oldRFIE = RFIE;
                
                RFIE = 0;
            #endif
            
            EE_nCS = 0;
            
            #if MCHP_EEPROM < MCHP_4KBIT
                EESPIPut(SPI_READ);
                EESPIPut(addr);
            #elif MCHP_EEPROM == MCHP_4KBIT
                if( addr > 0xFF )
                {
                    EESPIPut(SPI_READ | 0x08);
                }
                else
                {
                    EESPIPut(SPI_READ);
                }
                EESPIPut(addr);
            #elif MCHP_EEPROM < MCHP_1MBIT
                EESPIPut(SPI_READ);
                EESPIPut(addr>>8);
                EESPIPut(addr);
            #endif
            
        	while( count > 0 )
            {
                *dest++ = EESPIGet();
                count--;
            }
            EE_nCS = 1;
            
            #if defined(__18CXX)
                INTCONbits.GIEH = oldGIEH;
            #else
                RFIE = oldRFIE;
            #endif
        }

    	void NVMWrite(BYTE *source, WORD addr, WORD count)
        {
            BYTE PageCounter = 0;
            //BYTE i;
            #if defined(__18CXX)
                BYTE oldGIEH = INTCONbits.GIEH;   
                INTCONbits.GIEH = 0;
            #else
                BYTE oldRFIE = RFIE;
                RFIE = 0;
            #endif
           
            
EEPROM_NEXT_PAGE:
            do
            {
                EE_nCS = 0;
                EESPIPut(SPI_RD_STATUS);
                PageCounter = EESPIGet();
                EE_nCS = 1;
                MacroNop();
            } while(PageCounter & 0x01 );
    
            EE_nCS = 0;
            EESPIPut(SPI_EN_WRT);
            EE_nCS = 1;
            MacroNop();
            EE_nCS = 0;
            #if MCHP_EEPROM < MCHP_4KBIT
                EESPIPut(SPI_WRITE);
                EESPIPut(addr);
            #elif MCHP_EEPROM == MCHP_4KBIT
                if( addr > 0xFF )
                {
                    EESPIPut(SPI_WRITE | 0x08);
                }
                else
                {
                    EESPIPut(SPI_WRITE);
                }
                EESPIPut(addr);
            #elif MCHP_EEPROM < MCHP_1MBIT
                EESPIPut(SPI_WRITE);
                EESPIPut(addr>>8);
                EESPIPut(addr);
            #endif
            PageCounter = 0;
        	while( count > 0 )
            {
                EESPIPut(*source++);
                count--;
                PageCounter++;
                if( ((addr + PageCounter) & (NVM_PAGE_SIZE-1)) == 0 )
                {
                    EE_nCS = 1;
                    addr += PageCounter;
                    goto EEPROM_NEXT_PAGE;
                }
            }
            EE_nCS = 1;
            
            #if defined(__18CXX)
                INTCONbits.GIEH = oldGIEH;
            #else
                RFIE = oldRFIE;
            #endif
            
        }
    #endif
 
 
    #if defined(USE_PROGRAMMING_SPACE)
    	void NVMWrite(BYTE *src, ROM BYTE* dest, WORD count)
        {
            ROM char *pEraseBlock;
            static BYTE memBlock[ERASE_BLOCK_SIZE];
            BYTE *pMemBlock;
            BYTE writeIndex;
            BYTE writeStart;
            BYTE writeCount;
            BYTE oldGIEH;
            DWORD oldTBLPTR;
        
            #if defined(VERIFY_WRITE)
                while( memcmppgm2ram( src, (MEM_MODEL ROM void *)dest, count))
            #elif defined(CHECK_BEFORE_WRITE)
                if (memcmppgm2ram( src, (MEM_MODEL ROM void *)dest, count ))
            #endif
            {
                // First of all get nearest "left" erase block boundary
                pEraseBlock = (ROM char*)((long)dest & (long)(~(ERASE_BLOCK_SIZE-1)));
                writeStart = (BYTE)((BYTE)dest & (BYTE)(ERASE_BLOCK_SIZE-1));

                while( count )
                {
                    // Now read the entire erase block size into RAM.
                    NVMRead(memBlock, (far ROM void*)pEraseBlock, ERASE_BLOCK_SIZE);
                                
                    // Erase the block.
                    // Erase flash memory, enable write control.
                    EECON1 = 0x94;
                    
                    oldGIEH = INTCONbits.GIEH;
                    INTCONbits.GIEH = 0;
                    
                    #if defined(__18CXX) 
                        TBLPTR = (unsigned short long)pEraseBlock;
                    #endif

                    EECON2 = 0x55;
                    EECON2 = 0xaa;
                    EECON1bits.WR = 1;
                    MacroNop();
        
                    EECON1bits.WREN = 0;
        
                    oldTBLPTR = TBLPTR;

                    INTCONbits.GIEH = oldGIEH;
        
                    // Modify 64-byte block of RAM buffer as per what is required.
                    pMemBlock = &memBlock[writeStart];
                    while( writeStart < ERASE_BLOCK_SIZE && count )
                    {
                        *pMemBlock++ = *src++;
        
                        count--;
                        writeStart++;
                    }
        
                    // After first block write, next start would start from 0.
                    writeStart = 0;
        
                    // Now write entire 64 byte block in one write block at a time.
                    writeIndex = ERASE_BLOCK_SIZE / WRITE_BLOCK_SIZE;
                    pMemBlock = memBlock;
                    while( writeIndex )
                    {
        
                        oldGIEH = INTCONbits.GIEH;
                        INTCONbits.GIEH = 0;
        
                        TBLPTR = oldTBLPTR;
        
                        // Load individual block
                        writeCount = WRITE_BLOCK_SIZE;
                        while( writeCount-- )
                        {
                            TABLAT = *pMemBlock++;
        
                            //TBLWTPOSTINC();
                            _asm tblwtpostinc _endasm
                        }
        
                        // Start the write process: reposition tblptr back into memory block that we want to write to.
                        #if defined(__18CXX) 
                            _asm tblrdpostdec _endasm
                        #endif
        
                        // Write flash memory, enable write control.
                        EECON1 = 0x84;
        
                        EECON2 = 0x55;
                        EECON2 = 0xaa;
                        EECON1bits.WR = 1;
                        MacroNop();
                        EECON1bits.WREN = 0;
        
                        // One less block to write
                        writeIndex--;
        
                        TBLPTR++;
        
                        oldTBLPTR = TBLPTR;

                        INTCONbits.GIEH = oldGIEH;
                    }
        
                    // Go back and do it all over again until we write all
                    // data bytes - this time the next block.
                    #if !defined(WIN32)
                        pEraseBlock += ERASE_BLOCK_SIZE;
                    #endif
                   
                }
            }
        }       
    #endif 
 
    #if defined(USE_DATA_EEPROM)
    
    	void NVMRead(BYTE *dest, WORD addr, WORD count)
        {
            while( count )
            {
                EEADRH = addr >> 8;
                EEADR = addr;
                EECON1bits.EEPGD = 0;
                EECON1bits.CFGS = 0;
                EECON1bits.RD = 1;   
                MacroNop();
                *dest++ = EEDATA;
                count--;
                addr++;
            }            
        }
        
    	void NVMWrite(BYTE *source, WORD addr, WORD count)
        {
            BYTE oldGIEH;
            
            while(count)
            {   
                EEADRH = addr >> 8;
                EEADR = addr;
                EEDATA = *source++;   
                EECON1bits.EEPGD = 0;
                EECON1bits.CFGS = 0;
                EECON1bits.WREN = 1;
                oldGIEH = INTCONbits.GIEH;
                INTCONbits.GIEH = 0; 
                EECON2 = 0x55;
                EECON2 = 0xAA;
                EECON1bits.WR = 1;
                INTCONbits.GIEH = oldGIEH;
                while(EECON1bits.WR) ;
                EECON1bits.WREN = 0;
                count--;
                addr++;
            }
        }
    #endif
 
    
    #if defined(USE_DATA_EEPROM) || defined(USE_EXTERNAL_EEPROM)
        
    	WORD nextEEPosition;
        BOOL NVMalloc(WORD size, WORD *location)
        {
            //WORD retval;
    
            if ((nextEEPosition + size) > TOTAL_NVM_BYTES)
            {
                return FALSE;
            }
    
            *location = nextEEPosition;
            nextEEPosition += size;
            return TRUE;
        }
        
    
        BOOL NVMInit(void)
        {
            BOOL result = TRUE;
            
            nextEEPosition = 0;
            
            result &= NVMalloc(2, &nvmMyPANID);
            result &= NVMalloc(1, &nvmCurrentChannel);
            result &= NVMalloc(1, &nvmConnMode);
            result &= NVMalloc(sizeof(CONNECTION_ENTRY) * CONNECTION_SIZE, &nvmConnectionTable);
            result &= NVMalloc(4, &nvmOutFrameCounter);
            
            #if defined(PROTOCOL_MIWI)

                result &= NVMalloc(2, &nvmMyShortAddress);
                result &= NVMalloc(1, &nvmMyParent);
                
                #if defined(NWK_ROLE_COORDINATOR)
                    result &= NVMalloc(8, &nvmRoutingTable);
                    result &= NVMalloc(1, &nvmKnownCoordinators);
                    result &= NVMalloc(1, &nvmRole);
                #endif
            #endif
            
            #if defined(PROTOCOL_MIWI_PRO)

                result &= NVMalloc(2, &nvmMyShortAddress);
                result &= NVMalloc(1, &nvmMyParent);
                
                #if defined(NWK_ROLE_COORDINATOR)
                    result &= NVMalloc((NUM_COORDINATOR/8), &nvmRoutingTable);
                    result &= NVMalloc(((WORD)NUM_COORDINATOR/8*(WORD)NUM_COORDINATOR), &nvmNeighborRoutingTable);
                    result &= NVMalloc(NUM_COORDINATOR, &nvmFamilyTree);
                    result &= NVMalloc(1, &nvmRole);
                #endif
            #endif
            
            return result;
        }
        
    #endif 
 
 
 
 
#else
    extern char bogusVar;
    
#endif   
