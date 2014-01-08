/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        mTouchAcquisition.c
 *  Dependencies:    mTouch.h
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch sensors acquisition module.
 *                   This module provides a group of functions performing the
 *                   sensor acquisition using CVD or CTMU.
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


///////////////////////////////
// INCLUDES 
///////////////////////////////
#include "mTouch.h"

///////////////////////////////
// FUNCTIONS
///////////////////////////////
#if defined(MTOUCH_USE_CTMU)

volatile INT8 chargeDelay;

INT16 MTouchAcquisitionCTMU(SENSOR* pSensor)
{
SFR      tris;
SFR      lat;
IOMASK   ioMask;
UINT8    channelNumber;
INT16    result;

    // Read sensor info
    tris          = pSensor->trisReg;
    lat           = pSensor->latReg;
    ioMask        = pSensor->ioMask;
    channelNumber = pSensor->channelNumber;
    chargeDelay   = (UINT16)pSensor->chargeDelay;

 	// Rearm the CTMU (clear CTMUCON.EDGE1STAT and CTMUCON.EDGE2STAT bits simultaneously)
    MTouchCTMURearm();

    // Switch channel to the current sensor
    MTouchADCSetChannel(channelNumber);

    // Set current sensor to GND (discharge external)
    *lat &= ~ioMask; *tris &= ~ioMask;

    // Wait to discharge
    MTouchDischargeDelay();

    // Disconnect current sensor from GND (HI-Z)
    *tris |= ioMask;

    // Start charging
	MTouchCTMUStart();

    // Wait for the sensor charge
    MTouchChargeDelay(chargeDelay);

    // Stop charging
	MTouchCTMUStop();

    // Start conversion
    MTouchADCStart();

    // Ground sensor
    *tris &= ~ioMask;

    // Get result 
#if defined(MTOUCH_USE_12_BITS_ADC)
    result = 4096 - MTouchADCGetResult();
#else
    result = 1024 - MTouchADCGetResult();
#endif

    return result;
}

#elif defined(MTOUCH_USE_CVD)

INT16 MTouchAcquisitionCVD(SENSOR* pCurrentSensor, SENSOR* pPreviousSensor)
{
SFR     prevTris;
SFR     prevLat;
IOMASK  prevIOMask;
SFR     curTris;
SFR     curLat;
IOMASK  curIOMask;
UINT8   prevChannelNumber;
UINT8   curChannelNumber;
INT16   result;


    // Read sensor info
    prevTris          = pPreviousSensor->trisReg;
    prevLat           = pPreviousSensor->latReg;
    prevIOMask        = pPreviousSensor->ioMask;
    curTris           = pCurrentSensor->trisReg;
    curLat            = pCurrentSensor->latReg;
    curIOMask         = pCurrentSensor->ioMask;
    prevChannelNumber = pPreviousSensor->channelNumber;
    curChannelNumber  = pCurrentSensor->channelNumber;


    // Scan A

    // Set channel to the previous sensor
    MTouchADCSetChannel(prevChannelNumber);

    // Set previous sensor to Vdd (charge internal)
    *prevLat |= prevIOMask; *prevTris &= ~prevIOMask;

    // Set current sensor to GND (discharge external)
    *curLat &= ~curIOMask; *curTris &= ~curIOMask;
     
    // Wait to charge/discharge
    MTouchDischargeDelay();

    // Disconnect current sensor from GND (HI-Z)
    *curTris |= curIOMask;

    // Switch channel to the current sensor
    MTouchADCSetChannel(curChannelNumber);

    // Wait to settle
    MTouchSettleDelay();

    // Start conversion
    MTouchADCStart();

    // Ground sensors
    *curTris &= ~curIOMask;
    *prevLat &= ~prevIOMask;

    // Get result
#if defined(MTOUCH_USE_12_BITS_ADC)
    result = 4096;
#else
    result = 1024;
#endif
    result -= MTouchADCGetResult();

    // Scan B

    // Switch channel to the previous sensor
    MTouchADCSetChannel(prevChannelNumber);

    // Set previous sensor to GND (discharge internal)
    *prevLat &= ~prevIOMask; *prevTris &= ~prevIOMask;

    // Set current sensor to Vdd (charge external)
    *curLat |= curIOMask; *curTris &= ~curIOMask;

    // Wait to charge/discharge
    MTouchDischargeDelay();

    // Disconnect current sensor from Vdd (HI-Z)
    *curTris |= curIOMask;

    // Switch channel to the current sensor
    MTouchADCSetChannel(curChannelNumber);

    // Wait to settle
    MTouchSettleDelay();

    // Start conversion
    MTouchADCStart();

    // Ground sensors
    *curLat &= ~curIOMask;
    *curTris &= ~curIOMask;

    // Substructing creates the 'differential' value
    result += MTouchADCGetResult();

    return (UINT16)result;
}

#else

#error MTOUCH_USE_CTMU or MTOUCH_USE_CVD must be defined in mTouchConfig.h ...

#endif
