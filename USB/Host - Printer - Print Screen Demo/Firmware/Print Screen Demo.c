/******************************************************************************

Print Screen Demo
    
This demonstration shows how to print a screen capture to a printer.  It uses 
the Graphics Library with a very minimal set of graphics states.  The function
PrintSignature() sets up the call to the graphics library interface function
PrintScreen(), which performs the actual screen capture and print.

This demo runs on an Explorer 16 (DM240001) with a PIC24FJ256GB110 (USB) PIM
(MA240014) and a USB PICtail Plus Daughter Board (AC164127).

In its initial state, this demo supports three printers:

    * Epson TM-T88IV - thermal receipt printer that uses the ESC/POS printer
        language.  This is a fast thermal printer that supports all of the
        features currently available in the ESC/POS printer language driver.
    * Lexmark E250dn - laser printer that supports both PostScript and PCL 5.
        This demo is configured to explicitly use PostScript for this printer.
    * HP Deskjet 460 - mobile inkjet printer that uses PCL 3, a limited
        subset of PCL 5.

To modify this demo for a different printer, run the configuration tool
USBConfig.exe to generate the appropriate configuration files usb_config.c and
usb_config.h.

******************************************************************************/

/******************************************************************************

* FileName:         Print Screen Demo.c
* Dependencies:     USB Embedded Host library
* Processor:        PIC24F/PIC32
* Compiler:         C30 v3.10b/C32 v1.02
* Company:          Microchip Technology, Inc.

*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "uart2.h"

// Graphics Library includes
#include "GraphicsConfig.h"
#include "Graphics/Graphics.h"
#include "EEPROM.h"
#include "SST25VF016.h"    
#include "TouchScreen.h"

// USB Embedded Host Stack includes
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_host_printer.h"
#include "USB/usb_host_printer_primitives.h"


// *****************************************************************************
// *****************************************************************************
// Constants
// *****************************************************************************
// *****************************************************************************

#define MAX_ALLOWED_CURRENT                 250

enum
{
    ID_WINDOW_SIGNATURE             = 100,
    ID_BUTTON_CLEAR,
    ID_BUTTON_PRINT,
    ID_TEXT_TITLE,
    ID_TEXT_TOUCH,
} GRAPHIC_ID;


// States of the demo application's state machine.
typedef enum
{
    SCREEN_DISPLAY_SIGNATURE,
    SCREEN_DISPLAY_SIGNATURE_PANEL,
    SCREEN_SIGNATURE,
    SCREEN_DISPLAY_DEVICE_ERROR,
    SCREEN_DEVICE_ERROR
} SCREEN_STATES;


// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************

#if defined __C30__ || defined __XC16__
    #if defined(__PIC24FJ256GB110__)
        _CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2 & IESO_OFF) // Primary HS OSC with PLL, USBPLL /2
        _CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off
    #elif defined(__PIC24FJ64GB004__)
        _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
        _CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
        _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
    #elif defined(__PIC24FJ256GB106__)
        _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
        _CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
    #elif defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
        _CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
        _CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
    #endif

#elif defined( __PIC32MX__ )

    #pragma config UPLLEN   = ON            // USB PLL Enabled
    #pragma config FPLLMUL  = MUL_15        // PLL Multiplier
    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
    #pragma config FPLLODIV = DIV_1         // PLL Output Divider
    #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
    #pragma config FWDTEN   = OFF           // Watchdog Timer
    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
    #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config POSCMOD  = HS            // Primary Oscillator
    #pragma config IESO     = OFF           // Internal/External Switch-over
    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
    #pragma config FNOSC    = PRIPLL        // Oscillator Selection
    #pragma config CP       = OFF           // Code Protect
    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
   

#else

    #error Cannot define configuration bits.
    
#endif

// *****************************************************************************
// *****************************************************************************
// Data Structures
// *****************************************************************************
// *****************************************************************************

typedef struct
{
    BYTE    printer;
    BYTE    printerAttached     : 1;
    BYTE    pagePrinted         : 1;
    BYTE    overcurrentStateUSB : 1;
} DEMO_STATUS;


// *****************************************************************************
// *****************************************************************************
// Internal Function Prototypes
// *****************************************************************************
// *****************************************************************************

void PrintSignature( void );
WORD ProcessMessageSignature( WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
void ShowScreenDeviceError( void );
void ShowScreenSignature( void );

// *****************************************************************************
// *****************************************************************************
// Macros
// *****************************************************************************
// *****************************************************************************

#define SCREEN_TOUCHED()    (TouchGetX() != -1)

/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
#ifdef SPI_CHANNEL_1_ENABLE
    #if defined (USE_SST25VF016)
        #ifdef __PIC32MX
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
        #else    
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
        #endif
    #elif defined (USE_MCHP25LC256)       
        const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};
    #endif    
#endif

#ifdef SPI_CHANNEL_2_ENABLE
    #if defined (USE_SST25VF016)
        #ifdef __PIC32MX
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
        #else    
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
        #endif
    #elif defined (USE_MCHP25LC256)       
        const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};
    #endif    
#endif

#ifdef SPI_CHANNEL_3_ENABLE
    #if defined (USE_SST25VF016)
        #ifdef __PIC32MX
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
        #else
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
        #endif
    #elif defined (USE_MCHP25LC256)
        const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};
    #endif
#endif
#ifdef SPI_CHANNEL_4_ENABLE
    #if defined (USE_SST25VF016)
        #ifdef __PIC32MX
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
        #else
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
        #endif
    #elif defined (USE_MCHP25LC256)
        const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};
    #endif
#endif

/////////////////////////////////////////////////////////////////////////////
// SPI Device Initialization Function 
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST25VF016)
    // initialize GFX3 SST25 flash SPI
    #define FlashInit(pInitData) SST25Init((DRV_SPI_INIT_DATA*)pInitData)                    
	#define ReadArray(address, pdata, len) SST39ReadArray(address, pdata, len)
#elif defined (USE_MCHP25LC256)
    // initialize EEPROM on Explorer 16
    #define FlashInit(pInitData) MCHP25LC256Init((DRV_SPI_INIT_DATA*)pInitData)
	#define ReadArray(address, pdata, len) SST25ReadArray(address, pdata, len)
#endif

// *****************************************************************************
// *****************************************************************************
// Global Variables
// *****************************************************************************
// *****************************************************************************

GOL_SCHEME                      *altScheme;                  // alternative style scheme
char                            buffer[50];
GOL_SCHEME                      *errorScheme;                // alternative style scheme
USB_PRINTER_GRAPHICS_PARAMETERS params;
USB_PRINTER_DEVICE_ID           printerInfo;
USB_PRINT_SCREEN_INFO           printScreen;
BYTE                            returnCode;
SCREEN_STATES                   screenState;
DEMO_STATUS                     status;
XCHAR                           touchToContinue[]   = "Touch screen to continue";
XCHAR                           usbErrorText[50];


//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event,
                void *data, DWORD size )

  Summary:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.

  Description:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.  If the application is able to handle the event, it
    returns TRUE.  Otherwise, it returns FALSE.

  Precondition:
    None

  Parameters:
    BYTE address    - Address of device where event occurred
    USB_EVENT event - Identifies the event that occured
    void *data      - Pointer to event-specific data
    DWORD size      - Size of the event-specific data

  Return Values:
    TRUE    - The event was handled
    FALSE   - The event was not handled

  Remarks:
    The application may also implement an event handling routine if it
    requires knowledge of events.  To do so, it must implement a routine that
    matches this function signature and define the USB_HOST_APP_EVENT_HANDLER
    macro as the name of that function.
  ***************************************************************************/

BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch( (INT)event )
    {
        // --------------------------------------------------------------------------
        // Printer events

        case EVENT_PRINTER_ATTACH:
            UART2PrintString( "APP:  Printer attached.\r\n" );
            status.printerAttached  = 1;
            printerInfo = *(USB_PRINTER_DEVICE_ID *)data;
            return TRUE;
            break;

        case EVENT_PRINTER_UNSUPPORTED:
            UART2PrintString( "APP: Unsupported printer attached\r\n" );
            return TRUE;
            break;
            
        case EVENT_PRINTER_DETACH:
            UART2PrintString( "APP:  Printer detached\r\n" );
            status.printerAttached      = 0;
            status.pagePrinted          = 0;
            printerInfo.deviceAddress   = 0;
            return TRUE;
            break;

        case EVENT_PRINTER_TX_DONE:
//            UART2PrintString( "APP: OUT transfer complete\r\n" );
            return TRUE;
            break;

        case EVENT_PRINTER_RX_DONE:
//            UART2PrintString( "APP: IN transfer complete\r\n" );
            return TRUE;
            break;

        case EVENT_PRINTER_REQUEST_DONE:
//            UART2PrintString( "APP: Printer Status: " );
//            UART2PutHex( status.printer );
//            UART2PrintString( "\r\n" );
            return TRUE;
            break;

        // --------------------------------------------------------------------------
        // Host events

        case EVENT_VBUS_REQUEST_POWER:
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            if (((USB_VBUS_POWER_EVENT_DATA*)data)->current <= (MAX_ALLOWED_CURRENT / 2))
            {
                return TRUE;
            }
            else
            {
                screenState = SCREEN_DISPLAY_DEVICE_ERROR;
                strcpy( usbErrorText, "Device requires too much current." );
            }
            break;

        case EVENT_VBUS_RELEASE_POWER:
            // Turn off Vbus power.
            // The PIC24F with the Explorer 16 cannot turn off Vbus through software.
            return TRUE;
            break;

        case EVENT_HUB_ATTACH:
            screenState = SCREEN_DISPLAY_DEVICE_ERROR;
            strcpy( usbErrorText, "Hubs are not supported." );
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            screenState = SCREEN_DISPLAY_DEVICE_ERROR;
            strcpy( usbErrorText, "Device is not supported." );
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            screenState = SCREEN_DISPLAY_DEVICE_ERROR;
            strcpy( usbErrorText, "Cannot enumerate device." );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            screenState = SCREEN_DISPLAY_DEVICE_ERROR;
            strcpy( usbErrorText, "Client driver initialization error." );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            screenState = SCREEN_DISPLAY_DEVICE_ERROR;
            strcpy( usbErrorText, "Out of heap memory." );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
            screenState = SCREEN_DISPLAY_DEVICE_ERROR;
            strcpy( usbErrorText, "Unspecified USB error." );
            return TRUE;
            break;

        default:
            break;
    }

    return FALSE;
}



