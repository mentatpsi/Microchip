/******************************************************************************

Demos.h

This is the header file for the PIC24F Starter Kit demonstrations that are
implemented in Demos.c.

******************************************************************************/

#ifndef _DEMOS_H_
#define _DEMOS_H_


#define MAX_LOG_BUFFER_SIZE             512
#define NUM_LOG_BUFFERS                 2
#define RGB_SATURATION_MINIMUM          0
#define RGB_SATURATION_MAXIMUM          0xFE    // See comments in Demo.c


typedef struct _LOG_DATA
{
    BYTE        buffer[MAX_LOG_BUFFER_SIZE];
    WORD        index;
    BOOL        bufferFull;
} LOG_DATA;


extern void BarHS2(SHORT xtop, SHORT ytop,SHORT xbot, SHORT ybot);
extern void BarHSP(SHORT xtop, SHORT ytop,SHORT xbot, SHORT ybot, SHORT pat, SHORT xo);

void CaptureMonitorMedia( void );

void GraphReadPotentiometer( void );

void RGBMapColorPins( void );
void RGBSetBlue( WORD saturation );
void RGBSetGreen( WORD saturation );
void RGBSetRed( WORD saturation );
void RGBTurnOffLED( void );
void RGBTurnOnLED( WORD initialRed, WORD initialGreen, WORD initialBlue );

#define ProcessMessageCapture ProcessMessageGraph
WORD ProcessMessageDemos( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageGraph( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageRGB( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );
WORD ProcessMessageBarDemo( WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg );

void ShowScreenCapture( void );
void ShowScreenCaptureMedia( void );
void ShowScreenDemos( void );
void ShowScreenGraph( void );
void ShowScreenRGB( void );
void ShowScreenBarDemo( void );

#define TranslateMessageCapture TranslateMessageGraph
void TranslateMessageRGB( GOL_MSG *pMsg );
void TranslateMessageGraph( GOL_MSG *pMsg );
void TranslateMessageBarDemo( GOL_MSG *pMsg );

void UpdateCapture( void );
void UpdateGraph( void );
void DrawBarDemo ( void );

#endif

