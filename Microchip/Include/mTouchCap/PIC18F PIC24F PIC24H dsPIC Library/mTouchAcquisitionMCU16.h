/*************************************************************************
 *  © 2012 Microchip Technology Inc.                             
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchAcquisitionMCU16.h
 *  Dependencies:    None.
 *  Processor:       PIC24, dsPIC
 *  Compiler:        MPLAB C30, XC16
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
    Provides a delay to discharge sensor.

  Description:
    Provides a delay to discharge sensor.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    This delay depends on resistance of the PIC analog switch (less than 5 kOhm)
    and capacitance of the PIC ADC hold capacitor (less than 50pF). Number of NOPs
    should be set to provide a delay at least 1uS (16 NOPs for PIC24 @ 32MHz).
***************************************************************************/
#define MTouchDischargeDelay()          {asm volatile("Repeat #20"); asm volatile("nop");}

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
#define MTouchChargeDelay(chargeDelay) {asm volatile ("repeat %0" : : "r"((unsigned short)chargeDelay)); asm volatile ("nop");}


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
    should be set to provide a delay at least 1uS (16 NOPs for PIC24 @ 32MHz).
***************************************************************************/
#define MTouchSettleDelay()             {asm volatile("Repeat #20"); asm volatile("nop");}

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
#define MTouchSetCTMUCurrent(current)   {CTMUICON = ((UINT16)(current)<<8);}

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
#if defined(MTOUCH_CTMU_HAS_NO_CTMUCON2_REG)
#define MTouchCTMUInit()                {CTMUCON = 0; MTouchSetCTMUCurrent(MTOUCH_CTMU_CURRENT); CTMUCONbits.CTMUEN = 1;}
#endif
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)
#define MTouchCTMUInit()                {CTMUCON1 = 0; MTouchSetCTMUCurrent(MTOUCH_CTMU_CURRENT); CTMUCON1bits.CTMUEN = 1;}
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
#if defined(MTOUCH_CTMU_HAS_NO_CTMUCON2_REG)
#define MTouchCTMURearm()               {CTMUCON = CTMUCON&0xFFFC;}
#endif
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)
#define MTouchCTMURearm()               {CTMUCON2 = CTMUCON2&0xFCFF;}
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
#if defined(MTOUCH_CTMU_HAS_NO_CTMUCON2_REG)
#define MTouchCTMUStart()               {CTMUCONbits.EDG1STAT = 1;}
#endif
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#if defined(__PIC24F16KA301__)|| defined (__PIC24F16KA302__) || defined(__PIC24F16KA304__)\
|| defined(__PIC24F32KA301__) || defined (__PIC24F32KA302__) || defined(__PIC24F32KA304__)\
|| defined(__PIC24FV16KA301__)|| defined (__PIC24FV16KA302__)|| defined(__PIC24FV16KA304__)\
|| defined(__PIC24FV32KA301__)|| defined (__PIC24FV32KA302__)|| defined(__PIC24FV32KA304__)

#define MTouchCTMUStart()               {CTMUCON2bits.EDG1 = 1;}

#else

#define MTouchCTMUStart()               {CTMUCON2bits.EDG1STAT = 1;}

#endif

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
#if defined(MTOUCH_CTMU_HAS_NO_CTMUCON2_REG)
#define MTouchCTMUStop()                {CTMUCONbits.EDG2STAT = 1;}
#endif
#if defined(MTOUCH_CTMU_HAS_CTMUCON2_REG)

#if defined(__PIC24F16KA301__)|| defined (__PIC24F16KA302__) || defined(__PIC24F16KA304__)\
|| defined(__PIC24F32KA301__) || defined (__PIC24F32KA302__) || defined(__PIC24F32KA304__)\
|| defined(__PIC24FV16KA301__)|| defined (__PIC24FV16KA302__)|| defined(__PIC24FV16KA304__)\
|| defined(__PIC24FV32KA301__)|| defined (__PIC24FV32KA302__)|| defined(__PIC24FV32KA304__)

#define MTouchCTMUStop()                {CTMUCON2bits.EDG2 = 1;}

#else

#define MTouchCTMUStop()                {CTMUCON2bits.EDG2STAT = 1;}

#endif

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
    For PIC24 ADC the ASAM bit must be set (sample switch is closed
    when ADC is idle).
***************************************************************************/
#if defined(MTOUCH_USE_10_BITS_ADC)
#define MTouchADCInit()                 {AD1CON1 = 0;AD1CON2 = 0;AD1CON3 = 0; AD1CON1bits.ASAM = 1; AD1CON3bits.SAMC = 1; AD1CON3bits.ADCS = 2; AD1CON1bits.ADON = 1;}
#endif
#if defined(MTOUCH_USE_12_BITS_ADC)
#if defined(__PIC24F__)
#define MTouchADCInit()                 {AD1CON1 = 0;AD1CON2 = 0;AD1CON3 = 0; AD1CON1bits.ASAM = 1; AD1CON3bits.SAMC = 1; AD1CON3bits.ADCS = 2; AD1CON1bits.MODE12 = 1; AD1CON1bits.ADON = 1;}
#elif defined(__dsPIC30F__)
#define MTouchADCInit()                 {ADCON1 = 0;ADCON2 = 0;ADCON3 = 0; ADCON1bits.ASAM = 1; ADCON3bits.SAMC = 1; ADCON3bits.ADCS = 2; ADCON1bits.ADON = 1;}
#else
#define MTouchADCInit()                 {AD1CON1 = 0;AD1CON2 = 0;AD1CON3 = 0; AD1CON1bits.ASAM = 1; AD1CON3bits.SAMC = 1; AD1CON3bits.ADCS = 2; AD1CON1bits.AD12B = 1; AD1CON1bits.ADON = 1;}
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
#if defined(__PIC24F__)
#define MTouchADCSetChannel(channel)    {AD1CHS = channel;} 
#elif defined(__dsPIC30F__)
#define MTouchADCSetChannel(channel)    {ADCHS = channel;} 
#else
#define MTouchADCSetChannel(channel)    {AD1CHS0 = channel;} 
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
#if defined(__dsPIC30F__)
#define MTouchADCStart()                {ADCON1bits.SAMP = 0; Nop(); Nop();Nop(); Nop();Nop(); Nop(); while(!ADCON1bits.DONE);}
#else
#define MTouchADCStart()                {AD1CON1bits.SAMP = 0; Nop(); Nop();Nop(); Nop();Nop(); Nop(); while(!AD1CON1bits.DONE);}
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
#if defined(__dsPIC30F__)
#define MTouchADCGetResult()           ((INT16)ADCBUF0)
#else
#define MTouchADCGetResult()           ((INT16)ADC1BUF0)
#endif

#endif
