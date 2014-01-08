/********************************************************************
 FileName:		user.c
 Dependencies:  See INCLUDES section
 Processor:     PIC18, PIC24, and PIC32 USB Microcontrollers
 Hardware:      This demo is natively intended to be used on Microchip USB demo
                boards supported by the MCHPFSUSB stack.  See release notes for
                support matrix.  This demo can be modified for use on other hardware
                platforms.
 Complier:      Microchip C18 (for PIC18), C30 (for PIC24), C32 (for PIC32)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC� Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
  2.9f  05/14/2012   Update to add PIC18F97J94/PIC18F87J94 support.                   
********************************************************************/

/** INCLUDES *******************************************************/
#include "USB/usb.h"
#include "USB/usb_function_generic.h"

#include "HardwareProfile.h"
#include "user.h"
#include "temperature.h"

/** V A R I A B L E S ********************************************************/
#if defined(__18CXX)
    #pragma udata
#endif
BYTE old_sw2,old_sw3;
BYTE counter;
BYTE trf_state;
BYTE temp_mode;

#if defined(__18CXX)
    #pragma udata

    //The INPacket[] and OUTPacket[] arrays are used as
    //USB packet buffers in this firmware.  Therefore, they must be located in
    //a USB module accessible portion of microcontroller RAM.
    #if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
        #pragma udata usbram2
    #elif defined(__18F2455) || defined(__18F2550) || defined(__18F4455) || defined(__18F4550)\
        || defined(__18F2458) || defined(__18F2453) || defined(__18F4558) || defined(__18F4553)\
        || defined(__18LF24K50) || defined(__18F24K50) || defined(__18LF25K50)\
        || defined(__18F25K50) || defined(__18LF45K50) || defined(__18F45K50)
        #pragma udata USB_VARIABLES=0x500
    #elif defined(__18F4450) || defined(__18F2450)
        #pragma udata USB_VARIABLES=0x480
    #else
        #pragma udata
    #endif
#endif

#if defined(__XC8)
    #if defined(_18F14K50) || defined(_18F13K50) || defined(_18LF14K50) || defined(_18LF13K50)
        #define IN_DATA_BUFFER_ADDRESS 0x260
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + USBGEN_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #elif  defined(_18F2455)   || defined(_18F2550)   || defined(_18F4455)  || defined(_18F4550)\
        || defined(_18F2458)   || defined(_18F2453)   || defined(_18F4558)  || defined(_18F4553)\
        || defined(_18LF24K50) || defined(_18F24K50)  || defined(_18LF25K50)\
        || defined(_18F25K50)  || defined(_18LF45K50) || defined(_18F45K50)
        #define IN_DATA_BUFFER_ADDRESS 0x500
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + USBGEN_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #elif defined(_18F4450) || defined(_18F2450)
        #define IN_DATA_BUFFER_ADDRESS 0x480
        #define OUT_DATA_BUFFER_ADDRESS (IN_DATA_BUFFER_ADDRESS + USBGEN_EP_SIZE)
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #elif defined(_16F1459)
        #define IN_DATA_BUFFER_ADDRESS 0x2140
        #define OUT_DATA_BUFFER_ADDRESS 0x2190
        #define IN_DATA_BUFFER_ADDRESS_TAG @IN_DATA_BUFFER_ADDRESS
        #define OUT_DATA_BUFFER_ADDRESS_TAG @OUT_DATA_BUFFER_ADDRESS
    #else
        #define IN_DATA_BUFFER_ADDRESS_TAG
        #define OUT_DATA_BUFFER_ADDRESS_TAG
    #endif
#else
    #define IN_DATA_BUFFER_ADDRESS_TAG
    #define OUT_DATA_BUFFER_ADDRESS_TAG
#endif

DATA_PACKET INPacket IN_DATA_BUFFER_ADDRESS_TAG;    //USB packet buffer.  Must be located in USB module accessible portion of RAM.
DATA_PACKET OUTPacket OUT_DATA_BUFFER_ADDRESS_TAG;   //USB packet buffer.  Must be located in USB module accessible portion of RAM.

#if defined(__18CXX)
    #pragma udata
#endif

BYTE pTemp;                     // Pointer to current logging position, will
                                // loop to zero once the max index is reached
BYTE valid_temp;                // Keeps count of the valid data points
WORD temp_data[30];             // 30 points of data

