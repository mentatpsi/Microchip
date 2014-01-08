/*******************************************************************************

    USB Android Accessory basic demo with accessory in host mode

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

FileName:        main.c
Dependencies:    None
Processor:       PIC24/dsPIC30/dsPIC33/PIC32MX
Compiler:        C30/C32
Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the "Company") for its PICmicro(R) Microcontroller is intended and
supplied to you, the Company's customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
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

Change History
  Rev      Description
  -----    ----------------------------------
  2.9      Initial revision
*******************************************************************************/

// Include files
#include <stdbool.h>
#include <stdint.h>

#include "Compiler.h"
#include "HardwareProfile.h"

#include "USB/usb.h"
#include "USB/usb_host_android.h"
#include "USB/usb_host_audio_v1.h"

#include "pot.h"
#include "button.h"

// If a maximum current rating hasn't been defined, then define 500mA by default
#ifndef MAX_ALLOWED_CURRENT
    #define MAX_ALLOWED_CURRENT             (500)         // Maximum power we can supply in mA
#endif

/***********************************************************
Type definitions, enumerations, and constants
***********************************************************/

typedef struct
{
    USB_AUDIO_V1_DEVICE_ID* audioDeviceID;
    BOOL interfaceSet;
} AUDIO_ACCESSORY_EXAMPLE_VARS;

typedef struct __attribute__((packed))
{
    union __attribute__((packed))
    {
        struct __attribute__((packed))
        {
            unsigned play_pause         :1;
            unsigned scan_next          :1;
            unsigned scan_previous      :1;
            unsigned volume_increment   :1;
            unsigned volume_decrement   :1;
            unsigned                    :3;
        } bits;
        unsigned char value;
    } controls;
} HID_REPORT;

/***********************************************************
Local prototypes
***********************************************************/
static int8_t VolumeControl(uint8_t currentVolume, uint8_t potReading);

/***********************************************************
Local variables
***********************************************************/
static void* device_handle = NULL;
static bool device_attached = false;
static bool registered = false;
static AUDIO_ACCESSORY_EXAMPLE_VARS audioAccessoryVars =    
{
    NULL,
    FALSE
};                                                         

//static char manufacturer[] = "Microchip Technology Inc.";
//static char model[] = "Basic Accessory Demo";
static char description[] = DEMO_BOARD_NAME_STRING;
static char version[] = "2.0";
static char uri[] = "http://www.microchip.com/android";
static char serial[] = "N/A";

static ANDROID_ACCESSORY_INFORMATION myDeviceInfo =
{
    //Pass in NULL for the manufacturer and model string if you don't want
    //  an app to pop up when the accessory is plugged in.
    NULL,       //manufacturer,
    0,          //sizeof(manufacturer),

    NULL,       //model,
    0,          //sizeof(model),

    description,
    sizeof(description),

    version,
    sizeof(version),

    uri,
    sizeof(uri),

    serial,
    sizeof(serial),

    //ANDROID_AUDIO_MODE__NONE
    ANDROID_AUDIO_MODE__44K_16B_PCM
};

static char ReportDescriptor[] = {
    0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,                    // USAGE (Consumer Control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0xcd,                    //   USAGE (Play/Pause)
    0x09, 0xb5,                    //   USAGE (Scan Next Track)
    0x09, 0xb6,                    //   USAGE (Scan Previous Track)
    0x09, 0xe9,                    //   USAGE (Volume Increment)
    0x09, 0xea,                    //   USAGE (Volume Decrement)
    0x09, 0x00,                    //   USAGE (Unassigned)
    0x09, 0x00,                    //   USAGE (Unassigned)
    0x09, 0x00,                    //   USAGE (Unassigned)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x81, 0x42,                    //   INPUT (Data,Ary,Var,Null)
    0xc0                           // END_COLLECTION
};

static BUTTON button1 = {false, false, 0};
static BUTTON button2 = {false, false, 0};
static BUTTON button3 = {false, false, 0};
static BUTTON button4 = {false, false, 0};

static HID_REPORT report;

