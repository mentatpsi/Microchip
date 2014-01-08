/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        PIC24FJXXXGA3XXScanInSleep.h
 *  Dependencies:    mTouch.h
 *  Processor:       PIC24F
 *  Compiler:        MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to perform scanning of the
 *                   capasitive sensors in sleep for PIC24FJXXXGA3XX devices.
 *                   These devices have a hardware implementation of mTouch
 *                   functionality. In sleep CTMU charges sensors and 
 *                   ADC gets samples. If the sample value is less than
 *                   a threshold set in hardware the PIC wakes up.
 *                   For additional inforamtion about this feature please
 *                   read:
 *                   PIC24F Family Ref. Manual, "Section 51. 12-Bit A/D
 *                   Converter with Threshold Detect"
 *                   and
 *                   PIC24F Family Ref. Manual, "Section 53. Charge Time
 *                   Measurement Unit (CTMU) with Threshold Detect"
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

#ifndef _PIC24FJXXXGA3XX_SCAN_IN_SLEEP_
#define _PIC24FJXXXGA3XX_SCAN_IN_SLEEP_

#include "mTouch.h" 

/****************************************************************************
  Define time between 2 ADC samples. ADC is clocked by LPRC 31kHz oscillator
  (approx. 32uS period).
  ***************************************************************************/
#define SCAN_PERIOD               620   // 18.7 mS
/****************************************************************************
  CTMU current settings.
  Bits 1-0 select the current source range (IRNG) and bits 7-2 select current
  trim value (ITRIM, signed).
  ***************************************************************************/
#define SCAN_CTMU_CURRENT         0x02  // 5.5 uA
/****************************************************************************
  Threshold calculation factor. The threshold will be set
  to (1-1/SCAN_THRESHOLD_FACTOR)x100% of the maximum value for unpressed 
  sensor. Can be set to 1(threshold is 50% of maximum value),
  2(threshold is 75% of maximum value), 3(87.5% of maximum value) and so on.  
  ***************************************************************************/
#define SCAN_THRESHOLD_FACTOR     2     // 75% of maximum sensor value

/****************************************************************************
  Function:
    void PIC24FJXXXGA3XXScanInSleep(void)

  Summary:
    Sets hardware to scan sensor and enters sleep mode.

  Description:
    Sets hardware to scan sensor and enters sleep mode.
    When sample from any sensor is less than threshold the
    function wakes up device.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void PIC24FJXXXGA3XXScanInSleep(void);

#endif