//******************************************************************************
//******************************************************************************
// Internal Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    WORD GOLDrawCallback( void )

  Description:
    This callback is performed at the beginning of the GOLDraw() function.
    Processing that needs to be done on a regular basis and is not triggered
    by a message should be done here.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    All graphic control adding and removing must be done from this function,
    not from the message callback.
  ***************************************************************************/

// Dimensions for signature box
#define SIGNATURE_PANEL_LEFT   30
#define SIGNATURE_PANEL_RIGHT  290
#define SIGNATURE_PANEL_TOP    50
#define SIGNATURE_PANEL_BOTTOM 190

#define SIGNATURE_STATE_SET   0
#define SIGNATURE_STATE_DRAW  1

WORD GOLDrawCallback( void )
{
    static BYTE     state       = SIGNATURE_STATE_SET;

    switch (screenState)
    {
        case SCREEN_DISPLAY_SIGNATURE:
            ShowScreenSignature();
            screenState = SCREEN_DISPLAY_SIGNATURE_PANEL;
            break;

        case SCREEN_DISPLAY_SIGNATURE_PANEL:
            if (state == SIGNATURE_STATE_SET)
            { // set data for panel drawing
                GOLPanelDraw( SIGNATURE_PANEL_LEFT, SIGNATURE_PANEL_TOP,
                        SIGNATURE_PANEL_RIGHT, SIGNATURE_PANEL_BOTTOM,
                        0, WHITE,
                        altScheme->EmbossLtColor,
                        altScheme->EmbossDkColor,
                        NULL, GOL_EMBOSS_SIZE );

                state = SIGNATURE_STATE_DRAW; // change state
            }

            if(!GOLPanelDrawTsk())
            {
                return 0; // drawing is not completed
            }
            else
            {
                state       = SIGNATURE_STATE_SET; // set state to initial
                screenState = SCREEN_SIGNATURE;
            }
            break;

        case SCREEN_SIGNATURE:
            // No extra drawing is done here.
            break;

        case SCREEN_DISPLAY_DEVICE_ERROR:
            // We have a USB error.
            ShowScreenDeviceError();
            screenState = SCREEN_DEVICE_ERROR;
            break;

        case SCREEN_DEVICE_ERROR:
            if (SCREEN_TOUCHED())
            {
                screenState = SCREEN_DISPLAY_SIGNATURE;
            }
            break;
    }

    return 1;
}


