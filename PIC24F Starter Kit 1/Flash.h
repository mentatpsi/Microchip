/******************************************************************************

Flash.h

This is the header file for the USB flash drive demo of the PIC24F Starter
Kit, implemented in Flash.c.

******************************************************************************/


#define USB_APP_CANNOT_OPEN_FILE    0xF0

void FlashMonitorMedia( void );

WORD ProcessMessageFlash( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageFlashError( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );

void ShowScreenFlash( void );
void ShowScreenFlashError( void );

void TranslateMessageFlash( GOL_MSG *pMsg );
void TranslateMessageFlashError( GOL_MSG *pMsg );