USB_HANDLE USBGenericOutHandle; // Must be initialized to 0 at startup
USB_HANDLE USBGenericInHandle;  // Must be initialized to 0 at startup
BOOL blinkStatusValid;

// Timer0 - 1 second interval setup.
// Fosc/4 = 12MHz
// Use /256 prescalar, this brings counter freq down to 46,875 Hz
// Timer0 should = 65536 - 46875 = 18661 or 0x48E5
#define TIMER0L_VAL         0xE5
#define TIMER0H_VAL         0x48

/** P R I V A T E  P R O T O T Y P E S ***************************************/

void BlinkUSBStatus(void);
BOOL Switch2IsPressed(void);
BOOL Switch3IsPressed(void);
void ResetTempLog(void);
WORD_VAL ReadPOT(void);
void ServiceRequests(void);

/** D E C L A R A T I O N S **************************************************/
#if defined(__18CXX)
    #pragma code
#endif
void UserInit(void)
{
    mInitAllLEDs();
    mInitAllSwitches();
    old_sw2 = sw2;
    old_sw3 = sw3;
    
    InitTempSensor();
    mInitPOT();
  
    ResetTempLog();
    temp_mode = TEMP_REAL_TIME;
    
    #if defined(__18CXX)
    /* Init Timer0 for data logging interval (every 1 second) */
    T0CON = 0b10010111;
    /* Timer0 is already enabled by default */
    #elif defined(__C30__) || defined __XC16__
    #endif

    USBGenericInHandle = 0;
    USBGenericOutHandle = 0;
    blinkStatusValid = TRUE;
}//end UserInit


/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{   
    //Blink the LEDs according to the USB device status
    if(blinkStatusValid)
    {
        BlinkUSBStatus();
    }

    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

    //respond to any USB commands that might have come over the bus
	PollTempOnHPCExplorer();			
    ServiceRequests();

    //if we are logging temp data
    if(temp_mode == TEMP_LOGGING)
    {
        #if defined(__18CXX)
        //if the 1 second timer has expired (PIC18 only)
        if(INTCONbits.TMR0IF == 1)
        #endif
        {
            #if defined(__18CXX)
            INTCONbits.TMR0IF = 0;          // Clear flag
            TMR0H = TIMER0H_VAL;
            TMR0L = TIMER0L_VAL;            // Reinit timer value;
            #endif

            //get a new temp reading
            if(AcquireTemperature())
            {
                //store the new value in the buffer
                temp_data[pTemp] = temperature.Val;
                
                // First update valid_temp
                if(valid_temp < 30)         // 30 data points max
                    valid_temp++;
                    
                // Next update pTemp
                if(pTemp == 29)
                    pTemp = 0;
                else
                    pTemp++;
            }//end if
        }//end if
    }//end if
}//end ProcessIO

/******************************************************************************
 * Function:        void ResetTempLog(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    pTemp = 0; valid_temp = 0;
 *
 * Overview:        This function resets the temperature logging variables
 *
 * Note:            None
 *****************************************************************************/
void ResetTempLog(void)
{
    pTemp = 0;
    valid_temp = 0;
}//end ResetLog

/******************************************************************************
 * Function:        WORD_VAL ReadPOT(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          WORD_VAL - the 10-bit right justified POT value
 *
 * Side Effects:    ADC buffer value updated
 *
 * Overview:        This function reads the POT and leaves the value in the 
 *                  ADC buffer register
 *
 * Note:            None
 *****************************************************************************/
