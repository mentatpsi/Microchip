/*********************************************************************
 *                  LCC Graphics Dirver (mchpGfxLCC)
 *********************************************************************
 * FileName:        mchpGfxLCC.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32 v1 or higher
 *                  MPLAB IDE v8 or higher
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ********************************************************************/
#include "HardwareProfile.h"

#if defined (GFX_USE_DISPLAY_CONTROLLER_DMA)
#include "Compiler.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "Graphics/mchpGfxLCC.h"
#include "Graphics/gfxtcon.h"
#include "Graphics/Primitive.h"

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;

#ifdef USE_PALETTE
    #include "Graphics/Palette.h"
    extern void *_palette;
    extern BYTE PaletteBpp;
    extern BYTE blPaletteChangeError;
    extern void *pPendingPalette;
    extern WORD PendingStartEntry;
    extern WORD PendingLength;
    WORD LUT[256];
#endif

SHORT _GFXActivePage=0;
volatile WORD VisualPage=0;

/*This defines the color of the system*/
GFX_COLOR _color;
#ifdef USE_TRANSPARENT_COLOR
GFX_COLOR   _colorTransparent;
SHORT       _colorTransparentEnable;
#endif

/*Functions*/
int _VirtToPhys(const void* p);

#if defined (USE_PALETTE) 
unsigned int HBackPorch = 2*((DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)-1);     
BYTE HFrontPorch = 2;
#else
BYTE HBackPorch = (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)-1;     
BYTE HFrontPorch = 1;          
#endif

volatile BYTE DrawCount =0;                          /* The current status of how many pixels have been drawn inbetween a DMA IR*/
volatile BYTE_VAL overflowcount;                      /* The count for the amount of overflows that have happened in the PMP Adress*/

#ifdef LCC_INTERNAL_MEMORY
BYTE GraphicsFrame[FRAME_HEIGHT][LINE_LENGTH];                   //8 BPP QVGA graphics frame
#else 
char GraphicsFrame[LINE_LENGTH];
#endif 

#ifdef LCC_INTERNAL_MEMORY
     #define PMP_ADDRESS_LINES PMP_PEN_OFF 

     #ifdef USE_PALETTE
         #define PMP_DATA_LENGTH PMP_DATA_BUS_16
     #else
         #define PMP_DATA_LENGTH PMP_DATA_BUS_8
     #endif
 
#else

     #define PMP_ADDRESS_LINES PMP_PEN_ALL
     #define PMP_DATA_LENGTH PMP_DATA_BUS_8
#endif

#define PMP_CONTROL	(PMP_ON | PMP_MUX_OFF | PMP_READ_WRITE_EN|\
             		 PMP_CS2_EN | PMP_WRITE_POL_LO | PCLK_POLARITY)

#define PMP_MODE     (PMP_DATA_LENGTH | PMP_MODE_MASTER2 |\
                     PMP_WAIT_BEG_1 | PMP_WAIT_MID_2 | PMP_WAIT_END_1 )

#ifdef USE_PIP
short _GFXPIPPage = PIP_BUFFER;
volatile WORD PipActive = 0;
#endif

#ifdef USE_DOUBLE_BUFFERING
    #define USE_ALPHABLEND
    BYTE blInvalidateAll;
    BYTE blEnableDoubleBuffering;
    BYTE NoOfInvalidatedRectangleAreas;
    RectangleArea InvalidatedArea[GFX_MAX_INVALIDATE_AREAS];

    volatile DWORD  _drawbuffer;
    volatile BYTE   blDisplayUpdatePending;
	
	/*********************************************************************
	* Function: WORD GetDrawBufferAddress(void)
	*
	* PreCondition: none
	*
	* Input: none
	*
	* Output: Address of the Draw Buffer
	*
	* Side Effects: none
	*
	* Overview: Returns the address of Draw Buffer
	*
	* Note:
	*
	********************************************************************/
    DWORD GetDrawBufferAddress(void)
	{
        return _drawbuffer;
    }
	
	/*********************************************************************
	* Function: DWORD GetFrameBufferAddress(void)
	*
	* PreCondition: _drawbuffer must be set to either GFX_BUFFER1 or GFX_BUFFER2
	*
	* Input: none
	*
	* Output: Address of the Frame Buffer
	*
	* Side Effects: none
	*
	* Overview: Returns the address of Frame Buffer
	*
	* Note: 
	*
	********************************************************************/
    DWORD GetFrameBufferAddress(void)
	{

        if(_drawbuffer == GFX_BUFFER1)
        {
            return GFX_BUFFER2;
        }
        else
        {
            return GFX_BUFFER1;
        }
    }

#endif //USE_DOUBLE_BUFFERING


/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/

