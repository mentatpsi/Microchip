/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        main.c
 *  Dependencies:    mTouch.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     Main routine
 *                   - Example implementation of the framework's API calls
 *                   - All application code should be implemented in this
 *                     or some other user-created file. This will allow
 *                     for better customer support and easier upgrades to
 *                     later firmware versions.
 *                   - See the documentation located in the docs/ folder
 *                     for a detailed guide on getting started with your
 *                     application and the mTouch framework.
 *************************************************************************/
/*************************************************************************
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
/** @file   main.c
*   @brief  Example implementation of the mTouch Framework's API calls
*/
/// @cond

#include "mTouch.h"                            // Required Include

// CONFIGURATION SETTINGS
#include "generic_processorConfigBits.h"        // Provided for ease-of-development. 
                                                // Should not be used in an actual 
                                                // application.

    // TIP: Valid configuration bit labels can be found in Hi-Tech's folder.
    //      C:\Program Files\HI-TECH Software\<COMPILER NAME>\<VERSION NUMBER>\include
    //      Open the 'pic' header file that corresponds to your microcontroller.
    //      Ex: 'pic16f1937.h'  --  NOT 'as16f1937.h' or 'cas16f1937.h'
    
    //uint8_t     matrixOutput[2];
    
// PROTOTYPES
    void            Example_System_Init (void);
    void interrupt  ISR                 (void);

