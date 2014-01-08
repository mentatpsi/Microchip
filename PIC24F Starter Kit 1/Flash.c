/******************************************************************************

USB Flash Drive Directory Explorer

This portion of the PIC24F Starter Kit demo shows how the PIC24FJ256GB1
can act as a USB Embedded Host.  The demo will be notified when a USB device
is inserted. When a USB device is present and the demo wishes to communicate
with it, the USBTasks() macro must be called periodically to ensure proper
USB operation.

To access this demo, select the Flash option from the main menu.  The demo will
display the volume label of the currently attached flash drive, and a list box
containing the files in the current working directory.  Use the up and down
touchpads to scroll through the files.  Use the center touchpad when a
subdirectory is highlighted to change the current working directory to that
subdirectory and display the files contained in that subdirectory. (To return
to the previous directory, highlight and select the .\.. directory.)  Touch the
left touchpad to return to the main menu.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

* File Name:       Flash.c
* Dependencies:    None
* Processor:       PIC24FJ256GB106
* Compiler:        C30 v2.01
* Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
software is owned by the Company and/or its supplier, and is
protected under applicable copyright laws. All rights are reserved.
Any use in violation of the foregoing restrictions may subject the
user to criminal sanctions under applicable laws, as well as to
civil liability for the breach of the terms and conditions of this
license.

THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

Author          Date    Comments
--------------------------------------------------------------------------------
KO          14-Feb-2008 First release

*******************************************************************************/
//DOM-IGNORE-END

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PIC24F Starter Kit.h"

//******************************************************************************
//******************************************************************************
// Constants
//******************************************************************************
//******************************************************************************

#define ID_FLASH_VOLUME             ID_TITLE
#define ID_FLASH_FILES              30
#define ID_FLASH_SLIDER             31
#define ID_FLASH_HOME               32
#define ID_FLASH_SELECT             33
#define ID_FLASH_ERROR              34
#define ID_QUIT                     35

#define BUTTON_HEIGHT               0
#define DIRECTORY_NAME_POSITION     (2*sizeof(XCHAR))
#define FILE_INFO_SIZE              (12+4+1)    // 8.3, up to 4 leading chars, null
#define FILENAME_IS_FILE            2
#define FILENAME_IS_SUBDIRECTORY    1
#define HELLO_WORLD                 "Insert media..."
#define LISTBOX_BOTTOM              (GetMaxY()-GOL_EMBOSS_SIZE)
#define LISTBOX_LEFT                1
#define LISTBOX_RIGHT               (GetMaxX()-SLIDER_WIDTH-1)
#define LISTBOX_TOP                 14
#define SLIDER_WIDTH                12 //28

//******************************************************************************
//******************************************************************************
// Data Structures
//******************************************************************************
//******************************************************************************

typedef struct _VOLUME_INFO
{
    char        label[12];
    BYTE        valid;
} VOLUME_INFO;


//******************************************************************************
//******************************************************************************
// Global Variables
//******************************************************************************
//******************************************************************************

extern IMAGE_FLASH      iconFolderSmall;
BOOL                    mediaPresent;
LISTBOX                 *pFlashFiles;
SLIDER                  *pSlider;
BYTE                    usbErrorCode;
char                    usbErrorText[20];
VOLUME_INFO             volume;


//******************************************************************************
//******************************************************************************
// Function Prototypes
//******************************************************************************
//******************************************************************************

void FlashDeleteListBoxItems( void );
void FlashDisplayDirectory( void );
void FlashDisplayFiles( int fileType );
char * FlashFormatFileInformation( SearchRec searchRecord );
void FlashUpdateVolume( void );

