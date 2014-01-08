/********************************************************************
* FileName:		MSPI.c
* Dependencies: MSPI.h
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
*   Configures and enables usage of the SPI ports
*
* Change History:
*  Rev   Date         Author    Description
*  0.1   11/09/2006   yfy       Initial revision
*  1.0   01/09/2007   yfy       Initial release
*  2.0   4/15/2009    yfy       MiMAC and MiApp revision
*  3.1   5/28/2010    yfy       MiWi DE 3.1
*  4.1   6/3/2011     yfy       MAL v2011-06
********************************************************************/

/************************ HEADERS **********************************/
#include "SystemProfile.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "WirelessProtocols/Console.h"

#if defined(__dsPIC30F__) || defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__) || defined(__PIC32MX__)

/************************ FUNCTIONS ********************************/

/*********************************************************************
* Function:         void SPIPut(BYTE v)
*
* PreCondition:     SPI has been configured 
*
* Input:		    v - is the byte that needs to be transfered
*
* Output:		    none
*
* Side Effects:	    SPI transmits the byte
*
* Overview:		    This function will send a byte over the SPI
*
* Note:			    None
********************************************************************/
void SPIPut(BYTE v)
{
    BYTE i;
    
    #if !defined(HARDWARE_SPI)
        
        SPI_SDO = 0;
        SPI_SCK = 0;
        
        for(i = 0; i < 8; i++)
        {
            SPI_SDO = (v >> (7-i));      
            SPI_SCK = 1;
            SPI_SCK = 0;  
        }  
        SPI_SDO = 0; 
    #else
    
        #if defined(__PIC32MX__)
            putcSPI1(v);
            i = (BYTE)getcSPI1();
        #else
            IFS0bits.SPI1IF = 0;
            i = SPI1BUF;
            SPI1BUF = v;
            while(IFS0bits.SPI1IF == 0){}
        #endif
    #endif
}


/*********************************************************************
* Function:         BYTE SPIGet(void)
*
* PreCondition:     SPI has been configured 
*
* Input:		    none
*
* Output:		    BYTE - the byte that was last received by the SPI
*
* Side Effects:	    none
*
* Overview:		    This function will read a byte over the SPI
*
* Note:			    None
********************************************************************/
BYTE SPIGet(void)
{
    #if !defined(HARDWARE_SPI)
        BYTE i;
        BYTE spidata = 0;

        
        SPI_SDO = 0;
        SPI_SCK = 0;
        
        for(i = 0; i < 8; i++)
        {
            spidata = (spidata << 1) | SPI_SDI;  
            SPI_SCK = 1;
            SPI_SCK = 0; 
        }
        
        return spidata;
    #else 
        #if defined(__PIC32MX__)
            BYTE dummy;

            putcSPI1(0x00);
            dummy = (BYTE)getcSPI1();
            return(dummy);
        #else
            SPIPut(0x00);
            return SPI1BUF;
        #endif
    #endif
}

#if defined(SUPPORT_TWO_SPI)
    /*********************************************************************
    * Function:         void SPIPut2(BYTE v)
    *
    * PreCondition:     SPI has been configured 
    *
    * Input:		    v - is the byte that needs to be transfered
    *
    * Output:		    none
    *
    * Side Effects:	    SPI transmits the byte
    *
    * Overview:		    This function will send a byte over the SPI
    *
    * Note:			    None
    ********************************************************************/
    void SPIPut2(BYTE v)
    {
        BYTE i;
        
        #if !defined(HARDWARE_SPI)
            
            SPI_SDO2 = 0;
            SPI_SCK2 = 0;
            
            for(i = 0; i < 8; i++)
            {
                SPI_SDO2 = (v >> (7-i));      
                SPI_SCK2 = 1;
                SPI_SCK2 = 0;  
            }  
            SPI_SDO2 = 0; 
        #else
        
            #if defined(__PIC32MX__)
                putcSPI2(v);
                i = (BYTE)getcSPI2();
            #else
                IFS2bits.SPI2IF = 0;
                i = SPI2BUF;
                SPI2BUF = v;
                while(IFS2bits.SPI2IF == 0){}
            #endif
        #endif
    }
    
    
    /*********************************************************************
    * Function:         BYTE SPIGet2(void)
    *
    * PreCondition:     SPI has been configured 
    *
    * Input:		    none
    *
    * Output:		    BYTE - the byte that was last received by the SPI
    *
    * Side Effects:	    none
    *
    * Overview:		    This function will read a byte over the SPI
    *
    * Note:			    None
    ********************************************************************/
    BYTE SPIGet2(void)
    {
        #if !defined(HARDWARE_SPI)
            BYTE i;
            BYTE spidata = 0;
    
            SPI_SDO2 = 0;
            SPI_SCK2 = 0;
            
            for(i = 0; i < 8; i++)
            {
                spidata = (spidata << 1) | SPI_SDI2;  
                SPI_SCK2 = 1;
                SPI_SCK2 = 0; 
            }
            
            return spidata;
        #else 
            #if defined(__PIC32MX__)
                BYTE dummy;
    
                putcSPI2(0x00);
                dummy = (BYTE)getcSPI2();
                return(dummy);
            #else
                SPIPut2(0x00);
                return SPI2BUF;
            #endif
        #endif
    }
