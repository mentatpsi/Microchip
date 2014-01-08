/********************************************************************
 Author: 	Daniel Szot
 			MB Turnkey Design.LLC
			http://www.mbturnkeydesign.com

 FileName:     main.c
 Dependencies: See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
********************************************************************/

/** INCLUDES *******************************************************/
#define MAIN_C
#include "GenericTypeDefs.h"
#include "Compiler.h"

#include "HardwareProfile.h"

#include "oled.h"
#include "soft_start.h"
#include "mtouch.h"
#include "BMA150.h"

#include "physics.h"
#include "graph.h"
#include "levels.h"


/** CONFIGURATION **************************************************/
//   //Watchdog Timer Enable bit:
//     #pragma config WDTEN = OFF          //WDT disabled (control is placed on SWDTEN bit)
//   //PLL Prescaler Selection bits:
//     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
//   //Stack Overflow/Underflow Reset Enable bit:
//     #pragma config STVREN = ON            //Reset on stack overflow/underflow enabled
//   //Extended Instruction Set Enable bit:
//     #pragma config XINST = OFF          //Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
//   //CPU System Clock Postscaler:
//     #pragma config CPUDIV = OSC1        //No CPU system clock divide
//   //Code Protection bit:
//     #pragma config CP0 = OFF            //Program memory is not code-protected
//   //Oscillator Selection bits:
//     #pragma config OSC = ECPLL          //HS oscillator, PLL enabled, HSPLL used by USB
//   //Secondary Clock Source T1OSCEN Enforcement:
//     #pragma config T1DIG = ON           //Secondary Oscillator clock source may be selected
//   //Low-Power Timer1 Oscillator Enable bit:
//     #pragma config LPT1OSC = OFF        //Timer1 oscillator configured for higher power operation
//   //Fail-Safe Clock Monitor Enable bit:
//     #pragma config FCMEN = OFF           //Fail-Safe Clock Monitor disabled
//   //Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit:
//     #pragma config IESO = OFF           //Two-Speed Start-up disabled
//   //Watchdog Timer Postscaler Select bits:
//     #pragma config WDTPS = 32768        //1:32768
//   //DSWDT Reference Clock Select bit:
//     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as reference clock
//   //RTCC Reference Clock Select bit:
//     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as reference clock
//   //Deep Sleep BOR Enable bit:
//     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep (does not affect operation in non-Deep Sleep modes)
//   //Deep Sleep Watchdog Timer Enable bit:
//     #pragma config DSWDTEN = OFF        //Disabled
//   //Deep Sleep Watchdog Timer Postscale Select bits:
//     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
//   //IOLOCK One-Way Set Enable bit:
//     #pragma config IOL1WAY = OFF        //The IOLOCK bit (PPSCON<0>) can be set and cleared as needed
//   //MSSP address mask:
//     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
//   //Write Protect Program Flash Pages:
//     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
//   //Write Protection End Page (valid when WPDIS = 0):
//     #pragma config WPEND = PAGE_0       //Write/Erase protect Flash Memory pages starting at page 0 and ending with page WPFP[5:0]
//   //Write/Erase Protect Last Page In User Flash bit:
//     #pragma config WPCFG = OFF          //Write/Erase Protection of last page Disabled
//   //Write Protect Disable bit:
//     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored

     #pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
     #pragma config STVREN = ON            //stack overflow/underflow reset enabled
     #pragma config XINST = OFF          //Extended instruction set disabled
     #pragma config CPUDIV = OSC1        //No CPU system clock divide
     #pragma config CP0 = OFF            //Program memory is not code-protected
     #pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
     #pragma config T1DIG = ON           //Sec Osc clock source may be selected
     #pragma config LPT1OSC = OFF        //high power Timer1 mode
     #pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
     #pragma config IESO = OFF           //Two-Speed Start-up disabled
     #pragma config WDTPS = 32768        //1:32768
     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
     #pragma config DSWDTEN = OFF        //Disabled
     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
     #pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
     #pragma config WPEND = PAGE_0       //Start protection at page 0
     #pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 

/** V A R I A B L E S ********************************************************/
#pragma udata

typedef enum _SCROLL_WHEEL_DIRECTION
{
    NO_TOUCH = 0,
    SCROLL_DOWN,
	SCROLL_UP
}SCROLL_WHEEL_DIRECTION;

typedef enum _SCROLL_WHEEL_POSITION
{
    NO_TOUCH = 0,
    UP,
    DOWN,
    MIDDLE
} SCROLL_WHEEL_POSITION;

