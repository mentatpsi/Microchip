/********************************************************************
* FileName:		Security.h
* Dependencies: none
* Processor:	PIC18, PIC24F, PIC32, dsPIC30, dsPIC33
*               tested with 18F4620, dsPIC33FJ256GP710	
* Hardware:		PICDEM Z, Explorer 16
* Complier:     Microchip C18 v3.04 or higher
*				Microchip C30 v2.03 or higher	
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
*  Defines configurations and interfaces for security module
*
* Change History:
*  Rev   Date         Author    Description
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#ifndef __SECURITY_H
    #define __SECURITY_H

    #include "Transceivers/Transceivers.h"

    #if defined(SOFTWARE_SECURITY)
    
        //#define XTEA_128
        #define XTEA_64
        
        #define XTEA_ROUND  32

        #define SEC_LEVEL_CTR           0
        #define SEC_LEVEL_CBC_MAC_16    1
        #define SEC_LEVEL_CBC_MAC_32    2
        #define SEC_LEVEL_CBC_MAC_64    3
        #define SEC_LEVEL_CCM_16        4
        #define SEC_LEVEL_CCM_32        5
        #define SEC_LEVEL_CCM_64        6

        #if defined(XTEA_128)
            #define BLOCK_SIZE 8
            #define BLOCK_UNIT DWORD
            #define KEY_SIZE 16
        #elif defined(XTEA_64)
            #define BLOCK_SIZE 4
            #define BLOCK_UNIT WORD
            #define KEY_SIZE 8
        #endif
        
        #if SECURITY_LEVEL == SEC_LEVEL_CTR
            #define SEC_MIC_LEN     0
        #elif SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_16
            #define SEC_MIC_LEN     2
        #elif SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_32
            #define SEC_MIC_LEN     4
        #elif SECURITY_LEVEL == SEC_LEVEL_CBC_MAC_64
            #define SEC_MIC_LEN     8
        #elif SECURITY_LEVEL == SEC_LEVEL_CCM_16
            #define SEC_MIC_LEN     2
        #elif SECURITY_LEVEL == SEC_LEVEL_CCM_32
            #define SEC_MIC_LEN     4
        #elif SECURITY_LEVEL == SEC_LEVEL_CCM_64
            #define SEC_MIC_LEN     8
        #endif
        
        
        extern ROM const unsigned char mySecurityKey[];

        void CTR(BYTE *text, BYTE len, BYTE *key, BYTE *nounce);
        void CBC_MAC(BYTE *text, BYTE len, BYTE *key, BYTE *MIC);
        void CCM_Enc(BYTE *text, BYTE headerLen, BYTE payloadLen, BYTE *key);
        BOOL CCM_Dec(BYTE *text, BYTE headerLen, BYTE payloadLen, BYTE *key);
    
    #endif

#endif