/****************************************************************************
  Function:
    WORD GOLMsgCallback( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This callback is executed when a graphics message is being processed.
    Based on the currently displayed screen, we determine the next operation.

  Precondition:
    None

  Parameters:
    WORD translatedMsg  - The message as translated by the processing
                            routine for the object type.
    OBJ_HEADER* pObj    - Pointer to the graphics object header.
    GOL_MSG* pMsg       - Original graphics message.

  Return Values:
    0 - Do not call the default message processing routine for the object.
    1 - Call the default message processing routine for the object.

  Remarks:
    This function is called before the default processing.  If default
    processing is required before other processing, it must be done
    explicitly in this function, and 0 must be returned.  If default
    processing can be performed after this function, simply return 1.
  ***************************************************************************/

WORD GOLMsgCallback( WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    switch( screenState )
    {
        case SCREEN_DISPLAY_SIGNATURE:
            // No messages are processed in this state.
            break;

        case SCREEN_DISPLAY_SIGNATURE_PANEL:
            // No messages are processed in this state.
            break;

        case SCREEN_SIGNATURE:
            return ProcessMessageSignature( objMsg, pObj, pMsg );
            break;

        case SCREEN_DISPLAY_DEVICE_ERROR:
            // No messages are processed in this state.
            break;

        case SCREEN_DEVICE_ERROR:
            // A generic touch is detected in the Draw callback.
            break;
    }
    return 1;
}


/****************************************************************************
  Function:

  Description:

  Precondition:

  Parameters:

  Returns:

  Remarks:
  ***************************************************************************/