/****************************************************************************
  Function:
    void FlashDeleteListBoxItems( void )

  Description:
    This function deletes all items from the file list box. We cannot use the
    standard function, LbDelItemsList(), because all of our listbox items were
    also allocated.  We have to free the string, then free the item.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void FlashDeleteListBoxItems( void )
{
    LISTITEM    *pItem;
    LISTITEM    *pNextItem;

    pItem = pFlashFiles->pItemList;
    while( pItem != NULL )
    {
        free( pItem->pText );
        pNextItem = pItem->pNextItem;
        free(pItem);
        pItem = pNextItem;
    }
    pFlashFiles->pItemList = NULL;
}

/****************************************************************************
  Function:
    void FlashDisplayDirectory( void )

  Description:
    This function displays the files in the current working directory.  First,
    it deletes the files that are currently being displayed.  Then it finds
    and displays all subdirectories, so they are listed first in the list box.
    Then it finds and displays all other files.

  Precondition:
    * A USB flash drive is attached, enumerated, and initialized.
    * ShowScreenFlash() must have been called.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void FlashDisplayDirectory( void )
{
    // Delete all the files that are currently displayed.
    FlashDeleteListBoxItems();

    // Display all of the subdirectories in the directory, then all the
    // files in the directory.
    FlashDisplayFiles( FILENAME_IS_SUBDIRECTORY );
    FlashDisplayFiles( FILENAME_IS_FILE );

    // Focus and select the first item in the list, and set the range and position of the slider.
    if(pFlashFiles->pItemList != NULL)
    {
        LbSetFocusedItem( pFlashFiles, 1 );
        LbSetSel( pFlashFiles, pFlashFiles->pItemList );  // This does reverse video
    }

    SldSetRange( pSlider, LbGetCount(pFlashFiles) );
    SldSetPos(   pSlider, LbGetCount(pFlashFiles)-1 );

    // Redraw the listbox and the slider.
    SetState( pFlashFiles, LB_DRAW );
    SetState( pSlider, SLD_DRAW );
}


/****************************************************************************
  Function:
    void FlashDisplayFiles( int fileType )

  Description:
    This function finds all files of the requested file type and displays
    them in the global list box.

  Precondition:
    ShowScreenFlash() must have been called.

  Parameters:
    int fileType    - The type of file to be found.  Valid values are:
                        * FILENAME_IS_SUBDIRECTORY
                        * FILENAME_IS_FILE

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void FlashDisplayFiles( int fileType )
{
    void            *bitmap;
    BYTE            fileAttributes;
    char            *fileInformation;
    SearchRec       searchRecord;

    if (fileType == FILENAME_IS_SUBDIRECTORY)
    {
        fileAttributes = ATTR_DIRECTORY;
        bitmap = &iconFolderSmall;
    }
    else
    {
        fileAttributes = ATTR_ARCHIVE | ATTR_READ_ONLY | ATTR_HIDDEN;
        bitmap = NULL;
    }

    if (!FindFirst( "*.*", fileAttributes, &searchRecord ))
    {
        if ((fileInformation = FlashFormatFileInformation( searchRecord )) != NULL)
        {
            LbAddItem( pFlashFiles, NULL, fileInformation, bitmap, LB_STS_REDRAW, 0 );
            while (!FindNext( &searchRecord ))
            {
                if ((fileInformation = FlashFormatFileInformation( searchRecord )) != NULL)
                {
                    LbAddItem( pFlashFiles, NULL, fileInformation, bitmap, LB_STS_REDRAW, fileType );
                }
                else
                {
                    // We are out of memory
                }
            }
        }
        else
        {
            // We are out of memory
        }
    }
}

/****************************************************************************
  Function:
    char * FlashFormatFileInformation( SearchRec searchRecord )

  Description:
    This routine creates a formatted string containing the name of the file
    specified in the input parameter.  If the file is a directory, a .\ is
    prepended to the file name.  Otherwise, space padding is prepended so the
    file names are aligned.

  Precondition:
    None

  Parameters:
    SearchRec searchRecord  - Information about the file returned by the
                                File System library

  Return Values:
    char * - A pointer to the newly allocated buffer containing the file name.

  Remarks:
    None
  ***************************************************************************/

