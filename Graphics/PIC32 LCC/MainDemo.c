/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        AppNotesDemo.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 03/17/11	    ...
 *****************************************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics/Graphics.h"
#include "drv_spi.h"
#include "SST25VF016.h"   
#include "TouchScreen.h"
#include "AlphaBlendScreen.h"
#include "JPEGImage.h"
#include "BitmapImage.h"
#include "InternalResources.h"
#include "Graphics/mchpGfxLCC.h"
#include "TimeDelay.h"

// Configuration bits
#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
#pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = OFF, FNOSC = PRIPLL
#pragma config CP = OFF, BWP = OFF, PWP = OFF

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    CREATE_MAINMENU = 0,
    DISPLAY_MAINMENU,
    CREATE_ALPHABLEND,
    DISPLAY_ALPHABLEND,
} SCREEN_STATES;

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT'S IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_ICON1 		    0xF010
#define ID_ICON2 		    0xF011
#define ID_ICON3 		    0xF012
#define ID_ICON4 		    0xF013
#define ID_SCROLL1 		    0xF014
#define ID_SCROLL2 		    0xF015
#define ID_SCROLL3 		    0xF016
#define ID_SCROLL4 		    0xF017
#define ID_BACK             0xF050

/////////////////////////////////////////////////////////////////////////////
//                              OBJECT DIMENSIONS
/////////////////////////////////////////////////////////////////////////////
#define ICON_WIDTH          (72)
#define ICON_HEIGHT         (100)   
#define ICON_COLUMNSNUM     (2)
#define ICON_ROWSNUM        (2)
#define ICON_XINDENT        ((GetMaxX() - ICON_COLUMNSNUM * ICON_WIDTH) / (ICON_COLUMNSNUM + 1))
#define ICON_YINDENT        ((GetMaxY() - ICON_ROWSNUM * ICON_HEIGHT) / (ICON_ROWSNUM + 1))
#define IconTop(row)        (((row + 1) * ICON_YINDENT) + (row * ICON_HEIGHT))
#define IconBottom(row)     ((row + 1) * (ICON_YINDENT + ICON_HEIGHT))
#define IconLeft(column)    (((column + 1) * ICON_XINDENT) + (column * ICON_WIDTH))
#define IconRight(column)   ((column + 1) * (ICON_XINDENT + ICON_WIDTH))

/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
#ifdef SPI_CHANNEL_1_ENABLE
    #if defined (USE_SST25VF016)
        #ifdef __PIC32MX
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
        #else    
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
        #endif
    #elif defined (USE_MCHP25LC256)       
        const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};
    #endif    
#endif

/////////////////////////////////////////////////////////////////////////////
// SPI Device Initialization Function 
/////////////////////////////////////////////////////////////////////////////
    // initialize GFX3 SST25 flash SPI
    #define FlashInit(pInitData) SST25Init((DRV_SPI_INIT_DATA*)pInitData)                    


/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
// iinternal flash image
extern const IMAGE_FLASH    MCHPFolderFile_8bpp_72x72;
extern const IMAGE_FLASH    MCHPFolderEmpty_8bpp_72x72;

/////////////////////////////////////////////////////////////////////////////
//                            FONTS USED
/////////////////////////////////////////////////////////////////////////////
extern const FONT_FLASH     Gentium_16;

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            TickInit(void);            // starts tick counter
void            InitializeBoard(void);          
WORD            CreateMainMenu();
WORD            CreateBackButton();
void            SpeedDemo(void);
void            PIPDemo(void);
void            PagingDemo(void);
void            ScrollScreen(BYTE direction, WORD delay);

volatile DWORD  tick = 0, prevTick;             // tick counter,

/////////////////////////////////////////////////////////////////////////////
//                            IMAGES USED
/////////////////////////////////////////////////////////////////////////////
// see InternalResources.h

/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
GOL_SCHEME      *demoScheme;                // alternative style scheme
GOL_SCHEME      *AlphaScheme;                // alternative style scheme
IMAGE_FLASH     *pIconPressed  = (void*) &MCHPFolderFile_8bpp_72x72;
IMAGE_FLASH     *pIconReleased = (void*) &MCHPFolderEmpty_8bpp_72x72;