void ResetDevice(void)
{

    DMACONbits.SUSPEND =1;    //Suspend ALL DMA transfers

    BMXCONbits.BMXARB = 0x02; //Faster Refresh Rate
    BMXCONbits.BMXCHEDMA = 1;

    LCD_DC_TRIS =0;
    HSYNC_TRIS =0;
    LCD_CS_TRIS =0;
    VSYNC_TRIS =0;
    LCD_RESET_TRIS =0;

    BACKLIGHT_TRIS=0;
    DATA_ENABLE_TRIS=0;

    SRAM_TRIS =0;

    ADDR15_TRIS=0;
    ADDR16_TRIS=0;
    ADDR17_TRIS =0;
    ADDR18_TRIS =0;

    LCD_RESET =1;  
    LCD_CS    =1;      
    LCD_DC    =1;       

    SRAM_CS   =0;
    ADDR17    =0;     
    ADDR18    =0;

    PIXELCLOCK_TRIS =0;

    #ifdef DISP_INV_LSHIFT
    PIXELCLOCK =1;
    #else
    PIXELCLOCK =0;
    #endif

    #if defined(USE_TCON_MODULE)
    GfxTconInit();
    #endif

    // setup the PMP
    mPMPOpen(PMP_CONTROL, PMP_MODE, PMP_ADDRESS_LINES, PMP_INT_ON);
    PMADDR = 0x0000;

	// Open the desired DMA channel.
	DmaChnOpen(1, 0, DMA_OPEN_DEFAULT);

    // set the transfer event control: what event is to start the DMA transfer
     DmaChnSetEventControl(1, DMA_EV_START_IRQ(_TIMER_2_IRQ)); 

   	// set the transfer parameters: source & destination address, source & destination size, number of bytes per event
    #ifdef LCC_INTERNAL_MEMORY
    BACKLIGHT =0;     //Turn Backlight on

    #ifdef USE_PALETTE
    DmaChnSetTxfer(1, &GraphicsFrame[0], (void*)&PMDIN, HBackPorch, 2, 2); 
    #else
    DmaChnSetTxfer(1, &GraphicsFrame[0], (void*)&PMDIN, HBackPorch, 1, 1); 
    #endif
    #else    
    #if defined(GFX_USE_DISPLAY_PANEL_TFT_G240320LTSW_118W_E)
    BACKLIGHT =0;     //Turn Backlight on
    DmaChnSetTxfer(1, (void*)&PMDIN ,&GraphicsFrame[0] , 1, HBackPorch, 2);  
    #else
    BACKLIGHT =1;
    DmaChnSetTxfer(1, (void*)&PMDIN ,&GraphicsFrame[0] , 1, HBackPorch, 16);  
    #endif 
    #endif
    
    INTSetVectorPriority(INT_VECTOR_DMA(1), INT_PRIORITY_LEVEL_6);		        // set INT controller priority
    DmaChnSetEvEnableFlags(1, DMA_EV_BLOCK_DONE);	// enable the transfer done interrupt, when all buffer transferred
    INTEnable(INT_SOURCE_DMA(1), INT_ENABLED);		// enable the chn interrupt in the INT controller

    DCH1CONbits.CHPRI = 0b11;  //DMA channel has highest priority  

    // once we configured the DMA channel we can enable it
	DmaChnEnable(1);
    
   #ifdef LCC_INTERNAL_MEMORY
        #ifdef USE_PALETTE 
        OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, 70); //Start Timer
        #else
   OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, 27); //Start Timer
   #endif
   #else  //External Memory
   OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, 2); //Start Timer
   #endif

   DMACONbits.SUSPEND = 0;

    #ifdef USE_DOUBLE_BUFFERING
    // initialize double buffering feature
    blInvalidateAll = 1;            
    blDisplayUpdatePending = 0;
    NoOfInvalidatedRectangleAreas = 0;
    _drawbuffer = GFX_BUFFER1;
    SetActivePage(_drawbuffer);
    SwitchOnDoubleBuffering();
    #endif //USE_DOUBLE_BUFFERING

}

#ifdef USE_PIP
//PIP Variables (NULL at start)
WORD PipStartT = 0;
WORD PipStartL = 0;
WORD PipVLength = 0;
WORD PipHLength = 0;
static DWORD PipX,PipY;
extern volatile WORD PipActive;
#endif

#ifdef USE_LCC_SCROLLING
WORD scroll,scrollLine,scrollPage =0;
#endif

