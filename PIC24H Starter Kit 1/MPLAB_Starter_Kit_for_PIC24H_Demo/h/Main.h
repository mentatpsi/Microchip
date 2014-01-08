/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Main.h
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24HJ128GP504
* Compiler:        		MPLAB® C30 v3.11b or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "GraphicsConfig.h"
#include "Graphics/Graphics.h"
#include "Accelerometer.h"
#include "SK_PIC24H.h"


/************************************************************************
 Constants													
 ************************************************************************/
 #if defined USE_GFX_DISPLAY_CONTROLLER_SH1101A
    #define NUM_ACCELEROMETERPADS                   5                       // Number of ACCELEROMETERPADs
    #define STARTING_ADC_CHANNEL            		8
#elif defined USE_GFX_DISPLAY_CONTROLLER_SSD1303
    #define NUM_ACCELEROMETERPADS                   4                       // Number of ACCELEROMETERPADs
    #define STARTING_ADC_CHANNEL            		9
#else
    #error Graphics board not defined
#endif

// ID's for mapping the accelerometer movements to buttons
#define ID_BUTTON_UP                ID_ACCELEROMETER_BUTTON_01
#define ID_BUTTON_RIGHT             ID_ACCELEROMETER_BUTTON_02
#define ID_BUTTON_DOWN              ID_ACCELEROMETER_BUTTON_03
#define ID_BUTTON_LEFT              ID_ACCELEROMETER_BUTTON_04
#define ID_BUTTON_S2	            ID_ACCELEROMETER_BUTTON_05
#define ID_BUTTON_S1	            ID_ACCELEROMETER_BUTTON_06

// ID's of controls on the generic menu screen
#define ID_ICON                     200
#define ID_TITLE                    201
#define ID_TOP_LEFT                 202
#define ID_BOTTOM_LEFT              203
#define ID_TOP_RIGHT                204
#define ID_BOTTOM_RIGHT             205
#define ID_INSTRUCTIONS             206
#define ID_TITLE_X                  207
#define ID_TITLE_Y                  208
#define ID_TITLE_Z                  209
#define ID_INSTRUCTIONSX            210
#define ID_INSTRUCTIONSY            211
#define ID_INSTRUCTIONSZ            212

// Information about sizes when using the generic menu screen
#define LINE_SIZE_INSTRUCTION   40
#define LINE_SIZE_TITLE         30
#define LINE_SIZE_BUTTON_TEXT   15
#define LOCATION_MIN_Y          16

#define ID_MAIN_GRAPH               ID_TOP_LEFT
#define ID_MAIN_EXTSENSOR           ID_TOP_RIGHT
#define ID_MAIN_ORIENTATION         ID_BOTTOM_LEFT
#define ID_MAIN_GAMES               ID_BOTTOM_RIGHT

#define MESSAGE_DEAD_TIME           (1000/(MILLISECONDS_PER_TICK))

#define GRAPH_UPDATE_INTERVAL_INIT  (100 / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_MAX   (2000 / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_MIN   (MILLISECONDS_PER_TICK / MILLISECONDS_PER_TICK)
#define GRAPH_UPDATE_INTERVAL_STEP  (MILLISECONDS_PER_TICK / MILLISECONDS_PER_TICK)

#define  ACCELEROMETERPAD_ALL_BUTTONS_MASK   ( ACCELEROMETERPAD1_MASK |  ACCELEROMETERPAD2_MASK |  ACCELEROMETERPAD3_MASK |  ACCELEROMETERPAD4_MASK |  ACCELEROMETERPAD5_MASK |  ACCELEROMETERPAD6_MASK )

/************************************************************************
 Data Structures													
 ************************************************************************/
typedef enum {
     ACCELEROMETERPAD1 = 0,
     ACCELEROMETERPAD2,
     ACCELEROMETERPAD3,
     ACCELEROMETERPAD4,
     ACCELEROMETERPAD5,
     ACCELEROMETERPAD6,
     ACCELEROMETERPAD_NONE
}  ACCELEROMETERPAD;

typedef enum {
     ACCELEROMETERPAD_NONE_MASK    = 0,
     ACCELEROMETERPAD1_MASK        = 0x01,
     ACCELEROMETERPAD2_MASK        = 0x02,
     ACCELEROMETERPAD3_MASK        = 0x04,
     ACCELEROMETERPAD4_MASK        = 0x08,
     ACCELEROMETERPAD5_MASK        = 0x10,
     ACCELEROMETERPAD6_MASK		  = 0x20
}  ACCELEROMETERPAD_MASK;

