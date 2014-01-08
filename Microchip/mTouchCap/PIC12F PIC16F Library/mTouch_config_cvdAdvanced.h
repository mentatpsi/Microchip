/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mTouch_config_cvdAdvanced.h
 *  Dependencies:    
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     Advanced mTouch CVD Configuration File
 *                   - See documentation for better explanations of all
 *                     configuration options.
 *************************************************************************/
/***********************************************************************
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

/** @file   mTouch_config_cvdAdvanced.h
*   @brief  Framework Configuration! Advanced CVD Scanning Options
*/


//****************************************************************************************************
// __        __               __                        _____ _           _             
// \ \      / /_ ___   _____ / _| ___  _ __ _ __ ___   |_   _(_)_ __ ___ (_)_ __   __ _ 
//  \ \ /\ / / _` \ \ / / _ \ |_ / _ \| '__| '_ ` _ \    | | | | '_ ` _ \| | '_ \ / _` |
//   \ V  V / (_| |\ V /  __/  _| (_) | |  | | | | | |   | | | | | | | | | | | | | (_| |
//    \_/\_/ \__,_| \_/ \___|_|  \___/|_|  |_| |_| |_|   |_| |_|_| |_| |_|_|_| |_|\__, |
//                                                                                |___/ 
//****************************************************************************************************
    
    // NOTE: If this is a HARDWARE CVD processor, these options do not apply. See the bottom of this
    //       file for all related options.
    
    #define CVD_SETTLING_DELAY          5       // <-- Adjusts the number of additional instruction cycles
                                                //     to add to the Chold&Csensor settling time.
                                                //     The larger this value is, the more susceptible to
                                                //     noise the application will be. However, there is a
                                                //     minimum amount of time required to get full 
                                                //     sensitivity. 
                                                //     For information on how to tune this parameter,
                                                //     please refer to the mTouch Framework documentation.

    #define CVD_CHOLD_CHARGE_DELAY      5       // <-- Adjusts the number of additional instruction cycles
                                                //     to add to the Chold-charge stage of the CVD scan.
                                                //     For information on how to tune this parameter,
                                                //     please refer to the mTouch Framework documentation.

    #define CVD_SWITCH_DELAY            0       // <-- Ideal value for noise immunity is 0.
                                                //     Determines how many NOPs will be added between
                                                //     when the sensor is set to an input and the ADC
                                                //     mux is pointed to the sensor. This value should 
                                                //     ONLY be increased if the output driver is not 
                                                //     disabling itself quickly enough. This will be 
                                                //     noticeable on the second-sample of the waveform: 
                                                //     the settling point will be slightly unstable and 
                                                //     will be visibly coupling to VDD.
                                                //     The behavior is most-noticeable on the second scan
                                                //     due to P-type transistors being less ideal than
                                                //     N-type transistors.
                                                //     The behavior is most-common when Fosc is a high
                                                //     value (16MHz or greater).
                                                //     If you are seeing the behavior, the correct value 
                                                //     for this option will vary based on Fosc and the  
                                                //     microcontroller in question; but, in general,  
                                                //     1-3 NOPs for 16MHz, 3-5 for 32MHz.
                                                
                                                
    //#define CVD_FORCE_REF_DAC                 // <-- If defined, the framework will use the DAC as the
                                                //     reference source option.

    
    //#define CVD_JITTER_2ND_SAMPLE             // <-- If defined, the second CVD sample of the differential
                                                //     CVD waveform will be randomly delayed. This can help
                                                //     to reduce radiated emissions and may increase noise
                                                //     immunity.
                                                //     NOTE: Do not define if normal jittering is not implemented.
                                                
        #define CVD_JITTER_2ND_MASK     0x3F    // <-- Determines the mask used on the 8-bit random variable
                                                //     when loading the decrementing delay counter. The
                                                //     more bits are kept, the larger the randomness.

    
    