// current state of main demo state mashine
SCREEN_STATES   screenState = CREATE_MAINMENU;               


XCHAR Labels[4][10] = { {'P','I','P',0},
                       {'S','p','e','e','d',0},
                       {'A','.','B','l','e','n','d',0},
                       {'P','a','g','i','n','g',0},
                     };

XCHAR Scroll[4][3] = { {'<',0},
                       {'/',0x5c,0},
                       {'>',0},
                       {0x5c,'/',0},
                     };

XCHAR MainMenuStr[5] = {'M','A','I','N',0};

/* */
int main(void)
{
    GOL_MSG msg;                    // GOL message structure to interact with GOL
   
    InitializeBoard(); 

    ImageDecoderInit();
	JPEGInit();                  // Initialize JPEG

    demoScheme = GOLCreateScheme();  // create alternative style scheme
    AlphaScheme = GOLCreateScheme();  // create alternative style scheme

    demoScheme->TextColor0 = BRIGHTBLUE;
    demoScheme->TextColor1 = BRIGHTRED;
    demoScheme->pFont = (void*)&Gentium_16;
 
    AlphaScheme->Color0 = RGBConvert(0x4C, 0x8E, 0xFF);
    AlphaScheme->Color1 = RGBConvert(255, 102, 0);
    AlphaScheme->EmbossDkColor = RGBConvert(0x1E, 0x00, 0xE5);
    AlphaScheme->EmbossLtColor = RGBConvert(0xA9, 0xDB, 0xEF);
    AlphaScheme->ColorDisabled = RGBConvert(0xD4, 0xE1, 0xF7);
    AlphaScheme->TextColor1 = BRIGHTBLUE;
    AlphaScheme->TextColor0 = RGBConvert(255, 102, 0);
    AlphaScheme->TextColorDisabled = RGBConvert(0xB8, 0xB9, 0xBC);
    AlphaScheme->AlphaValue = 50;
    
    CreateMainMenu();

    screenState = DISPLAY_MAINMENU;    

    while(1)
    {
        if(GOLDraw())
        {                           // Draw GOL object
                TouchGetMsg(&msg);      // Get message from touch screen
                GOLMsg(&msg);           // Process message    
        }
    }

}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateBackButton()
// Input: none
// Output: Returns 0 if the screen is created and 0 if not.
// Overview: Creates the four icons to launch each of the application 
//           note demos.
/////////////////////////////////////////////////////////////////////////////
WORD CreateBackButton()
{
    BUTTON *pObj;

    pObj = BtnCreate (  ID_BACK,     // object�s ID
                        20, 10,
                        70, 35, 0,
                        BTN_DRAW,   
                        NULL,
                        MainMenuStr,  
                        demoScheme
                     );
                     
    if(pObj == NULL)
        return (0);        
        
    return 1;
    
}    