WORD_VAL ReadPOT(void)
{
    WORD_VAL w;

    #if defined(__18CXX) || defined(__XC8)
        #if defined(PIC18F97J94_PIM) 
            ADCON1Hbits.MODE12 = 0;         // PC application expects 10-bit result
            ADCHS0L = 0x03;                 // AN3 has potentiometer on LCD Explorer 3
            ADCON1Lbits.SAMP = 1;           // Start AD sampling/convert sequence
            while(ADCON1Lbits.DONE == 0);   // Wait for result complete
            w.Val = ADCBUF3;
        #elif defined(PIC18F87J94_PIM)
            ADCON1Hbits.MODE12 = 0;         // PC application expects 10-bit result
            ADCHS0L = 0x00;                 // AN0 has potentiometer on PIC18 Explorer
            ADCON1Lbits.SAMP = 1;           // Start AD sampling/convert sequence
            while(ADCON1Lbits.DONE == 0);   // Wait for result complete
            w.Val = ADCBUF0;
        #else
            ADCON0bits.GO = 1;              // Start AD conversion
            while(ADCON0bits.GO);     // Wait for conversion
            w.v[0] = ADRESL;
            w.v[1] = ADRESH;
        #endif
    #elif defined(__C30__) || defined(__C32__) || defined __XC16__
        #if defined(PIC24FJ256GB110_PIM) || defined(PIC24FJ256DA210_DEV_BOARD) || defined(PIC24FJ256GB210_PIM)
            AD1CHS = 0x5;           //MUXA uses AN5

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

        #elif defined(PIC24FJ64GB004_PIM)
            AD1CHS = 0x7;           //MUXA uses AN7

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete
            
        #elif defined(PIC24FJ64GB502_MICROSTICK)
            AD1CHS = 0x7;           //MUXA uses AN7

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

        #elif defined(PIC24F_STARTER_KIT)
            AD1CHS = 0x0;           //MUXA uses AN0

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

        #elif defined(PIC32MX460F512L_PIM) || defined(PIC32MX795F512L_PIM) || defined(PIC32_USB_STARTER_KIT)
            AD1PCFG = 0xFFFB; // PORTB = Digital; RB2 = analog
            AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling ...
            // and starts converting
            AD1CHS = 0x00020000; // Connect RB2/AN2 as CH0 input ..
            // in this example RB2/AN2 is the input
            AD1CSSL = 0;
            AD1CON3 = 0x0002; // Manual Sample, Tad = internal 6 TPB
            AD1CON2 = 0;
            AD1CON1SET = 0x8000; // turn ADC ON

            AD1CON1SET = 0x0002; // start sampling ...
            for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1CLR = 0x0002; // start Converting
            while (!(AD1CON1 & 0x0001));// conversion done?
            
        #elif defined (DSPIC33EP512MU810_PIM) || defined (PIC24EP512GU810_PIM)
            AD1CHS0 = 0x2;           //MUXA uses AN5

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<10000;w.Val++); //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<10000;w.Val++); //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete
        #else
        
            #error "Please implement this for the target hardware platform"
        #endif

        w.Val = ADC1BUF0;

    #endif

    return w;
}//end ReadPOT

/******************************************************************************
 * Function:        void ServiceRequests(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    USB traffic can be generated
 *
 * Overview:        This function takes in the commands from the PC from the
 *                  application and executes the commands requested
 *
 * Note:            None
 *****************************************************************************/
void ServiceRequests(void)
{
    BYTE index;
    
    //Check to see if data has arrived
    if(!USBHandleBusy(USBGenericOutHandle))
    {        
        //if the handle is no longer busy then the last
        //transmission is complete
       
        counter = 0;

        INPacket.CMD=OUTPacket.CMD;
        INPacket.len=OUTPacket.len;

        //process the command
        switch(OUTPacket.CMD)
        {
            case READ_VERSION:
                //dataPacket._byte[1] is len
                INPacket._byte[2] = MINOR_VERSION;
                INPacket._byte[3] = MAJOR_VERSION;
                counter=0x04;
                break;

            case ID_BOARD:
                counter = 0x01;
                if(OUTPacket.ID == 0)
                {
                    mLED_3_Off();mLED_4_Off();
                }
                else if(OUTPacket.ID == 1)
                {
                    mLED_3_Off();mLED_4_On();
                }
                else if(OUTPacket.ID == 2)
                {
                    mLED_3_On();mLED_4_Off();
                }
                else if(OUTPacket.ID == 3)
                {
                    mLED_3_On();mLED_4_On();
                }
                else
                    counter = 0x00;
                break;

            case UPDATE_LED:
                #if defined(PIC18F87J50_PIM) || defined(PIC18F46J50_PIM) || defined(PIC18F47J53_PIM)
                    blinkStatusValid = FALSE;
                #endif
                // LED1 & LED2 are used as USB event indicators.
                if(OUTPacket.led_num == 3)
                {
                    if(OUTPacket.led_status)
                    {
                        mLED_3_On(); 
                    }
                    else
                    {
                        mLED_3_Off();
                    }
                    counter = 0x01;
                }//end if
                else if(OUTPacket.led_num == 4)
                {
                    if(OUTPacket.led_status)
                    {
                        mLED_4_On(); 
                    }
                    else
                    {
                        mLED_4_Off();
                    }
                    counter = 0x01;
                }//end if else
                break;
                
            case SET_TEMP_REAL:
                temp_mode = TEMP_REAL_TIME;
                ResetTempLog();
                counter = 0x01;
                break;

            case RD_TEMP:
                if(AcquireTemperature())
                {
                    INPacket._byte[1] = temperature.v[0];
                    INPacket._byte[2] = temperature.v[1];
                    counter=0x03;
                }//end if

                break;

            case SET_TEMP_LOGGING:
                temp_mode = TEMP_LOGGING;
                ResetTempLog();
                counter=0x01;
                break;

            case RD_TEMP_LOGGING:
                counter = (valid_temp<<1)+2;  // Update count in byte
                INPacket.len = (valid_temp<<1);

                for(index = valid_temp; index > 0; index--)
                {
                    if(pTemp == 0)
                        pTemp = 29;
                    else
                        pTemp--;
                    INPacket._word[index] = temp_data[pTemp];
                }//end for
                
                ResetTempLog();             // Once read, log will restart
                break;

            case RD_POT:
                {
                    WORD_VAL w;

                    mInitPOT();
                    w = ReadPOT();

                    INPacket._byte[1] = w.v[0];
                    INPacket._byte[2] = w.v[1];

                    counter=0x03;
                }
                break;
                
            case RESET:
                Reset();
                break;
                
            default:
                break;
        }//end switch()
        if(counter != 0)
        {
            if(!USBHandleBusy(USBGenericInHandle))
            {
                USBGenericInHandle = USBGenWrite(USBGEN_EP_NUM,(BYTE*)&INPacket,counter);
            }
        }//end if
        
        //Re-arm the OUT endpoint for the next packet
        USBGenericOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,USBGEN_EP_SIZE);
    }//end if

}//end ServiceRequests

