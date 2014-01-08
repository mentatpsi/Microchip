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
            
            #define LED0   LATC0
            #define LED1   LATC0
            #define LED2   LATC0
            #define LED3   LATC0
            #define LED4   LATC0
            #define LED5   LATC0
            #define LED6   LATC0
            #define LED7   LATC0
            #define LED8   LATC0
            #define LED9   LATC0
            #define LED10  LATC0
            #define LED11  LATC0
            #define LED12  LATC0
            #define LED13  LATC0
            #define LED14  LATC0
            #define LED15  LATC0
            #define LED16  LATC0
            #define LED17  LATC0
            #define LED18  LATC0
            #define LED19  LATC0
            #define LED20  LATC0
            #define LED21  LATC0
            #define LED22  LATC0
            #define LED23  LATC0
            #define LED24  LATC0
            #define LED25  LATC0
            #define LED26  LATC0
            #define LED27  LATC0
            #define LED28  LATC0
            #define LED29  LATC0

            if (mTouch_GetButtonState(0)  < MTOUCH_PRESSED) { LED0  = LED_OFF; } else { LED0  = LED_ON; }  // Sensor0
            if (mTouch_GetButtonState(1)  < MTOUCH_PRESSED) { LED1  = LED_OFF; } else { LED1  = LED_ON; }  // Sensor1
            if (mTouch_GetButtonState(2)  < MTOUCH_PRESSED) { LED2  = LED_OFF; } else { LED2  = LED_ON; }  // Sensor2
            if (mTouch_GetButtonState(3)  < MTOUCH_PRESSED) { LED3  = LED_OFF; } else { LED3  = LED_ON; }  // Sensor3
            if (mTouch_GetButtonState(4)  < MTOUCH_PRESSED) { LED4  = LED_OFF; } else { LED4  = LED_ON; }  // Sensor4
            if (mTouch_GetButtonState(5)  < MTOUCH_PRESSED) { LED5  = LED_OFF; } else { LED5  = LED_ON; }  // Sensor5
            if (mTouch_GetButtonState(6)  < MTOUCH_PRESSED) { LED6  = LED_OFF; } else { LED6  = LED_ON; }  // Sensor6
            if (mTouch_GetButtonState(7)  < MTOUCH_PRESSED) { LED7  = LED_OFF; } else { LED7  = LED_ON; }  // Sensor7
            if (mTouch_GetButtonState(8)  < MTOUCH_PRESSED) { LED8  = LED_OFF; } else { LED8  = LED_ON; }  // Sensor8
            if (mTouch_GetButtonState(9)  < MTOUCH_PRESSED) { LED9  = LED_OFF; } else { LED9  = LED_ON; }  // Sensor9
            if (mTouch_GetButtonState(10) < MTOUCH_PRESSED) { LED10 = LED_OFF; } else { LED10 = LED_ON; }  // Sensor10
            if (mTouch_GetButtonState(11) < MTOUCH_PRESSED) { LED11 = LED_OFF; } else { LED11 = LED_ON; }  // Sensor11
            if (mTouch_GetButtonState(12) < MTOUCH_PRESSED) { LED12 = LED_OFF; } else { LED12 = LED_ON; }  // Sensor12
            if (mTouch_GetButtonState(13) < MTOUCH_PRESSED) { LED13 = LED_OFF; } else { LED13 = LED_ON; }  // Sensor13
            if (mTouch_GetButtonState(14) < MTOUCH_PRESSED) { LED14 = LED_OFF; } else { LED14 = LED_ON; }  // Sensor14
            if (mTouch_GetButtonState(15) < MTOUCH_PRESSED) { LED15 = LED_OFF; } else { LED15 = LED_ON; }  // Sensor15
            if (mTouch_GetButtonState(16) < MTOUCH_PRESSED) { LED16 = LED_OFF; } else { LED16 = LED_ON; }  // Sensor16
            if (mTouch_GetButtonState(17) < MTOUCH_PRESSED) { LED17 = LED_OFF; } else { LED17 = LED_ON; }  // Sensor17
            if (mTouch_GetButtonState(18) < MTOUCH_PRESSED) { LED18 = LED_OFF; } else { LED18 = LED_ON; }  // Sensor18
            if (mTouch_GetButtonState(19) < MTOUCH_PRESSED) { LED19 = LED_OFF; } else { LED19 = LED_ON; }  // Sensor19
            if (mTouch_GetButtonState(20) < MTOUCH_PRESSED) { LED20 = LED_OFF; } else { LED20 = LED_ON; }  // Sensor20
            if (mTouch_GetButtonState(21) < MTOUCH_PRESSED) { LED21 = LED_OFF; } else { LED21 = LED_ON; }  // Sensor21
            if (mTouch_GetButtonState(22) < MTOUCH_PRESSED) { LED22 = LED_OFF; } else { LED22 = LED_ON; }  // Sensor22
            if (mTouch_GetButtonState(23) < MTOUCH_PRESSED) { LED23 = LED_OFF; } else { LED23 = LED_ON; }  // Sensor23
            if (mTouch_GetButtonState(24) < MTOUCH_PRESSED) { LED24 = LED_OFF; } else { LED24 = LED_ON; }  // Sensor24
            if (mTouch_GetButtonState(25) < MTOUCH_PRESSED) { LED25 = LED_OFF; } else { LED25 = LED_ON; }  // Sensor25
            if (mTouch_GetButtonState(26) < MTOUCH_PRESSED) { LED26 = LED_OFF; } else { LED26 = LED_ON; }  // Sensor26
            if (mTouch_GetButtonState(27) < MTOUCH_PRESSED) { LED27 = LED_OFF; } else { LED27 = LED_ON; }  // Sensor27
            if (mTouch_GetButtonState(28) < MTOUCH_PRESSED) { LED28 = LED_OFF; } else { LED28 = LED_ON; }  // Sensor28
            if (mTouch_GetButtonState(29) < MTOUCH_PRESSED) { LED29 = LED_OFF; } else { LED29 = LED_ON; }  // Sensor29
            
            
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
    OSCCON  = 0b01111000;       // 16 MHz Fosc 
    
    
    // EXAMPLE PORT INITIALIZATION
    //
    // mTouch sensor pins should be initialized as digital, output low.
    ANSELA  = 0b00000000;
    ANSELB  = 0b00000000;
    ANSELD  = 0b00000000;
    ANSELE  = 0b00000000;
    ANSELF  = 0b00000000;
    ANSELG  = 0b00000000;
    LATA    = 0b00000000;
    LATB    = 0b00000000;
    LATC    = 0b00000000;
    LATD    = 0b00000000;
    LATE    = 0b00000000;
    LATF    = 0b00000000;
    LATG    = 0b00000000;
    TRISA   = 0b00000000;
    TRISB   = 0b00000000;
    TRISC   = 0b00000000;
    TRISD   = 0b00000000;
    TRISE   = 0b00000000;
    TRISF   = 0b00000000;
    TRISG   = 0b00000000;
    
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
