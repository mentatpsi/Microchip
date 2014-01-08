/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        system.h
 *  Dependencies:    Compiler.h, GenericTypeDefs.h, mTouchConfig.h
 *  Processor:       PIC24F
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
#include "mTouchConfig.h"

/****************************************************************************
  System oscillator frequency. 
  ***************************************************************************/
#define CLOCK_FREQUENCY  32000000

#if !defined ( __C30__ )
    #error This system file is for PIC24/dsPIC only.
#endif

#ifdef _SYSTEM_C_
// PRIVATE

// PIC24FJ128GB106 CONFIGURATION SETTINGS
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx2 & FWDTEN_OFF & WDTPS_PS128 & FWPSA_PR32)
_CONFIG2( IESO_OFF & PLLDIV_DIV2 & FNOSC_PRIPLL & FCKSM_CSDCMD & IOL1WAY_OFF & DISUVREG_ON & POSCMOD_HS)

/****************************************************************************
  Macro:
    SysLedsInit()

  Summary:
    Initializes board's LEDs.

  Description:
    Initializes board's LEDs.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysLedsInit()  {LATD &= ~0x00ff; LATE &= ~0x00ff; TRISD &= ~0x00ff; TRISE &= ~0x00ff;}

/****************************************************************************
  Macro:
    SysDelay1Ms()

  Summary:
    Delay execution on 1 mS.

  Description:
    Delay execution on 1 mS.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysDelay1Ms()  {asm volatile("Repeat #16000"); asm volatile("nop");}

#endif

// PUBLIC

// main(...) returns int for C30 compiler
#define RET_MAIN	int

// SENSORS CONNECTIONS
#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0

#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1

#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2

#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3

#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5

#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6

#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7

#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8

#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9

#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 10
#define CH10_AN_NUM 10

#define CH11_TRIS   &TRISB
#define CH11_LAT    &LATB
#define CH11_IO_BIT 11
#define CH11_AN_NUM 11

#define CH12_TRIS   &TRISB
#define CH12_LAT    &LATB
#define CH12_IO_BIT 12
#define CH12_AN_NUM 12

#define CH13_TRIS   &TRISB
#define CH13_LAT    &LATB
#define CH13_IO_BIT 13
#define CH13_AN_NUM 13

#define CH14_TRIS   &TRISB
#define CH14_LAT    &LATB
#define CH14_IO_BIT 14
#define CH14_AN_NUM 14

#define CH15_TRIS   &TRISB
#define CH15_LAT    &LATB
#define CH15_IO_BIT 15
#define CH15_AN_NUM 15


// LEDS CONTROL
#define Led0On()     {LATDbits.LATD0 = 1;}
#define Led0Off()    {LATDbits.LATD0 = 0;}

#define Led1On()     {LATDbits.LATD1 = 1;}
#define Led1Off()    {LATDbits.LATD1 = 0;}

#define Led2On()     {LATDbits.LATD2 = 1;}
#define Led2Off()    {LATDbits.LATD2 = 0;}

#define Led3On()     {LATDbits.LATD3 = 1;}
#define Led3Off()    {LATDbits.LATD3 = 0;}

#define Led4On()     {LATDbits.LATD4 = 1;}
#define Led4Off()    {LATDbits.LATD4 = 0;}

#define Led5On()     {LATDbits.LATD5 = 1;}
#define Led5Off()    {LATDbits.LATD5 = 0;}

#define Led6On()     {LATDbits.LATD6 = 1;}
#define Led6Off()    {LATDbits.LATD6 = 0;}

#define Led7On()     {LATDbits.LATD7 = 1;}
#define Led7Off()    {LATDbits.LATD7 = 0;}

#define Led8On()     {LATEbits.LATE0 = 1;}
#define Led8Off()    {LATEbits.LATE0 = 0;}

#define Led9On()     {LATEbits.LATE1 = 1;}
#define Led9Off()    {LATEbits.LATE1 = 0;}

#define Led10On()    {LATEbits.LATE2 = 1;}
#define Led10Off()   {LATEbits.LATE2 = 0;}

#define Led11On()    {LATEbits.LATE3 = 1;}
#define Led11Off()   {LATEbits.LATE3 = 0;}

#define Led12On()    {LATEbits.LATE4 = 1;}
#define Led12Off()   {LATEbits.LATE4 = 0;}

#define Led13On()    {LATEbits.LATE5 = 1;}
#define Led13Off()   {LATEbits.LATE5 = 0;}

#define Led14On()    {LATEbits.LATE6 = 1;}
#define Led14Off()   {LATEbits.LATE6 = 0;}

#define Led15On()    {LATEbits.LATE7 = 1;}
#define Led15Off()   {LATEbits.LATE7 = 0;}

#define Led_ALLOn()  {LATD |= 0x00ff; LATE |= 0x00ff;}
#define Led_ALLOff() {LATD &= ~0x00ff; LATE &= ~0x00ff;}

/****************************************************************************
  Macro:
    SysTimerInit()

  Summary:
    Initializes timer.

  Description:
    Initializes timer.
 
  Precondition:
    SysTimerInterrupt() interrupt handler must be defined in application code.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    Timer generates interrupts and calls SysTimerInterrupt() every 0.25 mS.
  ***************************************************************************/
