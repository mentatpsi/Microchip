/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        PIC24FJXXXGA3XXScanInSleep.c
 *  Dependencies:    PIC24FJXXXGA3XXScanInSleep.h
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

#include "PIC24FJXXXGA3XXScanInSleep.h" 

void PIC24FJXXXGA3XXScanInSleep(void)
{
UINT8   i;
SENSOR* pSensor;
UINT8   channelNumber;
SFR     tris;
IOMASK  ioMask;
volatile unsigned int* pBuffer;
UINT16  threshold;


    //////////////////////////////////////////////// 
    // Timer1 setup.
    // Timer1 will be used to run ADC conversion in sleep.
    //////////////////////////////////////////////// 
    T1CON = 0;
	TMR1=0;
	PR1 = SCAN_PERIOD;
    // Internal LPRC is used to clock the timer.
    T1CONbits.TCS = 1;
    T1CONbits.TIECS = 2;
    // Enable timer.
    T1CONbits.TON = 1;

    //////////////////////////////////////////////// 
    // CTMU setup.
    // CTMU will charge sensors.
    ////////////////////////////////////////////////    
    // Clear previous settings.
    CTMUCON1 = 0;
    CTMUCON2 = 0;
    CTMUICON = 0;
    // Set CTMU current.
    CTMUICON = (SCAN_CTMU_CURRENT<<8);
    // Enable CTMU.
    CTMUCON1bits.CTMUEN = 1;

    //////////////////////////////////////////////// 
    // ADC setup.
    ////////////////////////////////////////////////    
    // Clear previous settings.
    AD1CHS = 0;
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON4 = 0;
    AD1CON5 = 0;

    // 12bit conversion.
    AD1CON1bits.MODE12 = 1;
    // Conversion is executed by timer 1 in low power mode.
    AD1CON1bits.SSRC = 6;

    // Each channel has a separate buffer.
    AD1CON2bits.BUFREGEN = 1;
    // Inputs scan is enabled.
    AD1CON2bits.CSCNA = 1;
    // Number of inputs to scan.
    AD1CON2bits.SMPI = (MTOUCH_SENSORS_NUMBER-1);

    // ADC is clocked by LPRC.
    AD1CON3bits.ADRC = 1;
    // Each Tad is one LPRC period.
    AD1CON3bits.ADCS = 7;
    // Sample time is one Tad (CTMU charge delay).
    AD1CON3bits.SAMC = 7;
     
    // Define inputs to scan.
    pSensor = (SENSOR*)sensors;
    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        channelNumber = pSensor->channelNumber;
        tris = pSensor->trisReg;
        ioMask = pSensor->ioMask;

        // Disconnect sensor.
        *tris |= ioMask;

        if(channelNumber > 15)
        {
#if defined(__PIC24FJ64GA310__) || defined(__PIC24FJ128GA310__)
            channelNumber -= 16;
            // Add the channel to scan.
            AD1CSSH  |= (1<<channelNumber);
            // Add CTMU for this channel.
            AD1CTMENH |= (1<<channelNumber);
#endif
        }else{
            // Add the channel to scan.
            AD1CSSL  |= (1<<channelNumber);
            // Add CTMU for this channel.
            AD1CTMENL |= (1<<channelNumber);
        }

        // Move to the next sensor.
        pSensor++;
    }

    // Auto-scan is enabled.
    AD1CON5bits.ASEN = 1;
    // CTMU enabled for scanning.
    AD1CON5bits.CTMREQ = 1;
    // Low power is enabled after scan.
    AD1CON5bits.LPEN = 1;
    // ADC will generate an interrupt and wakeup PIC from sleep
    // after threshold Detect sequence completed and valid compare has occurred.
    AD1CON5bits.ASINT = 3; 
    // Compare mode is LESS THAN.
    AD1CON5bits.CM = 0;

    // Scan result will be written into buffers.
    AD1CON5bits.WM = 0;
    
    // Clear ADC interrupt flag.
    IFS0bits.AD1IF = 0;

    AD1CON1bits.ADON = 1;

    // Wait for 1st scan is completed.
    while(IFS0bits.AD1IF == 0);

    // Stop ADC.
    AD1CON1bits.ADON = 0;

    // Result is used to set thresholds.
    pSensor = (SENSOR*)sensors;
    for(i=0; i<MTOUCH_SENSORS_NUMBER; i++)
    {
        // Calculate buffer address. 
        channelNumber = pSensor->channelNumber;
        pBuffer = &ADC1BUF0 + channelNumber;
        threshold = *pBuffer;

        // Set threshold.
        threshold -= (threshold>>SCAN_THRESHOLD_FACTOR);
        *pBuffer = threshold;

        // Move to the next sensor.
        pSensor++;
    }


    // Scan result will not be written into buffers.
    AD1CON5bits.WM = 2;

    // Clear ADC interrupt flag.
    IFS0bits.AD1IF = 0;

    // Enable ADC interrupt to wakeup.     
    IEC0bits.AD1IE = 1;

    // Enable ADC. 
    AD1CON1bits.ADON = 1;
    
    // Go to sleep and
    // wait for data from sensors will be below threshold.
    Sleep();

    // At this point a press was detected. Resume operation.
    // Disable timer 1.
    T1CON = 0;

    // Clear previous CTMU settings.
    CTMUCON1 = 0;
    CTMUCON2 = 0;
    CTMUICON = 0;

    // Clear previous ADC settings.
    AD1CHS = 0;
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON4 = 0;
    AD1CON5 = 0;
#if defined(__PIC24FJ64GA310__) || defined(__PIC24FJ128GA310__)
    AD1CSSH = 0;
    AD1CTMENH = 0;
#endif
    AD1CSSL = 0;
    AD1CTMENL = 0;

    // Initialize mTouch library to continue the regular sensors' operation.
    MTouchInit();
}

void __attribute__ ((__interrupt__, __no_auto_psv__)) _ADC1Interrupt(void)
{
    // Disable ADC interrupt.
    IEC0bits.AD1IE = 0;
}
