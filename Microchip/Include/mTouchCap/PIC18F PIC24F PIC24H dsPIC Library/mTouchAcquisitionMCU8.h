/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                      
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchAcquisitionMCU8.h
 *  Dependencies:    None.
 *  Processor:       PIC18
 *  Compiler:        HI-TECH, MPLAB C18, XC8
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains all definitions for acquisition 
 *                   module.
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

#ifndef _MTOUCH_ACQUISITION_H_
#define _MTOUCH_ACQUISITION_H_


/****************************************************************************
  Macro:
    MTouchDischargeDelay()

  Summary:
    Provides a delay to discharge sensor and Chold capacitor.

  Description:
    Provides a delay to discharge sensor and Chold capacitor.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This delay depends on resistance of the PIC analog switch (less than 5 kOhm)
    and capacitance of the PIC ADC hold capacitor (less than 50pF). Number of NOPs
    should be set to provide a delay at least 1uS (16 NOPs for PIC18 @ 48MHz).
***************************************************************************/
#define MTouchDischargeDelay()          {\
                                         Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();\
                                         Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();\
                                         }

/****************************************************************************
  Macro:
    MTouchChargeDelay(chargeDelay)

  Summary:
    Provides a delay to charge sensor. Used for CTMU acquisition only.

  Description:
    Provides a delay to charge sensor.
 
  Precondition:
    None.

  Parameters:
    chargeDelay - time to charge sensor.

  Returns:
    None.

  Remarks:
    None.
 ***************************************************************************/
#if defined(__18CXX) && !defined(__PICC18__)

#define MTouchChargeDelay(chargeDelay)   {while(chargeDelay--);}

#else

#define MTouchChargeDelay(chargeDelay)\
asm ("BANKSEL(_" ___mkstr(chargeDelay) ")");\
asm ("decfsz _"___mkstr(chargeDelay) ", f");\
asm ("goto  $-1");

#endif

/****************************************************************************
  Macro:
    MTouchSettleDelay()

  Summary:
    Provides a delay to selltle charges. Used for CVD acquisition only.

  Description:
    Provides a delay to charge sensor and ADC hold capacitors. Also it is
    used to wait when the charge will be settled between sensor and ADC hold capasitors.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This delay depends on resistance of the PIC analog switch (less than 5 kOhm)
    and capacitance of the PIC ADC hold capacitor (less than 50pF). Number of NOPs
    should be set to provide a delay at least 1uS (16 NOPs for PIC18 @ 48MHz).
***************************************************************************/
#define MTouchSettleDelay()              {\
                                         Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();\
                                         Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();\
                                         }

