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
    
    #define SUPPORT_TWO_SPI
    
    // Define EEPROM_SHARE_SPI if external EEPROM shares the SPI 
    // bus with RF transceiver
    //#define EEPROM_SHARE_SPI
    
    #if defined(MRF24J40)
        // MRF24J40 Pin Definitions
        #define RFIF                INTCONbits.INT0IF
        #define RFIE                INTCONbits.INT0IE
        #define PHY_CS              LATAbits.LATA5
        #define PHY_CS_TRIS         TRISAbits.TRISA5
        #define RF_INT_PIN          PORTBbits.RB0
        #define RF_INT_TRIS         TRISBbits.TRISB0
        #define PHY_WAKE            LATDbits.LATD1
        #define PHY_WAKE_TRIS       TRISDbits.TRISD1
        #define PHY_RESETn          LATDbits.LATD0
        #define PHY_RESETn_TRIS     TRISDbits.TRISD0

    #elif defined(MRF89XA)
    	#define Data_nCS            LATAbits.LATA5
    	#define Data_nCS_TRIS       TRISAbits.TRISA5  
    	#define Config_nCS          LATDbits.LATD1
        #define Config_nCS_TRIS     TRISDbits.TRISD1 

        #define IRQ0_INT_PIN        PORTBbits.RB0
        #define IRQ0_INT_TRIS       TRISBbits.TRISB0
    	#define PHY_IRQ0            INTCONbits.INT0IF
    	#define PHY_IRQ0_En         INTCONbits.INT0IE
    	
        #define IRQ1_INT_PIN        PORTCbits.RC6
        #define IRQ1_INT_TRIS       TRISCbits.TRISC6    	             	  	
    	#define PHY_IRQ1            INTCON3bits.INT1IF
    	#define PHY_IRQ1_En         INTCON3bits.INT1IE 	
  	#define PHY_RESETn          LATDbits.LATD0
        #define PHY_RESETn_TRIS     TRISDbits.TRISD0

    #endif
    
        // EEProm Pin Definitions
     	#define RF_EEnCS		    LATDbits.LATD5
    	#define RF_EEnCS_TRIS	    TRISDbits.TRISD5
    	           
        // SPI1 Pin Definitions
        #define SPI_SDI             PORTBbits.RB5               
        #define SDI_TRIS            TRISBbits.TRISB5
        #define SPI_SDO             LATCbits.LATC7               
        #define SDO_TRIS            TRISCbits.TRISC7
        #define SPI_SCK             LATBbits.LATB4               
        #define SCK_TRIS            TRISBbits.TRISB4
        
        // SPI2 Pin Definitions
        #define SPI_SDI2            PORTDbits.RD6               
        #define SDI2_TRIS           TRISDbits.TRISD6
        #define SPI_SDO2            LATDbits.LATD2               
        #define SDO2_TRIS           TRISDbits.TRISD2
        #define SPI_SCK2            LATDbits.LATD4               
        #define SCK2_TRIS           TRISDbits.TRISD4
        
        #define SPI2SSPIF           PIR3bits.SSP2IF
        #define SPI2WCOL            SSP2CON1bits.WCOL
        #define SPI2SSPBUF          SSP2BUF
        
        // Switch and LED Pin Definitions
        #define SW1_PORT            PORTBbits.RB1
        #define SW1_TRIS            TRISBbits.TRISB1
        #define SW2_PORT            PORTBbits.RB2
        #define SW2_TRIS            TRISBbits.TRISB2
                      
        #define LED0                LATCbits.LATC2
        #define LED0_TRIS           TRISCbits.TRISC2
        #define LED1                LATAbits.LATA6
        #define LED1_TRIS           TRISAbits.TRISA6
		#define LED2				LATAbits.LATA7
		#define LED2_TRIS			TRISAbits.TRISA7

        // External EEPROM Pin Definitions
		//#define EE_nCS_TRIS         TRISDbits.TRISD5
        #define MAC_nCS             LATDbits.LATD5
        #define TMRL                TMR0L

        //External SST Serial Flash Definitions
        #define EE_nCS              LATEbits.LATE2
        #define EE_nCS_TRIS         TRISEbits.TRISE2

        // LCD Pin Definitions
        #define LCD_CS_TRIS         TRISDbits.TRISD7
        #define LCD_CS              LATDbits.LATD7
        #define LCD_RS_TRIS         TRISDbits.TRISD3
        #define LCD_RS              LATDbits.LATD3
        #define LCD_RESET_TRIS      TRISEbits.TRISE0
        #define LCD_RESET           LATEbits.LATE0
        #define LCD_BKLT_TRIS       TRISEbits.TRISE1
        #define LCD_BKLT            LATEbits.LATE1
    
#endif