#ifdef LCC_EXTERNAL_MEMORY
// handler for the DMA channel 1 interrupt
void __attribute__((weak)) __ISR(_DMA1_VECTOR, ipl6) DmaHandler1(void)
{
  static WORD remaining=0;
  static short line =0;
  static BYTE GraphicsState = 1;
  static BYTE prevGraphicsState;
  static WORD pixelAmount=0;

#if defined(USE_PIP) || defined(USE_LCC_SCROLLING)
  static DWORD pixelAddress=0; 
#if defined(USE_PIP)
  static WORD pipLine =0;
#endif
#endif


#ifdef LEGACY_MODE
        static WORD dmatransfersremaining=0;

        if(dmatransfersremaining != 0)
        {

            if(dmatransfersremaining > MAX_DMA_TRANSFER)
             {
                   //Setup DMA Transfer
                   DCH1DSIZ =  MAX_DMA_TRANSFER;
             }
             else
             {
                   //Setup DMA Transfer
                   DCH1DSIZ =  dmatransfersremaining;          
             }

         dmatransfersremaining -= DCH1DSIZ;
         DrawCount=0;
         DCH1INTCLR = 0x08;  //CHBCIF = 0;
         IFS1CLR = 0x20000;  //DMA1IF =0;
         DCH1CONSET =0x80;   //CHEN =1; 
         return;
        }   
#endif

       switch(GraphicsState)
       {
         case 1:

           prevGraphicsState = ++GraphicsState;
           pixelAmount = LINE_LENGTH;
 
            if(line++ >= -DISP_VER_FRONT_PORCH)
            {                   
           
              if(line == (FRAME_HEIGHT)) 
              {         
                VSYNC =0;  
                line=  -VER_BLANK; 
                PMADDRCLR = 0xffff;
                ADDR15=0; ADDR16=0;
                #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
                ADDR17=0; ADDR18=0;
                #endif
                overflowcount.Val=0; 

                #ifdef USE_PIP
                pipLine = 0;
                #endif
 
               }

             else
             { 
               
               PMMODESET = 0x0800;   //INCM = 0b01;
               VSYNC =1; 
               DATA_ENABLE =1;
               #ifdef USE_PIP
               if((line >= PipStartT)&&(line <= (PipStartT + PipVLength))&&(PipVLength != 0))
               {    
                     pixelAddress =  ((line) * LINE_LENGTH);
                     PMADDR =  (pixelAddress)&(0x7FFF);
                     overflowcount.Val = pixelAddress/PMADDR_OVERFLOW;
                     ADDR15 = overflowcount.Val;          //count holds the additional address line count
                     ADDR16 = overflowcount.bits.b1;  
                     pixelAmount = PipStartL;
                     GraphicsState = 4;
                     prevGraphicsState = 4;
               }
               #endif
              #ifdef USE_LCC_SCROLLING
              else if(scroll >0)
              {   
                 switch(scroll)
                 {
                  case 1:             //Up
                  case 2:             //Down
                   if(line<scrollLine)
                   {
                   pixelAddress = (GetMaxY()-(scrollLine-line)) * LINE_LENGTH;
                   ADDR17 = _GFXActivePage;
                   ADDR18 = _GFXActivePage>>1;
                   }
                   else
                   {
                   pixelAddress = (line-scrollLine) * LINE_LENGTH;
                   ADDR17 = scrollPage;
                   ADDR18 = scrollPage>>1;
                   }

                   PMADDR =  (pixelAddress)&(0x7FFF);
                   overflowcount.Val = pixelAddress/PMADDR_OVERFLOW;
                   ADDR15 = overflowcount.Val;          //count holds the additional address line count
                   ADDR16 = overflowcount.bits.b1; 
                   break;   

                  case 3://Left
                  case 4://Right
                   pixelAddress = ((line+1) * LINE_LENGTH) + (GetMaxX()-scrollLine);
                   PMADDR =  (pixelAddress)&(0x7FFF);
                   overflowcount.Val = pixelAddress/PMADDR_OVERFLOW;
                   ADDR15 = overflowcount.Val;          //count holds the additional address line count
                   ADDR16 = overflowcount.bits.b1; 
                  if(scroll == 3)
                  {
                   ADDR17 = _GFXActivePage;
                   ADDR18 = _GFXActivePage>>1;
                  }
                  else
                  {
                   ADDR17 = scrollPage;
                   ADDR18 = scrollPage>>1;
                  }
                   pixelAmount = scrollLine;
                   prevGraphicsState = 7;
                   GraphicsState = 7;
                   break;  
                }
              }
              #endif

               if((PMADDR_OVERFLOW - PMADDR) < (pixelAmount))       
               {   

               GraphicsState = 3;      //Do Overflow routine
               remaining = ((PMADDR_OVERFLOW)- PMADDR);
                
               //Setup DMA Transfer
             #ifdef LEGACY_MODE
               if((remaining) > MAX_DMA_TRANSFER)
               {        
               DCH1DSIZ =  MAX_DMA_TRANSFER;
               dmatransfersremaining = (remaining )- MAX_DMA_TRANSFER; 
               }
                else
            #endif
               {
               DCH1DSIZ =  (WORD)(remaining);
               }    
               break;
               } 

                    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
                        if((PMADDR == 0) && (line >1))
                        {
                            ADDR15 = ++overflowcount.Val;          //count holds the additional address line count
                            ADDR16 = overflowcount.bits.b1; 
                            ADDR17 = overflowcount.bits.b2;
                            ADDR18 = overflowcount.bits.b3;
                        }
                    #endif
            
              }
             }

            //Setup DMA Transfer
            #ifdef LEGACY_MODE
            DCH1DSIZ =  MAX_DMA_TRANSFER;
            dmatransfersremaining = (pixelAmount) - MAX_DMA_TRANSFER; 
            #else
            DCH1DSIZ =  (WORD)(pixelAmount);
            #endif

            break;

       case 3:                    //Adjust the address lines that aren't part of PMP
             remaining = (pixelAmount-remaining);
 
             ADDR15 = ++overflowcount.Val;          //count holds the additional address line count
             ADDR16 = overflowcount.bits.b1; 
             #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
             ADDR17 = overflowcount.bits.b2;
             ADDR18 = overflowcount.bits.b3;
             #endif
             
               //Setup DMA Transfer
              #ifdef LEGACY_MODE
               if((remaining) > MAX_DMA_TRANSFER)
               {        
               DCH1DSIZ =  MAX_DMA_TRANSFER;
               dmatransfersremaining = (remaining)- MAX_DMA_TRANSFER; 
               }
               else
              #endif
               {
               DCH1DSIZ =  (WORD)(remaining);
               } 

             GraphicsState = prevGraphicsState; //goto Front Porch
             break;

   #ifdef USE_PIP
       case 4:     //Draw PIP Line      
                  ADDR17=_GFXPIPPage;
                  ADDR18=_GFXPIPPage>>1;
                  PipActive = 1;
 
                  pixelAddress = (DWORD)(((PipY+pipLine++)*(DISP_HOR_RESOLUTION))+(PipX));

                  PMADDR = pixelAddress&(0x7FFF);
                  overflowcount.Val = pixelAddress/PMADDR_OVERFLOW;
                  ADDR15 = overflowcount.Val;          //count holds the additional address line count
                  ADDR16 = overflowcount.bits.b1; 
                  pixelAmount = PipHLength;

                  if(((PMADDR_OVERFLOW - PMADDR) < (pixelAmount)))
                  {
                    prevGraphicsState = 5;
                    GraphicsState = 3; 
                    remaining = ((PMADDR_OVERFLOW)- PMADDR);
                    DCH1DSIZ =  (WORD)((remaining));    
                  }
                  else
                  {
                    DCH1DSIZ = (WORD)(pixelAmount);
                    GraphicsState = 5;
                  }
                  break;

        case 5:    //Finish PIPLine
               
               PipActive = 0;
               pixelAddress = ((line*LINE_LENGTH)+PipHLength+PipStartL);
               PMADDR = pixelAddress&(0x7FFF);
               overflowcount.Val = pixelAddress/PMADDR_OVERFLOW;
               ADDR15 = overflowcount.Val;          //count holds the additional address line count
               ADDR16 = overflowcount.bits.b1;  
               ADDR17 = VisualPage;
               ADDR18 = VisualPage>>1;
               pixelAmount = LINE_LENGTH-PipHLength-PipStartL; 

               if(((PMADDR_OVERFLOW - PMADDR) < pixelAmount)) 
               {   
               prevGraphicsState = 2;
               GraphicsState = 3;      //Do Overflow routine
               remaining = ((PMADDR_OVERFLOW)- PMADDR);
               DCH1DSIZ =  (WORD)((remaining));
               break;
               }

               DCH1DSIZ =  (WORD)((pixelAmount));                  
               GraphicsState = 2; //Draw Back Porch State
               break;
  #endif

  #ifdef USE_LCC_SCROLLING
        case 7://Finish Scroll Line Left/Right
                   pixelAmount = LINE_LENGTH - scrollLine;
                   pixelAddress = ((line+1) * LINE_LENGTH);
                   PMADDR =  (pixelAddress)&(0x7FFF);
                   overflowcount.Val = pixelAddress/PMADDR_OVERFLOW;
                   ADDR15 = overflowcount.Val;          //count holds the additional address line count
                   ADDR16 = overflowcount.bits.b1; 

                if(scroll == 4)
                  {
                   ADDR17 = _GFXActivePage;
                   ADDR18 = _GFXActivePage>>1;
                  }
                  else
                  {
                   ADDR17 = scrollPage;
                   ADDR18 = scrollPage>>1;
                  }

               if(((PMADDR_OVERFLOW - PMADDR) < (pixelAmount))) 
               {   
               prevGraphicsState = 2;
               GraphicsState = 3;      //Do Overflow routine
               remaining = ((PMADDR_OVERFLOW)- PMADDR);
 
               //Setup DMA Transfer
               DCH1DSIZ =  (WORD)(remaining);               
               break;
               } 

             DCH1DSIZ =  (pixelAmount);
             GraphicsState = 2; //goto Front Porch
             break;
    #endif
 
  case 2:   //Front Porch then Back Porch Start
           HSYNC = 0; 
           DATA_ENABLE = 0; 
           PMMODECLR = 0x0800; //  INCM = 0b00; No Address Incrementing here

           GraphicsState = PMDIN;
    
            HSYNC = 1;
            //Setup DMA Back Porch
            DCH1DSIZ =  HBackPorch;

            GraphicsState=1;         
           break;

   default: 
           break;
}

    DrawCount=0;
    DCH1INTCLR = 0x08;  //CHBCIF = 0;
    IFS1CLR = 0x20000;  //DMA1IF =0;
    DCH1CONSET =0x80;   //CHEN =1; 

}
#endif
#ifdef LCC_INTERNAL_MEMORY

