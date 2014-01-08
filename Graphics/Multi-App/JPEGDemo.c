/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        JPEGDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software LicenseJpeg Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Pradeep Budagutta 	03/03/08	...
 * Paolo Tamayo         09/20/08    Ported to ESC Demo
 * Paolo Tamayo         02/17/09    Ported to GMAP Demo, Added SD Card mode
 *****************************************************************************/
#include "MainDemo.h"

#if defined(ENABLE_USB_MSD_DEMO) || defined(ENABLE_SD_MSD_DEMO)
    #include "Image Decoders/ImageDecoder.h"
    #include "MDD File System/FSIO.h"

/////////////////////////////////////////////////////////////////////////////
//                            MACROS
/////////////////////////////////////////////////////////////////////////////
    #define MAX_ELEMENTS            32
    #define MAX_BUTTONS             5
    #define SLIDER_WIDTH            40
    #define SLIDER_BUTTON_HEIGHT    40

    #if defined(__PIC32MX__)
        #define SLIDERSCROLLDELAY   50
        #define JPEGSLIDESHOWDELAY  1500
    #else
        #define SLIDERSCROLLDELAY   200
        #define JPEGSLIDESHOWDELAY  3000
    #endif

/////////////////////////////////////////////////////////////////////////////

//                            STRUCTURES
/////////////////////////////////////////////////////////////////////////////

// structure for thumb drive information
typedef struct _VOLUME_INFO
{
    char    label[12];
    BYTE    valid;
} VOLUME_INFO;

// structure for folder/directory information
typedef struct _FolderElement
{
    char            Name[13];
    XCHAR           XName[13];
    BYTE            blFolder : 1;
    BYTE            blText : 1;
    IMG_FILE_FORMAT ImgType;
} FolderElement;

/////////////////////////////////////////////////////////////////////////////
//                            GLOBALS
/////////////////////////////////////////////////////////////////////////////
FolderElement               aFolderElement[MAX_ELEMENTS];
BYTE                        bStartingElement;
BYTE                        bNumElementsInFolder;
BYTE                        blImageOnScreen;
OBJ_HEADER                  *pSave;
BYTE                        blSlideShowOn;
WORD                        blSlideShowDelay;
BYTE                        blTextOnScreen;
BYTE                        NextSlide;
BYTE                        mediaPresent = FALSE;
VOLUME_INFO                 volume;

// pointers to screen objects
BUTTON                      *pBtnUp, *pBtnDn;
LISTBOX                     *pListBox;
SLIDER                      *pSlider;

XCHAR                       *pJPGItemList = NULL;

extern XCHAR                ExitStr[];
extern XCHAR                RightArrowStr[];
extern XCHAR                LeftArrowStr[];
extern DWORD                tick;   // tick counter

/////////////////////////////////////////////////////////////////////////////

//                            STRINGS USED
/////////////////////////////////////////////////////////////////////////////
const XCHAR                 ImageDisplayStr[] = {'I','M','A','G','E',' ','D','I','S','P','L','A','Y',' ','D','E','M','O',0};
    #if defined ENABLE_USB_MSD_DEMO
const XCHAR                 DetectingStr[] = {'D','e','t','e','c','t','i','n','g',' ','U','S','B',0};
const XCHAR                 ThumbDriveStr[] = {'T','h','u','m','b',' ','D','r','i','v','e',0};
    #elif defined ENABLE_SD_MSD_DEMO
const XCHAR                 DetectingStr[] = {'D','e','t','e','c','t','i','n','g',0};
const XCHAR                 ThumbDriveStr[] = {'S','D','C','a','r','d',' ','D','r','i','v','e',0};
    #endif
const XCHAR                 PleaseWaitStr[] = {'P','l','e','a','s','e',' ','W','a','i','t','.','.','.',0};

const XCHAR                 JpegUpArrowStr[] = {0x1e,0};
const XCHAR                 JpegDownArrowStr[] = {0x1f,0};

const XCHAR                 SlideShowStr[] = {'S','l','i','d','e',' ','s','h','o','w',0};
const XCHAR                 Exiting1Str[] = {'N','o',' ','D','r','i','v','e',' ','D','e','t','e','c','t','e','d',0};
const XCHAR                 Exiting2Str[] = {'E','x','i','t','i','n','g',0};