char * FlashFormatFileInformation( SearchRec searchRecord )
{
    char        *pBuffer;

    if ((pBuffer = malloc( FILE_INFO_SIZE )) != NULL)
    {
        // Display the file size.  If the file is actually a directory, display an indication.
        if (searchRecord.attributes & ATTR_DIRECTORY)
        {
            sprintf( pBuffer, ".\\" );
        }
        else
        {
            sprintf( pBuffer, "    " );
        }

        // Display the file name.
        strcat( pBuffer, searchRecord.filename );
    }
    return pBuffer;
}


/****************************************************************************
  Function:
    void FlashMonitorMedia( void )

  Description:
    This function calls the background tasks necessary to support USB Host
    operation.  Upon initial insertion of the media, it initializes the file
    system support and reads the volume label. Upon removal of the media, the
    volume label is marked invalid.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/

void FlashMonitorMedia( void )
{
    BOOL            mediaPresentNow;
    BYTE            mountTries;
    SearchRec       searchRecord;

    USBTasks();

    mediaPresentNow = USBHostMSDSCSIMediaDetect();
    if (mediaPresentNow != mediaPresent)
    {
        if (mediaPresentNow)
        {
            // Media has attached.
            mountTries = 10;
            while(!FSInit() && mountTries--);
            if (!mountTries)
            {
                // Could not mount media
                mediaPresent = FALSE;
            }
            else
            {
                mediaPresent = TRUE;

                // Find the volume label.  We need to do this here while we are at the
                // root directory.
                if (!FindFirst( "*.*", ATTR_VOLUME, &searchRecord ))
                {
                    strcpy( volume.label, searchRecord.filename );
                    volume.valid = TRUE;
                }

                FlashDisplayDirectory();
            }
        }
        else
        {
            // Media has detached.
            mediaPresent = FALSE;
            volume.valid = FALSE;

            // Delete all items from the list box.
            FlashDeleteListBoxItems();

            // Redraw the listbox and the slider.  Use a dummy range and position for the slider.
            SldSetRange( pSlider, 2 );
            SldSetPos(   pSlider, 1 );
            SetState( pFlashFiles, LB_DRAW );
            SetState( pSlider, SLD_DRAW );
        }

        FlashUpdateVolume();

    }
}

/****************************************************************************
  Function:
    void FlashUpdateVolume( void )

  Description:
    This function updates the volume label of the installed USB flash drive.
    The label is stored in the global pageInformation structure.  If the
    media is present, the current working directory is used.  If the media
    is not present, a constant text string is copied.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/

void FlashUpdateVolume( void )
{
    STATICTEXT  *pHeaderText;

    pHeaderText = (STATICTEXT*)GOLFindObject( ID_FLASH_VOLUME );

    pageInformation.title[0] = 0;

    if (mediaPresent)
    {
        if (volume.valid)
        {
            strcpy( pageInformation.title, volume.label );
            strcat( pageInformation.title, ":" );
        }

        FSgetcwd( pageInformation.title+strlen(pageInformation.title), LINE_SIZE_TITLE-strlen(pageInformation.title) );
    }
    else
    {
        strcpy( pageInformation.title, HELLO_WORLD );
    }

    SetState( pHeaderText, ST_DRAW );
}


/****************************************************************************
  Function:
    WORD ProcessMessageFlash( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes the messages for the Flash Drive manipulation
    screen.  It allows the user to display a list of the files on the Flash
    Drive, and to traverse the Flash Drive's directories.

  Precondition:
    Call ShowScreenFlash() prior to using this function to display the correct
    screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    The global screenState is updated to proceed to the user's selection.
  ***************************************************************************/