#define SysTimerInit() {PR2=4000; IEC0bits.T2IE=1; T2CONbits.TON = 1;}

/****************************************************************************
  Macro:
    SysEnableWDT()

  Summary:
    Enables watchdog timer.

  Description:
    Enables watchdog timer. Watchdog timer period is 128 mS (see configuration bits).
 
  Precondition:
    None

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysEnableWDT()   {ClrWdt(); RCONbits.SWDTEN = 1;}

/****************************************************************************
  Macro:
    SysDisableWDT()

  Summary:
    Disables watchdog timer.

  Description:
    Disables watchdog timer.
 
  Precondition:
    None

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysDisableWDT()   {RCONbits.SWDTEN = 0;}

/****************************************************************************
  Macro:
    SysEnableTimerInterrupt()

  Summary:
    Enables timer interrupt.

  Description:
    Enables timer interrupt.
 
  Precondition:
    None

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysEnableTimerInterrupt()   {IFS0bits.T2IF=0; IEC0bits.T2IE=1;}

/****************************************************************************
  Macro:
    SysDisableTimerInterrupt()

  Summary:
    Disables timer interrupt.

  Description:
    Disables timer interrupt.
 
  Precondition:
    None

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysDisableTimerInterrupt()   {IFS0bits.T2IF=0; IEC0bits.T2IE=0;}

/****************************************************************************
  Function:
    void SysDelayMs(UINT16 delay)

  Summary:
    Delay execution on defined time.

  Description:
    Delay execution on defined time.
 
  Precondition:
    None

  Parameters:
    delay  - delay time in mS.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void SysDelayMs(UINT16 delay);

/****************************************************************************
  Macro:
    SysLedsChaser()

  Summary:
    Turn on and off all LEDs in sequence.

  Description:
    Turn on and off all LEDs in sequence.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysLedsChaser() {\
Led0On(); SysDelayMs(20);Led0Off();Led1On(); SysDelayMs(20);Led1Off();Led2On(); SysDelayMs(20);Led2Off();Led3On(); SysDelayMs(20);Led3Off();\
Led4On(); SysDelayMs(20);Led4Off();Led5On(); SysDelayMs(20);Led5Off();Led6On(); SysDelayMs(20);Led6Off();Led7On(); SysDelayMs(20);Led7Off();\
Led8On(); SysDelayMs(20);Led8Off();Led9On(); SysDelayMs(20);Led9Off();Led10On(); SysDelayMs(20);Led10Off();Led11On(); SysDelayMs(20);Led11Off();\
Led12On(); SysDelayMs(20);Led12Off();Led13On(); SysDelayMs(20);Led13Off();Led14On(); SysDelayMs(20);Led14Off();Led15On(); SysDelayMs(20);Led15Off();\
Led15On(); SysDelayMs(20);Led15Off();Led14On(); SysDelayMs(20);Led14Off();Led13On(); SysDelayMs(20);Led13Off();Led12On(); SysDelayMs(20);Led12Off();\
Led11On(); SysDelayMs(20);Led11Off();Led10On(); SysDelayMs(20);Led10Off();Led9On(); SysDelayMs(20);Led9Off();Led8On(); SysDelayMs(20);Led8Off();\
Led7On(); SysDelayMs(20);Led7Off();Led6On(); SysDelayMs(20);Led6Off();Led5On(); SysDelayMs(20);Led5Off();Led4On(); SysDelayMs(20);Led4Off();\
Led3On(); SysDelayMs(20);Led3Off();Led2On(); SysDelayMs(20);Led2Off();Led1On(); SysDelayMs(20);Led1Off();Led0On(); SysDelayMs(20);Led0Off();\
}

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