SCROLL_WHEEL_POSITION scrollBarState = NO_TOUCH;

unsigned char scrollCounter = 0;
#define SCROLL_CONSTANT 100

unsigned char commandIndex = 0;
unsigned int executionTick = 0;
unsigned char DemoIntroState = 0;
unsigned char g_menuSelected = 0;
unsigned int w1, w2;
unsigned char touchedNow;
unsigned char firstLevel = 0;

signed short int axisX, axisY, axisZ;
signed int g_touchStartVal = 0;
unsigned char g_isTouched = 0;
signed int g_scroolCount = 0;


/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void ProcessIO(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void UserInit(void);
void DemoIntroduction(void);
BOOL CheckButtonPressed(void);

SCROLL_WHEEL_DIRECTION GetScrollDirection();
//void ReadAccState();
void SelectLevel();
void loadLevel(unsigned char l);
void CalibrateAcc();

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
	//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
	//the reset, high priority interrupt, and low priority interrupt
	//vectors.  However, the current Microchip USB bootloader 
	//examples are intended to occupy addresses 0x00-0x7FF or
	//0x00-0xFFF depending on which bootloader is used.  Therefore,
	//the bootloader code remaps these vectors to new locations
	//as indicated below.  This remapping is only necessary if you
	//wish to program the hex file generated from this project with
	//the USB bootloader.  If no bootloader is used, edit the
	//usb_config.h file and comment out the following defines:
	//#define PROGRAMMABLE_WITH_SD_BOOTLOADER
	
	#if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)
		#define REMAPPED_RESET_VECTOR_ADDRESS			0xA000
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0xA008
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0xA018
	#else	
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
	#endif
	
	#if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	extern void main(void);
	//#pragma code _startup_section = 0xeea2
	/*void _mstartup (void)
	{
	  _asm
	    // Initialize the stack pointer
	    lfsr 1, _stack
	    lfsr 2, _stack
	
	    clrf TBLPTRU, 0 // 1st silicon doesn't do this on POR
	
	  _endasm 
loop:
	
	  // If user defined __init is not found, the one in clib.lib will be used
	  __init ();
	
	  // Call the user's main routine
	  main ();
	
	  goto loop;
	}     */ 


	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
	#endif
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
	#if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)
	//Note: If this project is built while one of the bootloaders has
	//been defined, but then the output hex file is not programmed with
	//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
	//As a result, if an actual interrupt was enabled and occured, the PC would jump
	//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
	//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
	//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
	//would effective reset the application.
	
	//To fix this situation, we should always deliberately place a 
	//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
	//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
	//hex file of this project is programmed with the bootloader, these sections do not
	//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
	//programmed using the bootloader, then the below goto instructions do get programmed,
	//and the hex file still works like normal.  The below section is only required to fix this
	//scenario.
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#endif	//end of "#if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)"

	#pragma code
	
	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 
#endif




/** DECLARATIONS ***************************************************/
#pragma code
//#define REMAPPED_MAIN_VECTOR_ADDRESS 0xB100
#define ROM_STRING rom unsigned char*
ROM_STRING empty_line = (ROM_STRING)"                     ";

/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *****************************************************************************/

//#pragma code MAIN_SECTION = REMAPPED_MAIN_VECTOR_ADDRESS
void main(void)
{   
    InitializeSystem();

    while(1)
    {
		// Check bus status and service USB interrupts.
        //USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
        				  // this function periodically.  This function will take care
        				  // of processing and responding to SETUP transactions 
        				  // (such as during the enumeration process when you first
        				  // plug in).  USB hosts require that USB devices should accept
        				  // and process SETUP packets in a timely fashion.  Therefore,
        				  // when using polling, this function should be called 
        				  // frequently (such as once about every 100 microseconds) at any
        				  // time that a SETUP packet might reasonably be expected to
        				  // be sent by the host to your device.  In most cases, the
        				  // USBDeviceTasks() function does not take very long to
        				  // execute (~50 instruction cycles) before it returns.
    				  

		// Application-specific tasks.
		// Application related code may be added here, or in the ProcessIO() function.
        ProcessIO();        
    }//end while
}//end main

SCROLL_WHEEL_POSITION GetScrollState(unsigned int w1, unsigned int w2)
{
	if ((w1 < 700) || (w2 < 700))
	{
		if (w1 < w2)
		{
            return UP;			
		}
		else
		{
            return DOWN;	
		}
	}
	else
	{
		return NO_TOUCH;
	}
}

void CalibrateAcc()
{
	unsigned int w1;
	static unsigned int filterPass = 0;
	char buffer[64];

	ReadAccState();
	
	axisX += g_devAccelerationX-g_devAccOffsetX;
	axisY += g_devAccelerationY-g_devAccOffsetY;
	//axisZ += g_devAccelerationZ-g_devAccOffsetZ;
	
	if (filterPass == 200)
	{
		oledPutROMString((ROM_STRING)"   CALIBRATE SENSOR  ",0,0);
		sprintf((char *)buffer, (const far rom char *)("   X:  %d             "), axisX/200);
		oledPutString((unsigned char *)buffer,2,0);
		sprintf((char *)buffer, (const far rom char *)("   Y:  %d             "), axisY/200);
		oledPutString((unsigned char *)buffer,3,0);
	
		axisX = 0;
		axisY = 0;
		//axisZ = 0;
	
		filterPass = 0;
	}

	w1 = mTouchReadButton(0);

	if (w1 < 600)
	{
		g_devAccOffsetX = g_devAccelerationX;
		g_devAccOffsetY = g_devAccelerationY;
		//g_devAccOffsetZ = g_devAccelerationZ;
	}

	w1 = mTouchReadButton(3);
	if (w1 < 600)
	{
		DemoIntroState = 6;
	}

	filterPass ++;
}

SCROLL_WHEEL_DIRECTION GetScrollDirection()
{
	w1 = mTouchReadButton(1);
	w2 = mTouchReadButton(2);
	scrollBarState = GetScrollState(w1, w2);
			
	if ((scrollBarState != NO_TOUCH) && (g_isTouched == 0))
	{		
		g_isTouched = 1;
		g_touchStartVal = w1-w2;
	}
	
	if (scrollBarState == NO_TOUCH)
	{
		g_isTouched = 0;
		return NO_TOUCH;
	}

	if (g_isTouched && ((signed int)g_touchStartVal - ((signed int)w1-(signed int)w2)) > 100)
	{
		g_touchStartVal = w1-w2;
		return SCROLL_UP;
		
	}
	else if (g_isTouched && ((signed int)g_touchStartVal - ((signed int)w1-(signed int)w2)) < -100)
	{
		g_touchStartVal = w1-w2;
		return SCROLL_DOWN;
	}	
}

void SelectLevel()
{
	unsigned char i, j;
	SCROLL_WHEEL_DIRECTION scrollDir;
	char buffer[32];
	oledPutROMString((ROM_STRING)"    SELECT  LEVEL    ",0,0);
	
	scrollDir = GetScrollDirection();

	if (scrollDir == SCROLL_UP)
	{
		if (g_menuSelected > 0)
			g_menuSelected --;
	}
	else if (scrollDir == SCROLL_DOWN)
	{
		if (g_menuSelected < (LEVELS-1))
			g_menuSelected ++;
	}	

	if ((g_menuSelected - firstLevel) == 3 && (firstLevel < (LEVELS-4)))
		firstLevel++;

	if ((g_menuSelected - firstLevel) == 0 && (firstLevel > 0))
		firstLevel--;

	j = 0;
	for (i = firstLevel; i < firstLevel+4; i++)
	{
		sprintf((char *)buffer, (const far rom char *)("       Level %d      "), i);

		if (g_menuSelected == i)
		{
			oledPutNegString((unsigned char *)buffer,j+2,0);
		}
		else
			oledPutString((unsigned char *)buffer,j+2,0);

		j++;
	}

	w1 = mTouchReadButton(3);
	if (w1 < 600)
	{
		DemoIntroState = 6;
		g_menuSelected = 1;
	}

	w1 = mTouchReadButton(0);
	if (w1 < 600)
	{
		DemoIntroState = 0xFF;
		g_level=g_menuSelected;
		loadLevel(g_level);
	}
}

void DemoIntroduction(void)
{
	unsigned char i ;
	unsigned int w1, w2;
	unsigned char touchedNow;
	SCROLL_WHEEL_DIRECTION scrollDir;
	
    switch(DemoIntroState)
    {
        case 0:
            ResetDevice();  
        
            FillDisplay(0x00);
            oledPutROMString((ROM_STRING)" Labyrinth Demo v1.0 ",0,0);
            oledPutROMString((ROM_STRING)"   by Daniel Szot    ",1,0);
			oledPutROMString((ROM_STRING)"                     ",2,0);
			oledPutROMString((ROM_STRING)"  MB Turnkey Design  ",3,0);
			oledPutROMString((ROM_STRING)" mbturnkeydesign.com ",4,0);
            oledPutROMString((ROM_STRING)"Press Menu to proceed",6,0);

            DemoIntroState = 1;
            break;

        case 1:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState=4;
            }
            break;

        case 4:
            oledPutROMString((ROM_STRING)" 1. Use scroll to    ",0,0);
            oledPutROMString((ROM_STRING)" select menu items.  ",1,0);
            oledPutROMString((ROM_STRING)" 2. Tilt the board   ",2,0);
            oledPutROMString((ROM_STRING)" to move the ball.   ",3,0);
            oledPutROMString((ROM_STRING)" 3. Have a good time.",4,0);
            oledPutROMString((ROM_STRING)"                     ",5,0);
            oledPutROMString((ROM_STRING)"Press Menu to proceed",6,0);
            DemoIntroState = 5;
            break;

        case 5:
            if(CheckButtonPressed() == TRUE)
            {
                DemoIntroState++;
            }
            break;

        case 6:
			scrollDir = GetScrollDirection();

			if (scrollDir == SCROLL_UP)
			{
				if (g_menuSelected > 0)
					g_menuSelected --;
			}
			else if (scrollDir == SCROLL_DOWN)
			{
				if (g_menuSelected < 3)
					g_menuSelected ++;
			}		

            oledPutROMString((ROM_STRING)"   LABIRYNTH  MENU   ",0,0);
            oledPutROMString(empty_line,1,0);

			if (g_menuSelected == 0)
				oledPutROMNegString((ROM_STRING)"      NEW  GAME      ",2,0);	
			else
            	oledPutROMString((ROM_STRING)"      NEW  GAME      ",2,0);

            if (g_menuSelected == 1)
				oledPutROMNegString((ROM_STRING)"     SELECT LEVEL    ",3,0);
			else
				oledPutROMString((ROM_STRING)"     SELECT LEVEL    ",3,0);
		    
			if (g_menuSelected == 2)
            	oledPutROMNegString((ROM_STRING)"      CALIBRATE      ",4,0);
			else
				oledPutROMString((ROM_STRING)"      CALIBRATE      ",4,0);
			
			if (g_menuSelected == 3)
            	oledPutROMNegString((ROM_STRING)"      QUIT GAME      ",5,0);
			else
				oledPutROMString((ROM_STRING)"      QUIT GAME      ",5,0);

            oledPutROMString(empty_line,6,0);

			w1 = mTouchReadButton(0);
			if (w1 < 600)
			{
				switch(g_menuSelected)
				{
					case 0: DemoIntroState = 0xFF; g_level=0; loadLevel(g_level); break;
					case 1: DemoIntroState = 9; DelayMs(100); FillDisplay(0x00); g_menuSelected = 0; firstLevel = 0; break;
					case 2: DemoIntroState = 7; DelayMs(100); FillDisplay(0x00);  break;
					case 3: 
						_asm
						RESET
						_endasm
						 break;
				}
			}
            break;

        case 7:
			CalibrateAcc();
            break;

		case 8:
			FillDisplay(0xFF);
			oledPutROMNegString((ROM_STRING)"  CORNGRATULATIONS!  ",0,0);
			oledPutROMNegString((ROM_STRING)"   You won nothing   ",2,0);
			oledPutROMNegString((ROM_STRING)" You just waste your ",3,0);
			oledPutROMNegString((ROM_STRING)"   PRECIOUS time :)  ",4,0);
			DelayMs(2000);
			DemoIntroState = 6;
			FillDisplay(0x00);
			g_level = 0;
			g_ballGtt = 0;
			break;

		case 9:
			SelectLevel();
			break;

        default:
            break;
    }
}