/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateMainMenu()
// Input: none
// Output: Returns 0 if the screen is created and 0 if not.
// Overview: Creates the four icons to launch each of the application 
//           note demos.
/////////////////////////////////////////////////////////////////////////////
WORD CreateMainMenu()
{

    SetColor(WHITE);
    ClearDevice();

    int i;
    SHORT left = 0, top = 0, right = 0, bottom = 0;

            BtnCreate ( ID_SCROLL1,     // object�s ID
                        0, 26,
                        25, GetMaxY()-26,
                        0,              
                        BTN_DRAW,   
                        NULL,  // use this bitmap 
                        Scroll[0],      // use this text
                        demoScheme);      // use alternative style scheme
            BtnCreate ( ID_SCROLL2,     // object�s ID
                        0, 0,
                        GetMaxX(), 25,
                        0,              
                        BTN_DRAW,   
                        NULL,  // use this bitmap 
                        Scroll[1],      // use this text
                        demoScheme);      // use alternative style scheme
            BtnCreate ( ID_SCROLL3,     // object�s ID
                        GetMaxX()-25, 26,
                        GetMaxX(), GetMaxY()-26,
                        0,              // radius if rounded edge
                        // draw the object after creation with
                        // text drawn at the bottom and no panel
                        BTN_DRAW,   
                        NULL,  // use this bitmap 
                        Scroll[2],      // use this text
                        demoScheme);      // use alternative style scheme
             BtnCreate( ID_SCROLL4,     // object�s ID
                        0, GetMaxY()-25,
                        GetMaxX(), GetMaxY(),
                        0,
                        BTN_DRAW,   
                        NULL,  // use this bitmap 
                        Scroll[3],      // use this text
                        demoScheme);      // use alternative style scheme

    // loop to create the four icons
    for (i=0; i<4; i++)
    {
        switch (i)
        {
            case 0:
                top    = IconTop(0);
                bottom = IconBottom(0);
                left   = IconLeft(0);
                right  = IconRight(0);      
                break;
            case 1:
                top    = IconTop(0);
                bottom = IconBottom(0);
                left   = IconLeft(1);
                right  = IconRight(1);      
                break;
            case 2:
                top    = IconTop(1);
                bottom = IconBottom(1);
                left   = IconLeft(0);
                right  = IconRight(0);      
                break;
            case 3:
                top    = IconTop(1);
                bottom = IconBottom(1);
                left   = IconLeft(1);
                right  = IconRight(1);      
                break;
            default: break;    
        }    

        if (!BtnCreate (
                        ID_ICON1+i,     // object�s ID
                        left, top,
                        right, bottom,
                        0,              // radius if rounded edge
                        // draw the object after creation with
                        // text drawn at the bottom and no panel
                        BTN_DRAW|BTN_NOPANEL|BTN_TEXTBOTTOM,   
                        pIconReleased,  // use this bitmap 
                        Labels[i],      // use this text
                        demoScheme      // use alternative style scheme
                        )
            )
            return 0;   
    }
    return 1;
    
}    

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed by default
// Overview: it's a user defined function. GOLMsg() function calls it each
//           time the valid message for the object received
/////////////////////////////////////////////////////////////////////////////
WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD objID;

    // check first if we need to exit the selected demo (if applicable)
    if ((objMsg == BTN_MSG_RELEASED) && (pObj->ID == ID_BACK))
    {   
        // check if the button is indeed pressed
        if (GetState(pObj, BTN_PRESSED))
        {
            screenState = CREATE_MAINMENU;
            return 1;
        }    
    }
    // process messages for demo screens
    switch(screenState)
    {
        case DISPLAY_MAINMENU:

            // In this portion, all management of the Button states are 
            // done here. This portion will always return 0 to inhibit
            // the button default message processing to take place.
                        
            objID = GetObjID(pObj);
            if ((objID == ID_ICON1) || (objID == ID_ICON2) ||
                (objID == ID_ICON3) || (objID == ID_ICON4))
            {    
                if(objMsg == BTN_MSG_PRESSED) 
                {    
                    // check if button is pressed and show pressed icon
                    BtnSetBitmap(pObj, pIconPressed);               
                    SetState(pObj, BTN_PRESSED);
                }
                else if(objMsg == BTN_MSG_RELEASED)
                {
                    // check if button is released and show release icon
                    BtnSetBitmap(pObj, pIconReleased);              
                    ClrState(pObj, BTN_PRESSED);
                    if (objID == ID_ICON1)
                        {
                         SwitchOffDoubleBuffering();
                         PIPDemo();
                        
                         screenState = CREATE_MAINMENU;
                         }
                    else if (objID == ID_ICON2)
                        {
                         SwitchOffDoubleBuffering();
                         SpeedDemo();
                         SwitchOnDoubleBuffering();
                         screenState = CREATE_MAINMENU;
                        }  
                        
                    else if (objID == ID_ICON3)
                        {
                        screenState = CREATE_ALPHABLEND;
                        } 
                   else if (objID == ID_ICON4)
                        {
                        SwitchOffDoubleBuffering();
                        PagingDemo();
                        SwitchOnDoubleBuffering();
                        screenState = CREATE_MAINMENU;
                        }
                    else
                        // this should not happen 
                        screenState = DISPLAY_MAINMENU;    
                }
                else if(objMsg == BTN_MSG_STILLPRESSED)
                {
                    // do not do anything since it is already pressed
                    return 0;     
                }    
                else
                {
                    // check if button is released and show release icon
                    BtnSetBitmap(pObj, pIconReleased);              
                    ClrState(pObj, BTN_PRESSED);              // reset pressed
                }            
                
                SetState(pObj, DRAW);
                return 0;
            }
            if ((objID == ID_SCROLL1) || (objID == ID_SCROLL2) ||
                (objID == ID_SCROLL3) || (objID == ID_SCROLL4))
            {    
                if(objMsg == BTN_MSG_RELEASED) 
                {
                 switch(objID) //Up = 1, Down = 2, Left = 3, Right = 4 
                 {
                    case ID_SCROLL1:
                    ScrollScreen(3,100);
                    break;
                    case ID_SCROLL2:
                    ScrollScreen(1,100);
                    break;
                    case ID_SCROLL3:
                    ScrollScreen(4,100);
                    break;
                    case ID_SCROLL4:
                    ScrollScreen(2,100);
                    break;
                 }

                }
            }
            break;

        case DISPLAY_ALPHABLEND:
            return (MsgAlphaBlendScreenCallback(objMsg, pObj, pMsg));

        default:
            break;
    }        

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: it's a user defined function. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while