WORD ProcessMessageFlash( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    WORD        controlID;
    SearchRec   searchRecord;

    controlID = GetObjID(pObj);

    switch (controlID)
    {
        case ID_FLASH_FILES:
            LbMsgDefault( translatedMsg, (LISTBOX *)pObj, pMsg );
            LbSetSel( pFlashFiles, pFlashFiles->pFocusItem );  // This does reverse video
            if (pMsg->param2 == SCAN_UP_PRESSED)
            {
                SldSetPos( pSlider, SldGetPos(pSlider)+1 );
            }
            else
            {
                SldSetPos( pSlider, SldGetPos(pSlider)-1 );
            }
            SetState( pSlider, SLD_DRAW_THUMB );
            return 0;   // Perform default listbox processing.
            break;

        case ID_FLASH_HOME:
            // Delete all items from the list box.
            FlashDeleteListBoxItems();

            // Shut down the USB.
            USBHostShutdown();

            screenState = SCREEN_DISPLAY_MAIN;
            break;

        case ID_FLASH_SELECT:
            // See if the focused file is a directory
            {
                LISTITEM    *pCurrentFile;

                pCurrentFile = LbGetSel( pFlashFiles, NULL );
                if (pCurrentFile != NULL)
                {
                    if (!strncmp( pCurrentFile->pText+DIRECTORY_NAME_POSITION, ".",  1 ) ||
                        !strncmp( pCurrentFile->pText+DIRECTORY_NAME_POSITION, "..", 2 ) ||
                        !FindFirst( pCurrentFile->pText+DIRECTORY_NAME_POSITION, ATTR_DIRECTORY, &searchRecord ))
                    {
                        // Change to the specified directory
                        FSchdir( pCurrentFile->pText+DIRECTORY_NAME_POSITION );
                        // Display the current directory
                        FlashDisplayDirectory();
                    }
                }
            }
            break;
    }

    return 0;   // We do not need the default handler
}


/****************************************************************************
  Function:
    WORD ProcessMessageFlashError( WORD translatedMsg, OBJ_HEADER* pObj,
                        GOL_MSG* pMsg )

  Description:
    This function processes the messages for the USB error display screen.
    When the user touches any button after that time, we return to the main
    menu.

  Precondition:
    Call ShowScreenFlashError() prior to using this function to display the
    correct screen.

  Parameters:
    WORD translatedMsg  - The translated control-level message
    OBJ_HEADER* pObj    - Object to which the message applies
    GOL_MSG* pMsg       - The original system message information

  Return Values:
    0   - Do not call the default message handler.
    1   - Call the default message handler.

  Remarks:
    The global screenState is updated to send the user back to the main
    screen.
  ***************************************************************************/

WORD ProcessMessageFlashError( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )
{
    screenState = SCREEN_DISPLAY_MAIN;

    return 0;
}


/****************************************************************************
  Function:
    void ShowScreenFlash( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen that shows the volume name and contents of the current
    directory of the installed Flash Drive (if present).

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenFlash( void )
{
    strcpy( pageInformation.title, HELLO_WORLD );
    InitializeScreen();

    // Create the visible screen controls.

    pFlashFiles = LbCreate(
                    ID_FLASH_FILES,
                    LISTBOX_LEFT, LISTBOX_TOP, LISTBOX_RIGHT, LISTBOX_BOTTOM,
                    LB_SINGLE_SEL|LB_DRAW|LB_FOCUSED,
                    NULL,
                    NULL );

    pSlider = SldCreate(
                    ID_FLASH_SLIDER,                // ID
                    LISTBOX_RIGHT+1, LISTBOX_TOP, GetMaxX()-2, LISTBOX_BOTTOM,          // dimension
                    SLD_DRAW|SLD_SCROLLBAR|SLD_VERTICAL,   // vertical, will be dislayed after creation
                    8,                      // range, dummy
                    1,                          // page
                    1,                          // pos
                    NULL );          // Slider scheme

    BtnCreate(
                    ID_FLASH_HOME,
                    GetMaxX()-6, 0, GetMaxX()-4, 2, 0,
                    BTN_HIDE,
                    NULL,
                    NULL,
                    NULL );

    BtnCreate(
                    ID_FLASH_SELECT,
                    GetMaxX()-4, 0, GetMaxX()-2, 2, 0,
                    BTN_HIDE,
                    NULL,
                    NULL,
                    NULL );


    mediaPresent   = FALSE;
    volume.valid   = FALSE;

    // Initialize USB Embedded Host mode.
    USBInitialize( 0 );
}


/****************************************************************************
  Function:
    void ShowScreenFlashError( void )

  Description:
    This function erases the currently displayed screen and replaces it with
    the screen that shows the error that occurred with the flash drive.

  Precondition:
    usbErrorCode must be set before calling this routine.

  Parameters:
    None

  Returns:
    None

  Remarks:
    The global list of graphics objects is destroyed and replaced.  Therefore,
    this function cannot be called during the message processing of
    GOLMsgCallback().  It must be called during GOLDrawCallback() processing.
  ***************************************************************************/