BOOL CheckButtonPressed(void)
{
    static char buttonPressed = FALSE;
    static unsigned long buttonPressCounter = 0;

    if(PORTBbits.RB0 == 0)
    {
        if(buttonPressCounter++ > 2000)
        {
            buttonPressCounter = 0;
            buttonPressed = TRUE;
        }
    }
    else
    {
        if(buttonPressed == TRUE)
        {
            if(buttonPressCounter == 0)
            {
                buttonPressed = FALSE;
                return TRUE;
            }
            else
            {
                buttonPressCounter--;
            }
        }
    }

    return FALSE;
}

/******************************************************************************
  Function:
    void BL_CheckLoaderEnabled( void )

  Description:
    This routine is called to determine if the boot loader should attempt to
    load a new application.

  Precondition:
	None

  Parameters:
    None

  Returns:
    None


  Remarks:
    This function must be implemented.
******************************************************************************/
/*void BL_CheckLoaderEnabled ( void )
{
#if defined(PROGRAMMABLE_WITH_SD_BOOTLOADER)

	if (CheckButtonPressed() == TRUE)	
	{
		_asm
		RESET
		_endasm
	}
	return;
#endif
}*/

/********************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
static void InitializeSystem(void)
{
    ADCON1 |= 0x0F;                 // Default all pins to digital

	//On the PIC18F87J50 Family of USB microcontrollers, the PLL will not power up and be enabled
	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
	//This allows the device to power up at a lower initial operating frequency, which can be
	//advantageous when powered from a source which is not gauranteed to be adequate for 48MHz
	//operation.  On these devices, user firmware needs to manually set the OSCTUNE<PLLEN> bit to
	//power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        OSCTUNEbits.PLLEN = 1;  //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
        while(pll_startup_counter--);
    }
    //Device switches over automatically to PLL output after PLL is locked and ready.

	//Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
	//use the ANCONx registers to control this, which is different from other devices which
	//use the ADCON1 register for this purpose.
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
    
//	The USB specifications require that USB peripheral devices must never source
//	current onto the Vbus pin.  Additionally, USB peripherals should not source
//	current on D+ or D- when the host/hub is not actively powering the Vbus line.
//	When designing a self powered (as opposed to bus powered) USB peripheral
//	device, the firmware should make sure not to turn on the USB module and D+
//	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
//	firmware needs some means to detect when Vbus is being powered by the host.
//	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
// 	can be used to detect when Vbus is high (host actively powering), or low
//	(host is shut down or otherwise not supplying power).  The USB firmware
// 	can then periodically poll this I/O pin to know when it is okay to turn on
//	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
//	peripheral device, it is not possible to source current on D+ or D- when the
//	host is not actively providing power on Vbus. Therefore, implementing this
//	bus sense feature is optional.  This firmware can be made to use this bus
//	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
//	HardwareProfile.h file.    
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    #endif
    
//	If the host PC sends a GetStatus (device) request, the firmware must respond
//	and let the host know if the USB peripheral device is currently bus powered
//	or self powered.  See chapter 9 in the official USB specifications for details
//	regarding this request.  If the peripheral device is capable of being both
//	self and bus powered, it should not return a hard coded value for this request.
//	Instead, firmware should check if it is currently self or bus powered, and
//	respond accordingly.  If the hardware has been configured like demonstrated
//	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
//	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
//	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
//	has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
//	to it in HardwareProfile.h.
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;	// See HardwareProfile.h
    #endif

    UserInit();

}//end InitializeSystem



/******************************************************************************
 * Function:        void UserInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine should take care of all of the demo code
 *                  initialization that is required.
 *
 * Note:            
 *
 *****************************************************************************/
