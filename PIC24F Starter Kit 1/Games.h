/******************************************************************************

Games.h

This is the header file for the interactive graphical demonstrations that are
provided with the PIC24F Starter Kit, inplemented in Games.c.

******************************************************************************/

void DrawBlaster( void );
void DrawSnake( void );

WORD ProcessMessageBlaster( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageScore( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageShapelet( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageSnake( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageGames( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );

void ShowScreenBlaster( void );
void ShowScreenScore( void );
void ShowScreenShapelet( void );
void ShowScreenSnake( void );
void ShowScreenGames( void );

void TranslateMessageBlaster( GOL_MSG *pMsg );
void TranslateMessageScore( GOL_MSG *pMsg );
void TranslateMessageShapelet( GOL_MSG *pMsg );
void TranslateMessageSnake( GOL_MSG *pMsg );