void __ISR(_DMA1_VECTOR, ipl6) DmaHandler1(void)
{
  static BYTE GraphicsState = 0;
  static short line =0;

#ifdef USE_PALETTE
  static BYTE *LUTAddress= &GraphicsFrame[0][0];
#endif

    GraphicsState ^=1;

    if(GraphicsState == 1)
    {
            #ifdef USE_PALETTE
            DCH1SSIZ =  LINE_LENGTH<<1;
            #else
            DCH1SSIZ =  LINE_LENGTH;
            #endif

           if(line++ >= -1)       
           { 
            VSYNC =1; 
            DATA_ENABLE =1;
           
            #ifndef USE_PALETTE
            DCH1SSA = _VirtToPhys(&GraphicsFrame[line][0]);
            #endif

            if(line == (FRAME_HEIGHT)) 
            {
                 VSYNC = 0;
                 line = -(VER_BLANK);
                #ifdef USE_PALETTE
                 LUTAddress = &GraphicsFrame[0][0];
                #endif
            }

           #ifdef USE_PALETTE
           BYTE i=0;

          /*Do color LUT Here. Each line is 240 bytes*/
           while(i++<10)  
           {  
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];
            PMDIN = LUT[*LUTAddress++];     
           }

           HSYNC =0; 
           DATA_ENABLE =0;
           //Perform Back Porch Clock Signal
           PMDINSET=1;          
           HSYNC = 1;
           DCH1SSIZ =  HBackPorch;
           GraphicsState = 0;
           #endif

           }        
    }
    else
    {     
           HSYNC =0; 
           DATA_ENABLE =0;

           //Perform Back Porch Clock Signal
           PMDINSET=1;
           
           HSYNC = 1;
           DCH1SSIZ =  HBackPorch;
    }
 
    DCH1INTCLR = 0x08; //CHBCIF = 0;
    IFS1CLR = 0x20000; //DMA1IF =0;
    DCH1CONSET =0x80;  //CHEN =1;
}


#endif

