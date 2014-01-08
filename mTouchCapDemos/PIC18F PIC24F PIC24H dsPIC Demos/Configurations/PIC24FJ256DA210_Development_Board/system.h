/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        system.h
 *  Dependencies:    Compiler.h, GenericTypeDefs.h, mTouchConfig.h
 *  Processor:       PIC24FJ256DA210
 *  Compiler:        MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains hardware depended definitions.
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

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"

#include "drv_spi.h"
#include "SST25VF016.h"   

#include "mTouchConfig.h"

#if !defined ( __C30__ )
    #error This system file is for PIC24/dsPIC only.
#endif

#ifdef _SYSTEM_C_

// PRIVATE

/////////////////////////////////////////////////////////////////////////////
// PIC24FJ256DA210 CONFIGURATION SETTINGS
/////////////////////////////////////////////////////////////////////////////
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & ALTPMP_ALTPMPEN /*ALTPMP_ALPMPDIS */ & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)

/****************************************************************************
  Macro:
    SysTimerInit()

  Summary:
    Initializes timer 4.

  Description:
    Initializes timer 4.
 
  Precondition:
    SysTimerInterrupt() interrupt handler must be defined in application code.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    Timer generates interrupts and calls SysTimerInterrupt() every 0.5 mS.
  ***************************************************************************/
#define SysTimerInit() {TMR4 = 0; PR4=((GetPeripheralClock() * 500) / 4000000); IEC1bits.T4IE=1; T4CONbits.TON = 1;}

#endif

// PUBLIC

// SENSORS CONNECTIONS
#define CH0_TRIS    &TRISG
#define CH0_LAT     &LATG
#define CH0_IO_BIT  8
#define CH0_AN_NUM  19

#define CH1_TRIS    &TRISE
#define CH1_LAT     &LATE
#define CH1_IO_BIT  9
#define CH1_AN_NUM  21

#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  5
#define CH2_AN_NUM  5


// LEDS CONTROL
#define Led0On()     {TRISGbits.TRISG8 = 0;LATGbits.LATG8 = 1;}
#define Led0Off()    {TRISGbits.TRISG8 = 1;}

#define Led1On()     {TRISEbits.TRISE9 = 0;LATEbits.LATE9 = 1;}
#define Led1Off()    {TRISEbits.TRISE9 = 1;}

#define Led2On()     {TRISBbits.TRISB5 = 0;LATBbits.LATB5 = 1;}
#define Led2Off()    {TRISBbits.TRISB5 = 1;}

#define Led_ALLOn()  {Led0On();Led1On();Led2On()}
#define Led_ALLOff() {Led0Off();Led1Off();Led2Off()}


/****************************************************************************
  Function:
    void SysBoardInit(void)

  Summary:
    Initializes the board hardware.

  Description:
    Initializes the board hardware.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void SysBoardInit(void);

/****************************************************************************
  Function:
    void SysTimerInterrupt(void)

  Summary:
    Callback function, called every 1 ms.

  Description:
    Callback function, called every 1 ms.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    SysTimerInterrupt() must be defined in the application code.
  ***************************************************************************/
void SysTimerInterrupt(void);

#endif