void UserInit(void)
{
    //Make sure that the SD-card is not selected
    TRISCbits.TRISC6 = 0;
    LATCbits.LATC6 = 1;
    PORTCbits.RC6 = 1;

	/* Initialize the mTouch library */
	mTouchInit();

	/* Call the mTouch callibration function */
	mTouchCalibrate();

	/* Initialize the accelerometer */
	InitBma150(); 

    //make sure that the accelerometer is not selected
    LATCbits.LATC7 = 1;
    PORTCbits.RC7 = 1;

	Fill(0x00);
	g_level = 0;
	loadLevel(g_level);
	InitPhysics();
}//end UserInit

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{   
    //BMA150_REG reg_MSB;
    //BMA150_REG reg_LSB;
	unsigned int w1;

	if(DemoIntroState == 0xFF)
    {
	    //BL_CheckLoaderEnabled();
		
		if (g_ballGth == 0 && g_ballGtt == 0)
			Step(0.04f, 1);
		else
			Step(0.04f, 0);
		DrawScene();

		if (g_endGame == 1)
		{
			g_endGame = 0;
			DemoIntroState = 8;
		}
		
		w1 = mTouchReadButton(3);
		if (w1 < 600)
		{
			DemoIntroState = 6;
			g_menuSelected = 0;
			FillDisplay(0x00);
		}
		
    }
	
    // User Application USB tasks
    //if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	// Soft Start the APP_VDD
    if(AppPowerReady() == FALSE) return;

    DemoIntroduction();
}		//end ProcessIO

/** EOF main.c *************************************************/