const XCHAR                 JPEGUnsupportedStr[] = {'U','n','s','u','p','p','o','r','t','e','d',0};
const XCHAR                 JPEGImageStr[] = {'I','m','a','g','e',0};
const XCHAR                 JPEGFormatStr[] = {'F','o','r','m','a','t',0};

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void                        FillNewElements(void);
void                        DisplayErrorInfo(void);
BYTE                        ShowNextSlide(void);

    #define WAIT_UNTIL_FINISH(x)    while(!x)

    /************************************************************************
 Function: WORD CreateJPEGDemo(void)
                                                                       
 Overview: Creates the JPEG demo screen.
  		                                         
 Input: none
                                                                       
 Output: Return 0 when memory is not enough for objects of the demo.
************************************************************************/
    WORD CreateJPEGDemo(void)
{
    BYTE    TextHeight;
    WORD    TextX;

    // Free memory for the objects in the previous linked list and start new list to display
    // the files seen on the media
    GOLFree();

    // initialize the image decoder
    ImageDecoderInit();

    // initialize the screen	
    SetColor(WHITE);
    ClearDevice();

    SetFont((void *) &GOLFontDefault);
    TextHeight = GetTextHeight((void *) &GOLFontDefault);

    SetColor(BRIGHTRED);
    TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)ImageDisplayStr, (void *) &GOLFontDefault)) / 2;
    WAIT_UNTIL_FINISH(OutTextXY(TextX, 1 * TextHeight, (XCHAR *)ImageDisplayStr));

    SetColor(BLACK);
    TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)DetectingStr, (void *) &GOLFontDefault)) / 2;
    WAIT_UNTIL_FINISH(OutTextXY(TextX, 3 * TextHeight, (XCHAR *)DetectingStr));
    TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)ThumbDriveStr, (void *) &GOLFontDefault)) / 2;
    WAIT_UNTIL_FINISH(OutTextXY(TextX, 4 * TextHeight, (XCHAR *)ThumbDriveStr));
    TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)PleaseWaitStr, (void *) &GOLFontDefault)) / 2;
    WAIT_UNTIL_FINISH(OutTextXY(TextX, 6 * TextHeight, (XCHAR *)PleaseWaitStr));

        #if defined(ENABLE_SD_MSD_DEMO)
    MDD_SDSPI_InitIO();
        #endif
    MonitorDriveMedia();

    if(mediaPresent == 0)
    {

        // erase the last line
        SetColor(WHITE);
        TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)PleaseWaitStr, (void *) &GOLFontDefault)) / 2;
        WAIT_UNTIL_FINISH(OutTextXY(TextX, 6 * TextHeight, (XCHAR *)PleaseWaitStr));

        // replace it with these
        SetColor(BRIGHTRED);
        TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)Exiting1Str, (void *) &GOLFontDefault)) / 2;
        WAIT_UNTIL_FINISH(OutTextXY(TextX, 6 * TextHeight, (XCHAR *)Exiting1Str));

        TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)Exiting2Str, (void *) &GOLFontDefault)) / 2;
        WAIT_UNTIL_FINISH(OutTextXY(TextX, 7 * TextHeight, (XCHAR *)Exiting2Str));
        DelayMs(1000);
        return (0);
    }

    blImageOnScreen = 0;
    blTextOnScreen = 0;
    pSave = NULL;
    blSlideShowOn = 0;
    NextSlide = 0xFF;

    // create the listbox, slider and buttons that will emulate a
    // list box with controls.
    pListBox = LbCreate
        (
            ID_JPGLISTBOX,
            LBJPGXPOS,
            LBJPGYPOS,
            LBJPGXPOS + LBJPGWIDTH,
            LBJPGYPOS + LBJPGHEIGHT,
            LB_DRAW | LB_SINGLE_SEL,
            pJPGItemList,
            NULL
        );

    if(pListBox == NULL)
        return (0);

    pSlider = SldCreate
        (
            ID_SLD4LB,
            SLD4LBXPOS,
            SLD4LBYPOS,
            SLD4LBXPOS + SLD4LBWIDTH,
            SLD4LBYPOS + SLD4LBHEIGHT,
            SLD_DRAW | SLD_VERTICAL | SLD_SCROLLBAR,
            2,
            1,
            1,  // these are temporary fill items will set to proper values
            NULL
        );
    if(pSlider == NULL)
        return (0);

    pBtnUp = BtnCreate
        (
            ID_BTNUP4LB,
            BTNUP4LBXPOS,
            BTNUP4LBYPOS,
            BTNUP4LBXPOS + BTNUP4LBWIDTH,
            BTNUP4LBYPOS + BTNUP4LBHEIGHT,
            0,
            BTN_DRAW,
            NULL,
            (XCHAR *)JpegUpArrowStr,
            NULL
        );

    if(pBtnUp == NULL)
        return (0);

    pBtnDn = BtnCreate
        (
            ID_BTNDN4LB,
            BTNDN4LBXPOS,
            BTNDN4LBYPOS,
            BTNDN4LBXPOS + BTNDN4LBWIDTH,
            BTNDN4LBYPOS + BTNDN4LBHEIGHT,
            0,
            BTN_DRAW,
            NULL,
            (XCHAR *)JpegDownArrowStr,
            NULL
        );

    if(pBtnUp == NULL)
        return (0);

    // create the control buttons at the bottom of the screen
    CreateCtrlButtons(ExitStr, NULL, NULL, RightArrowStr);

    // fill the list box with the file names of images in the media
    FillNewElements();

    // set the first item to be focused
    LbSetFocusedItem(pListBox, 0);

    // successful creation of the JPEG demo screen
    return (1);
}

