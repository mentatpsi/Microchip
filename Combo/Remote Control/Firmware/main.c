/*****************************************************************************
 * FileName:        main.c
 * Dependencies:    see Include Section
 * Processor:       PIC24F
 * Compiler:       	See Release Notes
 * Linker:          See Release Notes
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 *****************************************************************************/

#include "main.h"

/*****************************************************************************
 * Section:  Configuration bits PIC24FJ256DA210
 *****************************************************************************/
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 

/*****************************************************************************
 * Section:  Scheduler state mashine
 *****************************************************************************/
enum _SCHEDULER_STATE_ {
   SCHEDULER_SLEEP,
   SCHEDULER_WAKEUP,
   SCHEDULER_TOUCH_SCREEN,
   SCHEDULER_CAP_TOUCH,
   SCHEDULER_BAT_LEVEL,
   SCHEDULER_LED
} SCHEDULER_STATE;

volatile SHORT scheduler_state = SCHEDULER_TOUCH_SCREEN;

/*****************************************************************************
 * Section:  Externs
 *****************************************************************************/
extern volatile CHAR dataReadyCTMU; 
extern          BYTE myDefaultChannel;
extern          USB_HANDLE USBGenericInHandle;
/*****************************************************************************
 * Section:  SPI Channel Settings
 *****************************************************************************/
#ifdef SPI_CHANNEL_2_ENABLE
    #if defined (USE_SST25VF016)
        DRV_SPI_INIT_DATA SPI_Init_Data =       \
        { 
            SST25_SPI_CHANNEL,          // SPI channel
            3,                          // Primary Pre-scale
            7,                          // secondary pre-scale
            0,                          // CKE
            1,                          // CKP
            1,                          // SMP
            0                           // mode
        };  // SPI in DA210 is not validated to run at max speed.   
    #endif    

    #if defined (USE_MCHP25AA02E48)
        DRV_SPI_INIT_DATA EEPROM_Init_Data =    \
        { 
            MCHP25AA02E48_SPI_CHANNEL,  // SPI channel
            0,                          // Primary Pre-scale
            0,                          // secondary pre-scale
            1,                          // CKE
            0,                          // CKP
            1,                          // SMP
            0                           // mode
        };  // SPI in DA210 is not validated to run at max speed.   
    #endif    
#endif

/*****************************************************************************
 * Section:  Local Prototypes
 *****************************************************************************/
void BoardInit(void);
void TickInit(void);
void GraphicsInit();
void CheckExternalFlashHex();
void mTouchAndResistiveTouchInit();

/*****************************************************************************
 * Section:  Globals
 *****************************************************************************/
volatile DWORD  tick = 0;               // tick counter
volatile UINT8  powerSaveMode = 0;      // controls power save mode
volatile UINT8  powerOffRequest = 0;    // set this flag to 1 to send power off command

extern const PALETTE_FLASH RemoteControlPalette;
extern SCREEN_STATES  screenState;

/*****************************************************************************
 * int main(void)
 *****************************************************************************/