void ShowScreenFlashError( void )
{
    strcpy( pageInformation.title, "USB Error" );
    InitializeScreen();

    switch( usbErrorCode )
    {
        case USB_APP_CANNOT_OPEN_FILE:
            sprintf( usbErrorText, "Cannot open file" );
            break;

        case USB_DEVICE_ATTACHED:
            sprintf( usbErrorText, "Device attached" );
            break;

        case USB_DEVICE_DETACHED:
            sprintf( usbErrorText, "Device detached" );
            break;

        case USB_DEVICE_ENUMERATING:
            sprintf( usbErrorText, "Enumerating" );
            break;

        case USB_HOLDING_OUT_OF_MEMORY:
            sprintf( usbErrorText, "Out of memory" );
            break;

        case USB_HOLDING_UNSUPPORTED_DEVICE:
            sprintf( usbErrorText, "Unsupported device" );
            break;

        case USB_HOLDING_UNSUPPORTED_HUB:
            sprintf( usbErrorText, "Hub attached" );
            break;

        case USB_HOLDING_INVALID_CONFIGURATION:
            sprintf( usbErrorText, "Invalid config." );
            break;

        case USB_HOLDING_PROCESSING_CAPACITY:
            sprintf( usbErrorText, "Processing excess" );
            break;

        case USB_HOLDING_POWER_REQUIREMENT:
            sprintf( usbErrorText, "Excessive power" );
            break;

        case USB_HOLDING_CLIENT_INIT_ERROR:
            sprintf( usbErrorText, "Client init error" );
            break;

        case USB_MSD_COMMAND_FAILED:
            sprintf( usbErrorText, "MSD command failed" );
            break;

        case USB_MSD_PHASE_ERROR:
            sprintf( usbErrorText, "MSD phase error" );
            break;

        case USB_MSD_OUT_OF_MEMORY:
            sprintf( usbErrorText, "MSD out of memory" );
            break;

        case USB_MSD_CBW_ERROR:
            sprintf( usbErrorText, "MSD CBW error" );
            break;

        case USB_MSD_CSW_ERROR:
            sprintf( usbErrorText, "MSD CSW error" );
            break;

        case USB_MSD_DEVICE_NOT_FOUND:
            sprintf( usbErrorText, "MSD no device" );
            break;

        case USB_MSD_DEVICE_BUSY:
            sprintf( usbErrorText, "MSD busy" );
            break;

        case USB_MSD_INVALID_LUN:
            sprintf( usbErrorText, "MSD bad LUN" );
            break;

        case USB_MSD_MEDIA_INTERFACE_ERROR:
            sprintf( usbErrorText, "MSD no interface" );
            break;

        case USB_MSD_RESET_ERROR:
            sprintf( usbErrorText, "MSD reset error" );
            break;

        case USB_MSD_ILLEGAL_REQUEST:
            sprintf( usbErrorText, "MSD bad request" );
            break;

        default:
            sprintf( usbErrorText, "Unspecified error" );
            break;
    }

    StCreate(
                ID_FLASH_ERROR,                 // ID
                0,                              // left
                20,                             // top
                GetMaxX(),                      // right
                20+GetTextHeight( (void *)&FONTDEFAULT ),  // bottom
                ST_DRAW,
                usbErrorText,
                NULL );

    strcpy( pageInformation.instructions, "Touch a key to continue..." );
    StCreate(
                ID_INSTRUCTIONS,
                0, 47, 127, 47+GetTextHeight( (void *)&FONTDEFAULT )-1,
                ST_DRAW,
                pageInformation.instructions,
                NULL );

    // Create a hidden control to receive button messages.  Otherwise, no
    // messages will be sent through the system.
    BtnCreate(
                        ID_QUIT,
                        GetMaxX()-4, 0, GetMaxX()-2, 2, 0,
                        BTN_HIDE,
                        NULL,
                        NULL,
                        NULL );
}


