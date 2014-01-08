/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        system.h
 *  Dependencies:    Compiler.h, GenericTypeDefs.h, mTouchConfig.h
 *  Processor:       PIC18
 *  Compiler:        HI-TECH, MPLAB C18 and XC8
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
#define CLOCK_FREQUENCY  48000000


#if (!defined(__PICC18__) && !defined(__18CXX))
    #error This system file is for PIC18 only.
#endif

#ifdef _SYSTEM_C_
// PRIVATE

// PIC18F46J50 CONFIGURATION SETTINGS
#if defined(__PICC18__)

__CONFIG(1, WDTEN_OFF & XINST_OFF & PLLDIV_2 & CPUDIV_OSC1);
__CONFIG(2, OSC_HSPLL & WDTPS_128);
__CONFIG(3, DSBOREN_OFF & DSWDTEN_OFF & IOL1WAY_OFF);

#else

#pragma config WDTEN = OFF, WDTPS = 128, PLLDIV = 2, XINST = OFF, CPUDIV = OSC1 
#pragma config OSC=HSPLL, DSBOREN = OFF, DSWDTEN = OFF, IOL1WAY = OFF 

#endif

/****************************************************************************
  Macro:
    SysLedsInit()

  Summary:
    Initializes board's LEDs.

  Description:
    Initializes board's LEDs.
 
  Precondition:


  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#define SysLedsInit()  {LATD = 0; TRISD = 0;}

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
#define SysDelay1Ms()  { UINT16 d=800; while(d--){Nop();} }

#endif

// PUBLIC

// main(...) returns void for C18 compiler
#define RET_MAIN	void

// SLEEP TIMEOUT DELAY FOR LOW POWER DEMO 
#define  SLEEP_TIMEOUT_DELAY                  ((UINT32)150000)

// SENSORS CONNECTIONS
#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0

#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1

#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2

#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3

#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISE
#define CH5_LAT     &LATE
#define CH5_IO_BIT  0
#define CH5_AN_NUM  5

#define CH6_TRIS    &TRISE
#define CH6_LAT     &LATE
#define CH6_IO_BIT  1
#define CH6_AN_NUM  6

#define CH7_TRIS    &TRISE
#define CH7_LAT     &LATE
#define CH7_IO_BIT  2
#define CH7_AN_NUM  7

#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  2
#define CH8_AN_NUM  8

#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  3
#define CH9_AN_NUM  9

#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 1
#define CH10_AN_NUM 10

#define CH11_TRIS   &TRISC
#define CH11_LAT    &LATC
#define CH11_IO_BIT 2
#define CH11_AN_NUM 11

#define CH12_TRIS   &TRISB
#define CH12_LAT    &LATB
#define CH12_IO_BIT 0
#define CH12_AN_NUM 12


// LEDS CONTROL
#define Led0On()     {LATD = 0xe0; LATDbits.LATD0 = 1;}
#define Led0Off()    {LATD = 0;}

#define Led1On()     {LATD = 0xd0; LATDbits.LATD0 = 1;}
#define Led1Off()    {LATD = 0;}

#define Led2On()     {LATD = 0xb0; LATDbits.LATD0 = 1;}
#define Led2Off()    {LATD = 0;}

#define Led3On()     {LATD = 0x70; LATDbits.LATD0 = 1;}
#define Led3Off()    {LATD = 0;}

#define Led4On()     {LATD = 0xe0; LATDbits.LATD1 = 1;}
#define Led4Off()    {LATD = 0;}

#define Led5On()     {LATD = 0xd0; LATDbits.LATD1 = 1;}
#define Led5Off()    {LATD = 0;}

#define Led6On()     {LATD = 0xb0; LATDbits.LATD1 = 1;}
#define Led6Off()    {LATD = 0;}

#define Led7On()     {LATD = 0x70; LATDbits.LATD1 = 1;}
#define Led7Off()    {LATD = 0;}

#define Led8On()     {LATD = 0xe0; LATDbits.LATD2 = 1;}
#define Led8Off()    {LATD = 0;}

#define Led9On()     {LATD = 0xd0; LATDbits.LATD2 = 1;}
#define Led9Off()    {LATD = 0;}

#define Led10On()    {LATD = 0xb0; LATDbits.LATD2 = 1;}
#define Led10Off()   {LATD = 0;}

#define Led11On()    {LATD = 0x70; LATDbits.LATD2 = 1;}
#define Led11Off()   {LATD = 0;}

#define Led12On()    {LATD = 0xe0; LATDbits.LATD3 = 1;}
#define Led12Off()   {LATD = 0;}

#define Led13On()    {LATD = 0xd0; LATDbits.LATD3 = 1;}
#define Led13Off()   {LATD = 0;}

#define Led14On()    {LATD = 0xb0; LATDbits.LATD3 = 1;}
#define Led14Off()   {LATD = 0;}

#define Led15On()    {LATD = 0x70; LATDbits.LATD3 = 1;}
#define Led15Off()   {LATD = 0;}

#define Led_ALLOn()  {LATD = 0x0f;}
#define Led_ALLOff() {LATD = 0;}

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
    Timer generates interrupts and calls SysTimerInterrupt() every 1 mS.
  ***************************************************************************/
#define SysTimerInit() {T2CONbits.T2CKPS = 3; PR2 = 180; T2CONbits.TMR2ON = 1; PIR1bits.TMR2IF = 0; PIE1bits.TMR2IE = 1;}

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
#define SysEnableWDT()   {ClrWdt(); WDTCONbits.SWDTEN = 1;}

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
#define SysDisableWDT()   {WDTCONbits.SWDTEN = 0;}

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
#define SysEnableTimerInterrupt()   {PIR1bits.TMR2IF = 0; PIE1bits.TMR2IE = 1;}

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
#define SysDisableTimerInterrupt()   {PIR1bits.TMR2IF = 0; PIE1bits.TMR2IE = 0;}

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
    SysLogInit()

  Summary:
    Initializes board's UART for debug log.

  Description:
    Initializes board's UART for debug log.
 
  Precondition:


  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void SysLogInit(void);


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