/****************************************************************************
  Macro:
    MTouchSetCTMUCurrent(current)

  Summary:
    Sets CTMU current.

  Description:
    Sets CTMU current range and trim bits.
 
  Precondition:
    None.

  Parameters:
    current - current value. Bits 1-0 define the current source range (IRNG) and
              bits 7-2 define current trim value (ITRIM, signed).
  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchSetCTMUCurrent(current)   {CTMUCON2 = (current);}

#else

#define MTouchSetCTMUCurrent(current)   {CTMUICON = (current);} 

#endif        

/****************************************************************************
  Macro:
    MTouchCTMUInit()

  Summary:
    Initializes PIC CTMU module. Used for CTMU acquisition only.

  Description:
    Initializes PIC CTMU module.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchCTMUInit()                {CTMUCON1 = 0; CTMUCON2 = 0; CTMUCON3 = 0; CTMUCON4 = 0; MTouchSetCTMUCurrent(MTOUCH_CTMU_CURRENT); CTMUCON1bits.CTMUEN = 1;}

#else

#define MTouchCTMUInit()                {CTMUCONL = 0; CTMUCONH = 0; MTouchSetCTMUCurrent(MTOUCH_CTMU_CURRENT); CTMUCONHbits.CTMUEN = 1;}

#endif
	

/****************************************************************************
  Macro:
    MTouchCTMURearm()

  Summary:
    Switch off CTMU current source. Used for CTMU acquisition only.

  Description:
    Sets both edge flags to inactive state (CTMU current source is off).
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchCTMURearm()               {CTMUCON3 = CTMUCON3&0xFC;}

#else

#define MTouchCTMURearm()               {CTMUCONL = CTMUCONL&0xFC;}

#endif

/****************************************************************************
  Macro:
    MTouchCTMUStart()

  Summary:
    Starts CTMU current source. Used for CTMU acquisition only.

  Description:
    Starts charging of sensor.
 
  Precondition:
    Both edge flags must be zero.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchCTMUStart()               {CTMUCON4bits.EDG1STAT = 1;}

#else

#define MTouchCTMUStart()               {CTMUCONLbits.EDG1STAT = 1;}

#endif

/****************************************************************************
  Macro:
    MTouchCTMUStop()

  Summary:
    Stops CTMU current source. Used for CTMU acquisition only.

  Description:
    Stops charging of sensor.
 
  Precondition:
    Edge 2 flag must be zero.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchCTMUStop()                {CTMUCON4bits.EDG1STAT = 0;}

#else

#define MTouchCTMUStop()                {CTMUCONLbits.EDG1STAT = 0;}

#endif

/****************************************************************************
  Macro:
    MTouchADCInit()

  Summary:
    Initializes ADC module.

  Description:
    Initializes ADC module.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_USE_10_BITS_ADC)

#if defined(__18F66J90) || defined(__18F67J90) || defined(__18F86J90) || defined(__18F87J90)\
 || defined(__18F24K50) || defined(__18F25K50) || defined(__18F45K50)\
 || defined(__18LF24K50)|| defined(__18LF25K50)|| defined(__18LF45K50)

#define MTouchADCInit()                 {ADCON0=0; ADCON1=0; ADCON2=0; ADCON2bits.ADFM=1; ADCON2bits.ACQT=0;\
                                         ADCON2bits.ADCS = 1; ADCON0bits.ADON = 1;}

#else

#define MTouchADCInit()                 {ADCON0=0; ADCON1=0; ADCON1bits.ADFM=1; ADCON1bits.ACQT=0;\
                                         ADCON1bits.ADCS = 1; ADCON0bits.ADON = 1;}
#endif

#endif

#if defined(MTOUCH_USE_12_BITS_ADC)

#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchADCInit() 				{ADCON1H=0; ADCON1L=0; ADCON2H=0; ADCON2L=0; ADCON3H=0; ADCON3L=0; ADCON5L=0; ADCON5H=0;\
										 ADCON1Hbits.FORM=1;ADCON1Hbits.MODE12=1; ADCON3Hbits.SAMC=0;ADCON3Lbits.ADCS=1; ADCON1Hbits.ADON=1;}
#else

#define MTouchADCInit()                 {ADCON0=0; ADCON1=0; ADCON2=0; ADCON2bits.ADFM=1; ADCON2bits.ACQT=0;\
                                         ADCON2bits.ADCS = 1; ADCON0bits.ADON = 1;}
#endif
#endif

/****************************************************************************
  Macro:
    MTouchADCSetChannel(channel)

  Summary:
    Sets ADC channel.

  Description:
    Sets ADC channel.
 
  Precondition:
    None.

  Parameters:
    channel - analog channel number.

  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchADCSetChannel(channel)    {ADCHS0L = channel;} 

#else

#define MTouchADCSetChannel(channel)    {ADCON0bits.CHS = channel;} 

#endif

/****************************************************************************
  Macro:
    MTouchADCStart()

  Summary:
    Starts ADC conversion and waits for it is done.

  Description:
    Starts ADC conversion and waits for it is done.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchADCStart()                {ADCON1Lbits.SAMP = 0;while(!ADCON1Lbits.DONE);}

#else

#define MTouchADCStart()                {ADCON0bits.DONE = 1;while(ADCON0bits.DONE);}

#endif

/****************************************************************************
  Macro:
    MTouchADCGetResult()

  Summary:
    Returns result of ADC conversion.

  Description:
    Returns result of ADC conversion.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Result of ADC conversion.

  Remarks:
    None.
***************************************************************************/
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#define MTouchADCGetResult() 			((INT16)ADCBUF0)

#else

#define MTouchADCGetResult()             (((INT16)ADRESH << 8)+ADRESL)

#endif


#endif