void PrintSignature( void )
{
    USB_PRINT_SCREEN_INFO    printScreen;

    UART2PrintString( "Starting print job...\r\n" );
    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_JOB_START, USB_NULL, 0, 0 );

    printScreen.xL          = SIGNATURE_PANEL_LEFT+GOL_EMBOSS_SIZE+1;
    printScreen.yT          = SIGNATURE_PANEL_TOP+GOL_EMBOSS_SIZE+1;
    printScreen.xR          = SIGNATURE_PANEL_RIGHT-GOL_EMBOSS_SIZE-1;
    printScreen.yB          = SIGNATURE_PANEL_BOTTOM-GOL_EMBOSS_SIZE-1;
    printScreen.colorBlack  = BRIGHTRED;
    printScreen.printerType = printerInfo.support;

    if (printerInfo.support.supportFlags.supportsPOS)
    {
        // POS Printers
        UART2PrintString( "Center signature...\r\n" );
        USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_JUSTIFICATION_CENTER, USB_NULL, 0, 0 );
        
        #ifdef USB_PRINTER_POS_24_DOT_IMAGE_SUPPORT
            printScreen.printerInfo.densityVertical     = 24;
            printScreen.printerInfo.densityHorizontal   = 2;
        #else
            printScreen.printerInfo.densityVertical     = 8;
            printScreen.printerInfo.densityHorizontal   = 1;
        #endif

        UART2PrintString( "Print signature...\r\n" );
        PrintScreen( printerInfo.deviceAddress, &printScreen );

        #ifdef USB_PRINTER_POS_CUTTER_SUPPORT
            UART2PrintString( "Feed and cut...\r\n" );
            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_CUT_PARTIAL, USB_NULL, 100, 0 );
        #else
            UART2PrintString( "Feed...\r\n" );
            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_POS_FEED, USB_NULL, 2, USB_PRINTER_TRANSFER_COPY_DATA );
        #endif                    
    }
    else
    {
        // Full Sheet Printers
        printScreen.printerInfo.positionX   = (PRINTER_PAGE_PORTRAIT_WIDTH  - ((SIGNATURE_PANEL_RIGHT-GOL_EMBOSS_SIZE-1)  - (SIGNATURE_PANEL_LEFT+GOL_EMBOSS_SIZE+1))) / 2;
        printScreen.printerInfo.positionY   = (PRINTER_PAGE_PORTRAIT_HEIGHT - ((SIGNATURE_PANEL_BOTTOM-GOL_EMBOSS_SIZE-1) - (SIGNATURE_PANEL_TOP+GOL_EMBOSS_SIZE+1)))  / 2;
        printScreen.printerInfo.resolution  = 75;
        printScreen.printerInfo.scale       = 1.0;
        UART2PrintString( "Print signature...\r\n" );
        PrintScreen( printerInfo.deviceAddress, &printScreen );

        // If we have vector graphics support on a full sheet printer, draw a rectangle around the signature.
        if (printerInfo.support.supportFlags.supportsVectorGraphics && !printerInfo.support.supportFlags.supportsPOS)
        {
            params.sRectangle.xL = printScreen.printerInfo.positionX - 10;     // X-axis position of the left side of the rectangle.
            params.sRectangle.yT = printScreen.printerInfo.positionY - 10;     // Y-axis position of the top of the rectangle.
            params.sRectangle.xR = printScreen.printerInfo.positionX + printScreen.printerInfo.width + 10;     // X-axis position of the right side of the rectangle.
            params.sRectangle.yB = printScreen.printerInfo.positionY + printScreen.printerInfo.height + 10;     // Y-axis position of the bottom of the rectangle.
            UART2PrintString( "Outline signature...\r\n" );
            USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_GRAPHICS_RECTANGLE, USB_DATA_POINTER_RAM(&params), sizeof(params.sRectangle), 0 );
        }
    }

    USBHostPrinterCommandWithReadyWait( &returnCode, printerInfo.deviceAddress, USB_PRINTER_JOB_STOP, USB_NULL, 0, 0 );
    UART2PrintString( "Print complete.\r\n" );
}    


/****************************************************************************
  Function:

  Description:

  Precondition:

  Parameters:

  Returns:

  Remarks:
  ***************************************************************************/