// START PROGRAM
void main(void)
{
    Example_System_Init();          // Your System Initialization Function

    //================================================================================================
    //  ____                  _              _           _____                _          _    ____ ___ 
    // |  _ \ ___  __ _ _   _(_)_ __ ___  __| |  _ __ __|_   _|__  _   _  ___| |__      / \  |  _ \_ _|
    // | |_) / _ \/ _` | | | | | '__/ _ \/ _` | | '_ ` _ \| |/ _ \| | | |/ __| '_ \    / _ \ | |_) | | 
    // |  _ <  __/ (_| | |_| | | | |  __/ (_| | | | | | | | | (_) | |_| | (__| | | |  / ___ \|  __/| | 
    // |_| \_\___|\__, |\__,_|_|_|  \___|\__,_| |_| |_| |_|_|\___/ \__,_|\___|_| |_| /_/   \_\_|  |___|
    //               |_|                    
    //================================================================================================                                                           
    
    mTouch_Init();                  // mTouch Initialization (Required)
    #if defined(MCOMM_ENABLED)
    mComm_Init();                   // mComm Initialization  (Required for communications)
    #endif                
      
    INTCONbits.GIE = 1;             // Initialization complete. Begin servicing interrupts.
    
    while(1)
    {    
    
        if (mTouch_isDataReady())   // Is new information ready?
        {
            mTouch_Service();       // Decode the newly captured data and transmit new data updates.

    //------------------------------------------------------------------------------------------------
    //                              REQUIRED MTOUCH API ENDS HERE
    //
    // NOTE: The below API examples show some possible ways to integrate your application with
    //       the mTouch framework. These are not required, but may be helpful.
    //------------------------------------------------------------------------------------------------
            
            // The mTouch framework does not have an automated, board-level output system. 
            // All system responses to sensor state changes must be programmed by you, as shown below.

            #define LED_ON  0
            #define LED_OFF 1
            
            #define LED0  LATC0
            #define LED1  LATA6
            #define LED2  LATA7
            #define LED3  LATE2
            #define LED4  LATE1
            #define LED5  LATE0
            #define LED6  LATA3
            #define LED7  LATA2
            #define LED8  LATA1
            #define LED9  LATB7     // ICSPDAT
            #define LED10 LATB6     // ICSPCLK
            #define LED11 LATC7     // RX - Don't use.
            #define LED12 LATC6     // TX - Don't use.
            #define LED13 LATC5
            #define LED14 LATC2
            #define LED15 LATC1
            
            //  ____  _ _     _                _    ____ ___ 
            // / ___|| (_) __| | ___ _ __     / \  |  _ \_ _|
            // \___ \| | |/ _` |/ _ \ '__|   / _ \ | |_) | | 
            //  ___) | | | (_| |  __/ |     / ___ \|  __/| | 
            // |____/|_|_|\__,_|\___|_|    /_/   \_\_|  |___|
             
            // Example slider output code
            #if defined(MTOUCH_SLIDER_SCALING_ENABLED)
                
                // Scaling Enabled: mTouch_GetSlider(i) returns a value between 0 and 255.
                
                    if (mTouch_GetSlider(0) >=  15) { LED0  = LED_ON; } else { LED0  = LED_OFF; }
                    if (mTouch_GetSlider(0) >=  30) { LED1  = LED_ON; } else { LED1  = LED_OFF; }
                    if (mTouch_GetSlider(0) >=  45) { LED2  = LED_ON; } else { LED2  = LED_OFF; }
                    if (mTouch_GetSlider(0) >=  60) { LED3  = LED_ON; } else { LED3  = LED_OFF; }
                    if (mTouch_GetSlider(0) >=  75) { LED4  = LED_ON; } else { LED4  = LED_OFF; }
                    if (mTouch_GetSlider(0) >=  90) { LED5  = LED_ON; } else { LED5  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 105) { LED6  = LED_ON; } else { LED6  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 120) { LED7  = LED_ON; } else { LED7  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 135) { LED8  = LED_ON; } else { LED8  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 150) { LED9  = LED_ON; } else { LED9  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 165) { LED10 = LED_ON; } else { LED10 = LED_OFF; }
                    //if (mTouch_GetSlider(0) >= 180) { LED11 = LED_ON; } else { LED11 = LED_OFF; }     // RX
                    //if (mTouch_GetSlider(0) >= 195) { LED12 = LED_ON; } else { LED12 = LED_OFF; }     // TX
                    if (mTouch_GetSlider(0) >= 210) { LED13 = LED_ON; } else { LED13 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 225) { LED14 = LED_ON; } else { LED14 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 240) { LED15 = LED_ON; } else { LED15 = LED_OFF; }
                
            #else
            
                // Scaling Disabled: mTouch_GetSlider(0) returns a value between 0 and MTOUCH_SLIDER0_MAX_VALUE.
                
                    #define MTOUCH_SLIDER0_MAX_VALUE     ((MTOUCH_NUMBER_SLIDER0_SENSORS - 1) * 256)
                    if (mTouch_GetSlider(0) >= (uint16_t)(  1/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED0  = LED_ON; } else { LED0  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  2/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED1  = LED_ON; } else { LED1  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  3/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED2  = LED_ON; } else { LED2  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  4/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED3  = LED_ON; } else { LED3  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  5/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED4  = LED_ON; } else { LED4  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  6/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED5  = LED_ON; } else { LED5  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  7/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED6  = LED_ON; } else { LED6  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  8/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED7  = LED_ON; } else { LED7  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)(  9/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED8  = LED_ON; } else { LED8  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 10/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED9  = LED_ON; } else { LED9  = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 11/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED10 = LED_ON; } else { LED10 = LED_OFF; }
                    //if (mTouch_GetSlider(0) >= (uint16_t)( 12/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED11 = LED_ON; } else { LED11 = LED_OFF; }   // RX
                    //if (mTouch_GetSlider(0) >= (uint16_t)( 13/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED12 = LED_ON; } else { LED12 = LED_OFF; }   // TX
                    if (mTouch_GetSlider(0) >= (uint16_t)( 14/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED13 = LED_ON; } else { LED13 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 15/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED14 = LED_ON; } else { LED14 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 16/17 * MTOUCH_SLIDER0_MAX_VALUE)) { LED15 = LED_ON; } else { LED15 = LED_OFF; }
                    // NOTE: This is an example of a slider using 8 LEDs to display its current value.
                    //       By defining it like this, the number of sensors in slider 0 can be adjusted 
                    //       without changing how the slider's output is displayed.
            #endif
            
            
        } // end - mTouch_isDataReady() check
    } // end - while(1) main loop
} // end - main() function


//================================================================================================
//  _____                           _        ____            _                   ___       _ _   
// | ____|_  ____ _ _ __ ___  _ __ | | ___  / ___| _   _ ___| |_ ___ _ __ ___   |_ _|_ __ (_) |_ 
// |  _| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \ \___ \| | | / __| __/ _ \ '_ ` _ \   | || '_ \| | __|
// | |___ >  < (_| | | | | | | |_) | |  __/  ___) | |_| \__ \ ||  __/ | | | | |  | || | | | | |_ 
// |_____/_/\_\__,_|_| |_| |_| .__/|_|\___| |____/ \__, |___/\__\___|_| |_| |_| |___|_| |_|_|\__|
//                           |_|                   |___/                          
//================================================================================================               

void Example_System_Init() 
{
    // The mTouch framework controls these modules:
    // *  TMR0  - YOU MUST INIT THE OPTION REGISTER / TMR0 PRESCALER
    //            Do not choose 1:1 prescaling.
    //
    // *  ADC   - automatic initialization
    // *  UART  - automatic initialization (if enabled and available)
    //
    // mTouch performs better as Fosc increases.    
    
    // NOTE: Update the configuration file if Fosc is changed!
    OSCCON  = 0b01110000;       // 32 MHz Fosc w/ PLLEN_ON (config bit)  
    
    
    // EXAMPLE PORT INITIALIZATION
    //
    // mTouch sensor pins should be initialized as digital, output low.
    ANSELA  = 0b00000000;
    ANSELB  = 0b00000000;
    ANSELD  = 0b00000000;
    ANSELE  = 0b00000000;
    LATA    = 0b11001110;
    LATB    = 0b11000000;
    LATC    = 0b11100111;
    LATD    = 0b00000000;
    LATE    = 0b00000111;
    TRISA   = 0b00000000;
    TRISB   = 0b00000000;
    TRISC   = 0b00000000;
    TRISD   = 0b00000000;
    TRISE   = 0b00000000;
    
    // EXAMPLE TIMER INITIALIZATION 
    //
    // Only an 8-bit timer may be used as the mTouch framework timer. 
    // TMR1/3/5 are not currently able to be used for this purpose.
    OPTION_REG  = 0b10000000;   // TMR0 Prescaler  = 1:2
}


//================================================================================================
//  _____                           _        ___ ____  ____  
// | ____|_  ____ _ _ __ ___  _ __ | | ___  |_ _/ ___||  _ \ 
// |  _| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \  | |\___ \| |_) |
// | |___ >  < (_| | | | | | | |_) | |  __/  | | ___) |  _ < 
// |_____/_/\_\__,_|_| |_| |_| .__/|_|\___| |___|____/|_| \_\
//                           |_|                                                                                     
//================================================================================================

void interrupt ISR(void)
{
    // EXAMPLE INTERRUPT SERVICE ROUTINE
    //
    // If MTOUCH_INTEGRATION_TYPE is defined as MTOUCH_CONTROLS_ISR, the framework will implement 
    // a dedicated ISR for the mTouch scans' use. If it is not defined, the application may implement 
    // its own ISR. 
    //
    // A few rules must be followed by custom ISR functions:
    //
    // 1. If MTOUCH_INTEGRATION_TYPE is defined as MTOUCH_CALLED_FROM_MAINLOOP, you must set 
    //    mTouch_state.isrServiced each time you enter the ISR. This tells the framework that the scan 
    //    was interrupted and needs to be repeated.
    //
    // 2. If MTOUCH_INTEGRATION_TYPE is defined as MTOUCH_CALLED_FROM_ISR, the example API usage below 
    //    is required to service mTouch scanning.

    #if defined(MCOMM_ENABLED) && defined(MCOMM_TWO_WAY_ENABLED)  
        // If the V2 comms have been enabled, we need to receive incoming requests.
        #if     (MCOMM_TYPE == MCOMM_UART_TWO_WAY)
        if (MCOMM_UART_RCIE && MCOMM_UART_RCIF)     // Check UART for new data
        #elif   (MCOMM_TYPE == MCOMM_I2C_TWO_WAY)
        if (MCOMM_I2C_SSPIE && MCOMM_I2C_SSPIF)      // Check I2C for new data
        #elif   (MCOMM_TYPE == MCOMM_SPI_TWO_WAY)
        if (MCOMM_SPI_SSPIE && MCOMM_SPI_SSPIF)      // Check SPI for new data
        #endif
        {
            mComm_Receive();            // Two-way Communication Receive Service Function
        }        
    #endif  
    
    
    #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_ISR)
        if (mTouch_checkInterrupt())    // Checks if the TMRxIE and TMRxIF flags are both equal to 1.
        {
            mTouch_Scan();              // Required if running as ISR slave. The mTouch timer interrupt 
                                        // flag is cleared inside the mTouch_Scan() function.
        }
    #elif (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
        mTouch_state.isrServiced = 1;   // Alerts the mTouch scanning routine that an interrupt may 
                                        // have disrupted a scan. This is cleared at the start of a
                                        // new scan and is checked at the end of the scan.
                                        // Bad data can affect the readings if this flag is not set.
    #endif

}