/****************************************************************************
  Function:
    void TranslateMessageFlash( GOL_MSG *pMsg )

  Description:
    This routine translates the touchpad message for the flash drive
    directory display screen.

  Precondition:
    None

  Parameters:
    GOL_MSG *pMsg   - The current system message

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageFlash( GOL_MSG *pMsg )
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD) &&
        (pMsg->param1  == ID_TOUCH_PAD))
    {
        if ((pMsg->param2 == SCAN_UP_PRESSED) ||
            (pMsg->param2 == SCAN_DOWN_PRESSED))
        {
            pMsg->param1 = ID_FLASH_FILES;
        }
        else if (pMsg->param2 == SCAN_CR_PRESSED)
        {
            pMsg->param1 = ID_FLASH_SELECT;
        }
        else if (pMsg->param2 == SCAN_LEFT_PRESSED)
        {
            pMsg->param1 = ID_FLASH_HOME;
            pMsg->param2 = SCAN_CR_PRESSED;
        }
        else
        {
            // Discard any other messages.
            pMsg->uiEvent = EVENT_INVALID;
        }
    }
}


/****************************************************************************
  Function:
    void TranslateMessageFlashError( GOL_MSG *pMsg )

  Description:
    This routine translates the touchpad message for the flash drive error
    screen.

  Precondition:
    None

  Parameters:
    GOL_MSG *pMsg   - The current system message

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/

void TranslateMessageFlashError( GOL_MSG *pMsg )
{
    if ((pMsg->uiEvent == EVENT_KEYSCAN) &&
        (pMsg->type    == TYPE_KEYBOARD))
    {
        pMsg->param1 = ID_QUIT;
        pMsg->param2 = SCAN_CR_PRESSED;
    }
}

//******************************************************************************
//******************************************************************************
// USB Support Functions
//******************************************************************************
//******************************************************************************

/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event,
                    void *data, DWORD size )

  Description:
    This routine handles USB events sent from the USB Embedded Host stack.

  Precondition:
    None

  Parameters:
    BYTE address    - Address of the USB device generating the event
    USB_EVENT event - Event that has occurred
    void *data      - Pointer to the data associated with the event
    DWORD size      - Size of the data pointed to by *data

  Return Values:
    TRUE    - The event was handled successfully
    FALSE   - The even was not handled successfully

  Remarks:
    We will default to returning TRUE for unknown events, and let operation
    proceed.  Other applications may wish to return FALSE, since we are not
    really handling the event.
  ***************************************************************************/

BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
    switch( event )
    {
        case EVENT_VBUS_REQUEST_POWER:
            // We will let everything attach.
            return TRUE;
            break;

        case EVENT_VBUS_RELEASE_POWER:
            // We are not monitoring power allocation, so we have
            // nothing to update.
            return TRUE;
            break;

        case EVENT_HUB_ATTACH:
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR: // This should never occur
            usbErrorCode = USBHostDeviceStatus(1);

            // Shut down the USB.
            USBHostShutdown();
            screenState = SCREEN_DISPLAY_FLASH_ERROR;
            return TRUE;
            break;

        default:
            break;
    }

    return FALSE;
}


