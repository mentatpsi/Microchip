/********************************************************************
* FileName:		BSP_MiWiCardDemoBd.h
* Dependencies:    
* Processor:	PIC18	
* Complier:     Microchip C18 v3.04 or higher	
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
*  This file defines functions used for demo board hardware
*
* Change History:
*  Rev   Date         Author    Description
*  1.0   2/09/2011    ccs       Initial revision
********************************************************************/
#ifndef __BSP_MIWI_DEMO_BD_H_
	#define __BSP_MIWI_DEMO_BD_H_

    // System Clock Frequency
    #define CLOCK_FREQ          16000000
    
    // There are three ways to use NVM to store data: External EPROM, Data EEPROM and 
    // programming space, with following definitions:
    //      #define USE_EXTERNAL_EEPROM
    //      #define USE_DATA_EEPROM
    //      #define USE_PROGRAMMING_SPACE  
    // Each demo board has defined the method of using NVM, as
    // required by Network Freezer feature.        
    #define USE_EXTERNAL_EEPROM
    
    // Define EEPROM_SHARE_SPI if external EEPROM shares the SPI 
    // bus with RF transceiver
    #define EEPROM_SHARE_SPI
    
    // MiWi Card Demo Board
    #if defined(MIWI_CARD_DEMO_BOARD)
        // MRF24J40 Pin Definitions
        #define RFIF                INTCON3bits.INT1IF
        #define RFIE                INTCON3bits.INT1IE
        #define PHY_CS              LATAbits.LATA5
        #define PHY_CS_TRIS         TRISAbits.TRISA5
        #define RF_INT_PIN          PORTBbits.RB0
        #define RF_INT_TRIS         TRISBbits.TRISB0
        #define PHY_WAKE            LATDbits.LATD1
        #define PHY_WAKE_TRIS       TRISDbits.TRISD1
		#define RF_EEnCS_TRIS	    TRISDbits.TRISD5
		#define RF_EEnCS		    LATDbits.LATD5
        #define PHY_RESETn          LATDbits.LATD0
        #define PHY_RESETn_TRIS     TRISDbits.TRISD0
      
        #define SPI_SDI             PORTBbits.RB5               
        #define SDI_TRIS            TRISBbits.TRISB5
        #define SPI_SDO             LATCbits.LATC7               
        #define SDO_TRIS            TRISCbits.TRISC7
        #define SPI_SCK             LATBbits.LATB4               
        #define SCK_TRIS            TRISBbits.TRISB4
        
        // Switch and LED Pin Definitions
        #define SW0_PORT            PORTBbits.RB1
        #define SW0_TRIS            TRISBbits.TRISB1
        #define SW1_PORT            PORTBbits.RB2
        #define SW1_TRIS            TRISBbits.TRISB2
                      
        #define LED0                LATCbits.LATC0
        #define LED0_TRIS           TRISCbits.TRISC0
        #define LED1                LATCbits.LATC2
        #define LED1_TRIS           TRISCbits.TRISC2
		#define LED2				LATCbits.LATC1
		#define LED2_TRIS			TRISCbits.TRISC1

        // External EEPROM Pin Definitions
		#define EE_nCS_TRIS         TRISDbits.TRISD5
        #define EE_nCS              LATDbits.LATD5
        #define TMRL                TMR0L    
    #endif
    
#endif
