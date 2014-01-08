/******************************************************************************

Utilities.h

This is the header file for the PIC24F Starter Kit utilities implemented in
Utilities.h.

******************************************************************************/

void CalibrateCTMU( void );

WORD ProcessMessageCTMU( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageTime( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageUtilities( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );

void ShowScreenCTMU( void );
void ShowScreenTime( void );
void ShowScreenUtilities(  void );

void TranslateMessageCTMU( GOL_MSG* pMsg );
void TranslateMessageTime( GOL_MSG* pMsg );