//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
WORD GOLDrawCallback(void)
{
    
   switch(screenState)
    {
        case CREATE_MAINMENU:
            GOLFree();
            SetColor(WHITE);
            ClearDevice();
            
            CreateMainMenu();
            
            screenState = DISPLAY_MAINMENU;       // switch to next state
            break;
            
        case DISPLAY_MAINMENU:
            break;         

        case CREATE_ALPHABLEND:
            GOLFree();
            
            CreateBackButton();
            CreateAlphaBlendScreen();
            
            screenState = DISPLAY_ALPHABLEND;         // switch to next state
            break;
            
        case DISPLAY_ALPHABLEND:
            break;
            
        default: 
            break;    
    }    
    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: Timer3 ISR
// Input: none
// Output: none
// Overview: increments tick counter. Tick is approx. 1 ms.
/////////////////////////////////////////////////////////////////////////////
    #define __T3_ISR    __ISR(_TIMER_3_VECTOR, ipl4)


/* */
void __T3_ISR _T3Interrupt(void)
{
    tick++;

    TMR3 = 0; 
	// Clear flag
    mT3ClearIntFlag();

	TouchDetectPosition();    
}

/////////////////////////////////////////////////////////////////////////////
// Function: void TickInit(void)
// Input: none
// Output: none
// Overview: Initilizes the tick timer.
/////////////////////////////////////////////////////////////////////////////

/*********************************************************************
 * Section: Tick Delay
 *********************************************************************/
#define SAMPLE_PERIOD       500 // us
#define TICK_PERIOD			(GetPeripheralClock() * SAMPLE_PERIOD) / 4000000

/* */
void TickInit(void)
{
    // Initialize Timer3
    OpenTimer3(T3_ON | T3_PS_1_8, TICK_PERIOD);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_4);   
}

/////////////////////////////////////////////////////////////////////////////
// Function: InitializeBoard()
// Input: none
// Output: none
// Overview: Initializes the hardware components including the PIC device
//           used.
/////////////////////////////////////////////////////////////////////////////
void InitializeBoard(void)
{

        /////////////////////////////////////////////////////////////////////////////
        // ADC Explorer 16 Development Board Errata (work around 2)
        // RB15 should be output
        /////////////////////////////////////////////////////////////////////////////
         LATBbits.LATB15 = 0;
         TRISBbits.TRISB15 = 0;

        INTEnableSystemMultiVectoredInt();
        SYSTEMConfigPerformance(GetSystemClock());

        GOLInit();

    //The following are PIC device specific settings for the SPI channel
     //used. 
    
     //Set IOs directions for SST25 SPI  
     SST25_CS_LAT = 1;
     SST25_CS_TRIS = 0;

    // initialize the Flash Memory driver
    FlashInit(&SPI_Init_Data);
    
    // initialize the timer that manages the tick counter
    TickInit();                     	

    // initialize the components for Resistive Touch Screen
    TouchInit(NVMWrite, NVMRead, NVMSectorErase, NULL);   	

} 