WORD ProcessMessageSignature( WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    static SHORT                    previousX   = -1;
    static SHORT                    previousY   = -1;
    SHORT                           x;
    SHORT                           y;

    switch(GetObjID(pObj))
    {
        case ID_WINDOW_SIGNATURE:
            // get touched position
            x = pMsg->param1;
            y = pMsg->param2;

            // Check if it falls in the signature box
            if( ( (SIGNATURE_PANEL_LEFT+GOL_EMBOSS_SIZE) < x) &&
                ( (SIGNATURE_PANEL_RIGHT-GOL_EMBOSS_SIZE) > x) &&
                ( (SIGNATURE_PANEL_TOP+GOL_EMBOSS_SIZE) < y) &&
                ( (SIGNATURE_PANEL_BOTTOM-GOL_EMBOSS_SIZE) > y) )
            {
                SetColor( BRIGHTRED );

                // if previous position is not valid or event is PRESS
                if ((pMsg->uiEvent == EVENT_PRESS) || (previousX < 0) )
                {
                    PutPixel( x, y );                  // draw pixel
                }
                else
                {
                    Line( previousX,   previousY,   x,   y );          // connect with thick line previous and current points
                    Line( previousX-1, previousY,   x-1, y );
                    Line( previousX+1, previousY,   x+1, y );
                    Line( previousX,   previousY-1, x,   y-1 );
                    Line( previousX,   previousY+1, x,   y+1 );
                }

                previousX = x;
                previousY = y;               // store position
            }
            else
            {
                // reset if outside of box
                previousX = -1;
            }
            return 1; // process by default
            break;

        case ID_BUTTON_CLEAR:
            if(objMsg == BTN_MSG_RELEASED)
            {
                screenState = SCREEN_DISPLAY_SIGNATURE;
            }
            return 1; // process by default
            break;

        case ID_BUTTON_PRINT:
            if(objMsg == BTN_MSG_RELEASED)
            {
                if (status.printerAttached)
                {
                    PrintSignature();
                    screenState = SCREEN_DISPLAY_SIGNATURE;
                }    
                else
                {
                    strcpy( usbErrorText, "No printer attached" );
                    screenState = SCREEN_DISPLAY_DEVICE_ERROR;
                }    
            }
            return 1; // process by default
            break;

        default:
            return 1; // process by default
    }
}


/****************************************************************************
  Function:

  Description:

  Precondition:

  Parameters:

  Returns:

  Remarks:
  ***************************************************************************/

void ShowScreenDeviceError( void )
{
    GOLFree();
    SetColor( errorScheme->CommonBkColor );
    ClearDevice();

    StCreate(   ID_TEXT_TITLE,
                (GetMaxX()-GetTextWidth((XCHAR*)usbErrorText, (void*)&FONTDEFAULT))>>1, GetMaxY()/2, 
                GetMaxX(), GetMaxY()/2+GetTextHeight( (void *)&FONTDEFAULT )-1,
                ST_DRAW,
                usbErrorText,
                errorScheme );

    StCreate(   ID_TEXT_TOUCH,
                (GetMaxX()-GetTextWidth((XCHAR*)touchToContinue, (void*)&FONTDEFAULT))>>1, GetMaxY() - GetTextHeight( (void *)&FONTDEFAULT ),
                GetMaxX(), GetMaxY(),
                ST_DRAW,
                touchToContinue,
                errorScheme );
}

/****************************************************************************
  Function:

  Description:

  Precondition:

  Parameters:

  Returns:

  Remarks:
  ***************************************************************************/

void ShowScreenSignature( void )
{
    GOLFree();
    SetColor( GOLSchemeDefault.CommonBkColor );
    ClearDevice();
    
    WndCreate( ID_WINDOW_SIGNATURE,             // Window ID
              0,0,GetMaxX(),GetMaxY(),          // dimension
              WND_DRAW,                         // will be dislayed after creation
              NULL, //(void*)&mchpLogo,                 // icon
              NULL, //signature,                        // set text
              NULL );                           // default GOL scheme

    BtnCreate(ID_BUTTON_CLEAR,                  // button ID
              5, GetMaxY()-35,                  // left, top corner
              75, GetMaxY()-5, 5,               // right, bottom corner (with radius = 5)
              BTN_DRAW,                         // will be dislayed after creation
              NULL,                             // no bitmap
              "Clear",                          // button text
              NULL );                           // use navigation scheme

    BtnCreate( ID_BUTTON_PRINT,                 // button ID
              GetMaxX()-75, GetMaxY()-35,       // left, top corner
              GetMaxX()-5,  GetMaxY()-5, 5,     // right, bottom corner (with radius = 5)
              BTN_DRAW,                         // will be dislayed and disabled after creation
              NULL,                             // no bitmap
              "Print",                          // button text
              NULL );                           // use navigation scheme
}