//****************************************************************************************************
//   ____                     _   ____  _           
//  / ___|_   _  __ _ _ __ __| | |  _ \(_)_ __  ___ 
// | |  _| | | |/ _` | '__/ _` | | |_) | | '_ \/ __|
// | |_| | |_| | (_| | | | (_| | |  __/| | | | \__ \
//  \____|\__,_|\__,_|_|  \__,_| |_|   |_|_| |_|___/
//
//****************************************************************************************************

    // NOTE: If this is a HARDWARE CVD processor, these options do not apply. See the bottom of this
    //       file for all related options.
    
    //#define CVD_GUARD_IO_ENABLED                                  // <-- If defined, the framework will implement a 
                                                                    //     guard sensor using an IO port.
                                                                    //     For information about guard sensors, please refer 
                                                                    //     to the mTouch Framework documentation.
                                                                
        #define CVD_GUARD_PORT              LATA                    // <-- The guard IO's LAT register, prefixed with an underscore.
        #define CVD_GUARD_PIN               2                       // <-- The guard IO's pin number

        
        
    //#define CVD_GUARD_DACOUT_ENABLED                              // <-- If defined, the framework will implement a
                                                                    //     guard sensor using the DAC's DACOUT pin.
                                                                    //     If available, this can produce better results 
                                                                    //     than the regular IO guard.
                                                                    
        #define CVD_GUARD_DACCON0_A     PIC_DACOUT_DACCON0_HALF_VDD // <-- This is the value that will be loaded into the DACCON0
                                                                    //     register prior to ScanA of the differential CVD scan.                                                                    
        #define CVD_GUARD_DACCON1_A     PIC_DACOUT_DACCON1_HALF_VDD // <-- This is the value that will be loaded into the DACCON1
                                                                    //     register prior to ScanA of the differential CVD scan.   
        #define CVD_GUARD_DACCON0_B     PIC_DACOUT_DACCON0_HALF_VDD // <-- This is the value that will be loaded into the DACCON0  
                                                                    //     register prior to ScanB of the differential CVD scan.                  
        #define CVD_GUARD_DACCON1_B     PIC_DACOUT_DACCON1_HALF_VDD // <-- This is the value that will be loaded into the DACCON1  
                                                                    //     register prior to ScanB of the differential CVD scan.  
                                                                    //     Valid Values:
                                                                    //          - Custom hex value for DACCON register (ex: 0xA0)
                                                                    //          - One of the following predefined values:
                                                                    //              - PIC_DACOUT_DACCON0_VSS        /   PIC_DACOUT_DACCON1_VSS
                                                                    //              - PIC_DACOUT_DACCON0_1_3RD_VDD  /   PIC_DACOUT_DACCON1_1_3RD_VDD
                                                                    //              - PIC_DACOUT_DACCON0_HALF_VDD   /   PIC_DACOUT_DACCON1_HALF_VDD
                                                                    //              - PIC_DACOUT_DACCON0_2_3RD_VDD  /   PIC_DACOUT_DACCON1_2_3RD_VDD
                                                                    //              - PIC_DACOUT_DACCON0_VDD        /   PIC_DACOUT_DACCON1_VDD
    
   


//****************************************************************************************************
//  __  __       _               _   ____                                
// |  \/  |_   _| |_ _   _  __ _| | / ___|  ___ _ __  ___  ___  _ __ ___ 
// | |\/| | | | | __| | | |/ _` | | \___ \ / _ \ '_ \/ __|/ _ \| '__/ __|
// | |  | | |_| | |_| |_| | (_| | |  ___) |  __/ | | \__ \ (_) | |  \__ \
// |_|  |_|\__,_|\__|\__,_|\__,_|_| |____/ \___|_| |_|___/\___/|_|  |___/
//  
//****************************************************************************************************

    // NOTE: If this is a HARDWARE CVD processor, these options do not apply. See the bottom of this
    //       file for all related options.

    //#define CVD_MUTUAL_ENABLED                      // <-- If defined, the framework will integrate a mutual
                                                    //     sensor into the differential CVD scan.
    //#define CVD_MUTUAL_EXCLUSIVE_ENABLED          // <-- If defined, the framework will perform an
                                                    //     exclusively-mutual scan. There will be no charge
                                                    //     averaging between the external and internal caps.
                                                
        #define CVD_MUTUAL_LAT              LATA3   // <-- The mutual sensor's LAT pin label
        #define CVD_MUTUAL_TRIS             TRISA3  // <-- The mutual sensor's TRIS pin label


        
        
        
        

