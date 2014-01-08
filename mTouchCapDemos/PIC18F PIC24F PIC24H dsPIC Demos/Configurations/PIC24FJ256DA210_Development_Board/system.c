/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        system.c
 *  Dependencies:    system.h
 *  Processor:       PIC24FJ256DA210
 *  Compiler:        MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This files contains all hardware depended functions.
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and 
 * any derivatives created by any person or entity by or on your behalf, 
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors 
 * retain all ownership and intellectual property rights in the 
 * accompanying software and in all derivatives hereto. 
 * 
 * This software and any accompanying information is for suggestion only. 
 * It does not modify Microchip's standard warranty for its products. You 
 * agree that you are solely responsible for testing the software and 
 * determining its suitability. Microchip has no obligation to modify, 
 * test, certify, or support the software. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH 
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY 
 * APPLICATION. 
 * 
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, 
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT 
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, 
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, 
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, 
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS 
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
 * 
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF 
 * THESE TERMS. 
 *************************************************************************/
#ifndef _SYSTEM_C_
#define _SYSTEM_C_

#include "system.h"

/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};


void SysBoardInit(void)
{
    // SPI flash initialization (used by touch screen).
    SST25_CS_LAT = 1;
    SST25_CS_TRIS = 0;

  	SST25_SDI_ANS = 0;
    SST25_SDO_ANS = 0;

    SST25_SCK_TRIS = 0;
    SST25_SDO_TRIS = 0;
    SST25_SDI_TRIS = 1;

    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    #if (SST25_SPI_CHANNEL == 1)
        RPOR1bits.RP2R = 8;                 // assign RP2 for SCK1
        RPOR0bits.RP1R = 7;                 // assign RP1 for SDO1
        RPINR20bits.SDI1R = 0;              // assign RP0 for SDI1
    #elif (SST25_SPI_CHANNEL == 2)
        RPOR1bits.RP2R = 11;                // assign RP2 for SCK2
    	RPOR0bits.RP1R = 10;                // assign RP1 for SDO2
        RPINR22bits.SDI2R = 0;              // assign RP0 for SDI2
    #endif

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS


    SST25Init((DRV_SPI_INIT_DATA*)&SPI_Init_Data);

    // Timer initialization
    SysTimerInit();   
}

void __attribute__((interrupt, shadow, auto_psv)) _T4Interrupt(void)
{
   SysTimerInterrupt();

   TMR4 = 0;
   IFS1bits.T4IF = 0;
}


////////////////////////////////////////////////////////////////////////////////
void __attribute__((interrupt, auto_psv)) _DefaultInterrupt ( void )
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
////////////////////////////////////////////////////////////////////////////////
void __attribute__((interrupt, auto_psv)) _OscillatorFail ( void )
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
////////////////////////////////////////////////////////////////////////////////
void __attribute__((interrupt, auto_psv)) _AddressError ( void )
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
////////////////////////////////////////////////////////////////////////////////
void __attribute__((interrupt, auto_psv)) _StackError ( void )
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
////////////////////////////////////////////////////////////////////////////////
void __attribute__((interrupt, auto_psv)) _MathError ( void )
{
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}

#endif