int main(void)
{
    // GOL message structure to interact with GOL
    GOL_MSG msg;             
    BYTE status;               

    // enable power saving module so we can turn off the display backlight while 
    // initializing the system
    PowerSaveInit();
    PowerSaveBacklightOff();

    // initiaze the board    
    BoardInit();
    
    // initialize the graphics component    
    GraphicsInit();              

    // initialize the SPI Flash driver
    SST25Init((DRV_SPI_INIT_DATA*)&SPI_Init_Data);                    

    // turn on the backlight
    PowerSaveBacklightOn();

    // check if the external memory has the valid icons
    CheckExternalFlashHex();
    
    // initialize the EEPROM
    SPIEEPROMConfigurePins();
    MCHP25AA02E48Init((DRV_SPI_INIT_DATA*)&EEPROM_Init_Data);

    // initialize RF4CE and initialize pair status LED 
    // in real applications the status may not be needed since
    // in this demo the status is used to detect the mode
    // where the application is running.    
    status = RF4CEControllerInit();   
    SetRemoteControlModeStrings(status);
    
    // update the pair LED status
    UpdatePairTableLEDStatus();

    // disable display power output, it is used for left key
    _DPPWROE = 0;                           
    
    // initialize resistive touch screen and mTouch library
    mTouchAndResistiveTouchInit(); 

    while(1)
    {
	    
        /**************************************************************************/
        // RF4CE API RF4CE_MessageAvailable is the function call to check if a new
        // message for application layer has been received.
        // The signature of the RF4CE_MessageAvailable function is
        //
        //      BOOL RF4CE_MessageAvailable(void)
        //
        // The return value is a boolean to indicate if a message has been received
        /**************************************************************************/
        if( RF4CE_MessageAvailable() )
        {
            // A remote control is not supposed to receive any message in this demo,
            // so ignore any received messages
            /*****************************************************************************/
            // RF4CE API RF4CE_DiscardMessage is the function call to notify the RF4CE
            // stack that it can discard the current message and the application layer
            // is ready to process the next message.
            // The signature of the RF4CE_DiscardMessage function is
            //
            //      void RF4CE_DiscardMessage(void)
            /******************************************************************************/
            RF4CE_DiscardMessage();
        }

        /**************************************************************************/
        // Graphics and TouchScreen 
        /**************************************************************************/       
		if(GOLDraw())               // Draw GOL objects
        {                           
            ShowScreenRendering();

            // Drawing is done here, process messages
            TouchGetMsg(&msg);      // Get message from touch screen
            GOLMsg(&msg);           // Process message
        }

        /**************************************************************************/
        //  power save button, mTouch and Joystick 
        /**************************************************************************/       
        // Decode buttons
        mTouch_ProcessKeys();       // Buttons handlers are in this function

        // Decode joystick
        JoystickDecode();           // Joystick handlers are in this function

        // Send power off command if requested
        if(powerOffRequest != 0)
        {
            RF4CEControllerPower(); // toggle power
            powerOffRequest = 0;
        }

    }
}

/*****************************************************************************
 * void BoardInit(void)
 *****************************************************************************/
void BoardInit(void)
{
	CLKDIVbits.RCDIV = 0;		// Set 1:1 8MHz FRC postscalar

    // Disable analog on all pins, analog inputs that will be used will be 
    // set up and enabled by the respective drivers of that functionality
    // This portion makes sure that all pins are set to digital mode.
    // This is a precautionary step since the pins that have analog 
    // functionality is by default enabled as analog pins. 
	ANSA = 0x0000;
	ANSB = 0x0000;
	ANSC = 0x0000;
	ANSD = 0x0000;
	ANSE = 0x0000;
	ANSF = 0x0000;
	ANSG = 0x0000;

    // set up the programmable pins
	UnlockPPS();  
	
    SPI1ConfigurePPS();
    INT1ConfigurePPS();
    SPI2ConfigurePPS();
    OC1ConfigurePPS(); // LCD backlight PWM

    // Use this only when using the UART which is also 
    // mapped to the programming pins. This application
    // demo is currently written not to use UART.
    //UART2ConfigurePPS();

    LockPPS(); 

    RFConfigureSPI();
    RFConfigureHardwareIO();
    RFConfigureInterrupt();

    JoystickInit();

    LEDInit();

    // initialize the wake button 
    WakeButtonInit();

    SPIFlashConfigurePins();
    SPIEEPROMConfigurePins();

}


/*****************************************************************************
 * void Get64BitNodeAddress(void)
 *****************************************************************************/