void PIPDemo(void)
{
        int        x,y;

        XCHAR Title1[] = {'P','I','P',' ','D','e','m','o',0};
        XCHAR Title2[] = {'P','a','g','e',0};
        XCHAR Title3[] = {'S','t','a','r','t',0};

        SetColor(BLACK);
        ClearDevice();

        SetActivePage(PIP_BUFFER);

        SetColor(BLACK);
        ClearDevice();

        PutImage(0,0, (void*)&mouse,1);    //Draw Mouse in cursor area
        PutImage(15,0, (void*)&Pacman,1);   //Draw Pacman in cursor area
   
        SetActivePage(0);
        SetFont((void *) &Monospaced_plain_36);
        SetColor(WHITE);
        OutTextXY(0,40,(XCHAR*)Title1);
        OutTextXY(0,90,(XCHAR*)Title2);

        BevelGradient(200,125,300,165,5,WHITE,BLACK,100,GRAD_DOWN);
        SetColor(BLACK); 
        OutTextXY(195,115,(XCHAR*)Title3);

        x=(1);
        y=(224);

             while(x<280)
             {
             SetPIPWindow(x,y--,15,15,0,0);
             DelayMs(40);
             x+=5;
             }

             DelayMs(500);

             BevelGradient(200,125,300,165,5,BRIGHTBLUE,BLACK,100,GRAD_DOWN);
             SetColor(WHITE); 
             OutTextXY(195,115,(XCHAR*)Title3);

             DelayMs(1000);
             SetPIPWindow(0,0,0,0,0,0);                   //Remove cursor
             DelayMs(1000);
           
             x = GetMaxX()-50;
             SetColor(BLACK);

             while(x>5)
             {
             SetPIPWindow(x,32,45,45,15,0);     DelayMs(40);
             SetPIPWindow(x-5,32,45,45,60,0);   DelayMs(40);                          
             x -= 10;
             }

              SetPIPWindow(0,0,0,0,0,0);

} 

void SpeedDemo(void)
{

WORD counter =0;
float       randl,randt,randr,randb,temp;

while(counter++ < 1500)
{
	    // draw random bars in the middle of the screen
        SetColor(rand()%65536);
 
        randl = rand()%GetMaxX();
        randr = rand()%GetMaxX();

        if(randl > randr)
        {
           temp = randl;
           randl = randr;
           randr = temp;
        }

        randt = rand()%GetMaxY();
        randb = rand()%GetMaxY();

        if(randt > randb)
        {
           temp = randt;
           randt = randb;
           randt = temp;
        }

         Bar(randl,randt,randr,randb);

}

}

