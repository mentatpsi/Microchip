/********************************************************************
* FileName:		security.c
* Dependencies: None
* Processor:	PIC18, PIC24, PIC32, dsPIC30, dsPIC33
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
*  This file implement eXtended Tiny Encryption Algorithm (XTEA) security 
*  engine and security mode wrappers that are built around XTEA. XTEA is
*  64bit block cipher with 128bit symmetric key. XTEA-64, the modified 
*  XTEA engine with 32bit block and 64bit symmetric key is also implemented
*  here to be freely distributed according to US export control regulation.
*  The security modes CTR, CBC-MAC and CCM are implemented around both XTEA 
*  engines. 
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/15/2009    yfy       Initial revision
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  2.1   6/20/2009    yfy       Add LCD support
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#include "SystemProfile.h"
#include "Transceivers/Transceivers.h"

#if defined(SOFTWARE_SECURITY) && defined(ENABLE_SECURITY)

    #include "GenericTypeDefs.h"
    #include "Transceivers/Security.h"                                
    #include "WirelessProtocols/Console.h"
    
    BYTE tmpBlock[BLOCK_SIZE];
        
    #if defined(XTEA_128)
		 /**************************************************************************
		  * To follow US export control regulation, this free download version 
		  * implements XTEA with 32bit block size and 64bit key length. To acquire 
		  * the XTEA engine with 64bit block size and 128bit key length, customer
		  * needs to authorize through MicrochipDirect web site or any other Microchip 
		  * agent.
		  ***************************************************************************/
    
    #elif defined(XTEA_64)
        #if defined(__18CXX)
            //#pragma romdata securityKey = 0x2E
        #endif
            ROM const unsigned char mySecurityKey[8] = {SECURITY_KEY_00, SECURITY_KEY_01, SECURITY_KEY_02,    // The 16-byte security key used in the
                SECURITY_KEY_03, SECURITY_KEY_04, SECURITY_KEY_05, SECURITY_KEY_06, SECURITY_KEY_07};   // security module.                                                              
        #if defined(__18CXX)
            //#pragma romdata
        #endif
        
        /*********************************************************************
         * void encode(INPUT WORD *text, INPUT WORD *key)
         *
         * Overview:        This function apply XTEA-64 security engine to
         *                  the input data buffer with input security key. 
         *                  The encoded data will replace the input data
         *
         * PreCondition:    None
         *
         * Input:       
         *          WORD *      data        The input buffer to the XTEA engine. The 
         *                                  encoded data will replace the original 
         *                                  content after the function call
         *          WORD *      key         The security key for the XTEA engine
         * Output:          
         *          None
         *
         * Side Effects:    None
         * 
         ********************************************************************/
        void encode(WORD *text, WORD *key)
        {
            WORD part1=text[0], part2=text[1];
            WORD sum=0, delta=0x9E37;
            BYTE i;
            
            for(i=0; i<XTEA_ROUND; i++) 
            {
                part1 += (((part2 << 4) ^ (part2 >> 5)) + part2) ^ (sum + key[sum & 3]);
                sum += delta;
                part2 += (((part1 << 4) ^ (part1 >> 5)) + part1) ^ (sum + key[(sum>>11) & 3]);
            }
            text[0]=part1; text[1]=part2;
        }
    #endif
    
    /*********************************************************************
     * void CTR(BYTE *text, 
     *          BYTE len, 
     *          BYTE *key, 
     *          BYTE *nounce)
     *
     * Overview:        This function implement Counter (CTR) mode of security 
     *                  engine to the input text. Counter mode is used to 
     *                  encrypt the data.
     *
     * PreCondition:    None
     *
     * Input:       
     *          BYTE *      text        The text to be encrypted. The encrypted
     *                                  data will replace the original content
     *                                  after this function call.
     *          BYTE        len         The length of the text to be encrypted
     *          BYTE *      key         The security key for the XTEA engine
     *          BYTE *      nounce      The nounce to be used in the CTR mode
     * Output:          
     *          None
     *
     * Side Effects:    None
     * 
     ********************************************************************/
    void CTR(BYTE *text, BYTE len, BYTE *key, BYTE *nounce)
    {
        BYTE block = len/BLOCK_SIZE+1;
        BYTE i, j;
        #if defined(__18CXX)
            BYTE ITStatus = INTCONbits.GIEH;
        
            INTCONbits.GIEH = 0;
        #endif

        for(i = 0; i < block; i++)
        {
            for(j = 0; j < BLOCK_SIZE-1; j++)
            {
                tmpBlock[j] = nounce[j];
            }
            tmpBlock[BLOCK_SIZE-1] = i;
            encode((BLOCK_UNIT *)tmpBlock, (BLOCK_UNIT *)key);
            for(j = 0; j < BLOCK_SIZE; j++)
            {
                if( (i * BLOCK_SIZE + j) >= len )
                {
                    break;
                }
                text[i * BLOCK_SIZE + j] ^= tmpBlock[j];
            }
        }

        #if defined(__18CXX)
            INTCONbits.GIEH = ITStatus;
        #endif
    }
        
    
    /*********************************************************************
     * void CBC_MAC(   BYTE *text, 
     *                 BYTE len, 
     *                 BYTE *key, 
     *                 BYTE *MIC)
     *
     * Overview:        This function implements CBC-MAC mode of security 
     *                  engine to the input text. CBC-MAC mode generates
     *                  Message Interity Code (MIC) to authenticate the 
     *                  input text.
     *
     * PreCondition:    None
     *
     * Input:       
     *          BYTE *      text        The text to be authenticated
     *          BYTE        len         The length of the input text
     *          BYTE *      key         The security key for the XTEA engine
     * Output:          
     *          BYTE *      MIC         The generated MIC from the CBC-MAC mode
     *
     * Side Effects:    None
     * 
     ********************************************************************/    
    void CBC_MAC(   BYTE *text, 
                    BYTE len, 
                    BYTE *key, 
                    BYTE *MIC)
    {
        BYTE block = len / BLOCK_SIZE + 1;
        BYTE i, j;
        #if defined(__18CXX)
            BYTE ITStatus = INTCONbits.GIEH;
        
            INTCONbits.GIEH = 0;
        #endif
        
        for(i = 0; i < BLOCK_SIZE; i++)
        {
            MIC[i] = 0;
        }
        
        for(i = 0; i < block; i++)
        {
            for(j = 0; j < BLOCK_SIZE; j++)
            {
                if( (i * BLOCK_SIZE + j) >= len )
                {
                    break;
                }
                MIC[j] ^= text[i * BLOCK_SIZE + j];    
            }
            encode((BLOCK_UNIT *)MIC, (BLOCK_UNIT *)key);
        }  
        #if defined(__18CXX)
            INTCONbits.GIEH = ITStatus;
        #endif  
    }
    


    /*********************************************************************
     * void CCM_Enc(    BYTE *text, 
     *                  BYTE headerLen, 
     *                  BYTE payloadLen, 
     *                  BYTE *key)
     *
     * Overview:        This function implements CCM mode of security 
     *                  engine to the input text. CCM mode ensures data
     *                  interity as well as secrecy. This function is used
     *                  to encode the data
     *
     * PreCondition:    None
     *
     * Input:       
     *          BYTE *      text        The text to be encrypted. The encrypted
     *                                  data will replace the original content
     *                                  after this function call.
     *          BYTE *      headerLen   The header length, used to authenticate, but
     *                                  not encrypted
     *          BYTE        payloadLen  The length of the text to be authenticated
     *                                  and encrypted
     *          BYTE *      key         The security key for the XTEA engine
     * Output:          
     *          None
     *
     * Side Effects:    None
     * 
     ********************************************************************/ 
    void CCM_Enc(   BYTE *text, 
                    BYTE headerLen, 
                    BYTE payloadLen, 
                    BYTE *key)
    {
        BYTE i;
        #if defined(__18CXX)
            BYTE ITStatus = INTCONbits.GIEH;
        
            INTCONbits.GIEH = 0;
        #endif
        
        CBC_MAC(text, (headerLen + payloadLen), key, tmpBlock);   
        
        for(i = 0; i < BLOCK_SIZE; i++)
        {
            text[headerLen + payloadLen + i] = tmpBlock[i];
        }
        
        for(i = 0; i < BLOCK_SIZE-1; i++)
        {
            tmpBlock[i] = (i < headerLen) ? text[i] : 0;
        }
    
        CTR(&(text[headerLen]), (payloadLen + BLOCK_SIZE), key, tmpBlock);    
        #if defined(__18CXX)
            INTCONbits.GIEH = ITStatus;
        #endif  
    }
    
    
    /*********************************************************************
     * void CCM_Dec(    BYTE *text, 
     *                  BYTE headerLen, 
     *                  BYTE payloadLen, 
     *                  BYTE *key)
     *
     * Overview:        This function implements CCM mode of security 
     *                  engine to the input text. CCM mode ensures data
     *                  interity as well as secrecy. This function is used
     *                  to decode the data
     *
     * PreCondition:    None
     *
     * Input:       
     *          BYTE *      text        The text to be encrypted. The decrypted
     *                                  data will replace the original content
     *                                  after this function call.
     *          BYTE *      headerLen   The header length, used to authenticate, but
     *                                  not decrypted
     *          BYTE        payloadLen  The length of the text to be authenticated
     *                                  and decrypted
     *          BYTE *      key         The security key for the XTEA engine
     * Output:          
     *          None
     *
     * Side Effects:    None
     * 
     ********************************************************************/ 
    BOOL CCM_Dec(BYTE *text, BYTE headerLen, BYTE payloadLen, BYTE *key)
    {
        BYTE i;
        #if defined(__18CXX)
            BYTE ITStatus = INTCONbits.GIEH;
        
            INTCONbits.GIEH = 0;
        #endif

        for(i = 0; i < BLOCK_SIZE-1; i++)
        {
            tmpBlock[i] = (i < headerLen) ? text[i] : 0;
        }
        CTR(&(text[headerLen]), payloadLen, key, tmpBlock);

        CBC_MAC(text, (headerLen + payloadLen - SEC_MIC_LEN), key, tmpBlock);
        for(i = 0; i < SEC_MIC_LEN; i++)
        {
            if( tmpBlock[i] != text[headerLen + payloadLen - SEC_MIC_LEN + i] )
            {
                #if defined(__18CXX)
                    INTCONbits.GIEH = ITStatus;
                #endif
                return FALSE;
            }       
        }
        #if defined(__18CXX)
            INTCONbits.GIEH = ITStatus;
        #endif  
        return TRUE;
    }

#endif

extern char bogus;