void RF4CE_CB_Get64BitNodeAddress(BYTE *address)
{
    MCHP25AA02E48GetEUI64NodeAddress(address);

    // check to make sure the node was loaded properly, IEEE assigned OUI
    if((address[0] != 0x00) || (address[1] != 0x04) || (address[2] != 0xA3))
    {
        address[0] = EUI_0;
        address[1] = EUI_1;
        address[2] = EUI_2;
        address[3] = EUI_3;
        address[4] = EUI_4;
        address[5] = EUI_5;
        address[6] = EUI_6;
        address[7] = EUI_7;
    }
}
/*****************************************************************************
 * DWORD RF4CE_CB_GetFrameCounter(void)
 *****************************************************************************/
DWORD RF4CE_CB_GetFrameCounter(void)
{
    DWORD frameCounter;

    MCHP25AA02E48ReadArray(RF4CE_FRAME_COUNTER_EEPROM_NVM_ADDRESS, (BYTE *)&frameCounter, 4);

    return frameCounter;
}
/*****************************************************************************
 * void RF4CE_CB_PutFrameCounter(DWORD frameCounter)
 *****************************************************************************/
void RF4CE_CB_PutFrameCounter(DWORD frameCounter)
{
    MCHP25AA02E48WriteArray(RF4CE_FRAME_COUNTER_EEPROM_NVM_ADDRESS, (BYTE *)&frameCounter, 4);
}
/*****************************************************************************
 * BYTE RF4CE_CB_GetDefaultChannel(void)
 *****************************************************************************/
BYTE RF4CE_CB_GetDefaultChannel(void)
{
    return MCHP25AA02E48ReadByte(RF4CE_DEFAULT_CHANNEL_EEPROM_NVM_ADDRESS);
}
/*****************************************************************************
 * void RF4CE_CB_PutDefaultChannel(BYTE defaultChannel)
 *****************************************************************************/
void RF4CE_CB_PutDefaultChannel(BYTE defaultChannel)
{
    MCHP25AA02E48WriteByte(RF4CE_DEFAULT_CHANNEL_EEPROM_NVM_ADDRESS, defaultChannel);
}
/*****************************************************************************
 * BYTE RF4CE_CB_GetPowerSaveMode(void)
 *****************************************************************************/
BYTE RF4CE_CB_GetPowerSaveMode(void)
{
    return MCHP25AA02E48ReadByte(RF4CE_POWER_SAVE_MODE_EEPROM_NVM_ADDRESS);
}
/*****************************************************************************
 * void RF4CE_CB_PutPowerSaveMode(BYTE powerSaveMode)
 *****************************************************************************/
void RF4CE_CB_PutPowerSaveMode(BYTE powerSaveMode)
{
    MCHP25AA02E48WriteByte(RF4CE_POWER_SAVE_MODE_EEPROM_NVM_ADDRESS, powerSaveMode);
}
/*****************************************************************************
 * WORD RF4CE_CB_GetShortAddress(void)
 *****************************************************************************/
WORD RF4CE_CB_GetShortAddress(void)
{
    return SST25ReadWord(RF4CE_SHORT_ADDRESS_NOR_NVM_ADDRESS);
}
/*****************************************************************************
 * void RF4CE_CB_PutShortAddress(WORD shortAddress)
 *****************************************************************************/
void RF4CE_CB_PutShortAddres(WORD shortAddress)
{
    SST25WriteWord(shortAddress, RF4CE_SHORT_ADDRESS_NOR_NVM_ADDRESS);
}
/*****************************************************************************
 * WORD RF4CE_CB_GetShortAddress(void)
 *****************************************************************************/
WORD RF4CE_CB_GetPanID(void)
{
    return SST25ReadWord(RF4CE_PAN_ID_NOR_NVM_ADDRESS);
}
/*****************************************************************************
 * void RF4CE_CB_PutShortAddress(WORD shortAddress)
 *****************************************************************************/
void RF4CE_CB_PutPanID(WORD panID)
{
    SST25WriteWord(panID, RF4CE_PAN_ID_NOR_NVM_ADDRESS);
}
/*****************************************************************************
 * void RF4CE_CB_GetPairTable(BYTE *pairTable)
 *****************************************************************************/
