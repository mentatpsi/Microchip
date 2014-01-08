/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        DA210 Graphics Demo Main.c
 *  Dependencies:    see INCLUDE section below
 *  Processor:       PIC24F
 *  Compiler:        MPLAB C30, XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This demo explains how to use mTouch library with
 *                   Graphics library. See ReadMe.txt file for details.
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

/////////////////////////////////////////////////////////////////////////////
//                                INCLUDE
/////////////////////////////////////////////////////////////////////////////        
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "Graphics/Graphics.h"
#include "TouchScreen.h"
#include "GraphicsCallbacks.h"
#include "CapButtons.h"
#include "system.h"

/////////////////////////////////////////////////////////////////////////////
//                    LOCAL VARIABLES AND PROTOTYPES
/////////////////////////////////////////////////////////////////////////////        

// ADC scheduler states
enum {
SCHEDULER_TOUCH_SCREEN,
SCHEDULER_CAPTOUCH_BUTTONS
} ADC_SCHEDULER_STATE;

// ADC current scheduler state
volatile UINT8  schedulerState = SCHEDULER_TOUCH_SCREEN;


/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
int main(void)
{
// GOL message structure to interact with graphics library
GOL_MSG msg;
   
    // Graphics library initialization
    GOLInit(); 

    // Initialize the capacitive buttons
    CapButtonsInit();

    // Initialize DA210 board hardware 
    SysBoardInit();

    // Initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL);   	
    
    // Create controls on the screen
    CreateScreen();

    while(1)
    {
        // Draw GOL object
        if(GOLDraw())
        {                           
            CapButtonsGetMsg(&msg); // Get message from capacitive buttons
            GOLMsg(&msg);           // Process message           

            TouchGetMsg(&msg);      // Get message from touch screen
            GOLMsg(&msg);           // Process message           
        }
    }

}

// This function is called every 0.5ms by timer 4 interrupt.
void SysTimerInterrupt(void)
{
    switch(schedulerState)
    {
        case SCHEDULER_TOUCH_SCREEN:
        {
            // Sample touch screen.
            if(TouchDetectPosition())
            {
                // Touch screen acquisition is done at this point.
                schedulerState = SCHEDULER_CAPTOUCH_BUTTONS;

                // Initialize ADC for Cap Touch Buttons operation.
                MTouchInit();
            }
        }
        break;

        case SCHEDULER_CAPTOUCH_BUTTONS:
        {
            // Switch to the next state.
            schedulerState = SCHEDULER_TOUCH_SCREEN;

            // Disconnect LEDs from touch pads.
            Led_ALLOff();

            // Sample cap touch buttons.
            MTouchAcquisition();
  
            // Initialize ADC for Touch Screen operation.
            Touch_ADCInit();

            // Turn on indication LED is the felt button is pressed.
            if(leftLedState){ Led0On();}

            // Turn on indication LED is the right button is pressed.
            if(rightLedState){ Led2On();}
            
        }
        break;
    }
}