void PutPixel(short x, short y)
{
 
    if(_clipRgn)
    {
        if(x < _clipLeft)
            return;
        if(x > _clipRight)
            return;
        if(y < _clipTop)
            return;
        if(y > _clipBottom)
            return;
    }

    #if (DISP_ORIENTATION	== 270)
    WORD tempx=x;
    x = (DISP_HOR_RESOLUTION-1) - y;
    y = tempx; 
    #elif (DISP_ORIENTATION == 90)
    WORD tempy=y;
    y = (DISP_VER_RESOLUTION-1) - x;
    x = tempy; 
    #elif (DISP_ORIENTATION == 180)
    x = (DISP_HOR_RESOLUTION-1) - x;
    y = (DISP_VER_RESOLUTION-1) - y;
    #endif

#ifdef LCC_INTERNAL_MEMORY
GraphicsFrame[y][x] = _color;
#else

static BYTE pixelcount = 1;
static GFX_COLOR color;
static DWORD_VAL address[2];
static BYTE addressbit[2][4];
static DWORD prevaddr;

    pixelcount ^= 1;      //Toggle between saving the pixel information and sending two pixels to the SRAM

    address[pixelcount].Val = (DWORD)(((y)*(DISP_HOR_RESOLUTION))+(x));
    addressbit[pixelcount][0] = address[pixelcount].bits.b15;
    addressbit[pixelcount][1] = address[pixelcount].bits.b16;
    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
    addressbit[pixelcount][2] = address[pixelcount].bits.b17;
    addressbit[pixelcount][3] = address[pixelcount].bits.b18;
    #endif

    if(!pixelcount)
    {
    color = _color;
    return;
    }    
    
    DrawCount++;

    while(DrawCount>PIXEL_DRAW_PER_DMA_TX){}   //Added to stabilize screen refresh rate

    //Suspend DMA
    DMACONSET = 0x1000;

    while(PMMODEbits.BUSY == 1);
    
    PMCONCLR=0x8000;
    PMMODEbits.MODE16 = 1;
    PMCONSET=0x8000;

    //Perform Write
    ADDR15 = addressbit[1][0];
    ADDR16 = addressbit[1][1];
    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
    ADDR17 = addressbit[1][2];
    ADDR18 = addressbit[1][3];
    #else
    ADDR17 = _GFXActivePage;
    ADDR18 = _GFXActivePage>>1;
    #endif

    //Save previous address value
    prevaddr = PMADDR;     
    PMADDR = address[1].Val;             
    PMDIN = _color;   
    while(PMMODEbits.BUSY == 1);

    //Setup Write 2
    ADDR15 = addressbit[0][0];
    ADDR16 = addressbit[0][1];
    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
    ADDR17 = addressbit[0][2];
    ADDR18 = addressbit[0][3];
    #else
    ADDR17 = _GFXActivePage;
    ADDR18 = _GFXActivePage>>1;
    #endif 
  
    PMADDR = address[0].Val;             
    PMDIN  = (WORD)color;
    while(PMMODEbits.BUSY == 1);

    //Clean-up Address Lines
    ADDR15 = overflowcount.bits.b0;          //count holds the additional address line count
    ADDR16 = overflowcount.bits.b1; 
    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
    ADDR17 = overflowcount.bits.b2;
    ADDR18 = overflowcount.bits.b3;
    #else
#ifdef USE_PIP
    if(PipActive == 1)
    {
      ADDR17=_GFXPIPPage;
      ADDR18=_GFXPIPPage>>1;
    }
   else
#endif 
    {
    ADDR17 = VisualPage;
    ADDR18 = VisualPage>>1;
    }
    #endif
   
    PMADDR = prevaddr; 
    PMMODEbits.MODE16 = 0;

//Restart DMA
    DMACONCLR = 0x1000;
 
#endif
}

/*********************************************************************
* Function: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
WORD IsDeviceBusy(void)
{  
    return 0;
}

#ifdef LCC_INTERNAL_MEMORY
/*********************************************************************
 * Function:        unsigned int _VirtToPhys(const void* p)
 * PreCondition:    None
 * Input:			p	- pointer to be converted.
 * Output:          a physical address corresponding to the virtual input pointer
 * Side Effects:    None
 * Overview:		Virtual to physical translation helper.
 * Note:            None.
 ********************************************************************/
int _VirtToPhys(const void* p)
{
	return (int)p<0?((int)p&0x1fffffffL):(unsigned int)((unsigned char*)p+0x40000000L);
}

#ifdef USE_PALETTE
void StartVBlankInterrupt(void){}
void  EnablePalette(void){}
BYTE SetPaletteBpp(BYTE bpp){return 0;}
BYTE SetPaletteFlash(PALETTE_ENTRY *pPaletteEntry, WORD startEntry, WORD length)
{
    WORD counter;

    if((pPaletteEntry == NULL) || ((startEntry + length) > 256))
    {
        return -1;
    }

    for(counter = 0; counter < length; counter++)
    {
        
        LUT[counter]  = pPaletteEntry[counter].value;

    }

    return 0;
}
#endif

#endif