//#define MTOUCH_UNIQUE_OVERSAMPLE_ENABLE

    #define MTOUCH_OVERSAMPLE0          10
    #define MTOUCH_OVERSAMPLE1          10
    #define MTOUCH_OVERSAMPLE2          10
    #define MTOUCH_OVERSAMPLE3          10
    #define MTOUCH_OVERSAMPLE4          10
    #define MTOUCH_OVERSAMPLE5          10
    #define MTOUCH_OVERSAMPLE6          10
    #define MTOUCH_OVERSAMPLE7          10
    #define MTOUCH_OVERSAMPLE8          10
    #define MTOUCH_OVERSAMPLE9          10
    #define MTOUCH_OVERSAMPLE10         10
    #define MTOUCH_OVERSAMPLE11         10
    #define MTOUCH_OVERSAMPLE12         10
    #define MTOUCH_OVERSAMPLE13         10
    #define MTOUCH_OVERSAMPLE14         10
    #define MTOUCH_OVERSAMPLE15         10
    #define MTOUCH_OVERSAMPLE16         10
    #define MTOUCH_OVERSAMPLE17         10
    #define MTOUCH_OVERSAMPLE18         10
    #define MTOUCH_OVERSAMPLE19         10
    #define MTOUCH_OVERSAMPLE20         10
    #define MTOUCH_OVERSAMPLE21         10
    #define MTOUCH_OVERSAMPLE22         10
    #define MTOUCH_OVERSAMPLE23         10
    #define MTOUCH_OVERSAMPLE24         10
    #define MTOUCH_OVERSAMPLE25         10
    #define MTOUCH_OVERSAMPLE26         10
    #define MTOUCH_OVERSAMPLE27         10
    #define MTOUCH_OVERSAMPLE28         10
    #define MTOUCH_OVERSAMPLE29         10
        
        
//#define MTOUCH_UNIQUE_REFERENCE_OPTIONS     // NOTE: CVD_FORCE_REF_DAC overrides this setting at all times.

    #define CVD_REFERENCE_SENSOR0           MTOUCH_SENSOR1    
    #define CVD_REFERENCE_SENSOR1           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR2           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR3           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR4           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR5           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR6           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR7           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR8           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR9           MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR10          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR11          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR12          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR13          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR14          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR15          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR16          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR17          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR18          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR19          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR20          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR21          MTOUCH_SENSOR0   
    #define CVD_REFERENCE_SENSOR22          MTOUCH_SENSOR0 
    #define CVD_REFERENCE_SENSOR23          MTOUCH_SENSOR0  
    #define CVD_REFERENCE_SENSOR24          MTOUCH_SENSOR0  
    #define CVD_REFERENCE_SENSOR25          MTOUCH_SENSOR0  
    #define CVD_REFERENCE_SENSOR26          MTOUCH_SENSOR0  
    #define CVD_REFERENCE_SENSOR27          MTOUCH_SENSOR0  
    #define CVD_REFERENCE_SENSOR28          MTOUCH_SENSOR0  
    #define CVD_REFERENCE_SENSOR29          MTOUCH_SENSOR0    


//
// HARDWARE CVD OPTIONS - Does not affect anything if a non-Hardware-CVD ADC is used.
//

    #define HCVD_PRECHARGE_DELAY        5
    #define HCVD_SETTLING_DELAY         5

    // #define HCVD_GUARD_ENABLE       
    
        #define HCVD_GRDAOE_BITVALUE    1
        #define HCVD_GRDBOE_BITVALUE    1
        
    #define HCVD_ADDCAP_AUTOCAL_ENABLE
    
    