/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BlinkUSBStatus(void)
{
    static WORD led_count=0;
    
    if(led_count == 0)led_count = 10000U;
    led_count--;

    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(USBSuspendControl == 1)
    {
        if(led_count==0)
        {
            mLED_1_Toggle();
            if(mGetLED_1())
            {
                mLED_2_On();
            }
            else
            {
                mLED_2_Off();
            }
        }//end if
    }
    else
    {
        if(USBDeviceState == DETACHED_STATE)
        {
            mLED_Both_Off();
        }
        else if(USBDeviceState == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if(USBDeviceState == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if(USBDeviceState == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if(USBDeviceState == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        }
        else if(USBDeviceState == CONFIGURED_STATE)
        {
            if(led_count==0)
            {      
                mLED_1_Toggle();         
                if(mGetLED_1())
                {
                    mLED_2_Off();
                }
                else
                {
                    mLED_2_On();
                }
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus


/******************************************************************************
 * Function:        BOOL Switch2IsPressed(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          BOOL - TRUE if the SW2 was pressed and FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        returns TRUE if the SW2 was pressed and FALSE otherwise
 *
 * Note:            None
 *****************************************************************************/
BOOL Switch2IsPressed(void)
{
    if(sw2 != old_sw2)
    {
        old_sw2 = sw2;                  // Save new value
        if(sw2 == 0)                    // If pressed
            return TRUE;                // Was pressed
    }//end if
    return FALSE;                       // Was not pressed
}//end Switch2IsPressed

/******************************************************************************
 * Function:        BOOL Switch3IsPressed(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          BOOL - TRUE if the SW3 was pressed and FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        returns TRUE if the SW3 was pressed and FALSE otherwise
 *
 * Note:            None
 *****************************************************************************/
BOOL Switch3IsPressed(void)
{
    if(sw3 != old_sw3)
    {
        old_sw3 = sw3;                  // Save new value
        if(sw3 == 0)                    // If pressed
            return TRUE;                // Was pressed
    }//end if
    return FALSE;                       // Was not pressed
}//end Switch3IsPressed

/******************************************************************************
 * Function:        void TXbyte(BYTE data)
 *
 * PreCondition:    None
 *
 * Input:           BYTE data - send data to the UART(PIC18 only)
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        sends 'data' over the UART
 *
 * Note:            None
 *****************************************************************************/
void TXbyte(BYTE data)
{
    #if defined(__18CXX)
    while(TXSTAbits.TRMT==0);
    TXREG = data;
    #endif
}//end TXbyte

/** EOF user.c ***************************************************************/
