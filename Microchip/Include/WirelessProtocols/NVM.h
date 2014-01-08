/********************************************************************
* FileName:		NVM.h
* Dependencies: none
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher
*               Microchip C32 v1.02 or higher	
* Company:		Microchip Technology, Inc.
*
* Copyright and Disclaimer Notice
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
*  Defines interface to access EUI EEPROM
*
* Change History:
*  Rev   Date         Author    Description
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  3.1   5/28/2010    yfy       MiWi DE 3.1
********************************************************************/
#ifndef __EEPROM_H
    #define __EEPROM_H
    
    #include "SystemProfile.h"
    
    #if defined(ENABLE_NVM)
        #include "SystemProfile.h"
        #include "GenericTypeDefs.h"
        #include "Compiler.h"
        #include "WirelessProtocols/MCHP_API.h"
       
        /******************************************************************************/
        // there are three ways to use NVM to store data: External EPROM, Data EEPROM 
        // and programming space. Definition should be found in HardwareProfile.h
        /******************************************************************************/
        //#define USE_EXTERNAL_EEPROM
        //#define USE_DATA_EEPROM
        //#define USE_PROGRAMMING_SPACE
        
        #if defined(USE_EXTERNAL_EEPROM)
        
            #define SPI_WRT_STATUS  0x01
            #define SPI_WRITE       0x02
            #define SPI_READ        0x03
            #define SPI_DIS_WRT     0x04
            #define SPI_RD_STATUS   0x05
            #define SPI_EN_WRT      0x06
    
            #define EEPROM_MAC_ADDR 0xFA
            
            #define MCHP_1KBIT              1       
            #define MCHP_2KBIT              2
            #define MCHP_4KBIT              3
            #define MCHP_8KBIT              4
            #define MCHP_16KBIT             5
            #define MCHP_32KBIT             6
            #define MCHP_64KBIT             7
            #define MCHP_128KBIT            8
            #define MCHP_256KBIT            9
            #define MCHP_512KBIT            10
            #define MCHP_1MBIT              11
    
            #define MCHP_EEPROM             MCHP_256KBIT
            
            
            #if MCHP_EEPROM == 0
                #error MCHP_EEPROM is not defined
            #elif MCHP_EEPROM < MCHP_32KBIT
                #define NVM_PAGE_SIZE   16
            #elif MCHP_EEPROM < MCHP_128KBIT
                #define NVM_PAGE_SIZE   32
            #elif MCHP_EEPROM < MCHP_512KBIT
                #define NVM_PAGE_SIZE   64
            #elif MCHP_EEPROM < MCHP_1MBIT
                #define NVM_PAGE_SIZE   128
            #elif MCHP_EEPROM == MCHP_1MBIT
                #error Microchip 1MBit EEPROM is not supported at this time. User need to modify the EEPROM access function to make it work. The address must be 3 bytes.
            #else
                #error Invalid MCHP EEPROM part
            #endif

            #if MCHP_EEPROM == MCHP_1KBIT
                #define TOTAL_NVM_BYTES 128
            #elif MCHP_EEPROM == MCHP_2KBIT
                #define TOTAL_NVM_BYTES 256
            #elif MCHP_EEPROM == MCHP_4KBIT
                #define TOTAL_NVM_BYTES 512
            #elif MCHP_EEPROM == MCHP_8KBIT
                #define TOTAL_NVM_BYTES 1024
            #elif MCHP_EEPROM == MCHP_16KBIT
                #define TOTAL_NVM_BYTES 2048
            #elif MCHP_EEPROM == MCHP_32KBIT
                #define TOTAL_NVM_BYTES 4096
            #elif MCHP_EEPROM == MCHP_64KBIT
                #define TOTAL_NVM_BYTES 8192
            #elif MCHP_EEPROM == MCHP_128KBIT
                #define TOTAL_NVM_BYTES 16384
            #elif MCHP_EEPROM == MCHP_256KBIT
                #define TOTAL_NVM_BYTES 32768
            #elif MCHP_EEPROM == MCHP_512KBIT
                #define TOTAL_NVM_BYTES 65535
            #elif MCHP_EEPROM == MCHP_1MBIT
                #error Microchip 1MBit EEPROM is not supported at this time. User need to modify the EEPROM access function to make it work. The address must be 3 bytes.
            #else
                #error MCHP_EEPROM is not defined
            #endif            

        #endif
        
        #if defined(USE_PROGRAMMING_SPACE)
            #define ERASE_BLOCK_SIZE    64
            #define WRITE_BLOCK_SIZE    64
            
            #define NVMRead(dest, addr, count) memcpypgm2ram(dest, addr, count)
            void NVMWrite(BYTE *source, ROM BYTE *addr, WORD count);
            
            #define NVMInit()   Nop()
        #endif
        
        #if defined(USE_DATA_EEPROM)
            #define TOTAL_NVM_BYTES     1024
        #endif
        
        #if defined(USE_DATA_EEPROM) || defined(USE_EXTERNAL_EEPROM)
        
            extern WORD        nvmMyPANID;
            extern WORD        nvmCurrentChannel;
            extern WORD        nvmConnMode;
            extern WORD        nvmConnectionTable;
            extern WORD        nvmOutFrameCounter;
                            
            #if defined(PROTOCOL_MIWI)
                extern WORD        nvmMyShortAddress;
                extern WORD        nvmMyParent;
                
                #ifdef NWK_ROLE_COORDINATOR
                    extern WORD    nvmRoutingTable;
                    extern WORD    nvmKnownCoordinators;
                    extern WORD    nvmRole;
                #endif
            #endif
        
            #if defined(PROTOCOL_MIWI_PRO)
                extern WORD        nvmMyShortAddress;
                extern WORD        nvmMyParent;
                
                #ifdef NWK_ROLE_COORDINATOR
                    extern WORD    nvmRoutingTable;
                    extern WORD    nvmFamilyTree;
                    extern WORD    nvmNeighborRoutingTable;
                    extern WORD    nvmRole;
                #endif
            #endif
        
            void NVMRead(BYTE *dest, WORD addr, WORD count);
            void NVMWrite(BYTE *source, WORD addr, WORD count);
            
            BOOL NVMInit(void);

            #define nvmGetMyPANID( x )                  NVMRead( (BYTE *)x, nvmMyPANID, 2)
            #define nvmPutMyPANID( x )                  NVMWrite((BYTE *)x, nvmMyPANID, 2)
            
            #define nvmGetCurrentChannel( x )           NVMRead( (BYTE *)x, nvmCurrentChannel, 1)
            #define nvmPutCurrentChannel( x )           NVMWrite((BYTE *)x, nvmCurrentChannel, 1)
            
            #define nvmGetConnMode( x )                 NVMRead( (BYTE *)x, nvmConnMode, 1)
            #define nvmPutConnMode( x )                 NVMWrite((BYTE *)x, nvmConnMode, 1)
            
            #define nvmGetConnectionTable( x )          NVMRead( (BYTE *)x, nvmConnectionTable, (WORD)CONNECTION_SIZE * sizeof(CONNECTION_ENTRY))
            #define nvmPutConnectionTable( x )          NVMWrite((BYTE *)x, nvmConnectionTable, (WORD)CONNECTION_SIZE * sizeof(CONNECTION_ENTRY))
            #define nvmPutConnectionTableIndex(x, y)    NVMWrite((BYTE *)x, nvmConnectionTable+((WORD)y * sizeof(CONNECTION_ENTRY)), sizeof(CONNECTION_ENTRY))
            
            #define nvmGetOutFrameCounter( x )          NVMRead( (BYTE *)x, nvmOutFrameCounter, 4)
            #define nvmPutOutFrameCounter( x )          NVMWrite((BYTE *)x, nvmOutFrameCounter, 4)
            
            #if defined(PROTOCOL_MIWI)

                #define nvmGetMyShortAddress( x )       NVMRead( (BYTE *)x, nvmMyShortAddress, 2)
                #define nvmPutMyShortAddress( x )       NVMWrite((BYTE *)x, nvmMyShortAddress, 2)
                
                #define nvmGetMyParent( x )             NVMRead( (BYTE *)x, nvmMyParent, 1)
                #define nvmPutMyParent( x )             NVMWrite((BYTE *)x, nvmMyParent, 1)
                
                #if defined(NWK_ROLE_COORDINATOR)
                    
                    #define nvmGetRoutingTable( x )         NVMRead( (BYTE *)x, nvmRoutingTable, 8)
                    #define nvmPutRoutingTable( x )         NVMWrite((BYTE *)x, nvmRoutingTable, 8)
                    
                    #define nvmGetKnownCoordinators( x )    NVMRead( (BYTE *)x, nvmKnownCoordinators, 1)
                    #define nvmPutKnownCoordinators( x )    NVMWrite((BYTE *)x, nvmKnownCoordinators, 1)
                    
                    #define nvmGetRole( x )                 NVMRead( (BYTE *)x, nvmRole, 1)
                    #define nvmPutRole( x )                 NVMWrite((BYTE *)x, nvmRole, 1)
                
                #endif
                
            #endif
            
            #if defined(PROTOCOL_MIWI_PRO)

                #define nvmGetMyShortAddress( x )       NVMRead( (BYTE *)x, nvmMyShortAddress, 2)
                #define nvmPutMyShortAddress( x )       NVMWrite((BYTE *)x, nvmMyShortAddress, 2)
                
                #define nvmGetMyParent( x )             NVMRead( (BYTE *)x, nvmMyParent, 1)
                #define nvmPutMyParent( x )             NVMWrite((BYTE *)x, nvmMyParent, 1)
                
                #if defined(NWK_ROLE_COORDINATOR)
                    
                    #define nvmGetRoutingTable( x )         NVMRead( (BYTE *)x, nvmRoutingTable, NUM_COORDINATOR/8)
                    #define nvmPutRoutingTable( x )         NVMWrite((BYTE *)x, nvmRoutingTable, NUM_COORDINATOR/8)
                    
                    #define nvmGetNeighborRoutingTable( x ) NVMRead( (BYTE *)x, nvmNeighborRoutingTable, ((WORD)NUM_COORDINATOR/8) * ((WORD)NUM_COORDINATOR))
                    #define nvmPutNeighborRoutingTable( x ) NVMWrite((BYTE *)x, nvmNeighborRoutingTable, ((WORD)NUM_COORDINATOR/8) * ((WORD)NUM_COORDINATOR))

                    #define nvmGetFamilyTree( x )           NVMRead( (BYTE *)x, nvmFamilyTree, NUM_COORDINATOR)
                    #define nvmPutFamilyTree( x )           NVMWrite((BYTE *)x, nvmFamilyTree, NUM_COORDINATOR)
                    
                    #define nvmGetRole( x )                 NVMRead( (BYTE *)x, nvmRole, 1)
                    #define nvmPutRole( x )                 NVMWrite((BYTE *)x, nvmRole, 1)
                
                #endif
                
            #endif    
            
      
        #else   
        
            extern ROM WORD_VAL            nvmMyShortAddress;
            extern ROM WORD_VAL            nvmMyPANID;
            extern ROM BYTE                nvmCurrentChannel;
            extern ROM BYTE                nvmConnMode;
            extern ROM CONNECTION_ENTRY    nvmConnectionTable[CONNECTION_SIZE];
            extern ROM DWORD_VAL           nvmOutFrameCounter;
            
            #if defined(PROTOCOL_MIWI_PRO)
                extern ROM BYTE            nvmMyParent;
                
                #if defined(NWK_ROLE_COORDINATOR)
                    extern ROM BYTE     nvmRole;
                    extern ROM BYTE     nvmNeighborRoutingTable[NUM_COORDINATOR][NUM_COORDINATOR/8];
                    extern ROM BYTE     nvmRoutingTable[NUM_COORDINATOR/8];
                    extern ROM BYTE     nvmFamilyTree[NUM_COORDINATOR];
                #endif
            #endif
        
            #if defined(PROTOCOL_MIWI)
                extern ROM BYTE            nvmMyParent;
                
                #if defined(NWK_ROLE_COORDINATOR)
                    extern ROM BYTE        nvmRoutingTable[8];
                    extern ROM BYTE        nvmKnownCoordinators;
                    extern ROM BYTE        nvmRole;
                #endif
            #endif
        
            
            #define nvmGetMyPANID( x )                  NVMRead( (BYTE *)x, (far ROM void *)&nvmMyPANID, 2)
            #define nvmPutMyPANID( x )                  NVMWrite((BYTE *)x, (ROM BYTE *)&nvmMyPANID, 2)
            
            #define nvmGetCurrentChannel( x )           NVMRead( (BYTE *)x, (far ROM void *)&nvmCurrentChannel, 1)
            #define nvmPutCurrentChannel( x )           NVMWrite((BYTE *)x, (ROM BYTE *)&nvmCurrentChannel, 1)
            
            #define nvmGetConnMode( x )                 NVMRead( (BYTE *)x, (far ROM void *)&nvmConnMode, 1)
            #define nvmPutConnMode( x )                 NVMWrite((BYTE *)x, (ROM BYTE *)&nvmConnMode, 1)
            
            #define nvmGetConnectionTable( x )          NVMRead( (BYTE *)x, (far ROM void *)&nvmConnectionTable, (WORD)CONNECTION_SIZE * sizeof(CONNECTION_ENTRY))
            #define nvmPutConnectionTable( x )          NVMWrite((BYTE *)x, (ROM BYTE *)&nvmConnectionTable, (WORD)CONNECTION_SIZE * sizeof(CONNECTION_ENTRY))
            #define nvmPutConnectionTableIndex(x, y)    NVMWrite((BYTE *)x, (ROM BYTE *)&(nvmConnectionTable[y]), sizeof(CONNECTION_ENTRY))
            
            #define nvmGetOutFrameCounter( x )          NVMRead( (BYTE *)x, (far ROM void *)&nvmOutFrameCounter, 4)
            #define nvmPutOutFrameCounter( x )          NVMWrite((BYTE *)x, (ROM BYTE *)&nvmOutFrameCounter, 4);
            
            #if defined(PROTOCOL_MIWI)

                #define nvmGetMyShortAddress( x )       NVMRead( (BYTE *)x, (far ROM void *)&nvmMyShortAddress, sizeof(WORD_VAL))
                #define nvmPutMyShortAddress( x )       NVMWrite((BYTE *)x, (ROM BYTE *)&nvmMyShortAddress, sizeof(WORD_VAL))
                
                #define nvmGetMyParent( x )             NVMRead( (BYTE *)x, (far ROM void *)&nvmMyParent, 1)
                #define nvmPutMyParent( x )             NVMWrite((BYTE *)x, (ROM BYTE *)&nvmMyParent, 1)
                
                #if defined(NWK_ROLE_COORDINATOR)
                    
                    #define nvmGetRoutingTable( x )         NVMRead( (BYTE *)x, (far ROM void *)&nvmRoutingTable, 8)
                    #define nvmPutRoutingTable( x )         NVMWrite((BYTE *)x, (ROM BYTE *)&nvmRoutingTable, 8)
                    
                    #define nvmGetKnownCoordinators( x )    NVMRead( (BYTE *)x, (far ROM void *)&nvmKnownCoordinators, 1)
                    #define nvmPutKnownCoordinators( x )    NVMWrite((BYTE *)x, (ROM BYTE *)&nvmKnownCoordinators, 1)
                    
                    #define nvmGetRole( x )                 NVMRead( (BYTE *)x, (far ROM void *)&nvmRole, 1)
                    #define nvmPutRole( x )                 NVMWrite((BYTE *)x, (ROM BYTE *)&nvmRole, 1)
                
                #endif
                
            #endif
            
            
            #if defined(PROTOCOL_MIWI_PRO)

                #define nvmGetMyShortAddress( x )       NVMRead( (BYTE *)x, (far ROM void *)&nvmMyShortAddress, sizeof(WORD_VAL))
                #define nvmPutMyShortAddress( x )       NVMWrite((BYTE *)x, (ROM BYTE *)&nvmMyShortAddress, sizeof(WORD_VAL))
                
                #define nvmGetMyParent( x )             NVMRead( (BYTE *)x, (far ROM void *)&nvmMyParent, 1)
                #define nvmPutMyParent( x )             NVMWrite((BYTE *)x, (ROM BYTE *)&nvmMyParent, 1)
                
                #if defined(NWK_ROLE_COORDINATOR)
                    
                    #define nvmGetRoutingTable( x )         NVMRead( (BYTE *)x, (far ROM void *)&nvmRoutingTable, NUM_COORDINATOR)
                    #define nvmPutRoutingTable( x )         NVMWrite((BYTE *)x, (ROM BYTE *)&nvmRoutingTable, NUM_COORDINATOR)
                    
                    #define nvmGetNeighborRoutingTable( x ) NVMRead((BYTE *)x, (far ROM void *)&nvmNeighborRoutingTable, ((WORD)NUM_COORDINATOR/8 * (WORD)NUM_COORDINATOR))
                    #define nvmPutNeighborRoutingTable( x ) NVMWrite((BYTE *)x, (ROM BYTE *)&nvmNeighborRoutingTable, ((WORD)NUM_COORDINATOR/8 * (WORD)NUM_COORDINATOR))

                    #define nvmGetFamilyTree( x )           NVMRead((BYTE *)x, (far ROM void *)&nvmFamilyTree, NUM_COORDINATOR)
                    #define nvmPutFamilyTree( x )           NVMWrite((BYTE *)x, (ROM BYTE *)&nvmFamilyTree, NUM_COORDINATOR)
            
                    #define nvmGetRole( x )                 NVMRead( (BYTE *)x, (far ROM void *)&nvmRole, 1)
                    #define nvmPutRole( x )                 NVMWrite((BYTE *)x, (ROM BYTE *)&nvmRole, 1)
                
                #endif
                
            #endif
            
            
        #endif
        
        
    #endif
#endif