void RF4CE_CB_GetPairTable(BYTE *pairTable)
{
    SST25ReadArray(RF4CE_PAIR_TABLE_NOR_NVM_ADDRESS, pairTable, (WORD)MAX_PAIR_SIZE * sizeof(PAIR_TABLE_ENTRY));
}
/*****************************************************************************
 * void RF4CE_CB_PutShortAddress(WORD shortAddress)
 *****************************************************************************/
void RF4CE_CB_PutPairTableIndex(BYTE *pairTable, WORD index)
{
    SST25WriteArray(RF4CE_PAIR_TABLE_NOR_NVM_ADDRESS + ((DWORD)index * sizeof(PAIR_TABLE_ENTRY)), pairTable, sizeof(PAIR_TABLE_ENTRY)); 
}
/*****************************************************************************
 * void RF4CE_CB_NVMClear(void)
 *****************************************************************************/
void RF4CE_CB_NVMClear(void)
{
    SST25SectorErase(RF4CE_NOR_NVM_START_ADDR);
}

/*****************************************************************************
 * void RF4CE_ControllerNVM_PutSelectedDevice(BYTE *pData)
 *****************************************************************************/
void RF4CE_ControllerNVM_PutSelectedDevice(BYTE *pData)
{
    SST25WriteArray(RF4CE_NVM_ACTIVE_DEVICE_ADDRESS, pData, sizeof(RF4CE_SELECTED_DEVICE)); 
}

/*****************************************************************************
 * void RF4CE_ControllerNVM_GetSelectedDevice(BYTE *pData)
 *****************************************************************************/
void RF4CE_ControllerNVM_GetSelectedDevice(BYTE *pData)
{
    SST25ReadArray(RF4CE_NVM_ACTIVE_DEVICE_ADDRESS, pData, sizeof(RF4CE_SELECTED_DEVICE)); 
}

/*****************************************************************************
 * void RF4CE_ControllerNVM_Clear(void)
 *****************************************************************************/
void RF4CE_ControllerNVM_Clear(void)
{
    SST25SectorErase(RF4CE_NVM_ACTIVE_DEVICE_ADDRESS);
}


/*****************************************************************************
 * Change Notice Interrupt Handler
 *****************************************************************************/
void _ISRFAST __attribute__((interrupt, auto_psv)) _CNInterrupt(void)
{
    JKTaskHandler();
    
    IFS1bits.CNIF = 0;
}

/*****************************************************************************
 * Wakeup Button Interrupt Handler
 *****************************************************************************/
void _ISRFAST __attribute__((interrupt, auto_psv)) _INT0Interrupt(void)
{
    
    // toggle power save mode
    if(powerSaveMode == 0)
    {
        powerSaveMode = 1;
    }else{
        powerSaveMode = 0;       
    }
       
    powerOffRequest = 1; // send power off command
    // disable the external interrupt first (used as a debounce)
    IEC0bits.INT0IE = 0;
    IFS0bits.INT0IF = 0;
}


/*******************************************************************
 * EnablePLL
 *******************************************************************/
void EnablePLL(void)
{

}
/*******************************************************************
 * RestorePLL
 *******************************************************************/
void RestorePLL(void)
{

}

/*************************************************************************
  Function:
           void RF4CE_CB_StatusReport(BYTE status, BYTE PairIndex)
    
  Summary:
    This is the callback function from the RF4CE stack to the application
    to report communication status, especially communication status in the
    pairing process.
  Description:
    This is the callback function from the RF4CE stack to the application
    to report communication status, especially in the pairing process.
    Application layer generally can ignore this status report, unless the
    status is not SUCCESS. In the case a communication fails during the
    pairing process, either because of timeout in the process of sending a
    command, or timeout to receive certain command, the status will be set
    to represent failure reason.
  Conditions:
    Transceiver initialization has been finished.
  Input:
    BYTE status -     The status of the communication status.
    BYTE PairIndex -  The index of the communication destination device in
                      the pair table
  Return:
    None
  Example:
    <code>
    void RF4CE_CB_StatusReport(BYTE status, BYTE PairIndex)
    {
        // handle the communication error if pairing process fail.
        if( status != SUCCESS )
        {
            // handle the error
        }
    }
    </code>
  Remarks:
    None                                                                  
  *************************************************************************/ 