/*********************************************************************
* Function: Timer3 ISR
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Timer3 ISR
*
* Note: none
*
********************************************************************/
#ifdef __PIC32MX__
#define __T3_ISR    __ISR(_TIMER_3_VECTOR, ipl4)
#else
#define __T3_ISR    __attribute__((interrupt, shadow, auto_psv))
#endif
    
void __T3_ISR _T3Interrupt(void)
{
    TMR3 = 0;
	// Clear flag
	#ifdef __PIC32MX__
    	mT3ClearIntFlag();
	#else
    	IFS0bits.T3IF = 0;
	#endif
	TouchDetectPosition();    

}	

/*********************************************************************
* Function: void TimerInit(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: sets ADC 
*
* Note: none
*
********************************************************************/

void TimerInit(void){

#define SAMPLE_PERIOD	100     // us
#define TIME_BASE 		(GetPeripheralClock()*SAMPLE_PERIOD)/4000000
  
#ifdef __PIC32MX__
    OpenTimer3(T3_ON | T3_PS_1_8, TIME_BASE);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_4);
#else
    // Initialize Timer3
    TMR3 = 0;
    PR3 = TIME_BASE;
    T3CONbits.TCKPS = 1;         // Set prescale to 1:8
    IFS0bits.T3IF = 0;           // Clear flag
    IEC0bits.T3IE = 1;           // Enable interrupt
    T3CONbits.TON = 1;           // Run timer  
#endif
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Main
//******************************************************************************
//******************************************************************************
//******************************************************************************


int main (void)
{
    GOL_MSG msg;                                    // GOL message structure to interact with GOL

    #if defined (__C30__) || defined __XC16__
        #if defined(__PIC24FJ256GB110__) || defined (__PIC24FJ256GB210__)
        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
        RPOR10bits.RP21R = 11;                  // assign RP21 for SCK2
        RPOR9bits.RP19R = 10;                   // assign RP19 for SDO2
        RPINR22bits.SDI2R = 26;                 // assign RP26 for SDI2
        // Configure U2RX - put on pin 49 (RP10)
        RPINR19bits.U2RXR = 10;
        // Configure U2TX - put on pin 50 (RP17)
        RPOR8bits.RP17R = 5;
        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
        #endif
        OSCCON = 0x3302;    // Enable secondary oscillator
        CLKDIV = 0x0000;    // Set PLL prescaler (1:1)
   #if defined(__PIC24FJ64GB004__)
	//On the PIC24FJ64GB004 Family of USB microcontrollers, the PLL will not power up and be enabled
	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
	//This allows the device to power up at a lower initial operating frequency, which can be
	//advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
	//operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
	//power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        CLKDIVbits.PLLEN = 1;
        while(pll_startup_counter--);
    }

    //Device switches over automatically to PLL output after PLL is locked and ready.
    #endif


    #elif defined(__PIC32MX__)
        {
            int  value;
    
            value = SYSTEMConfigWaitStatesAndPB( GetSystemClock() );
    
            // Enable the cache for the best performance
            CheKseg0CacheOn();
    
            INTEnableSystemMultiVectoredInt();
    
            value = OSCCON;
            while (!(value & 0x00000020))
            {
                value = OSCCON;    // Wait for PLL lock to stabilize
            }
        }
        
        AD1PCFG = 0xFFFF;   // Set analog pins to digital.
        TRISF   = 0x00;
    #else
        #error Cannot initialize.
    #endif

    #if defined (EXPLORER_16)