#endif    
    

#elif defined(__18CXX)

/************************ FUNCTIONS ********************************/
 
/*********************************************************************
* Function:         void SPIPut(BYTE v)
*
* PreCondition:     SPI has been configured 
*
* Input:		    v - is the byte that needs to be transfered
*
* Output:		    none
*
* Side Effects:	    SPI transmits the byte
*
* Overview:		    This function will send a byte over the SPI
*
* Note:			    None
********************************************************************/
 void SPIPut(BYTE v)
{
    BYTE i;
    
    #if !defined(HARDWARE_SPI)
        
        SPI_SDO = 0;
        SPI_SCK = 0;
        
        for(i = 0; i < 8; i++)
        {
            SPI_SDO = (v >> (7-i));  
            SPI_SCK = 1;
            SPI_SCK = 0;  
        }  
        SPI_SDO = 0; 
    #else

        PIR1bits.SSPIF = 0;
        i = SSPBUF;
        do
        {
            SSPCON1bits.WCOL = 0;
            SSPBUF = v;
        } while( SSPCON1bits.WCOL );
    
        while( PIR1bits.SSPIF == 0 );
        
    #endif
}

/*********************************************************************
* Function:         BYTE SPIGet(void)
*
* PreCondition:     SPI has been configured 
*
* Input:		    none
*
* Output:		    BYTE - the byte that was last received by the SPI
*
* Side Effects:	    none
*
* Overview:		    This function will read a byte over the SPI
*
* Note:			    None
********************************************************************/
BYTE SPIGet(void)
{
    #if !defined(HARDWARE_SPI)
        BYTE i;
        BYTE spidata = 0;

        
        SPI_SDO = 0;
        SPI_SCK = 0;
        
        for(i = 0; i < 8; i++)
        {
            spidata = (spidata << 1) | SPI_SDI;  
            SPI_SCK = 1;
            SPI_SCK = 0;     
        }
        
        return spidata;
    #else 
   
        SPIPut(0x00);
        return SSPBUF;
    #endif
}


    #if defined(SUPPORT_TWO_SPI)
        /*********************************************************************
        * Function:         void SPIPut2(BYTE v)
        *
        * PreCondition:     SPI has been configured 
        *
        * Input:		    v - is the byte that needs to be transfered
        *
        * Output:		    none
        *
        * Side Effects:	    SPI transmits the byte
        *
        * Overview:		    This function will send a byte over the SPI
        *
        * Note:			    None
        ********************************************************************/
         void SPIPut2(BYTE v)
        {
            BYTE i;
            
            #if !defined(HARDWARE_SPI)
                
                SPI_SDO2 = 0;
                SPI_SCK2 = 0;
                
                for(i = 0; i < 8; i++)
                {
                    SPI_SDO2 = (v >> (7-i));  
                    SPI_SCK2 = 1;
                    SPI_SCK2 = 0;  
                }  
                SPI_SDO2 = 0; 
            #else

                //Reset the Global interrupt pin
                //SPI2SSPIF = 0;
                PIR3bits.SSP2IF = 0;
                do
                {
                    //SPI2WCOL = 0;
                    SSP2CON1bits.WCOL = 0;
            		//Reset write collision bit
                    //SPI2SSPBUF = v;
                    SSP2BUF = v;
            		//load the buffer
                //} while( SPI2WCOL );
                } while( SSP2CON1bits.WCOL );
            	
            	//perform write again if write collision occurs
                //while( SPI2SSPIF == 0 );
                while( PIR3bits.SSP2IF == 0 );
                
            	//Wait until interrupt is received from the MSSP module
            	//SPI2SSPIF = 0;
            	PIR3bits.SSP2IF = 0;
            	//Reset the interrupt
            	
            #endif
        }
        
        /*********************************************************************
        * Function:         BYTE SPIGet2(void)
        *
        * PreCondition:     SPI has been configured 
        *
        * Input:		    none
        *
        * Output:		    BYTE - the byte that was last received by the SPI
        *
        * Side Effects:	    none
        *
        * Overview:		    This function will read a byte over the SPI
        *
        * Note:			    None
        ********************************************************************/
        BYTE SPIGet2(void)
        {
            #if !defined(HARDWARE_SPI)
                BYTE i;
                BYTE spidata = 0;
        
                
                SPI_SDO2 = 0;
                SPI_SCK2 = 0;
                
                for(i = 0; i < 8; i++)
                {
                    spidata = (spidata << 1) | SPI_SDI2;  
                    SPI_SCK2 = 1;
                    SPI_SCK2 = 0;     
                }
                
                return spidata;
            #else 
           
                SPIPut2(0x00);
                return SSP2BUF;
            #endif
        }
    #endif

#else
    #error Unknown processor.  See Compiler.h
#endif