/************************************************************************
 Function: WORD JPEGDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is in this particular demo.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD JPEGDrawCallback(void)
{
    static DWORD    prevTick = 0;   // keeps previous value of tick	
    SHORT           item;

    MonitorDriveMedia();

    // check if media is still present, if not return to main menu
    if(mediaPresent == FALSE)
    {
        screenState = CREATE_DEMOSELECTION;
        return (1);
    }

    if(blSlideShowOn == 1)
    {

        // moves to the next item in the list when doing the slide show
        if((tick - prevTick) > blSlideShowDelay)
        {
            ShowNextSlide();
            blSlideShowDelay = JPEGSLIDESHOWDELAY;
            prevTick = tick;
        }
    }
    else
    {

        // this implements the automatic scrolling of the list box contents
        // when the up or down buttons are continuously pressed.
        if((tick - prevTick) > SLIDERSCROLLDELAY)
        {
            item = LbGetFocusedItem(pListBox);
            if(GetState(pBtnUp, BTN_PRESSED))
            {

                // redraw only the listbox when the item that is focused has changed
                LbSetFocusedItem(pListBox, LbGetFocusedItem(pListBox) - 1);
                if(item != LbGetFocusedItem(pListBox))
                {
                    SetState(pListBox, LB_DRAW_ITEMS);
                }

                SldSetPos(pSlider, SldGetPos(pSlider) + 1);
                SetState(pSlider, SLD_DRAW_THUMB);
            }

            if(GetState(pBtnDn, BTN_PRESSED))
            {
                LbSetFocusedItem(pListBox, LbGetFocusedItem(pListBox) + 1);
                if(item != LbGetFocusedItem(pListBox))
                {
                    SetState(pListBox, LB_DRAW_ITEMS);
                }

                SldSetPos(pSlider, SldGetPos(pSlider) - 1);
                SetState(pSlider, SLD_DRAW_THUMB);
            }

            prevTick = tick;
        }
    }

    return (1);
}

/************************************************************************
 Function: WORD JPEGMsgCallback(WORD objMsg, OBJ_HEADER* pObj, 
 								GOL_MSG* pMsg)

 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
           
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the non-translated, raw GOL message 

 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD JPEGMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    LISTITEM    *pItemSel;

    // check if an image is being shown
    if((blImageOnScreen == 1) || (blSlideShowOn == 1))
    {

        // this is the routine to go back and show the list when an
        // image is being shown on the screen or the slide show is
        // currently ongoing
        if(pMsg->uiEvent == EVENT_RELEASE)
        {
            blImageOnScreen = 0;
            blSlideShowOn = 0;
            GOLRedrawRec(0, 0, GetMaxX(), GetMaxY());
        }

        return (0);
    }

    switch(GetObjID(pObj))
    {
        case ID_BUTTON_A:
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);
                screenState = CREATE_DEMOSELECTION;
            }

            return (1);

        case ID_BUTTON_D:
            if(objMsg == BTN_MSG_RELEASED)
            {   // check if button is pressed
                // do not process if user moved the touch away from the button
                // returning 1 wil update the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    return (1);

                // enable the slide show
                blSlideShowOn = 1;
				// initialize the screen	
				SetColor(BLACK);
    			ClearDevice();

                ClrState(pObj, BTN_PRESSED);            // clear the pressed state, will be shown only when redrawn
                blSlideShowDelay = 0;                   // force slide show to immediately start
                return (0);
            }

            return (1);

        case ID_JPGLISTBOX:
            if(pMsg->uiEvent == EVENT_MOVE)
            {
                pMsg->uiEvent = EVENT_PRESS;            // change event for listbox

                // Process message by default
                LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);

                // Set new list box position
                SldSetPos(pSlider, LbGetCount(pListBox) - LbGetFocusedItem(pListBox) - 1);
                SetState(pSlider, SLD_DRAW_THUMB);
                pMsg->uiEvent = EVENT_MOVE;             // restore event for listbox
            }
            else if(pMsg->uiEvent == EVENT_PRESS)
            {

                // call the message default processing of the list box to select the item
                LbMsgDefault(objMsg, (LISTBOX *)pObj, pMsg);
            }
            else if(pMsg->uiEvent == EVENT_RELEASE)
            {

                // check which item was selected and display appropriate screen
                pItemSel = LbGetSel(pListBox, NULL);    // get the selected item
                if(aFolderElement[pItemSel->data].blFolder == 1)
                {
                    if(FSchdir(aFolderElement[pItemSel->data].Name) == 0)
                    {
                        FillNewElements();
                        return (1);
                    }
                }
                else if(aFolderElement[pItemSel->data].blText == 0)
                {
                    IMG_FILE    *pImgFile = IMG_FOPEN(aFolderElement[pItemSel->data].Name, "rb");
                    if(pImgFile != NULL)
                    {
                        blImageOnScreen = 1;
                        // initialize the screen	
					    SetColor(BLACK);
    					ClearDevice();

                        if
                        (
                            ImageDecode
                                (
                                    pImgFile,
                                    aFolderElement[pItemSel->data].ImgType,
                                    0,
                                    0,
                                    IMG_SCREEN_WIDTH,
                                    IMG_SCREEN_HEIGHT,
                                    (IMG_DOWN_SCALE | IMG_ALIGN_CENTER),
                                    NULL,
                                    NULL
                                )
                        )
                        {
                            DisplayErrorInfo();
                        }

                        IMG_FCLOSE(pImgFile);
                    }
                }
            }

            // The message was processed. To avoid other objects processing the 
            // processed message reset the message.
            pMsg->uiEvent = EVENT_INVALID;
            return (0);

        case ID_SLD4LB:
        	
        	if((objMsg == SLD_MSG_INC) || (objMsg == SLD_MSG_DEC)) 
            {   // check slider was touched.

	            // Process message by default
	            SldMsgDefault(objMsg, (SLIDER *)pObj, pMsg);
	
	            // Set new list box position
	            if(LbGetFocusedItem(pListBox) != LbGetCount(pListBox) - SldGetPos(pSlider))
	            {
	                LbSetFocusedItem(pListBox, LbGetCount(pListBox) - SldGetPos(pSlider));
	                SetState(pListBox, LB_DRAW_ITEMS);
	            }
	            
	        }

            // The message was processed
            return (0);

        case ID_BTNUP4LB:                               // slider up button was pressed
            if(objMsg == BTN_MSG_RELEASED)
            {
				// check if we have reached the very first then stay there.
	            if (LbGetFocusedItem(pListBox) == 0)
	                LbSetFocusedItem(pListBox,0);
	            else    
                	LbSetFocusedItem(pListBox,LbGetFocusedItem(pListBox)-1);                
                SetState(pListBox, LB_DRAW_ITEMS);
                SldSetPos(pSlider, SldGetPos(pSlider) + 1);
                SetState(pSlider, SLD_DRAW_THUMB);
            }

            return (1);

        case ID_BTNDN4LB:                               // slider down button was pressed
            if(objMsg == BTN_MSG_RELEASED)
            {
	            // set all items to be not displayed
                pItemSel = pListBox->pItemList;
                while(pItemSel != NULL) {
                	pItemSel->status = 0;
                	pItemSel = pItemSel->pNextItem;
                }	
                LbSetFocusedItem(pListBox, LbGetFocusedItem(pListBox) + 1);
                SetState(pListBox, LB_DRAW_ITEMS);
                SldSetPos(pSlider, SldGetPos(pSlider) - 1);
                SetState(pSlider, SLD_DRAW_THUMB);
            }

            return (1);
    }

    return (1);
}

/************************************************************************
 Function: void FillNewElements(void)
                                                                       
 Overview: Fill the list box with the image and directory elements 
 		   found in the thumb drive.
 		                                         
 Input: none
                                                                       
 Output: none
************************************************************************/
void FillNewElements(void)
{
    SearchRec   searchRecord;
    BYTE        bCounter;
    BYTE        bFound;
    void        *pBitmap;

    NextSlide = 0;
    bNumElementsInFolder = 0;

    bFound = FindFirst("*", ATTR_DIRECTORY, &searchRecord);
    for(bCounter = 0; bCounter < MAX_ELEMENTS; bCounter++)
    {
        if(bFound == 0)
        {
            BYTE    i;
            if(searchRecord.filename[0] == '.' && searchRecord.filename[1] == '\0')
            {
                bFound = FindNext(&searchRecord);
                continue;
            }

            aFolderElement[bNumElementsInFolder].blFolder = 1;
            aFolderElement[bNumElementsInFolder].blText = 0;
            for(i = 0; i < 13; i++)
            {
                aFolderElement[bNumElementsInFolder].Name[i] = searchRecord.filename[i];
                aFolderElement[bNumElementsInFolder].XName[i] = searchRecord.filename[i];
            }

            bNumElementsInFolder++;
        }
        else
        {
            break;
        }

        bFound = FindNext(&searchRecord);
    }

    bFound = FindFirst("*.jpg", ATTR_MASK ^ ATTR_DIRECTORY, &searchRecord);
    for(bCounter = 0; bCounter < MAX_ELEMENTS && bNumElementsInFolder < MAX_ELEMENTS; bCounter++)
    {
        if(bFound == 0)
        {
            BYTE    i;
            aFolderElement[bNumElementsInFolder].blFolder = 0;
            aFolderElement[bNumElementsInFolder].blText = 0;
            aFolderElement[bNumElementsInFolder].ImgType = IMG_JPEG;
            for(i = 0; i < 13; i++)
            {
                aFolderElement[bNumElementsInFolder].Name[i] = searchRecord.filename[i];
                aFolderElement[bNumElementsInFolder].XName[i] = searchRecord.filename[i];
            }

            bNumElementsInFolder++;
        }
        else
        {
            break;
        }

        bFound = FindNext(&searchRecord);
    }

    bFound = FindFirst("*.bmp", ATTR_MASK ^ ATTR_DIRECTORY, &searchRecord);
    for(bCounter = 0; bCounter < MAX_ELEMENTS && bNumElementsInFolder < MAX_ELEMENTS; bCounter++)
    {
        if(bFound == 0)
        {
            BYTE    i;
            aFolderElement[bNumElementsInFolder].blFolder = 0;
            aFolderElement[bNumElementsInFolder].blText = 0;
            aFolderElement[bNumElementsInFolder].ImgType = IMG_BMP;
            for(i = 0; i < 13; i++)
            {
                aFolderElement[bNumElementsInFolder].Name[i] = searchRecord.filename[i];
                aFolderElement[bNumElementsInFolder].XName[i] = searchRecord.filename[i];
            }

            bNumElementsInFolder++;
        }
        else
        {
            break;
        }

        bFound = FindNext(&searchRecord);
    }

    // fill the list box items
    bStartingElement = 0;

    // clear the list first
    LbDelItemsList(pListBox);

    for(bCounter = 0; (bStartingElement + bCounter) < bNumElementsInFolder; bCounter++)
    {

        // set the proper bitmap icon
        if(aFolderElement[bStartingElement + bCounter].blFolder == 1)
        {
            pBitmap = (void *) &FolderIcon;
        }
        else if(aFolderElement[bStartingElement + bCounter].ImgType == IMG_JPEG)
        {
            pBitmap = (void *) &JpegIcon;
        }
        else if(aFolderElement[bStartingElement + bCounter].ImgType == IMG_BMP)
        {
            pBitmap = (void *) &BitmapIcon;
        }
        else
        {
            pBitmap = NULL;
        }

        // assign the item to the list
        if(NULL == LbAddItem(pListBox, NULL, aFolderElement[bStartingElement + bCounter].XName, pBitmap, 0, bCounter))
            break;
        else
        {

            // adjust the slider page and range
            SldSetRange(pSlider, bNumElementsInFolder);
            SldSetPage(pSlider, bNumElementsInFolder >> 1);

            // to completely redraw the object when GOLDraw() is executed.
            SetState(pSlider, DRAW);
            SetState(pListBox, DRAW);
        }
    }
}