typedef enum {
    STATE_START = 0,
    STATE_FIRST_PRESS,
    STATE_SEND_RELEASED_REPEAT,
    STATE_SEND_PRESSED,
    STATE_NEXT_PRESS_WAIT,
    STATE_SEND_RELEASED,
} ACCELEROMETERPAD_STATES;

typedef union
{
    struct
    {
        unsigned    BTN1:1;
        unsigned    BTN2:1;
        unsigned    BTN3:1;
        unsigned    BTN4:1;
        unsigned    BTN5:1;
        unsigned    BTN6:1;
    };
    WORD            val;
} ACCELEROMETERPADS;

// States of the demo application's state machine.
typedef enum
{
    SCREEN_START = 0               		,
    SCREEN_START_DELAY					,
    SCREEN_DISPLAY_MAIN                 ,
    SCREEN_MAIN                         ,
    SCREEN_DISPLAY_GAMES                ,
    SCREEN_GAMES                        ,
    SCREEN_DISPLAY_SNAKE                ,
    SCREEN_SNAKE                        ,
    SCREEN_DISPLAY_SCORE	          	,
    SCREEN_SCORE   	               		,
    SCREEN_DISPLAY_JET              	,
    SCREEN_JET                      	,
    SCREEN_DISPLAY_GRAPH                ,
    SCREEN_GRAPH                        ,
    SCREEN_DISPLAY_ES_INFO              ,
    SCREEN_ES_INFO                		,
    SCREEN_DISPLAY_ES_GRAPH             ,
    SCREEN_ES_GRAPH                		,
    SCREEN_DISPLAY_ORIENTATION			,
    SCREEN_ORIENTATION
} SCREEN_STATES;

typedef struct
{
    XCHAR   title[LINE_SIZE_TITLE];

    XCHAR   buttonTextTopLeft[LINE_SIZE_BUTTON_TEXT];
    XCHAR   buttonTextBottomLeft[LINE_SIZE_BUTTON_TEXT];
    XCHAR   buttonTextTopRight[LINE_SIZE_BUTTON_TEXT];
    XCHAR   buttonTextBottomRight[LINE_SIZE_BUTTON_TEXT];

    void    *buttonBitmapTopLeft;
    void    *buttonBitmapBottomLeft;
    void    *buttonBitmapTopRight;
    void    *buttonBitmapBottomRight;

    XCHAR   instructions[LINE_SIZE_INSTRUCTION];
    
    XCHAR   instructionsX[LINE_SIZE_INSTRUCTION];
    XCHAR   instructionsY[LINE_SIZE_INSTRUCTION];
    XCHAR   instructionsZ[LINE_SIZE_INSTRUCTION];

} PAGE_INFORMATION;

/************************************************************************
 Function Prototypes													
 ************************************************************************/
WORD GOLDrawCallback(void);
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
WORD ProcessMessageMain(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ShowScreenMain(void);
void TickInit(void);
void TranslateAccelerometer(GOL_MSG* pMsg);
WORD ProcessMessageGraph(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ShowScreenGraph(void);
void UpdateGraph(void);
void TranslateMessageGraph(GOL_MSG *pMsg);
void InitializeScreen( void );
void ShowScreenGeneric( void );

extern void initESADC(void);
extern void ShowScreenESInfo(void);
extern void ShowScreenESGraph(void);
extern void UpdateESGraph(void);
extern void ShowScreenOrientation(void);
extern void UpdateOrientation(void);
extern void ShowScreenGames(void);
extern void ShowScreenJet(void);
extern void DrawJet(void);
extern void ShowScreenSnake(void);
extern void DrawSnake(void);
extern void ShowScreenScore(void);
extern WORD ProcessMessageESGraph(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
extern WORD ProcessMessageOrientation(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
extern WORD ProcessMessageGames(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
extern WORD ProcessMessageSnake(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
extern WORD ProcessMessageJet(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
extern WORD ProcessMessageScore(WORD translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
extern void TranslateMessageESGraph(GOL_MSG *pMsg);
extern void TranslateMessageScore(GOL_MSG *pMsg);
extern void TranslateMessageJet(GOL_MSG *pMsg);
extern void TranslateMessageSnake(GOL_MSG *pMsg);
extern void ShowScreenScore(void);