void PagingDemo(void)
{

  int        x,y;
  XCHAR      Scroll[] = {'C','o','l','o','r','s',' ','C','h','a','n','g','e',' ','w',' ','T','e','m','p','s',0};
  XCHAR      Cloud[] = {'C','l','o','u','d','y',0};
  XCHAR      Temper[] = {'T','e','m','p','e','r','a','t','u','r','e',0};
  XCHAR      SetTemp[] = {'S','e','t',' ','T','e','m','p',0};
  XCHAR      Time[] = {'8',':','2','0',' ','P','M',0};
  XCHAR      Date[] = {'J','u','n','e',' ','1','1',',',' ','2','0','1','2',0};
  XCHAR      Room[] = {'7','4',0};

              SetActivePage(PIP_BUFFER);
              BarGradient(0,0,GetMaxX(),55,RGBConvert(55,0,0),BLACK,100,GRAD_DOWN);
              SetColor(WHITE);
             
              #if defined(GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)
              SetFont((void *) &GOLFontDefault);
              OutTextXY(0,0,(XCHAR*)Scroll);
              #else
              SetFont((void *) &Monospaced_plain_36);
              OutTextXY(0,0,(XCHAR*)Scroll);
              #endif


  XCHAR Temp[] ={'7','3',0}; 

for(x=0;x<2;x++)        
{  

  SetActivePage(x);

 if(x==0)
  {BarGradient(0,0,GetMaxX(),GetMaxY(),BRIGHTRED,BLACK,100,GRAD_DOWN);}
  else
  {BarGradient(0,0,GetMaxX(),GetMaxY(),BRIGHTBLUE,BLACK,100,GRAD_DOWN);
  }

  BevelGradient(340,120,370,140,10,WHITE,BLACK,100,GRAD_DOWN);
  BevelGradient(340,165,370,185,10,WHITE,BLACK,100,GRAD_DOWN);
  SetColor(WHITE);
  SetFont((void *) &GOLFontDefault);
  OutTextXY(50,75,(XCHAR*)Cloud);
  OutTextXY(190,75,(XCHAR*)Temper);
  OutTextXY(380,75,(XCHAR*)SetTemp);

#define RED5(color16)   (BYTE) ((color16 & 0xF800) >> 8)
#define GREEN6(color16) (BYTE) ((color16 & 0x07E0) >> 3)
#define BLUE5(color16)  (BYTE) ((color16 & 0x001F) << 3)

WORD r,g,b,col;
int z;
  for(z=0;z<(GetMaxX()+1);z++){
    for(y=0;y<32;y++){
      col = GetPixel(z,y);
      r = RED5(col); g = GREEN6(col); b = BLUE5(col);
      SetColor(RGBConvert(r>>2,g>>2,b>>2));
      PutPixel(z,y);
    }}

  SetColor(WHITE);
  OutTextXY(GetMaxX()-80,0,(XCHAR*)Time);
  OutTextXY(10,0,(XCHAR*)Date);

  SetFont((void *) &Monospaced_plain_36);
  OutTextXY(345,100,(XCHAR*)"+");
  OutTextXY(345,145,(XCHAR*)"-");

  SetFont((void *) &Monospaced_bold_Bold_72);

 if(x==0)
 { 
    OutTextXY(210,100,(XCHAR*)Temp);   //Room Temp
 }

  OutTextXY(385,100,(XCHAR*)Room);

        ImageDecode                            
        (                                      
            JPEGfopen((void *)&clouds),         
            IMG_JPEG,                          
            10,                              
            GetMaxY()-171,                               
            (GetMaxX() + 1),                   
            (GetMaxY() + 1),                   
            0,                                 
            &_jpegFileApi,                     
            JPEG_PixelOutput                   
        );

}

  SetActivePage(0);
  SetVisualPage(0);

  DelayMs(1000);
  SetPIPWindow(1,GetMaxY()-56,GetMaxX()-10,55,0,0);DelayMs(500);SetPIPWindow(0,0,0,0,0,0);DelayMs(500);
  SetPIPWindow(1,GetMaxY()-56,GetMaxX()-10,55,0,0);DelayMs(500);SetPIPWindow(0,0,0,0,0,0);DelayMs(500);

  x = 0;

  while(x++<465)
  {
   SetPIPWindow(1,GetMaxY()-56,GetMaxX()-10-x,55,x,0);
   DelayMs(10);
  }

  SetPIPWindow(0,0,0,0,0,0);
  SetColor(WHITE);

 for(x=0;x<1;x++)
 {
  Temp[1] += 1; DelayMs(1000);
  BarGradient(250,116,300,GetMaxY(),RGBConvert(148,0,0),BLACK,100,GRAD_DOWN);
  SetColor(WHITE);SetFont((void *) &GOLFontDefault);OutTextXY(190,75,(XCHAR*)Temper);
  SetFont((void *) &Monospaced_bold_Bold_72);OutTextXY(210,100,(XCHAR*)Temp); 
  } 

SetActivePage(1);
OutTextXY(210,100,(XCHAR*)Temp);
SetVisualPage(1); 

 DelayMs(2000);
 SetActivePage(0);
 SetColor(WHITE);
 ClearDevice();


}

void ScrollScreen(BYTE direction, WORD delay)
{

   scrollPage = 1;
   //Up = 1, Down = 2, Left = 3, Right = 4 

   switch(direction)
   {

   case 1:
     scroll = 1;
     scrollLine = GetMaxY();
     while(scrollLine > 0)
     {
      Delay10us(delay);
      scrollLine--;
     }
   break;
   case 2:
     scroll = 2;
   scrollLine = 0;
  while(scrollLine < GetMaxY())
  {
      Delay10us(delay);
      scrollLine++;
  }
   break;
   case 3:
     scroll = 3;
   scrollLine = GetMaxX();
  while(scrollLine > 1)
  {
      Delay10us(delay);
      scrollLine--;
  }
   break;
   case 4:
     scroll = 4;
   scrollLine = 1;
  while(scrollLine < (GetMaxX()))
  {
      Delay10us(delay);
      scrollLine++;
  }
   break;

   }

scroll = 0;

}

  