GFX_COLOR GetPixel(short x, short y)
{

    #if (DISP_ORIENTATION	== 270)
    WORD tempx=x;
    x = DISP_HOR_RESOLUTION - y;
    y = tempx; 
    #elif (DISP_ORIENTATION == 90)
    WORD tempy=y;
    y = DISP_VER_RESOLUTION - x;
    x = tempy; 
    #elif (DISP_ORIENTATION == 180)
    x = DISP_HOR_RESOLUTION - x;
    y = DISP_VER_RESOLUTION - y;
    #endif

#ifdef LCC_INTERNAL_MEMORY
return GraphicsFrame[y][x];
#else
static DWORD prevaddr;
static DWORD address; 
static BYTE addressbit0, addressbit1;
static GFX_COLOR getcolor;
#ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
static BYTE addressbit2,addressbit3;
#endif 

    address = (DWORD)(((y)*DISP_HOR_RESOLUTION+(x)));

    addressbit0 = address>>15;
    addressbit1 = address>>16;
    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
    addressbit2 = address >> 17;
    addressbit3 = address >> 18;
    #endif

    DrawCount++;

    while(DrawCount>PIXEL_DRAW_PER_DMA_TX){}   //Added in WQVGA Driver to stabilize refresh rate

//Suspend DMA
    DMACONSET = 0x1000;

    while(PMMODEbits.BUSY ==1);
    
    PMCONCLR=0x8000;
    PMMODEbits.MODE16 = 1;
    PMCONSET=0x8000;

    PMCONbits.PTRDEN=0;

//Perform Write
    ADDR15 = addressbit0;
    ADDR16 = addressbit1;
    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
    ADDR17 = addressbit2;
    ADDR18 = addressbit3;
    #else
    ADDR17 = _GFXActivePage;
    ADDR18 = _GFXActivePage>>1;
    #endif

//Save previous address value
    prevaddr = PMADDR;     

    PMADDR =   address;
           
    getcolor = PMDIN; 
    while(PMMODEbits.BUSY ==1);
    getcolor = PMDIN;
    while(PMMODEbits.BUSY ==1);

    PMCONbits.PTRDEN=1;

    //Clean-up Address Lines
    ADDR15 = overflowcount.Val;          //count holds the additional address line count
    ADDR16 = overflowcount.bits.b1; 
    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
    ADDR17 = overflowcount.bits.b2;
    ADDR18 = overflowcount.bits.b3;
    #else
#ifdef USE_PIP
    if(PipActive == 1)
    {
      ADDR17=_GFXPIPPage;
      ADDR18=_GFXPIPPage>>1;
    }
   else
#endif 
    {
    ADDR17 = VisualPage;
    ADDR18 = VisualPage>>1;
    }
    #endif

    PMADDR = prevaddr; 

    PMMODEbits.MODE16 = 0;

//ReStart DMA
    DMACONCLR = 0x1000;

return getcolor;
#endif
}

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*

* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    SHORT   x, y;

   if(_clipRgn)
   {
      if(left < _clipLeft)
      left = _clipLeft;
      if(right > _clipRight)
      right = _clipRight;
      if(top < _clipTop)
      top = _clipTop;
      if(bottom > _clipBottom)
      bottom = _clipBottom;
    }

        #ifdef USE_ALPHABLEND_LITE
        if(_alpha != 100)
        {
        BarAlpha(left,top,right,bottom);
        return(1);
        }
        #endif

    #if (DISP_ORIENTATION == 90)
    WORD hchange = right - left;
    WORD vchange = bottom - top;
    WORD tempy=top;
    top = (DISP_VER_RESOLUTION-1) - right;
    left = tempy; 
    bottom = top + hchange;
    right = left + vchange;
    #endif


#ifdef LCC_INTERNAL_MEMORY

   for(y = top; y <= bottom; y++)
      for(x = left; x <= right; x++)
        GraphicsFrame[y][x] = _color;
#else

    static DWORD address; 
    static WORD_VAL  overflowamount;
    static DWORD prevaddr;  
    int i; BYTE burstlength;
    WORD prevMode;

    for(y = top; y <= bottom; y++)
      {  

       address = (DWORD)(((y)*(DISP_HOR_RESOLUTION))+(left));

       x = left;

       while(x <= right)
            {    

                    if(((right-x) < 10))
                    {
                       burstlength = 1;
                       x++;
                    }
                    else
                    {
                       burstlength = 10;
                       x += burstlength;
                    } 

                    overflowamount.Val = (address/PMADDR_OVERFLOW);
                   
                    DrawCount++;
                    
                    while(DrawCount>(PIXEL_DRAW_PER_DMA_TX>>1)){}   //Added in WQVGA Driver to stabilize refresh rate

                    //Suspend DMA
                    DMACONSET = 0x1000;

                    while(PMMODEbits.BUSY==1);  //Make sure DMA is suspended

                    //Setup Write 
                     ADDR15 = overflowamount.Val;
                     ADDR16 = overflowamount.bits.b1;
                     #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
                     ADDR17 = overflowamount.bits.b2;
                     ADDR18 = overflowamount.bits.b3;
                     #else
                     ADDR17 = _GFXActivePage;
                     ADDR18 = _GFXActivePage>>1;
                     #endif

                    //Save previous address value
                    prevaddr = PMADDR;    
       
                    PMCONCLR = 0x8000; // PMCONbits.ON = 0;
  
                    prevMode = (WORD)PMMODE;

                    PMMODEbits.MODE16 = 1;

                    PMMODESET = 0x0800;     //  INCM = 0b01; Address Incrementing here
     
                    PMADDR = address;
                    PMCONSET = 0x8000;  // PMCONbits.ON = 1; 

                   if((address + burstlength) < (PMADDR_OVERFLOW * (overflowamount.Val+1)))
                   {
 
                   for(i=0;i<burstlength;i++)
                   {          
                      PMDIN = _color;
                      while(PMMODEbits.BUSY==1); 
                   }  
                      address += burstlength;
                   }
                   else{
                   for(i=0;i<burstlength;i++)
                   { 

                    //Setup Write 
                    if(overflowamount.Val != (address++/PMADDR_OVERFLOW))
                    {
                        overflowamount.Val = (address/PMADDR_OVERFLOW);
                        ADDR15 = overflowamount.Val;
                        ADDR16 = overflowamount.bits.b1;
                        #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
                        ADDR17 = overflowamount.bits.b2;
                        ADDR18 = overflowamount.bits.b3;
                        #endif
                    }
       
                    PMDIN = _color; 
                    while(PMMODEbits.BUSY==1);  
                   }  
                    }

                    //Clean-up Address Lines
                    ADDR15 = overflowcount.Val;          //count holds the additional address line count
                    ADDR16 = overflowcount.bits.b1; 
                    #ifdef GFX_USE_DISPLAY_PANEL_TFT_640480_8_E
                    ADDR17 = overflowcount.bits.b2;
                    ADDR18 = overflowcount.bits.b3;
                    #else
                   #ifdef USE_PIP
                     if(PipActive == 1)
                    {
                      ADDR17=_GFXPIPPage;
                      ADDR18=_GFXPIPPage>>1;
                    }
                   else
                #endif 
                    {
                    ADDR17 = VisualPage;
                    ADDR18 = VisualPage>>1;
                    }
                    #endif
                   
     
                    PMMODE = prevMode;
                    PMADDR = prevaddr;  

                    //ReStart DMA
                    DMACONCLR = 0x1000;

             }

      }
