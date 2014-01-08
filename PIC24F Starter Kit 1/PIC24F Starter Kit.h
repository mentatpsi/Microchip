/******************************************************************************

PIC24F Starter Kit.h

This is the main header file for the PIC24F Starter Kit.  It gathers all the
necessary include files used by the various files of the demo project.  It also
defines constants and data types that are used by multiple files.

******************************************************************************/


#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "GraphicsConfig.h"
#include "Graphics/Graphics.h"
#include "TouchSense.h"
#include "MDD File System/FSIO.h"
#include "USB/usb.h"
#include "USB/usb_host_msd.h"
#include "USB/usb_host_msd_scsi.h"

#include "Demos.h"
#include "Flash.h"
#include "Games.h"
#include "rtcc.h"
#include "Utilities.h"

#include "Graphics/SH1101A_SSD1303.h"

//*****************************************************************************
//*****************************************************************************
// Version Information
//*****************************************************************************
//*****************************************************************************

#define VERSION_STRING  "Demo Firmware v2.0"

//*****************************************************************************
//*****************************************************************************
// Constants and Enumerations
//*****************************************************************************
//*****************************************************************************

#if defined USE_GFX_DISPLAY_CONTROLLER_SH1101A
    #define NUM_TOUCHPADS                   5                       // Number of capacitive TOUCHPADs
    #define STARTING_ADC_CHANNEL            8
#elif defined USE_GFX_DISPLAY_CONTROLLER_SSD1303
    #define NUM_TOUCHPADS                   4                       // Number of capacitive TOUCHPADs
    #define STARTING_ADC_CHANNEL            9
#else
    #error Graphics board not defined
#endif


// ID's for mapping the touchpads to buttons

#define ID_BUTTON_UP                ID_TOUCH_BUTTON_01
#define ID_BUTTON_RIGHT             ID_TOUCH_BUTTON_02
#define ID_BUTTON_DOWN              ID_TOUCH_BUTTON_03
#define ID_BUTTON_LEFT              ID_TOUCH_BUTTON_04
#define ID_BUTTON_CENTER            ID_TOUCH_BUTTON_05


// ID's of controls on the generic menu screen

#define ID_ICON                     200
#define ID_TITLE                    201
#define ID_TOP_LEFT                 202
#define ID_BOTTOM_LEFT              203
#define ID_TOP_RIGHT                204
#define ID_BOTTOM_RIGHT             205
#define ID_THIRDROW					206
#define ID_INSTRUCTIONS             207


// Information about sizes when using the generic menu screen

#define LINE_SIZE_INSTRUCTION   40
#define LINE_SIZE_TITLE         30
#define LINE_SIZE_BUTTON_TEXT   15
#define LOCATION_MIN_Y          16


// States of the demo application's state machine.

typedef enum
{
    SCREEN_BOARD_TEST = 0               ,
    SCREEN_DISPLAY_MAIN                 ,
    SCREEN_MAIN                         ,
    SCREEN_DISPLAY_FLASH                ,
    SCREEN_FLASH                        ,
    SCREEN_DISPLAY_FLASH_ERROR          ,
    SCREEN_FLASH_ERROR                  ,
    SCREEN_DISPLAY_GAMES                ,
    SCREEN_GAMES                        ,
    SCREEN_DISPLAY_SNAKE                ,
    SCREEN_SNAKE                        ,
    SCREEN_DISPLAY_SNAKE_SCORE          ,
    SCREEN_SNAKE_SCORE                  ,
    SCREEN_DISPLAY_BLASTER              ,
    SCREEN_BLASTER                      ,
    SCREEN_DISPLAY_BLASTER_SCORE        ,
    SCREEN_BLASTER_SCORE                ,
    SCREEN_DISPLAY_SHAPELET             ,
    SCREEN_SHAPELET                     ,
    SCREEN_DISPLAY_SHAPELET_SCORE       ,
    SCREEN_SHAPELET_SCORE               ,
    SCREEN_DISPLAY_DEMOS                ,
    SCREEN_DEMOS                        ,
    SCREEN_DISPLAY_RGB                  ,
    SCREEN_RGB                          ,
    SCREEN_DISPLAY_GRAPH                ,
    SCREEN_GRAPH                        ,
    SCREEN_DISPLAY_CAPTURE_MEDIA        ,
    SCREEN_CAPTURE_MEDIA                ,
    SCREEN_DISPLAY_CAPTURE              ,
    SCREEN_CAPTURE                      ,
    SCREEN_DISPLAY_UTILITIES            ,
    SCREEN_UTILITIES                    ,
    SCREEN_DISPLAY_TIME                 ,
    SCREEN_TIME                         ,
    SCREEN_DISPLAY_CTMU                 ,
    SCREEN_CTMU							,
	SCREEN_DISPLAY_BAR_DEMO				,
	SCREEN_BAR_DEMO
} SCREEN_STATES;

//*****************************************************************************
//*****************************************************************************
// Data Types
//*****************************************************************************
//*****************************************************************************

typedef struct
{
    XCHAR   title[LINE_SIZE_TITLE];

    XCHAR   buttonTextTopLeft[LINE_SIZE_BUTTON_TEXT];
    XCHAR   buttonTextBottomLeft[LINE_SIZE_BUTTON_TEXT];
    XCHAR   buttonTextTopRight[LINE_SIZE_BUTTON_TEXT];
    XCHAR   buttonTextBottomRight[LINE_SIZE_BUTTON_TEXT];
	XCHAR 	buttonTextThirdrow[LINE_SIZE_BUTTON_TEXT];

    void    *buttonBitmapTopLeft;
    void    *buttonBitmapBottomLeft;
    void    *buttonBitmapTopRight;
    void    *buttonBitmapBottomRight;
	void	*buttonBitmapThirdrow;

    XCHAR   instructions[LINE_SIZE_INSTRUCTION];

} PAGE_INFORMATION;


//*****************************************************************************
//*****************************************************************************
// Frequently Used Global Variables
//*****************************************************************************
//*****************************************************************************

extern PAGE_INFORMATION         pageInformation;
extern DWORD                    previousTick;
extern SCREEN_STATES            screenState;
extern volatile DWORD           tick;


//*****************************************************************************
//*****************************************************************************
// Function Prototypes
//*****************************************************************************
//*****************************************************************************

void InitializeScreen( void );
void ShowScreenGeneric( void );