/************************************************************************
 Function: void DisplayErrorInfo(void)
                                                                       
 Overview: Display the error information when decoding an unsupported 
 		   image or thumb drive was removed when decoding.
 		                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
void DisplayErrorInfo(void)
{
    WORD    TextX, TextY, TextHeight;
    SetFont((void *) &GOLFontDefault);
    TextHeight = GetTextHeight((void *) &GOLFontDefault);

    SetColor(WHITE);
    ClearDevice();

    SetColor(BRIGHTRED);
    TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)JPEGUnsupportedStr, (void *) &GOLFontDefault)) / 2;
    TextY = (IMG_SCREEN_HEIGHT - 3 * TextHeight) / 2;
    WAIT_UNTIL_FINISH(OutTextXY(TextX, TextY, (XCHAR *)JPEGUnsupportedStr));
    TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)JPEGImageStr, (void *) &GOLFontDefault)) / 2;
    WAIT_UNTIL_FINISH(OutTextXY(TextX, TextY + TextHeight, (XCHAR *)JPEGImageStr));
    TextX = (IMG_SCREEN_WIDTH - GetTextWidth((XCHAR *)JPEGFormatStr, (void *) &GOLFontDefault)) / 2;
    WAIT_UNTIL_FINISH(OutTextXY(TextX, TextY + 2 * TextHeight, (XCHAR *)JPEGFormatStr));

    DelayMs(800);
}

/************************************************************************
 Function: BYTE ShowNextSlide(void)
                                                                       
 Overview: Display the next image when slide show is enabled.
 		                                          
 Input: none
                                                                       
 Output: Returns the status if at the end of the list.
************************************************************************/
BYTE ShowNextSlide(void)
{
    BYTE        temp = NextSlide;
    IMG_FILE    *pImgFile;

    if(NextSlide == 0xFF || NextSlide >= bNumElementsInFolder)
    {
        return (0);
    }

    while(aFolderElement[NextSlide].blFolder == 1 || aFolderElement[NextSlide].blText == 1)
    {
        NextSlide = (NextSlide + 1) % bNumElementsInFolder;
        if(NextSlide == temp)
        {
            return (0);
        }
    }

    pImgFile = IMG_FOPEN(aFolderElement[NextSlide].Name, "rb");
    if(pImgFile != NULL)
    {
        if
        (
            ImageDecode
                (
                    pImgFile,
                    aFolderElement[NextSlide].ImgType,
                    0,
                    0,
                    IMG_SCREEN_WIDTH,
                    IMG_SCREEN_HEIGHT,
                    (IMG_DOWN_SCALE | IMG_ALIGN_CENTER),
                    NULL,
                    NULL
                )
        )
        {
            DisplayErrorInfo();
        }

        IMG_FCLOSE(pImgFile);
        NextSlide = (NextSlide + 1) % bNumElementsInFolder;
        return (1);
    }

    return (0);
}

