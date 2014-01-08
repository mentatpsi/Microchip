/********************************************************************
* FileName:		MRF24J40.h
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
*  Defines all of the address and channel settings for the registers
*   in the MRF24J40
*
* Change History:
*  Rev   Date         Author    Description
*  0.1   11/09/2006   yfy       Initial revision
*  1.0   01/09/2007   yfy       Initial release
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#if !defined(_ZMRF24J40_H_) && defined(MRF24J40)
    #define _ZMRF24J40_H_
        
    #include "GenericTypeDefs.h"
    #include "SystemProfile.h"
    #include "Transceivers/Transceivers.h"
    #include "Transceivers/MCHP_MAC.h"    
    #include "Transceivers/MRF24J40/ConfigMRF24J40.h"


    #define SEC_LEVEL_CBC_MAC_32    7
    #define SEC_LEVEL_CBC_MAC_64    6
    #define SEC_LEVEL_CBC_MAC_128   5
    #define SEC_LEVEL_CCM_32        4
    #define SEC_LEVEL_CCM_64        3
    #define SEC_LEVEL_CCM_128       2
    #define SEC_LEVEL_CTR           1
        
    #if defined(PROTOCOL_MIWI) 
        #define PROTOCOL_HEADER_SIZE MIWI_HEADER_LEN
    #endif
    
    #if defined(PROTOCOL_MIWI_PRO)
        #define PROTOCOL_HEADER_SIZE MIWI_PRO_HEADER_LEN
    #endif
    
    #if defined(PROTOCOL_P2P)
        #define PROTOCOL_HEADER_SIZE 0
    #endif
    
    #if defined(ENABLE_SECURITY)
        
        #if SECURITY_LEVEL == 0x01
            #define MIC_SIZE 0
        #elif (SECURITY_LEVEL == 0x02) || (SECURITY_LEVEL == 0x05)
            #define MIC_SIZE 16
        #elif (SECURITY_LEVEL == 0x03) || (SECURITY_LEVEL == 0x06)
            #define MIC_SIZE 8
        #elif (SECURITY_LEVEL == 0x04) || (SECURITY_LEVEL == 0x07)
            #define MIC_SIZE 4
        #endif

        #define RX_PACKET_SIZE (RX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+MIC_SIZE+17)

        #if (RX_PACKET_SIZE > 127) && !defined(__18CXX)
            #warning  "Maximum application payload RX BUFFER SIZE is 94"
        #endif

    #else

        #define RX_PACKET_SIZE (RX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+12)
        
        #if (RX_PACKET_SIZE > 127) && !defined(__18CXX)
            #warning "Maximum application payload RX BUFFER SIZE is 99"
        #endif
    
    #endif    

    #if RX_PACKET_SIZE > 127
        #undef RX_PACKET_SIZE
        #define RX_PACKET_SIZE 127
    #endif

    //long address registers
    #define RFCTRL0 (0x200)
    #define RFCTRL1 (0x201)
    #define RFCTRL2 (0x202)
    #define RFCTRL3 (0x203)
    #define RFCTRL4 (0x204)
    #define RFCTRL5 (0x205)
    #define RFCTRL6 (0x206)
    #define RFCTRL7 (0x207)
    #define RFCTRL8 (0x208)
    #define CAL1 (0x209)
    #define CAL2 (0x20a)
    #define CAL3 (0x20b)
    #define SFCNTRH (0x20c)
    #define SFCNTRM (0x20d)
    #define SFCNTRL (0x20e)
    #define RFSTATE (0x20f)
    #define RSSI (0x210)
    #define CLKIRQCR (0x211)
    #define SRCADRMODE (0x212)
    #define SRCADDR0 (0x213)
    #define SRCADDR1 (0x214)
    #define SRCADDR2 (0x215)
    #define SRCADDR3 (0x216)
    #define SRCADDR4 (0x217)
    #define SRCADDR5 (0x218)
    #define SRCADDR6 (0x219)
    #define SRCADDR7 (0x21a)
    #define RXFRAMESTATE (0x21b)
    #define SECSTATUS (0x21c)
    #define STCCMP (0x21d)
    #define HLEN (0x21e)
    #define FLEN (0x21f)
    #define SCLKDIV (0x220)
    //#define reserved (0x221)
    #define WAKETIMEL (0x222)
    #define WAKETIMEH (0x223)
    #define TXREMCNTL (0x224)
    #define TXREMCNTH (0x225)
    #define TXMAINCNTL (0x226)
    #define TXMAINCNTM (0x227)
    #define TXMAINCNTH0 (0x228)
    #define TXMAINCNTH1 (0x229)
    #define RFMANUALCTRLEN (0x22a)
    #define RFMANUALCTRL (0x22b)
    #define RFRXCTRL RFMANUALCTRL
    #define TxDACMANUALCTRL (0x22c)
    #define RFMANUALCTRL2 (0x22d)
    #define TESTRSSI (0x22e)
    #define TESTMODE (0x22f)
    
    #define NORMAL_TX_FIFO  (0x000)
    #define BEACON_TX_FIFO  (0x080)
    #define GTS1_TX_FIFO    (0x100)
    #define GTS2_TX_FIFO    (0x180)
    
    #define RX_FIFO         (0x300)
    
    #define SECURITY_FIFO   (0x280)
    
    
    
    
    
    //short address registers for reading
    #define READ_RXMCR (0x00)
    #define READ_PANIDL (0x02)
    #define READ_PANIDH (0x04)
    #define READ_SADRL (0x06)
    #define READ_SADRH (0x08)
    #define READ_EADR0 (0x0A)
    #define READ_EADR1 (0x0C)
    #define READ_EADR2 (0x0E)
    #define READ_EADR3 (0x10)
    #define READ_EADR4 (0x12)
    #define READ_EADR5 (0x14)
    #define READ_EADR6 (0x16)
    #define READ_EADR7 (0x18)
    #define READ_RXFLUSH (0x1a)
    #define READ_TXSTATE0 (0x1c)
    #define READ_TXSTATE1 (0x1e)
    #define READ_ORDER (0x20)
    #define READ_TXMCR (0x22)
    #define READ_ACKTMOUT (0x24)
    #define READ_SLALLOC (0x26)
    #define READ_SYMTICKL (0x28)
    #define READ_SYMTICKH (0x2A)
    #define READ_PAONTIME (0x2C)
    #define READ_PAONSETUP (0x2E)
    #define READ_FFOEN (0x30)
    #define READ_CSMACR (0x32)
    #define READ_TXBCNTRIG (0x34)
    #define READ_TXNMTRIG (0x36)
    #define READ_TXG1TRIG (0x38)
    #define READ_TXG2TRIG (0x3A)
    #define READ_ESLOTG23 (0x3C)
    #define READ_ESLOTG45 (0x3E)
    #define READ_ESLOTG67 (0x40)
    #define READ_TXPEND (0x42)
    #define READ_TXBCNINTL (0x44)
    #define READ_FRMOFFSET (0x46)
    #define READ_TXSR (0x48)
    #define READ_TXLERR (0x4A)
    #define READ_GATE_CLK (0x4C)
    #define READ_TXOFFSET (0x4E)
    #define READ_HSYMTMR0 (0x50)
    #define READ_HSYMTMR1 (0x52)
    #define READ_SOFTRST (0x54)
    #define READ_BISTCR (0x56)
    #define READ_SECCR0 (0x58)
    #define READ_SECCR1 (0x5A)
    #define READ_TXPEMISP (0x5C)
    #define READ_SECISR (0x5E)
    #define READ_RXSR (0x60)
    #define READ_ISRSTS (0x62)
    #define READ_INTMSK (0x64)
    #define READ_GPIO (0x66)
    #define READ_GPIODIR (0x68)
    #define READ_SLPACK (0x6A)
    #define READ_RFCTL (0x6C)
    #define READ_SECCR2 (0x6E)
    #define READ_BBREG0 (0x70)
    #define READ_BBREG1 (0x72)
    #define READ_BBREG2 (0x74)
    #define READ_BBREG3 (0x76)
    #define READ_BBREG4 (0x78)
    #define READ_BBREG5 (0x7A)
    #define READ_BBREG6 (0x7C)
    #define READ_RSSITHCCA (0x7E)
    
    //short address registers for writing
    //short address registers for reading
    #define WRITE_RXMCR (0x01)
    #define WRITE_PANIDL (0x03)
    #define WRITE_PANIDH (0x05)
    #define WRITE_SADRL (0x07)
    #define WRITE_SADRH (0x09)
    #define WRITE_EADR0 (0x0B)
    #define WRITE_EADR1 (0x0D)
    #define WRITE_EADR2 (0x0F)
    #define WRITE_EADR3 (0x11)
    #define WRITE_EADR4 (0x13)
    #define WRITE_EADR5 (0x15)
    #define WRITE_EADR6 (0x17)
    #define WRITE_EADR7 (0x19)
    #define WRITE_RXFLUSH (0x1B)
    #define WRITE_TXSTATE0 (0x1D)
    #define WRITE_TXSTATE1 (0x1F)
    #define WRITE_ORDER (0x21)
    #define WRITE_TXMCR (0x23)
    #define WRITE_ACKTMOUT (0x25)
    #define WRITE_SLALLOC (0x27)
    #define WRITE_SYMTICKL (0x29)
    #define WRITE_SYMTICKH (0x2B)
    #define WRITE_PAONTIME (0x2D)
    #define WRITE_PAONSETUP (0x2F)
    #define WRITE_FFOEN (0x31)
    #define WRITE_CSMACR (0x33)
    #define WRITE_TXBCNTRIG (0x35)
    #define WRITE_TXNMTRIG (0x37)
    #define WRITE_TXG1TRIG (0x39)
    #define WRITE_TXG2TRIG (0x3B)
    #define WRITE_ESLOTG23 (0x3D)
    #define WRITE_ESLOTG45 (0x3F)
    #define WRITE_ESLOTG67 (0x41)
    #define WRITE_TXPEND (0x43)
    #define WRITE_TXBCNINTL (0x45)
    #define WRITE_FRMOFFSET (0x47)
    #define WRITE_TXSR (0x49)
    #define WRITE_TXLERR (0x4B)
    #define WRITE_GATE_CLK (0x4D)
    #define WRITE_TXOFFSET (0x4F)
    #define WRITE_HSYMTMR0 (0x51)
    #define WRITE_HSYMTMR1 (0x53)
    #define WRITE_SOFTRST (0x55)
    #define WRITE_BISTCR (0x57)
    #define WRITE_SECCR0 (0x59)
    #define WRITE_SECCR1 (0x5B)
    #define WRITE_TXPEMISP (0x5D)
    #define WRITE_SECISR (0x5F)
    #define WRITE_RXSR (0x61)
    #define WRITE_ISRSTS (0x63)
    #define WRITE_INTMSK (0x65)
    #define WRITE_GPIO (0x67)
    #define WRITE_GPIODIR (0x69)
    #define WRITE_SLPACK (0x6B)
    #define WRITE_RFCTL (0x6D)
    #define WRITE_SECCR2 (0x6F)
    #define WRITE_BBREG0 (0x71)
    #define WRITE_BBREG1 (0x73)
    #define WRITE_BBREG2 (0x75)
    #define WRITE_BBREG3 (0x77)
    #define WRITE_BBREG4 (0x79)
    #define WRITE_BBREG5 (0x7B)
    #define WRITE_BBREG6 (0x7D)
    #define WRITE_RSSITHCCA (0x7F)
    
    #define CHANNEL_11 0x00
    #define CHANNEL_12 0x10
    #define CHANNEL_13 0x20
    #define CHANNEL_14 0x30
    #define CHANNEL_15 0x40
    #define CHANNEL_16 0x50
    #define CHANNEL_17 0x60
    #define CHANNEL_18 0x70
    #define CHANNEL_19 0x80
    #define CHANNEL_20 0x90
    #define CHANNEL_21 0xa0
    #define CHANNEL_22 0xb0
    #define CHANNEL_23 0xc0
    #define CHANNEL_24 0xd0
    #define CHANNEL_25 0xe0
    #define CHANNEL_26 0xf0
    
    #if defined(ENABLE_PA_LNA) && (defined(MRF24J40MB) || defined(MRF24J40MC))
        #define FULL_CHANNEL_MAP        0x03FFF800
    #else
        #define FULL_CHANNEL_MAP        0x07FFF800
    #endif
    
    
    
    typedef union 
    {
        BYTE Val;               // value of interrupts
        struct 
        {
            BYTE RF_TXIF :1;    // transmission finish interrupt
            BYTE :2;
            BYTE RF_RXIF :1;    // receiving a packet interrupt
            BYTE SECIF :1;      // receiving a secured packet interrupt
            BYTE :4;
        }bits;                  // bit map of interrupts
    } MRF24J40_IFREG;
    
    typedef union
    {
        BYTE Val;
        struct
        {
            BYTE        TX_BUSY             : 1;
            BYTE        TX_PENDING_ACK      : 1;
            BYTE        TX_FAIL             : 1;
            BYTE        RX_SECURITY         : 1;
            BYTE        RX_IGNORE_SECURITY  : 1;
            BYTE        RX_BUFFERED         : 1;
        } bits;
    } MRF24J40_STATUS;

#endif