/************************************************************************
* For Explorer 16 RD12 is connected to EEPROM chip select.
* To prevent a conflict between this EEPROM and SST25 flash
* the chip select of the EEPROM SPI should be pulled up.
************************************************************************/
        // Set IOs directions for EEPROM SPI
        MCHP25LC256_CS_LAT = 1;			    // set initial CS value to 1 (not asserted)
    	MCHP25LC256_CS_TRIS = 0;			// set CS pin to output
	#endif // #if defined (EXPLORER_16)

    #if defined (GFX_PICTAIL_V3) || defined (MEB_BOARD) || defined (GFX_PICTAIL_V3E)   
    SST25_CS_LAT = 1;
    SST25_CS_TRIS = 0;
    #ifndef __PIC32MX__
        SST25_SCK_TRIS = 0;
        SST25_SDO_TRIS = 0;
        SST25_SDI_TRIS = 1;
        #if defined(__PIC24FJ256GB210__)
        	SST25_SDI_ANS = 0;
	    #endif
    #endif
    
    #elif defined (PIC24FJ256DA210_DEV_BOARD)
        SST25_CS_LAT = 1;
        SST25_CS_TRIS = 0;
    
        // Set the pins to be digital 
    	SST25_SDI_ANS = 0;
        SST25_SDO_ANS = 0;

        SST25_SCK_TRIS = 0;
        SST25_SDO_TRIS = 0;
        SST25_SDI_TRIS = 1;
        
    #elif defined (GFX_PICTAIL_V2)
        
        MCHP25LC256_CS_LAT = 1;
        MCHP25LC256_CS_TRIS = 0;
        MCHP25LC256_SCK_TRIS  = 0;
	    MCHP25LC256_SDO_TRIS = 0;
	    MCHP25LC256_SDI_TRIS = 1;
	    
	#endif

    UART2Init();

    UART2PrintString( "\r\n\r\nUSB Embedded Host Print Screen Demo\r\n" );

    // Initialize USB Embedded Host
    USBInitialize(0);

    FlashInit(&SPI_Init_Data);

    // Initialize the graphics library
    GOLInit();      			// Initialize graphics library and create default style scheme for GOL
	TimerInit();				// enable the timer that will sample the touch screen
    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL);   	

    altScheme = GOLCreateScheme();      

    /* for Truly display */
    altScheme->Color0               = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->Color1               = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    altScheme->EmbossDkColor        = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altScheme->EmbossLtColor        = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled        = RGB565CONVERT(0xD4, 0xE1, 0xF7);
    altScheme->TextColor1           = BRIGHTBLUE;
    altScheme->TextColor0           = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    altScheme->TextColorDisabled    = RGB565CONVERT(0xB8, 0xB9, 0xBC);

    errorScheme = GOLCreateScheme();      
    errorScheme->Color0             = BRIGHTCYAN;
    errorScheme->Color1             = CYAN;
    errorScheme->EmbossDkColor      = RED;
    errorScheme->EmbossLtColor      = RGB565CONVERT(0xC0, 0x00, 0x00);
    errorScheme->ColorDisabled      = GRAY032;
    errorScheme->TextColor1         = GRAY010;
    errorScheme->TextColor0         = WHITE;
    errorScheme->TextColorDisabled  = GRAY096;
    errorScheme->CommonBkColor      = BRIGHTRED;
    

    while(1)
    {
        USBTasks();

        if (GOLDraw())                          // Draw the screen
        {
            {
                TouchGetMsg( &msg);
                GOLMsg( &msg );                 // Process the message
            }
        }
    }
}


/*******************************************************************************
Function:       void __attribute__((__interrupt__, auto_psv)) _XXX(void)

These are just here to catch any spurious interrupts that we see during
debugging.

*******************************************************************************/

#if defined ( __C30__ ) || defined __XC16__

void __attribute__((interrupt, auto_psv)) _DefaultInterrupt(void)
{
  UART2PrintString( "!!! Default interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}

void __attribute__((interrupt, auto_psv)) _OscillatorFail(void)
{
  UART2PrintString( "!!! Oscillator Fail interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _AddressError(void)
{
  UART2PrintString( "!!! Address Error interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _StackError(void)
{
  UART2PrintString( "!!! Stack Error interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}
void __attribute__((interrupt, auto_psv)) _MathError(void)
{
  UART2PrintString( "!!! Math Error interrupt handler !!!\r\n" );
  while (1)
  {
      Nop();
      Nop();
      Nop();
  }
}

#endif