/************************************************************************
Function: void MonitorDriveMedia( void )

Precondition: None

Overview: This function calls the background tasks necessary to support
          USB Host operation.  Upon initial insertion of the media, it
          initializes the file system support and reads the volume label.
          Upon removal of the media, the volume label is marked invalid.

Input: None

Output: None
*************************************************************************/
void MonitorDriveMedia(void)
{
    BYTE        mediaPresentNow;
    BYTE        mountTries;
    SearchRec   searchRecord;

        #if defined ENABLE_USB_MSD_DEMO

    // Call to USBTasks() is assumed to be done in the main. If not you can
    // call it here.
    //USBTasks();	
    mediaPresentNow = USBHostMSDSCSIMediaDetect();
        #elif defined ENABLE_SD_MSD_DEMO
    mediaPresentNow = MDD_MediaDetect();
        #endif
    if(mediaPresentNow != mediaPresent)
    {
        if(mediaPresentNow)
        {
            mountTries = 10;
            while(!FSInit() && mountTries--);
            if(!mountTries)
            {
                //UART2PrintString("APP: Could not mount media\r\n");
                mediaPresent = FALSE;
            }
            else
            {
                mediaPresent = TRUE;

                // Find the volume label.  We need to do this here while we are at the
                // root directory.
                if(!FindFirst("*.*", ATTR_VOLUME, &searchRecord))
                {
                    strcpy(volume.label, searchRecord.filename);
                    volume.valid = TRUE;
                }
            }
        }
        else
        {
            mediaPresent = FALSE;
            volume.valid = FALSE;
        }
    }
}

    #ifdef ENABLE_SD_MSD_DEMO

