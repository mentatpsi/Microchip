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
    mComm_Init();
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
    
    
            //  ____                              ____  _        _            _    ____ ___ 
            // / ___|  ___ _ __  ___  ___  _ __  / ___|| |_ __ _| |_ ___     / \  |  _ \_ _|
            // \___ \ / _ \ '_ \/ __|/ _ \| '__| \___ \| __/ _` | __/ _ \   / _ \ | |_) | | 
            //  ___) |  __/ | | \__ \ (_) | |     ___) | || (_| | ||  __/  / ___ \|  __/| | 
            // |____/ \___|_| |_|___/\___/|_|    |____/ \__\__,_|\__\___| /_/   \_\_|  |___| 
            
            // The mTouch framework does not have an automated, board-level output system. 
            // All system responses to sensor state changes must be programmed by you, as shown below.

            #define LED_ON  0
            #define LED_OFF 1
            
            #define LED0  LATC2
            #define LED1  LATC1
            #define LED2  LATC0
            #define LED3  LATA7
            #define LED4  LATA6
            #define LED5  LATA4
            #define LED6  LATD7
            #define LED7  LATD6
            #define LED8  LATD5
            #define LED9  LATD4
            #define LED10 LATD3
            #define LED11 LATD2
            #define LED12 LATD1
            #define LED13 LATD0

            
            // #define LED0 RA2
            // #define LED1 RA3            
            
            // #define LED0 GP1
            // #define LED1 GP2
            
            // Example of a sensor's output being directed to an I/O pin
            //
            // Ex:  (User presses)  LED turns on.
            //      (User releases) LED turns off.
            //      etc.
            
            
            // if (mTouch_GetButtonState(0) < MTOUCH_PRESSED) { LED0 = LED_OFF; } else { LED0 = LED_ON; } // Sensor0
            // if (mTouch_GetButtonState(1) < MTOUCH_PRESSED) { LED1 = LED_OFF; } else { LED1 = LED_ON; } // Sensor1
            // if (mTouch_GetButtonState(2) < MTOUCH_PRESSED) { LED2 = LED_OFF; } else { LED2 = LED_ON; } // Sensor2
            // if (mTouch_GetButtonState(3) < MTOUCH_PRESSED) { LED3 = LED_OFF; } else { LED3 = LED_ON; } // Sensor3
            // if (mTouch_GetButtonState(4) < MTOUCH_PRESSED) { LED4 = LED_OFF; } else { LED4 = LED_ON; } // Sensor4
            // if (mTouch_GetButtonState(5) < MTOUCH_PRESSED) { LED5 = LED_OFF; } else { LED5 = LED_ON; } // Sensor5
            // if (mTouch_GetButtonState(6) < MTOUCH_PRESSED) { LED6 = LED_OFF; } else { LED6 = LED_ON; } // Sensor6
            // if (mTouch_GetButtonState(7) < MTOUCH_PRESSED) { LED7 = LED_OFF; } else { LED7 = LED_ON; } // Sensor7
                

            //  _____                 _           _    ____ ___ 
            // |_   _|__   __ _  __ _| | ___     / \  |  _ \_ _|
            //   | |/ _ \ / _` |/ _` | |/ _ \   / _ \ | |_) | | 
            //   | | (_) | (_| | (_| | |  __/  / ___ \|  __/| | 
            //   |_|\___/ \__, |\__, |_|\___| /_/   \_\_|  |___|
            //            |___/ |___/                       
                        
            // Example of a sensor using the toggle latch. This is a memory-efficient toggle implementation
            // that will alternate between on and off each time the sensor is pressed.
            //
            // Ex:  (User presses)  LED turns on. 
            //      (User releases) LED remains on. 
            //      (User presses)  LED turns off.
            //      (User releases) LED remains off.
            //      (User presses)  LED turns on.
            //      etc
            
            #if defined(MTOUCH_TOGGLE_ENABLED)
            if (mTouch_GetToggleState(0) == MTOUCH_TOGGLE_OFF) { LED0 = LED_OFF; } else { LED0 = LED_ON; }
            #endif
            
            
            //  ____  _ _     _                _    ____ ___ 
            // / ___|| (_) __| | ___ _ __     / \  |  _ \_ _|
            // \___ \| | |/ _` |/ _ \ '__|   / _ \ | |_) | | 
            //  ___) | | | (_| |  __/ |     / ___ \|  __/| | 
            // |____/|_|_|\__,_|\___|_|    /_/   \_\_|  |___|
             
            // Example slider output code
            #if defined(MTOUCH_SLIDERS_ENABLED)
            #if defined(MTOUCH_SLIDER_SCALING_ENABLED)
                
                // Scaling Enabled: mTouch_GetSlider(i) returns a value between 0 and 255.
                #define NUMBER_OF_SLIDER_LED_OUTPUT     8
                
                #if     (NUMBER_OF_SLIDER_LED_OUTPUT ==  8)     // 8-LED Slider Output Example :: 9 distinct slider output values
                    if (mTouch_GetSlider(0) >= 28)  { LED0 = LED_ON; } else { LED0 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 57)  { LED1 = LED_ON; } else { LED1 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 85)  { LED2 = LED_ON; } else { LED2 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 114) { LED3 = LED_ON; } else { LED3 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 142) { LED4 = LED_ON; } else { LED4 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 170) { LED5 = LED_ON; } else { LED5 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 199) { LED6 = LED_ON; } else { LED6 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 227) { LED7 = LED_ON; } else { LED7 = LED_OFF; }
                #elif   (NUMBER_OF_SLIDER_LED_OUTPUT ==  4)     // 4-LED Slider Output Example :: 5 distinct slider output values
                    if (mTouch_GetSlider(0) >= 51)   { LED0 = LED_ON; } else { LED0 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 102)  { LED1 = LED_ON; } else { LED1 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 153)  { LED2 = LED_ON; } else { LED2 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= 204)  { LED3 = LED_ON; } else { LED3 = LED_OFF; }
                #endif
                
            #else
            
                // Scaling Disabled: mTouch_GetSlider(0) returns a value between 0 and MTOUCH_SLIDER0_MAX_VALUE.
                
                    #define MTOUCH_SLIDER0_MAX_VALUE     ((MTOUCH_NUMBER_SLIDER0_SENSORS - 1) * 256)
                    if (mTouch_GetSlider(0) >= (uint16_t)( 1/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED0 = LED_ON; } else { LED0 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 2/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED1 = LED_ON; } else { LED1 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 3/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED2 = LED_ON; } else { LED2 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 4/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED3 = LED_ON; } else { LED3 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 5/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED4 = LED_ON; } else { LED4 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 6/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED5 = LED_ON; } else { LED5 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 7/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED6 = LED_ON; } else { LED6 = LED_OFF; }
                    if (mTouch_GetSlider(0) >= (uint16_t)( 8/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED7 = LED_ON; } else { LED7 = LED_OFF; }
                    // NOTE: This is an example of a slider using 8 LEDs to display its current value.
                    //       By defining it like this, the number of sensors in slider 0 can be adjusted 
                    //       without changing how the slider's output is displayed.
            #endif
            #endif
            
            
            //  __  __       _        _           _    ____ ___ 
            // |  \/  | __ _| |_ _ __(_)_  __    / \  |  _ \_ _|
            // | |\/| |/ _` | __| '__| \ \/ /   / _ \ | |_) | | 
            // | |  | | (_| | |_| |  | |>  <   / ___ \|  __/| | 
            // |_|  |_|\__,_|\__|_|  |_/_/\_\ /_/   \_\_|  |___|
            // Example matrix output code for a 3x4 Matrix

            #if defined(MTOUCH_MATRIX_ENABLED)  
                if (mTouch_Matrix_hasChanged())         // If the matrix value has changed since last time
                {
                    mTouch_Matrix_latchNewValue();      // Save the new value of the matrix
                    
                    LED1  = LED_OFF;                    // Reset all matrix outputs
                    LED2  = LED_OFF;
                    LED3  = LED_OFF;
                    LED4  = LED_OFF;
                    LED5  = LED_OFF;
                    LED6  = LED_OFF;
                    LED7  = LED_OFF;
                    LED8  = LED_OFF;
                    LED9  = LED_OFF;
                    LED10 = LED_OFF;
                    LED11 = LED_OFF;
                    LED12 = LED_OFF;
                }

                if (mTouch_Matrix_isPressed())          // If the matrix is being pressed
                {
                    switch(mTouch_Matrix_getRow())      // Find the row/column being pressed and output accordingly.
                    {
                        case 0:                         
                            switch(mTouch_Matrix_getColumn())
                            {
                                case 0: LED1  = LED_ON; break;  // (0, 0)
                                case 1: LED2  = LED_ON; break;  // (0, 1)
                                case 2: LED3  = LED_ON; break;  // (0, 2)
                                case 3: LED4  = LED_ON; break;  // (0, 3)
                            }
                            break;
                        case 1:
                            switch(mTouch_Matrix_getColumn())
                            {
                                case 0: LED5  = LED_ON; break;  // (1, 0)
                                case 1: LED6  = LED_ON; break;  // (1, 1)
                                case 2: LED7  = LED_ON; break;  // (1, 2)
                                case 3: LED8  = LED_ON; break;  // (1, 3)
                            }
                            break;
                        case 2:
                            switch(mTouch_Matrix_getColumn())
                            {
                                case 0: LED9  = LED_ON; break;  // (2, 0)
                                case 1: LED10 = LED_ON; break;  // (2, 1)
                                case 2: LED11 = LED_ON; break;  // (2, 2)
                                case 3: LED12 = LED_ON; break;  // (2, 3)
                            }
                            break;
                    }
                }
            #endif
            
            //  __  __           _           _    ____ ___ 
            // |  \/  | ___   __| | ___     / \  |  _ \_ _|
            // | |\/| |/ _ \ / _` |/ _ \   / _ \ | |_) | | 
            // | |  | | (_) | (_| |  __/  / ___ \|  __/| | 
            // |_|  |_|\___/ \__,_|\___| /_/   \_\_|  |___|
            // 
            // Modes allow for different sensors to be read in different application states.
            //
            // IMPORTANT NOTE:
            //  The logic, below, assumes debouncing is set to 0. If debouncing is higher
            //  than this, the state of the sensors will be checked before their state has
            //  had an opportunity to update itself. 
            //
            // This is just one way in which the mode switching may be implemented.
            
            
            #if MTOUCH_NUM_MODES > 1
        
            mTouch_DisableScanning();   // Temporarily disable scanning while this logic completes
            
            // Are all of the current sensors initialized? If not, stay in the current mode and keep scanning until 
            // they are. mTouch_state.areInitialized is a single-bit reserved for making temporary, local checks 
            // such as this.
            mTouch_state.areInitialized = 1;
            
            if (mTouch_GetButtonState(0) == MTOUCH_INITIALIZING) {   mTouch_state.areInitialized = 0;    }
            #if (MTOUCH_NUMBER_SENSORS > 1)        
            if (mTouch_GetButtonState(1) == MTOUCH_INITIALIZING) {   mTouch_state.areInitialized = 0;    }
            #endif
            #if (MTOUCH_NUMBER_SENSORS > 2)        
            if (mTouch_GetButtonState(2) == MTOUCH_INITIALIZING) {   mTouch_state.areInitialized = 0;    }
            #endif
            
            if (mTouch_state.areInitialized && !mTouch_state.skippedDecode)
            {
                if (mTouch_modeIndex == 0)
                {
                    if (mTouch_GetButtonState(1) == MTOUCH_PRESSED)
                    {
                        mTouch_ChangeMode(1);
                    }
                }
                else if (mTouch_modeIndex == 1)
                {
                    #if defined(MCOMM_ENABLED) && defined(MCOMM_UART_HARDWARE_USED) 
                                                        // If we are outputting data using a hardware UART...
                        while(MCOMM_UART_TXIF == 0);    // Finish all communications before entering sleep
                    #endif
                    
                    //PIC_SWDTEN_ON();                  // If using a software-enabled WDT, enable it now.
                    //SLEEP();                          // Sleep, if you want to.
                    //NOP();                            // One more instruction is executed before sleeping.
                    
                    if (mTouch_GetButtonState(0) == MTOUCH_PRESSED)
                    {
                        mTouch_ChangeMode(0);
                    }
                    
                    //PIC_SWDTEN_OFF();
                }
            }
            mTouch_EnableScanning();    // Re-enable scanning
            #endif
            
        } // end - mTouch_isDataReady() check
        
        
        
        //  _____                       ____       _            _   _                  _    ____ ___ 
        // | ____|_ __ _ __ ___  _ __  |  _ \  ___| |_ ___  ___| |_(_) ___  _ __      / \  |  _ \_ _|
        // |  _| | '__| '__/ _ \| '__| | | | |/ _ \ __/ _ \/ __| __| |/ _ \| '_ \    / _ \ | |_) | | 
        // | |___| |  | | | (_) | |    | |_| |  __/ ||  __/ (__| |_| | (_) | | | |  / ___ \|  __/| | 
        // |_____|_|  |_|  \___/|_|    |____/ \___|\__\___|\___|\__|_|\___/|_| |_| /_/   \_\_|  |___|
        
        // Checks whether any sensors are shorted to VDD or VSS
        #if defined(MTOUCH_ERROR_DETECTION_ENABLED)
        mTouch_ErrorDetect();
        if (mTouch_state.error)
        {
            // Perform action to notify the user of error.
        }
        #endif
        

        
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
    #if defined(_PIC14E)
        #if     _XTAL_FREQ == 32000000
        OSCCON  = 0b01110000;       // 32 MHz Fosc w/ PLLEN_ON (config bit)  
        #elif   _XTAL_FREQ == 16000000
        OSCCON  = 0b01111000;       // 16 MHz (PLLEN config bit doesn't matter)
        #endif
    #elif defined(_PIC18)
        OSCCONbits.IRCF = 7;        // 8 MHz
    #endif
    
    
    // EXAMPLE PORT INITIALIZATION
    //
    // mTouch sensor pins should be initialized as digital, output low.
    #if   defined(_16F1937)
    ANSELA      = 0b00000000;       
    ANSELB      = 0b00000000;       
    ANSELD      = 0b00000000;
    ANSELE      = 0b00000000;
    TRISA       = 0b00000000;
    TRISB       = 0b00000000;
    TRISC       = 0b00000000;
    TRISD       = 0b00000000;
    TRISE       = 0b00000000;
    PORTA       = 0b11011110;    
    PORTB       = 0b00000000;    
    PORTC       = 0b10100111;    
    PORTD       = 0b11111111;    
    PORTE       = 0b00000111; 
    #endif
    
    // EXAMPLE TIMER INITIALIZATIONS   
    //
    // Only an 8-bit timer may be used as the mTouch framework timer. 
    // TMR1/3/5 are not currently able to be used for this purpose.
    #if defined(MTOUCH_ISR_TIMER)
    #if (MTOUCH_ISR_TIMER == 0)
        #if defined(_PIC18)
        T0CON       = 0b11000000;   // TMR0 is 8-bit. Prescaler = 1:2
        #else
        OPTION_REG  = 0b10000000;   // TMR0 Prescaler  = 1:2
        #endif
    #elif (MTOUCH_ISR_TIMER == 2)
        T2CON       = 0b00001100;   // TMR2 Postscaler = 1:2
        PR2         = 0xFF;
    #elif (MTOUCH_ISR_TIMER == 4)
        T4CON       = 0b00001100;   // TMR4 Postscaler = 1:2
        PR4         = 0xFF;
    #elif (MTOUCH_ISR_TIMER == 6)
        T6CON       = 0b00001100;   // TMR6 Postscaler = 1:2
        PR6         = 0xFF;
    #endif
    #endif
}


//================================================================================================
//  _____                           _        ___ ____  ____  
// | ____|_  ____ _ _ __ ___  _ __ | | ___  |_ _/ ___||  _ \ 
// |  _| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \  | |\___ \| |_) |
// | |___ >  < (_| | | | | | | |_) | |  __/  | | ___) |  _ < 
// |_____/_/\_\__,_|_| |_| |_| .__/|_|\___| |___|____/|_| \_\
//                           |_|                                                                                     
//================================================================================================

#if (MTOUCH_INTEGRATION_TYPE != MTOUCH_CONTROLS_ISR)
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
#endif

/// @endcond