void RF4CE_CB_StatusReport(BYTE status, BYTE PairIndex)
{
    printf("\r\n\r\nRF4CE Comm Status: ");
    PrintChar(status);
    printf("\r\nPair Index: ");
    if( status == SUCCESS )
    {
        PrintChar(PairIndex);
    }   
    else
    {
        PrintChar(0xFF);
    }        
}    

/*********************************************************************
* Function: Timer4 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: increments tick counter. Tick is approx. 1 ms.
*
* Note: none
*
********************************************************************/
#define __T4_ISR    __attribute__((interrupt, shadow, auto_psv))

/* */
void __T4_ISR _T4Interrupt(void)
{
    tick++;

    switch(scheduler_state)
    {
       case SCHEDULER_SLEEP:
            if(powerOffRequest != 0)            // wait for power command end
            {
                break;
            }
            // make the delay 1 sec to have enough time for the RF module to 
            // wake up later when it is awakened.
            SetResponseTimeout(ONE_SECOND);
            PowerSaveHandler();                 // enter sleep mode
            scheduler_state = SCHEDULER_WAKEUP;
            break;

       case SCHEDULER_WAKEUP:
            if(powerOffRequest != 0)            // wait for power command end
            {
                break;
            } 
            // response time to sleep is not that long so set it to faster response time
            SetResponseTimeout(NWKC_MAX_DUTY_CYCLE);
            Touch_ADCInit();                    // initialize ADC for touch screen

            // enable wakeup button interrupt
            IFS0bits.INT0IF = 0;
            IEC0bits.INT0IE = 1;

            scheduler_state = SCHEDULER_TOUCH_SCREEN;
            break;

       case SCHEDULER_TOUCH_SCREEN:
            // if touch screen scan is finished then switch to cap touch task  
            if(TouchDetectPosition()!= 0)
            {
                MTouchInit();    // initialize ACD for cap touch
                scheduler_state = SCHEDULER_CAP_TOUCH;
            }
            break;
       case SCHEDULER_CAP_TOUCH:
            MTouchAcquisition();
            Battery_ADCInit();                  // initialize ACD for battery
            scheduler_state = SCHEDULER_BAT_LEVEL;
            break;     
       case SCHEDULER_BAT_LEVEL:
            // if battery voltage measurement is completed then switch to LED scheduler or 
            // sleep scheduler if power save mode is requested
            if(BatteryDetectLevel() != 0)
            {
                if(powerSaveMode != 0)
                {
            	    scheduler_state = SCHEDULER_SLEEP;
                }else{
                    Touch_ADCInit();            // initialize ACD for touch screen
                    scheduler_state = SCHEDULER_LED;
                }
			}
            break;     
       case SCHEDULER_LED:
            LEDTaskHandler();
            scheduler_state = SCHEDULER_TOUCH_SCREEN;
            break;     
    }

    // Clear flag
    TMR4 =0;
    IFS1bits.T4IF = 0;

}

/*********************************************************************
* Function: void TickInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: sets tick timer
*
* Note: none
*
********************************************************************/

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       500 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000

/* */
void TickInit(void)
{
    // Initialize Timer4
    TMR4 = 0;
    PR4 = TICK_PERIOD;
    IFS1bits.T4IF = 0;  //Clear flag
    IEC1bits.T4IE = 1;  //Enable interrupt
    T4CONbits.TON = 1;  //Run timer
}


/*****************************************************************************
 * void GraphicsInit(void)
 *****************************************************************************/