#endif
    return (1);
}

void SetActivePage(WORD page)
{
    _GFXActivePage = page;
}

void SetVisualPage(WORD page)
{
    ADDR17 = page;
    ADDR18 = page>>1;
    VisualPage = page;
} 

#ifdef USE_ALPHABLEND

short _GFXBackgroundPage = 3;
short _GFXDestinationPage = 0;
short _GFXForegroundPage = 2;

/*********************************************************************
* Function: void AlphaBlendWindow(DWORD foregroundArea, SHORT foregroundLeft, SHORT foregroundTop,
                                  DWORD backgroundArea, SHORT backgroundLeft, SHORT backgroundTop,
					    DWORD destinationArea, SHORT destinationLeft, SHORT destinationTop,		            
					    WORD  width, WORD height,  	
					    BYTE  alphaPercentage)
********************************************************************/
WORD AlphaBlendWindow(DWORD foregroundArea, SHORT foregroundLeft, SHORT foregroundTop,
                      DWORD backgroundArea, SHORT backgroundLeft, SHORT backgroundTop,
					  DWORD destinationArea, SHORT destinationLeft, SHORT destinationTop,		            
					  WORD  width, WORD height,  	
					  BYTE  alphaPercentage)
{

WORD x,y;
WORD bcolor;
WORD currentPage = _GFXActivePage; 

for(y=0;y<height;y++)
{
    for(x=0;x<width;x++)
    {

        SetActivePage(foregroundArea);
        _color = GetPixel(foregroundLeft++,foregroundTop);
       if(alphaPercentage != 100)
       {
        SetActivePage(backgroundArea);
        bcolor = GetPixel(backgroundLeft++,backgroundTop);


        switch(alphaPercentage)
        {
        case 50: _color = ConvertColor50(_color)+ ConvertColor50(bcolor);break;
        case 75: _color = ConvertColor75(_color)+ ConvertColor25(bcolor);break;
        case 25: _color = ConvertColor25(_color)+ ConvertColor75(bcolor);break;
        default: break;
        }
        }
        SetActivePage(destinationArea);
        PutPixel(destinationLeft++,destinationTop);
     }
   
    foregroundLeft -= width;
    backgroundLeft -= width;
    destinationLeft -= width;

    foregroundTop++;
    backgroundTop++;
    destinationTop++;
}

SetActivePage(currentPage);
return (1);
}

/*********************************************************************
* Function:  CopyPageWindow(srcPage, dstPage, srcX, srcY, dstX, dstY, 
*                       width, height)  
*
* Overview: This is a blocking call.
*           A windows is a rectangular area with the given width and height  
*           of a page. The source and destination window can be located in 
*           different pages and each page is assumed to have the same dimensions
*           (equal width and height).
*
* PreCondition: none
*
* Input: srcPage - page number of the source window,
*        dstPage - page number of the destination window,
*        srcX - x location of the left top corner of the source window 
*               respect to the srcPage.
*        srcY - y location of the left top corner of the source window 
*               respect to the srcPage.
*        dstX - x location of the left top corner of the destination window 
*               respect to the dstPage.
*        dstY - y location of the left top corner of the destination window 
*               respect to the dstPage.
*        width - the width in pixels of the window to copy
*        height - the height in pixels of the window to copy
*
* Output: None
*
* Side Effects: none
*
********************************************************************/
void CopyPageWindow( BYTE srcPage, BYTE dstPage,WORD srcX, WORD srcY,WORD dstX, WORD dstY,WORD width, WORD height)
{

         AlphaBlendWindow(srcPage,srcX,srcY,srcPage,srcX,srcY,dstPage,dstX,dstY, width, height,                               
         100);

} 

#endif   //USE_ALPHABLEND			

#ifdef USE_DOUBLE_BUFFERING