static BYTE requestStatus = USB_SUCCESS;
static ISOCHRONOUS_DATA audioData;
static BYTE device_address = 0;
static bool reportToSend = false;
static bool reportComplete = true;

/****************************************************************************
  Function:
    int main(void)

  Summary:
    main function

  Description:
    main function

  Precondition:
    None

  Parameters:
    None

  Return Values:
    int - exit code for main function

  Remarks:
    None
  ***************************************************************************/
int main(void)
{
    bool blank_sent = TRUE;
    uint8_t currentVolume = 8;
    int8_t volumeChange = 0;

    SYS_Init();

    PotentiometerInit();

    InitAllSwitches();

    USBInitialize(0);
    AndroidAppStart(&myDeviceInfo);

    while(1)
    {
        //Keep the USB stack running
        USBTasks();

        //If the device isn't attached yet,
        if( registered == false )
        {
            reportToSend = false;
            reportComplete = true;
            report.controls.value = 0;
            
            //Continue to the top of the while loop to start the check over again.
            continue;
        }

        ButtonDebounce( &button1, Switch1Pressed() );
        ButtonDebounce( &button2, Switch2Pressed() );
        ButtonDebounce( &button3, Switch3Pressed() );
        ButtonDebounce( &button4, Switch4Pressed() );

        if(report.controls.value != 0)
        {
            blank_sent = false;
            reportToSend = true;
        }
        else
        {
            if(blank_sent == false)
            {
                reportToSend = true;
                blank_sent = true;
            }
        }

        if( (reportToSend == false) && (reportComplete == true) )
        {
            if( (button1.toggled == true) && (button1.pressed == true) )
            {
                report.controls.bits.scan_previous = 1;
                button1.toggled = false;
            }
    
            if( (button2.toggled == true) && (button2.pressed == true) )
            {
                report.controls.bits.scan_next = 1;
                button2.toggled = false;
            }
    
            if( (button3.toggled == true) && (button3.pressed == true) )
            {
                report.controls.bits.play_pause = 1;
                button3.toggled = false;
            }
    
            volumeChange = VolumeControl(currentVolume, (PotentiometerRead10bit()/8) );

            switch(volumeChange)
            {
                case 1:
                    if(currentVolume < 16)
                    {
                        report.controls.bits.volume_increment = 1;
                        currentVolume++;
                    }
                    break;

                case -1:
                    if(currentVolume > 0)
                    {
                        report.controls.bits.volume_decrement = 1;
                        currentVolume--;
                    }
                    break;

                default:
                    break;
            }
        }

        if( (reportToSend == true) && (reportComplete == true) )
        {
            if(AndroidAppHIDSendEvent(device_address, 1, (BYTE*)&report, sizeof(HID_REPORT)) != USB_SUCCESS)
            {
                reportComplete = true;
                USBHostClearEndpointErrors(device_address, 0);
            }
            else
            {
                reportComplete = false;
                reportToSend = false;
            }
        }

    } //while(1) main loop
}



/****************************************************************************
  Function:
    BOOL USB_ApplicationDataEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )

  Summary:
    Handles USB data application events

  Description:
    Handles USB data application events

  Precondition:
    None

  Parameters:
    BYTE address - address of the device causing the event
    USB_EVENT event - the event that has occurred
    void* data - data associated with the event
    DWORD size - the size of the data in the data field

  Return Values:
    BOOL - Return TRUE of the event was processed.  Return FALSE if the event
           wasn't handled.

  Remarks:
    None
  ***************************************************************************/
BOOL USB_ApplicationDataEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch((int)event)
    {
        case EVENT_AUDIO_STREAM_RECEIVED:
            Nop();
            Nop();
            Nop();
            return TRUE;

        default:
            break;
    }

    return FALSE;
}


/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )

  Summary:
    Handles USB application events

  Description:
    Handles USB application events

  Precondition:
    None

  Parameters:
    BYTE address - address of the device causing the event
    USB_EVENT event - the event that has occurred
    void* data - data associated with the event
    DWORD size - the size of the data in the data field

  Return Values:
    BOOL - Return TRUE of the event was processed.  Return FALSE if the event
           wasn't handled.

  Remarks:
    None
  ***************************************************************************/
BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch( (INT)event )
    {
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
                DEBUG_PrintString( "\r\n***** USB Error - device requires too much current *****\r\n" );
            }
            break;

        case EVENT_VBUS_RELEASE_POWER:
        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:   // This should never be generated.
        case EVENT_DETACH:                   // USB cable has been detached (data: BYTE, address of device)
        case EVENT_ANDROID_DETACH:
            device_attached = false;
            registered = false;
            return TRUE;
            break;

        case EVENT_AUDIO_ATTACH:
            device_address = address;
            audioAccessoryVars.audioDeviceID = data;

            requestStatus = USBHostAudioV1SetInterfaceFullBandwidth(audioAccessoryVars.audioDeviceID->deviceAddress);
                
            if( requestStatus != USB_SUCCESS )
            {
                while(1){
                    Nop();
                }
            }
            return TRUE;

        case EVENT_AUDIO_INTERFACE_SET:
            if(USBHostIsochronousBuffersCreate(&audioData, 2, 1023) == FALSE)
            {
                while(1){
                    Nop();
                }
            }
    
            USBHostIsochronousBuffersReset(&audioData, 2);

            requestStatus = USBHostAudioV1ReceiveAudioData(audioAccessoryVars.audioDeviceID->deviceAddress, &audioData );

            if(requestStatus != USB_SUCCESS)
            {
                while(1){
                    Nop();
                }
            }
            return TRUE;

        case EVENT_AUDIO_DETACH:
            USBHostIsochronousBuffersDestroy(&audioData, 2);
            memset(&audioAccessoryVars, 0, sizeof(audioAccessoryVars));
            return TRUE;

        case EVENT_ANDROID_HID_REGISTRATION_COMPLETE:
            registered = true;
            return TRUE;

        case EVENT_ANDROID_HID_SEND_EVENT_COMPLETE:
            //If we sent any of these bits, we need to clear them so we don't
            //  send it again.
            report.controls.value = 0;
            reportComplete = true;
            return TRUE;

        // Android Specific events
        case EVENT_ANDROID_ATTACH:
            device_address = address;
            device_handle = data;
            device_attached = true;
            AndroidAppHIDRegister(device_address, 1, (BYTE*)ReportDescriptor, sizeof(ReportDescriptor));
            return TRUE;

        default :
            break;
    }
    return FALSE;
}


/***************************************************************************
  Function:
    char VolumeControl(BYTE currentVolume, BYTE potReading)

  Summary:
    Determines if the volume needs to be set higher or lower based on the 
    current pot value and the current setting.

  Description:
    Determines if the volume needs to be set higher or lower based on the 
    current pot value and the current setting.

  Precondition:
    None

  Parameters:
    BYTE currentVolume - the current volume setting 0-15
    BYTE potReading - the current pot reading (0-127)

  Return Values:
    char - 1, 0, or -1 based on if the volume should be turned up, stay the
    same, or be turned down.

  Remarks:
    None
***************************************************************************/
static int8_t VolumeControl(uint8_t currentVolume, uint8_t potReading)
{
    /*************************************************************************
     We are using a hysteresis to prevent glitching.  The equations in the
     formulas below are creating that hysteresis.  Here are a few example
     points run through the below logic:
     
     currentVolume potReading result
     ------------- ---------- ------
     0             1          0
     0             4          0
     0             5          1
     1             4          0
     1             3          0
     1             2          -1

     currentVolume +trip -trip
     ------------- ----- -----
     0             5     N/A
     1             13    2
     2             21    10
     3             29    18
     4             37    26
     5             45    34
     6             53    42
     7             61    50
     8             69    58
     9             77    66
     10            85    74
     11            93    82
     12            101   90
     13            109   98
     14            117   106
     15            125   114
     16            133   122

     *************************************************************************/
    
    if(potReading >= (((currentVolume + 1) * 8) - 3) )
    {
        return 1;
    }
    else if (currentVolume == 0)
    {
        return 0;
    }
    else if (potReading <= (((currentVolume - 1) * 8) + 2) )
    {
        return -1;
    }

    return 0;
}