void GraphicsInit()
{    
    // Initialize graphics library and crete default style scheme for GOL
    GOLInit();                              

    screenState = CREATE_HOME;     
    
    // set the palette color depth and initialize the palette used
    SetPaletteBpp(8);
    SetPalette((void*)&RemoteControlPalette, 0, 256);

	// enable the use of the palette
    EnablePalette();

    // Initialize modules used in GUI 
    InitializeGUI();

	// show SPI Flash Programming Mode Message
    SetColor(REMOTE_BLACK);
    ClearDevice();

}

/*****************************************************************************
 * void CheckExternalFlashHex(void)
 *****************************************************************************/
void CheckExternalFlashHex()
{

    typedef struct 
    {
        UINT32 mchpSignature;
        UINT32 mchpCRCData;
    } CRC_CHECK; 

    CRC_CHECK externalCRC, expectedCRC;   
    WORD textHeight;
    void *pFont;

    pFont = (void*) &REMOTEFONT_14;
    SetFont(pFont);
    textHeight = GetTextHeight(pFont);

    // check if the CRC matches the data stored in the external flash memory
    expectedCRC.mchpCRCData = GRC_CRC32_EXTERNAL_MARKER;
    expectedCRC.mchpSignature = 0x5048434D;                // this is "MCHP"

    SST25ReadArray(GRC_CRC32_EXTERNAL_ADDR, (BYTE *)&externalCRC, 8);
    if  ((expectedCRC.mchpCRCData != externalCRC.mchpCRCData) || \
         (expectedCRC.mchpSignature != externalCRC.mchpSignature))
    {

        // expected and read CRC does not match, proceed to programming flash first
        // run the flash programming 
        SetColor(REMOTE_BLACK);
        ClearDevice();

        SetColor(REMOTE_YELLOW);
   		OutTextXY(10,10 + (textHeight*6),"Invalid External Flash Data.");

        SetColor(REMOTE_WHITE);
        OutTextXY(10,10 + (textHeight*9 ),"Now in Programming Mode.");
        OutTextXY(10,10 + (textHeight*10),"Please send hex data");
        OutTextXY(10,10 + (textHeight*11),"via USB...");

  	    // Call the external flash programming routine
   	    ProgramFlash();       

        // check if USB has sent reply to host then do a detach
        while(!USBHandleBusy(USBGenericInHandle));
        USBDeviceDetach();

        DelayMs(100);

        // Force Reset to force the checking of the flash memory if programming was a success
        Reset();
    }
}	

/*****************************************************************************
 * void mTouchAndResistiveTouchInit(void)
 *****************************************************************************/
void mTouchAndResistiveTouchInit()
{
   
    // This assumes that the Non-Volatile Memory used by the touch screen
    // has already been initialized. If not call SST25Init(DRV_SPI_INIT_DATA*)

    // add keys 
    mTouch_CreateKeys();  

    // initialize ADC for mTouch 
    MTouchInit();                 

    // Start tick counter
    TickInit(); 

    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL);   	
}	

/*********************************************************************
* Function: WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
*
* PreCondition: none
*
* Input:  memory - pointer to the bitmap or font external memory structures
*                  (FONT_EXTERNAL or BITMAP_EXTERNAL)
*         offset - data offset
*         nCount - number of bytes to be transferred to the buffer
*         buffer - pointer to the buffer
*
* Output: number of bytes were transferred.
*
* Side Effects: none
*
* Overview: this function must be implemented in application. Graphics Library calls it
*           each time the data from external memory is required. The application must copy 
*           required data to the buffer provided.
*
* Note: none
*
********************************************************************/

// If there are several memories in the system they can be selected by IDs.
// In this demo ID for memory chip installed on Graphics PICTail board is assumed to be 0.
#define SST39_MEMORY    0

/* */

WORD ExternalMemoryCallback(GFX_EXTDATA *memory, LONG offset, WORD nCount, void *buffer)
{
    if(memory->ID == SST39_MEMORY)
    {
	    // Read data requested into buffer provided
        SST25ReadArray(memory->address + offset, // address to read from
        (BYTE *)buffer, nCount);
    }

    return (nCount);
}