/*********************************************************************
* Function:  SwitchOnDoubleBuffering()
*
* Overview: Switches on the double buffering.
*			Double buffering utilizes two buffers. The frame buffer and the
*           draw buffer. The frame buffer is the buffer that is being displayed
*			while the draw buffer is used for all rendering. 
*           When this function is called, it copies the contents of the frame buffer 
*           to the draw buffer once and all succeeding rendering will be performed on 
*           the draw buffer. To update the frame buffer with newly drawn 
*           items on the draw buffer call UpdateDisplayNow() or RequestDisplayUpdate().
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SwitchOnDoubleBuffering(void)
{
    if(blEnableDoubleBuffering == 0) 
    { 
        blEnableDoubleBuffering = 1; 
        InvalidateAll(); 
    }  
}

/*********************************************************************
* Function:  SwitchOffDoubleBuffering()
*
* Overview: Switches off the double buffering.
*           All rendering will be performed on the frame buffer. Calls
*           to UpdateDisplayNow() or RequestDisplayUpdate() will 
*           have no effect.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void SwitchOffDoubleBuffering(void)
{
    if(blEnableDoubleBuffering == 1) 
    { 
        UpdateDisplayNow(); 
        _drawbuffer = (_drawbuffer == GFX_BUFFER1)? GFX_BUFFER2: GFX_BUFFER1;
        SetActivePage(_drawbuffer); 
        blEnableDoubleBuffering = 0; 
    }
}

/*********************************************************************
* Function:  void InvalidateRectangle(WORD left, WORD top, WORD right, WORD bottom)
*
* Overview: Invalidates the specified rectangular area and if the
*           invalidated areas exceed the GFX_MAX_INVALIDATE_AREAS,
*           whole area is marked as invalidate
*
* PreCondition: None
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: None
*
* Side Effects: Only copies back the invalidated areas to the draw -
*               buffer after the exchange of draw and frame buffers
*
********************************************************************/
void InvalidateRectangle(WORD left, WORD top, WORD right, WORD bottom)
{
    if(blInvalidateAll == 1 || blEnableDoubleBuffering == 0)
    {
        return;
    }
    
    if(NoOfInvalidatedRectangleAreas >= GFX_MAX_INVALIDATE_AREAS)
    {
        blInvalidateAll = 1;
        return;
    }
    else
    {
        WORD width, height;

    #if (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

        width   =   right - left + 1;
        height  =   bottom - top + 1;

    #elif (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

        height  =   right - left + 1;
        width   =   bottom - top + 1;

    #endif

    #if (DISP_ORIENTATION == 90)

        bottom  =   DISP_VER_RESOLUTION - left;
        left    =   top;
        right   =   left + width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 180)

        right   =   DISP_HOR_RESOLUTION - left;
        bottom  =   DISP_VER_RESOLUTION - top;
        left    =   right - width;
        top     =   bottom - height;

    #elif (DISP_ORIENTATION == 270)

        right   =   DISP_HOR_RESOLUTION - top;
        top     =   left;
        bottom  =   top + height;
        left    =   right - width;

    #endif

        InvalidatedArea[NoOfInvalidatedRectangleAreas].X = left;
        InvalidatedArea[NoOfInvalidatedRectangleAreas].Y = top;
        InvalidatedArea[NoOfInvalidatedRectangleAreas].W = width;
        InvalidatedArea[NoOfInvalidatedRectangleAreas].H = height;
        NoOfInvalidatedRectangleAreas++;
    }
}

/*********************************************************************
* Function:  void UpdateDisplayNow(void)
*
* Overview: Synchronizes the draw and frame buffers immediately
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void UpdateDisplayNow(void)
{

    DWORD SourceBuffer, DestBuffer;
    
    if(blEnableDoubleBuffering == 0)
    {
        return;
    }
    
    if(_drawbuffer == GFX_BUFFER1)
    {
        SourceBuffer = GFX_BUFFER1;
        DestBuffer   = GFX_BUFFER2;
    }
    else
    {
        SourceBuffer = GFX_BUFFER2;
        DestBuffer   = GFX_BUFFER1;
    }

     _drawbuffer = DestBuffer;
     SetActivePage(_drawbuffer);

     #ifdef USE_ALPHABLEND
     _GFXDestinationPage = _drawbuffer;
     #endif

     SetVisualPage(SourceBuffer);

    if(blInvalidateAll == 1 || NoOfInvalidatedRectangleAreas > GFX_MAX_INVALIDATE_AREAS)
    {
        blInvalidateAll = 0;
        NoOfInvalidatedRectangleAreas = 0;
        CopyPageWindow( SourceBuffer, DestBuffer, 0, 0,0, 0,GetMaxX()+1, GetMaxY()+1);
    }
    else if(NoOfInvalidatedRectangleAreas)
    {
        while(NoOfInvalidatedRectangleAreas)
        {
            NoOfInvalidatedRectangleAreas--;
            CopyPageWindow( SourceBuffer, DestBuffer, InvalidatedArea[NoOfInvalidatedRectangleAreas].X, InvalidatedArea[NoOfInvalidatedRectangleAreas].Y,InvalidatedArea[NoOfInvalidatedRectangleAreas].X, InvalidatedArea[NoOfInvalidatedRectangleAreas].Y,InvalidatedArea[NoOfInvalidatedRectangleAreas].W, InvalidatedArea[NoOfInvalidatedRectangleAreas].H);
        }
    }

}

/*********************************************************************
* Function:  void RequestDisplayUpdate(void)
*
* Overview: Synchronizes the draw and frame buffers at next VBlank
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void RequestDisplayUpdate(void)
{
      UpdateDisplayNow();
}

#endif //USE_DOUBLE_BUFFERING

#ifdef USE_TRANSPARENT_COLOR
/*********************************************************************
* Function:  void TransparentColorEnable(GFX_COLOR color)
*
* Overview: Sets current transparent color.
*
* PreCondition: none
*
* Input: color - Color value chosen.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
void TransparentColorEnable(GFX_COLOR color)
{
    _colorTransparent = color;    
    _colorTransparentEnable = TRANSPARENT_COLOR_ENABLE;

}
#endif

#ifdef USE_PIP
void SetPIPWindow(WORD left, WORD top, WORD hlength, WORD vlength, WORD pipx, WORD pipy)
{

   #if (DISP_ORIENTATION == 90)
     PipStartL = (top);
     PipStartT = (DISP_VER_RESOLUTION-1)-(left + hlength);
     PipVLength = hlength;
     PipHLength = vlength;
     PipY = (DISP_VER_RESOLUTION-1) - (pipx+hlength);
     PipX = (pipy);
   #elif (DISP_ORIENTATION == 0) 
    PipStartT = top;
    PipStartL = left;
    PipVLength = vlength;
    PipHLength = hlength;
    PipX = pipx;
    PipY = pipy;
   #endif

}
#endif

#endif    //if defined (GFX_USE_DISPLAY_CONTROLLER_DMA)

