/*$6*/



#ifndef _JPEGDEMO_H
    #define _JPEGDEMO_H

////////////////////////////// INCLUDES //////////////////////////////
    #ifdef __PIC32MX
        #include <plib.h>
    #elif defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #else
        #include <p24Fxxxx.h>
    #endif
    #include "GenericTypeDefs.h"
    #include "Graphics/Graphics.h"

    #include "Image Decoders/ImageDecoder.h"
    #include "MDD File System/FSIO.h"

// Object IDs
    #define ID_JPGLISTBOX   0xFC10                              // List Box ID
    #define ID_BTNUP4LB     0xFC11                              // Up Button ID
    #define ID_BTNDN4LB     0xFC12                              // Down Button ID
    #define ID_SLD4LB       0xFC13                              // Slider ID
    
    /////////////////////////////////////////////////////////////////////////////

//                              OBJECT DIMENSIONS DEFINES
/////////////////////////////////////////////////////////////////////////////
    #define SCROLLBTNWIDTH  25
    #define SCROLLBTNHEIGHT 25

// list box dimensions
    #define LBJPGXPOS       (0)
    #define LBJPGYPOS       (0)
    #define LBJPGWIDTH      (GetMaxX() - SCROLLBTNWIDTH - 1)    // width		
    #define LBJPGHEIGHT     (GetMaxY() - 36)                    // height (36 is taken from the dimension of the navigation control buttons)
    #define BTNUP4LBXPOS    (LBJPGXPOS + LBJPGWIDTH + 1)
    #define BTNUP4LBYPOS    (LBJPGYPOS)
    #define BTNUP4LBWIDTH   (SCROLLBTNWIDTH)
    #define BTNUP4LBHEIGHT  (SCROLLBTNHEIGHT)
    #define SLD4LBXPOS      (BTNUP4LBXPOS)
    #define SLD4LBYPOS      (BTNUP4LBYPOS + SCROLLBTNHEIGHT + 1)
    #define SLD4LBWIDTH     (SCROLLBTNWIDTH)
    #define SLD4LBHEIGHT    (LBJPGHEIGHT - ((SCROLLBTNHEIGHT + 1) << 1))
    #define BTNDN4LBXPOS    (BTNUP4LBXPOS)
	#define BTNDN4LBYPOS	(LBJPGHEIGHT-SCROLLBTNHEIGHT)    
    #define BTNDN4LBWIDTH   (SCROLLBTNWIDTH)
    #define BTNDN4LBHEIGHT  (SCROLLBTNHEIGHT)

/************************************************************************
 Function: WORD CreateJPEGDemo(void)
                                                                       
 Overview: Creates the JPEG demo screen.
  		                                         
 Input: none
                                                                       
 Output: Return 0 when memory is not enough for objects of the demo.
************************************************************************/
WORD    CreateJPEGDemo(void);

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
WORD    JPEGMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

/************************************************************************
 Function: WORD JPEGDrawCallback(void)
 
 Overview: This function is called by GOLDraw() function when the state 
 		   is in this particular demo.

 Input: none

 Output: If the function returns non-zero the draw control will be passed 
		to GOL. GOLDraw() can proceed and re-draw objects that needs 
		to be redrawn.
************************************************************************/
WORD    JPEGDrawCallback(void);

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
void    MonitorDriveMedia(void);

    #ifdef ENABLE_SD_MSD_DEMO

/************************************************************************
Function: void Configure_SDSPI_PPS(void)

Precondition: None

Overview: This function sets the peripheral pin select registers
		  to map the SD SPI to the correct pins.

Input: None

Output: None
*************************************************************************/
void    Configure_SDSPI_PPS(void);
    #endif
#endif