// This funciton will configure peripheral pin select functions

// for the SPI1 or SPI2 modules on the PIC24FJ256GB110 microcontroller
void Configure_SDSPI_PPS(void)
{
            #if defined(__C30__) || defined(__XC16__)
                #if defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GB210__)

    // Although there are other SPI channels SPI 1 is chosen since USB PICtail does not
    // allow the use of the SD/MMC PICtail to be used with SPI2.
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    // if SPI 1 is used
    RPOR0bits.RP0R = 8;                     // assign RP0 for SCK1
    RPOR7bits.RP15R = 7;                    // assign RP15 for SDO1
    RPINR20bits.SDI1R = 23;                 // assign RP23 for SDI1

    // if SPI 2 is used, ONLY when USB PICTAIL is not used with
    // SD/MMC PICtail
    //RPOR10bits.RP21R = 11;  // assign RP21 for SCK2
    //RPOR9bits.RP19R = 10;  // assign RP19 for SDO2
    //RPINR22bits.SDI2R = 26; // assign RP26 for SDI2
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    //  enable a pull-up for the card detect, just in case the SD-Card isn't attached
    //  then lets have a pull-up to make sure we don't think it is there.
    // for SPI1 use this:
    CNPU5bits.CN68PUE = 1;
                #endif //#if defined( __PIC24FJ256GB110__ )
    			#if defined (__PIC24FJ256DA210__)            

    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR1bits.RP2R = 11;                    // assign RP2 for SCK2
    RPOR0bits.RP1R = 10;                    // assign RP1 for SDO2
    RPINR22bits.SDI2R = 0;                  // assign RP0 for SDI2
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS                

                #endif // #if defined (__PIC24FJ256DA210__)
                
                #if defined(__PIC24FJ256GA110__)

    // Although there are other SPI channels SPI 1 is chosen since USB PICtail does not
    // allow the use of the SD/MMC PICtail to be used with SPI2.
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    // use SPI2 for PIC24FJ256GA110 PIM
    RPOR10bits.RP21R = 11;                  // assign RP21 for SCK2
    RPOR9bits.RP19R = 10;                   // assign RP19 for SDO2
    RPINR22bits.SDI2R = 26;                 // assign RP26 for SDI2
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    //  enable a pull-up for the card detect, just in case the SD-Card isn't attached
    //  then lets have a pull-up to make sure we don't think it is there.
    // for SPI2 use this:
    CNPU5bits.CN77PUE = 1;
                #endif //#if defined( __PIC24FJ256GA110__ )
            #endif //#if defined (__C30__)        	
}

    #endif
#endif //ENABLE_USB_MSD_DEMO
