/********************************************************************
* FileName:		MRF49XA.h
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
*  Defines configurations and the address, channel settings for the 
*   MRF49XA
*
* Change History:
*  Rev   Date         Author    Description
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

#if !defined(__MRF49XA_H)
    #define __MRF49XA_H
    
    #include "SystemProfile.h"
    
    #if defined(MRF49XA)
        #include "GenericTypeDefs.h"
        #include "Compiler.h"
        #include "WirelessProtocols/SymbolTime.h"
        #include "Transceivers/MCHP_MAC.h"
        #include "Transceivers/MRF49XA/ConfigMRF49XA.h"
        #include "Transceivers/Security.h"
        #include "TimeDelay.h"

        // following should be in the def file
        #define XTAL_LD_CAP_85					0x0000
        #define XTAL_LD_CAP_9					0x0001
        #define XTAL_LD_CAP_95					0x0002
        #define XTAL_LD_CAP_10					0x0003
        #define XTAL_LD_CAP_105					0x0004
        #define XTAL_LD_CAP_11					0x0005
        #define XTAL_LD_CAP_115					0x0006
        #define XTAL_LD_CAP_12					0x0007
        #define XTAL_LD_CAP_125					0x0008
        #define XTAL_LD_CAP_13					0x0009
        #define XTAL_LD_CAP_135					0x000A
        #define XTAL_LD_CAP_14					0x000B
        #define XTAL_LD_CAP_145					0x000C
        #define XTAL_LD_CAP_15					0x000D
        #define XTAL_LD_CAP_155					0x000E
        #define XTAL_LD_CAP_16					0x000F    
     
    
        #define LNA_GAIN_0_DB                   0x0000
        #define LNA_GAIN_N_6_DB                 0x0008
        #define LNA_GAIN_N_14_DB                0x0010
        #define LNA_GAIN_N_20_DB                0x0018
        
        #define TX_POWER_0_DB                   0x0000
        #define TX_POWER_N_2_5_DB               0x0001
        #define TX_POWER_N_5_DB                 0x0002
        #define TX_POWER_N_7_5_DB               0x0003
        #define TX_POWER_N_10_DB                0x0004
        #define TX_POWER_N_12_5_DB              0x0005
        #define TX_POWER_N_15_DB                0x0006
        #define TX_POWER_N_17_5_DB              0x0007
    
    
        #define RSSI_THRESHOLD_103              0x0000
        #define RSSI_THRESHOLD_97               0x0001
        #define RSSI_THRESHOLD_91               0x0002
        #define RSSI_THRESHOLD_85               0x0003
        #define RSSI_THRESHOLD_79               0x0004
        #define RSSI_THRESHOLD_73               0x0005
     
    
        #if defined(BAND_434)
    
           #define              FREQ_BAND               0x0010              // 434MHz
           #if defined(DATA_RATE_1200)
                #define         DRVSREG                 0xC6A3              // 1200bps
                
                #define         RAW_RF_DEV              ((WORD)11+2*((WORD)CRYSTAL_PPM*434/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)        
                #define         FREQ_START              1332
                #define         FREQ_STEP               132
                #define         CHANNEL_NUM             5
                #define         FULL_CHANNEL_MAP        0x0000001F
           
           #elif defined(DATA_RATE_9600)
                
                #define         DRVSREG                 0xC623              // 9600bps
                
                #define         RAW_RF_DEV              ((WORD)19+2*((WORD)CRYSTAL_PPM*434/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)        
                #define         FREQ_START              1332
                #define         FREQ_STEP               132
                #define         CHANNEL_NUM             5
                #define         FULL_CHANNEL_MAP        0x0000001F
                
           #elif defined(DATA_RATE_19200)
           
                #define         DRVSREG                 0xC611              // 19200bps
                
                #define         RAW_RF_DEV              ((WORD)29+2*((WORD)CRYSTAL_PPM*434/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              1412
                #define         FREQ_STEP               154
                #define         CHANNEL_NUM             4 
                #define         FULL_CHANNEL_MAP        0x0000000F
           
           #elif defined(DATA_RATE_38400)
           
                #define         DRVSREG                 0xC608              // 38400
                
                #define         RAW_RF_DEV              ((WORD)48+2*((WORD)CRYSTAL_PPM*434/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              1388
                #define         FREQ_STEP               180
                #define         CHANNEL_NUM             4 
                #define         FULL_CHANNEL_MAP        0x0000000F
            
           #elif defined(DATA_RATE_57600)
           
                #define         DRVSREG                 0xC605
                
                #define         RAW_RF_DEV              ((WORD)67+2*((WORD)CRYSTAL_PPM*434/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              1444
                #define         FREQ_STEP               256
                #define         CHANNEL_NUM             3 
                #define         FULL_CHANNEL_MAP        0x00000007
                
           #elif defined(DATA_RATE_115200)
           
                #define         DRVSREG                 0xC602
                
                #define         RAW_RF_DEV              ((WORD)125+2*((WORD)CRYSTAL_PPM*434/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              1564
                #define         FREQ_STEP               345
                #define         CHANNEL_NUM             2
                #define         FULL_CHANNEL_MAP        0x00000003
           #else
                #error          "No valid data rate defined"     
           #endif
        
        #elif defined(BAND_868)
    
            #define              FREQ_BAND               0x0020              // 868MHz
            #if defined(DATA_RATE_1200)
                #define         DRVSREG                 0xC6A3
                
                #define         RAW_RF_DEV              ((WORD)11+2*((WORD)CRYSTAL_PPM*868/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              710
                #define         FREQ_STEP               90
                #define         CHANNEL_NUM             15
                #define         FULL_CHANNEL_MAP        0x00007FFF
            #elif defined(DATA_RATE_9600)
            
                #define         DRVSREG                 0xC623
                
                #define         RAW_RF_DEV              ((WORD)19+2*((WORD)CRYSTAL_PPM*868/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              710
                #define         FREQ_STEP               90
                #define         CHANNEL_NUM             15
                #define         FULL_CHANNEL_MAP        0x00007FFF
            
            #elif defined(DATA_RATE_19200)
            
                #define         DRVSREG                 0xC611
                
                #define         RAW_RF_DEV              ((WORD)29+2*((WORD)CRYSTAL_PPM*868/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              740
                #define         FREQ_STEP               110
                #define         CHANNEL_NUM             12
                #define         FULL_CHANNEL_MAP        0x00000FFF
    
            #elif defined(DATA_RATE_38400)
            
                #define         DRVSREG                 0xC608
                
                #define         RAW_RF_DEV              ((WORD)48+2*((WORD)CRYSTAL_PPM*868/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              724
                #define         FREQ_STEP               128
                #define         CHANNEL_NUM             11
                #define         FULL_CHANNEL_MAP        0x000007FF
                
            #elif defined(DATA_RATE_57600)
            
                #define         DRVSREG                 0xC605
                
                #define         RAW_RF_DEV              ((WORD)67+2*((WORD)CRYSTAL_PPM*868/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              750
                #define         FREQ_STEP               157
                #define         CHANNEL_NUM             9
                #define         FULL_CHANNEL_MAP        0x000001FF
                
            #elif defined(DATA_RATE_115200)
            
                #define         DRVSREG                 0xC602
                
                #define         RAW_RF_DEV              ((WORD)125+2*((WORD)CRYSTAL_PPM*868/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              800
                #define         FREQ_STEP               200
                #define         CHANNEL_NUM             7
                #define         FULL_CHANNEL_MAP        0x0000007F
            
            #else
                #error          "No valid data rate defined"
            #endif
        
        #elif defined(BAND_915)
    
           #define              FREQ_BAND               0x0030              // 915MHz
           #if defined(DATA_RATE_1200)
                #define         DRVSREG                 0xC6A3
                
                #define         RAW_RF_DEV              ((WORD)11+2*((WORD)CRYSTAL_PPM*915/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              320
                #define         FREQ_STEP               105
                #define         CHANNEL_NUM             32
                #define         FULL_CHANNEL_MAP        0xFFFFFFFF
           #elif defined(DATA_RATE_9600)
    
                #define         DRVSREG                 0xC623
                
                #define         RAW_RF_DEV              ((WORD)19+2*((WORD)CRYSTAL_PPM*915/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              320
                #define         FREQ_STEP               105
                #define         CHANNEL_NUM             32
                #define         FULL_CHANNEL_MAP        0xFFFFFFFF
            
            #elif defined(DATA_RATE_19200)
            
                #define         DRVSREG                 0xC611
                
                #define         RAW_RF_DEV              ((WORD)29+2*((WORD)CRYSTAL_PPM*915/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              320
                #define         FREQ_STEP               105
                #define         CHANNEL_NUM             32
                #define         FULL_CHANNEL_MAP        0xFFFFFFFF
            
            #elif defined(DATA_RATE_38400)
            
                #define         DRVSREG                 0xC608
                
                #define         RAW_RF_DEV              ((WORD)48+2*((WORD)CRYSTAL_PPM*915/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              320
                #define         FREQ_STEP               105
                #define         CHANNEL_NUM             32
                #define         FULL_CHANNEL_MAP        0xFFFFFFFF
                
            #elif defined(DATA_RATE_57600)
            
                #define         DRVSREG                 0xC605
                
                #define         RAW_RF_DEV              ((WORD)68+2*((WORD)CRYSTAL_PPM*915/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              320
                #define         FREQ_STEP               105
                #define         CHANNEL_NUM             32
                #define         FULL_CHANNEL_MAP        0xFFFFFFFF
           
           #elif defined(DATA_RATE_115200)
           
                #define         DRVSREG                 0xC602
                
                #define         RAW_RF_DEV              ((WORD)125+2*((WORD)CRYSTAL_PPM*915/1000))
                #define         RF_DEV                  ( ((RAW_RF_DEV % 15) < 8 ) ? (RAW_RF_DEV - (RAW_RF_DEV % 15)) : (RAW_RF_DEV - (RAW_RF_DEV % 15) + 15) )                 
                #define         TXCREG                  (0x9800 | (((WORD)RF_DEV/15 - 1)<<4) | TX_POWER)
                #define         FREQ_START              333
                #define         FREQ_STEP               134
                #define         CHANNEL_NUM             26
                #define         FULL_CHANNEL_MAP        0x03FFFFFF
              
           #else
                #error          "No valid data rate defined"
           #endif
    
        #else
            
            #error              "No valid frequency band defined"
           
        #endif
    
    
        #define         RXCREG                  (0x9400 | LNA_GAIN | RSSI_THRESHOLD)
        #define         GENCREG                 (0x8000|FREQ_BAND|XTAL_LD_CAP)
        #define         PMCREG                  0x8201
        #define         FIFORSTREG              0xCA81
        #define         AFCCREG                 0xC4B7
        #define         BBFCREG                 0xC2AC
    
        #if defined(PROTOCOL_MIWI)
            #define PROTOCOL_HEADER_SIZE 11
        #endif
        
        #if defined(PROTOCOL_MIWI_PRO)
            #define PROTOCOL_HEADER_SIZE 11
        #endif
        
        #if defined(PROTOCOL_P2P)
            #define PROTOCOL_HEADER_SIZE 0
        #endif
    
        #if defined(ENABLE_SECURITY)
            #define TX_PACKET_SIZE (TX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+BLOCK_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+10)
            #define RX_PACKET_SIZE (RX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+BLOCK_SIZE+10)
        #else
            #define TX_PACKET_SIZE  (TX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+5)
            #define RX_PACKET_SIZE  (RX_BUFFER_SIZE+PROTOCOL_HEADER_SIZE+MY_ADDRESS_LENGTH+MY_ADDRESS_LENGTH+5)
        #endif
    
    
        #if RX_PACKET_SIZE > 126
            #warning "RX\_BUFFER\_SIZE RX_BUFFER_SIZE may be defined too big"
            #undef RX_PACKET_SIZE
            #define RX_PACKET_SIZE 126
        #endif
        
        #if TX_PACKET_SIZE > 126
            #warning "TX\_BUFFER\_SIZE TX_BUFFER_SIZE may be defined too big"
            #undef TX_PACKET_SIZE
            #define TX_PACKET_SIZE 126
        #endif
    
    
        typedef struct
        {
            union
            {
                BYTE    Val;
                struct
                {
                    BYTE    Valid       :1;
                    BYTE    RSSI        :1;
                    BYTE    DQD         :1;
                } bits;
            } flags; 
            BYTE        Payload[RX_PACKET_SIZE];
            BYTE        PayloadLen;
        } RX_PACKET;
        
        typedef struct
        {
            BOOL        Valid;
            BYTE        Seq;
            WORD        CRC; 
            MIWI_TICK   startTick;
        } ACK_INFO;
        
        typedef union
        {
            WORD    Val;
            BYTE    v[2];
            struct 
            {
                BYTE    RSSI_ATS    :1;
                BYTE    FFEM        :1;
                BYTE    LBD         :1;
                BYTE    EXT         :1;
                BYTE    WKUP        :1;
                BYTE    RGUR_FFOV   :1;
                BYTE    POR         :1;
                BYTE    RG_FF_IT    :1;
                
                BYTE    filler      :5;
                BYTE    ATGL        :1;
                BYTE    CRL         :1;
                BYTE    DQD         :1;
            } bits;
        } TRANSCEIVER_STATUS;
        
      
        extern volatile TRANSCEIVER_STATUS   TransceiverStatus;
    #endif
#endif
