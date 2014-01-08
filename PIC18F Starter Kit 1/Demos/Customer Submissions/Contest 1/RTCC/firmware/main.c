/********************************************************************
 FileName:		main.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18F46J50 Microcontroller
 Hardware:		MPLAB Starter kit for Pic18F board
 				with 18F46J50 Controller.
 				Compatible with original SDBootloader
 				using the microcontroller's built in rtcc
 				Touchbuttons, oled screen and USB
 Complier:  	Microchip C18 v3.34 (hardware rtcc library)
 Coder:			Tibor Kiss
 				Hungary, Ebes
 				mrb@t-email.hu
 
 Code base originally copied from the starter kit CDC demo.

 If the program is running fine, but the clock still stops even
 the rtcc module is on, the answer is below.
 The 18F46J50 rtcc module clock depends on two source. The first
 and the most accurate is the Timer 1 source 32768 Hz crystal on
 RC0, RC1 pin. On the board this crystal doesn't exsist factory
 default. But, the original Bootloader Configuration bits showing
 to the controller to use it.
 The second source is the internal RC oscillator about 31000 Hz.
 It's less accurate, but runs the rtcc module everywhen the 
 good config bit is set. (It slips 1-2 seconds per minute)
 As I know, in microchip's J series, the config bit cannot set
 from the controller itself, only from external programmer.
 If you want to set it you must do the following list:
 
 1. Open an empty MPLAB IDE.
 2. File menu, Import command.
 3. Choose the PIC18F Starter Kit Bootloader.hex
    from c:\Microchip Starter Kits\C:\Microchip Starter Kits\
               PIC18F Starter Kit 1\Demos\Bootloader Hex File
 4. Configure manu Configuration Bits command.
 5. Clear flag near Configuration Bits set in code.
 	Accept the messagebox choosing OK.
 6. Find Address FFFC the value is 63
 7. Near here find RTCOSC, and in Setting column choose 
    RTCC uses INTRC
 8. The value will be 61
 9. Close this window.
10. Programmer menu, Select programmer submenu, Starter kits command
11. Connect the board's debugger usb connector to the PC.
12. Programmer menu, Program command.
13. Reprogram the clock.hex from the SD card with bootloader.

********************************************************************/

/** INCLUDES *******************************************************/
#include <ctype.h>
#include <stdlib.h>
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "USB/usb.h"
#include "USB/usb_function_cdc.h"
#include "HardwareProfile.h"
#include "oled.h"
#include "soft_start.h"
#include "mtouch.h"

#include <rtcc.h>
#ifndef RTCC_V1
 #error No hardware rtcc support in this version of C18
#endif


/** CONFIGURATION **************************************************/
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
     #pragma config RTCOSC = INTOSCREF//T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
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

#define CMDSTRLEN 15

unsigned int executionTick;
unsigned char screen;
unsigned char screenvalid;
unsigned char position;
unsigned char positionchanged;
unsigned char devicereset;
unsigned char clockss;
unsigned char resetcounter;
unsigned char alarmcnt;
char screencalibration;
char cbuf[64],outbuf[64],cmdstr[CMDSTRLEN+1];
char *outbufend;
char prevs,prevm,prevh;
char cx,cy;
BYTE_VAL buttonstate,buttonpressed;
rtccTimeDate screentd;
unsigned rom char helpstring[][21] = {
//23456789012345678901
"Touch U/D to switch",
"between screens, and",
"L/R to move cursor.",
"In set mode U/D is",
"modify the values.",
"Hold menu to reset." 
};
const far rom char onstr[]=" on",
	offstr[]="off";
unsigned rom char minusstr[]="-",
	xxstr[]="xx",alarmstr[]="Alarm";
unsigned rom char coordh[8][2]=
{ {0,25},{3,25},{5,25},{8,24},
  {10,23},{13,21},{15,20},{17,19}}; 

/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void ProcessIO(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void UserInit(void);
void oledWriteChar1xINV(char letter, unsigned char page, unsigned char column);
char *strend(char *str);
void strcatc(char *str,char c);
void btoh(unsigned char c,char *str);
void rjustify(char *str,char w);
unsigned char oledReadData();
void oledPlot(unsigned char x,unsigned char y,unsigned char mode);
void oledLine(unsigned char x1,unsigned char y1,
				unsigned char x2,unsigned char y2,
				unsigned char mode,unsigned char size);
void oledWriteChar1xINV(char letter, unsigned char page, unsigned char column);
void oledPutStringINV(unsigned char *ptr,unsigned char page, unsigned char col);
void dttoa(rtccTimeDate *td,char *str);
void tmtoa(rtccTimeDate *td,char *str);
void bcdadd1(unsigned char *bcd);
void bcdsub1(unsigned char *bcd);
void sec2coord(int s);
void printanalog();
void printdial();

void USBCBSendResume(void);

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
#define ROM_STRING rom unsigned char*

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


#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{
    InitializeSystem();

    USBDeviceInit();

    while(1)
    {
        USBDeviceTasks(); 
        ProcessIO();        
    }
}

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
    //ADCON1 |= 0x0F;                 // Default all pins to digital

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
	// Real time clock start
	RtccInitClock();
	RtccWrOn();                              
	mRtccAlrmDisable();
	{
		rtccTimeDate initd;
		initd.f.year=0x10;
		initd.f.mon=0x01;
		initd.f.mday=0x01;
		initd.f.wday=5;   // 2010.01.01 is friday
		initd.f.hour=0;
		initd.f.min=0;
		initd.f.sec=0;
		RtccWriteTimeDate(&initd,FALSE);
		RtccWriteAlrmTimeDate(&initd);
	}
	mRtccOn();
	
	// Mtouch init
	mTouchInit();
	mTouchCalibrate();
	
	cmdstr[0]=0;
	screen=0;
	screenvalid=0;
	position=0;
	buttonstate.Val=0;
	buttonpressed.Val=0;
    devicereset=0;  
    clockss=0;
	resetcounter=0;
	alarmcnt=0;
}//end UserInit

/******************************************************************************
 * Function:        char *strend(char *str)
 *
 * PreCondition:    None
 *
 * Input:           str - pointer to the zero terminated string
 *
 * Output:          pointer to the end of the input string
 *
 * Side Effects:    None
 *
 * Overview:        Searches the ending zero of the input string
 *
 * Note:            Using static variable for less code size
 *****************************************************************************/
char *strend(char *str)										
{
	static char *s;
	s=str;
	while(*s) s++;
	return s;
}

/******************************************************************************
 * Function:        void strcatc(char *str,char c)
 *
 * PreCondition:    None
 *
 * Input:           str - pointer to the zero terminated string
 *					c   - appendable character
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Append one character to a zero terminated string
 *
 * Note:            Using static variable for less code size
 *****************************************************************************/
void strcatc(char *str,char c)
{
	static char *s;
	s=strend(str);
	*s=c;
	s[1]=0;
}

/******************************************************************************
 * Function:        void btoh(unsigned char c,char *str)
 *
 * PreCondition:    None
 *
 * Input:           str - pointer to the zero terminated string
 *					c   - byte to convert
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Convert one byte to a 2 character lenght hexadecimal
 *					zero terminated string
 *
 * Note:            Using static variable for less code size
 *****************************************************************************/
void btoh(unsigned char c,char *str)
{
	static unsigned char x;
	x=c>>4;
	*str=x+(x>9?'W':'0');
	x=c&0x0f;
	str[1]=x+(x>9?'W':'0');
	str[2]=0;
}

/******************************************************************************
 * Function:        void rjustify(char *str,char w)
 *
 * PreCondition:    None
 *
 * Input:           str - pointer to the zero terminated string
 *					w   - given width
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Right justifies a zero terminated string in the
 *					given width
 *
 * Note:            None
 *****************************************************************************/
void rjustify(char *str,char w)
{
	char *s,*d;
	d=str+w;
	s=strend(str);
	if(d<=s) return;
	while(str<=s) *d--=*s--;
	while(str<=d) *d--=' ';
}	

/******************************************************************************
 * My oled extension because, the default cannot write inverse text,
 * and for a little graphic.
 ******************************************************************************/

// Defines

#define	oledWR			LATEbits.LATE1
#define	oledWR_TRIS		TRISEbits.TRISE1
#define	oledRD			LATEbits.LATE0
#define	oledRD_TRIS		TRISEbits.TRISE0
#define	oledCS			LATEbits.LATE2
#define	oledCS_TRIS		TRISEbits.TRISE2
#define	oledRESET		LATDbits.LATD1
#define	oledRESET_TRIS	TRISDbits.TRISD1
#define	oledD_C			LATBbits.LATB5
#define	oledD_C_TRIS	TRISBbits.TRISB5

/******************************************************************************
 * Function:        unsigned char oledReadData()
 *
 * PreCondition:    Page and column address must be set before
 *
 * Input:           None
 *
 * Output:          readed data
 *
 * Side Effects:    None
 *
 * Overview:        Read one byte from oled's memory buffer
 *
 * Note:            Using static variable for less code size
 *****************************************************************************/
unsigned char oledReadData()
{
	static unsigned char b;
    oledD_C	= 1;
    oledCS = 0;
    oledRD = 0;
    oledRD = 1;
	b=PORTD;	   
    oledCS = 1;
	return b;
}

/******************************************************************************
 * Function:        void oledPlot(unsigned char x,unsigned char y,unsigned char mode)
 *
 * PreCondition:    None
 *
 * Input:           x - dot's x coordinate
 *					y -  "    y      "
 *					mode -  0 : clear
 *							1 : set
 *							2 : alter (xor)
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Plots one dot on oled with a given x,y coordinates
 *					in a given mode
 *
 * Note:            Using static variable for less code size
 *					Using oled's Read-Modify-Write feature, for setting
 *					page and column only once (this feature increments 
 *                  column address only after writing)
 *					Dummy read is required because oled's internal buffering
 *					Unlike the dafault oled I'm using 0xB0 as starting page
 *                  Its easier to calculating from 0
 *****************************************************************************/
void oledPlot(unsigned char x,unsigned char y,unsigned char mode)
{
	static unsigned char data,mask;
	if((x&0x80)||(y&0xc0)) return;
	x+=OFFSET;
	mask=1<<(y&0x07);
	WriteCommand(0xB0|(y>>3));   		// page
	WriteCommand(x&0x0f);				// column lo
	WriteCommand(0x10|((x>>4)&0x0f));	// column hi
	WriteCommand(0xe0);					// read-modify-write
	data=oledReadData();				// dummy read
	data=oledReadData();						
	switch(mode)
	{
		case 0: data&=~mask;			// clear dot
				break;
		case 1: data|=mask;				// set dot
				break;
		case 2: data^=mask;				// alter dot
	}
	WriteData(data);
	WriteCommand(0xee);					// r-m-w end
}

/******************************************************************************
 * Function:        void oledLine(unsigned char x1,unsigned char y1,
 *									unsigned char x2,unsigned char y2,
 *									unsigned char mode,unsigned char size)
 *
 *
 * PreCondition:    None
 *
 * Input:           x1, y1 - starting point
 *					x2, y2 - ending point
 *					mode - drawing mode (described in oledPlot)
 *					size - line size values 1,2,3
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Draws a line between given coordinates with a given
 *                  mode and size
 *
 * Note:            Using static variable for less code size
 *					Size means, if the value is 2, it's draws a parallel line
 *					above the original one, if the value is 3 then draws a
 *					third parallel line below the original. (or left/right
 *                  depends on line's gradient)
 *****************************************************************************/
void oledLine(unsigned char x1,unsigned char y1,
				unsigned char x2,unsigned char y2,
				unsigned char mode,unsigned char size)
{
	static unsigned char i,x,y,dx,dy;
	static int sx,sy;
	if(x1>x2)
	{
		dx=x1-x2;
		sx=-1;
	}
	else
	{
		dx=x2-x1;
		sx=1;
	}
	if(y1>y2)
	{
		dy=y1-y2;
		sy=-1;
	}
	else
	{
		dy=y2-y1;
		sy=1;
	}
	if(dy>dx) 
		for(i=0;i!=dy;i++)
		{
			x=sx*i*dx/dy+x1;
			y=sy*i+y1;
			oledPlot(x,y,mode);
			if(size>1) oledPlot(x+1,y,mode);
			if(size>2) oledPlot(x-1,y,mode);
		}
	else 
		for(i=0;i!=dx;i++)
		{
			x=sx*i+x1;
			y=sy*i*dy/dx+y1;
			oledPlot(x,y,mode);
			if(size>1) oledPlot(x,y+1,mode);
			if(size>2) oledPlot(x,y-1,mode);
		}
}	

/******************************************************************************
 * Function:        void oledWriteChar1xINV(char letter, unsigned char page, unsigned char column)
 *
 * PreCondition:    Page must be given in Command format
 *
 * Input:           letter - letter to print
 *					page,column - starting column of character
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Prints one letter to oled screen to a given position in
 *                  inverse (data bytes are bit negated)
 *
 * Note:            Copied from original oled c file only bit negation operator inserted
 *****************************************************************************/
void oledWriteChar1xINV(char letter, unsigned char page, unsigned char column)
{
	letter -= ' ';					// Adjust character to table that starts at 0x20
	WriteCommand(page);
	column += OFFSET;
	WriteCommand(0x00+(column&0x0F));
	WriteCommand(0x10+((column>>4)&0x0F));
	WriteData(~g_pucFont[letter][0]);	// Write first column
	WriteData(~g_pucFont[letter][1]);	// Write second column
	WriteData(~g_pucFont[letter][2]);	// Write third column
	WriteData(~g_pucFont[letter][3]);	// Write fourth column
	WriteData(~g_pucFont[letter][4]);	// Write fifth column
	WriteData(0xFF);					// Write 1 column for buffer to next character
}

/******************************************************************************
 * Function:        void oledPutStringINV(unsigned char *ptr,unsigned char page, unsigned char col)
 *
 * PreCondition:    None
 *
 * Input:           ptr - pointer to a null terminated string
 *					page,column - starting column of first character
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Prints a ram string to oled screen to a given position in
 *                  inverse (data bytes are bit negated)
 *
 * Note:            Copied from original oled c file only using my oledWriteChar1xINV
 *					I kept that page is shifted by one (0xB1) for less calculating in program
 *****************************************************************************/
void oledPutStringINV(unsigned char *ptr,unsigned char page, unsigned char col)
{
	unsigned char i;
	
	i=col;
	page = page + 0xB1;
	while(*ptr)
	{
		oledWriteChar1xINV(*ptr,page,i);
		ptr++;
		i+=6;
	}
}

/******************************************************************************
 * Function:        void dttoa(rtccTimeDate *td,char *str)
 *
 * PreCondition:    None
 *
 * Input:           str - pointer to the zero terminated string
 *					dt  - TimeDate struct storing date value
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Convert rtcc Date to ANSI date (european) format YYYY-MM-DD
 *					to the given zero terminated string
 *
 * Note:            None
 *****************************************************************************/
void dttoa(rtccTimeDate *td,char *str)
{
	btoh(0x20,str);
	btoh(td->f.year,str+2);
	str[4]='-';
	btoh(td->f.mon,str+5);
	str[7]='-';
	btoh(td->f.mday,str+8);
}

/******************************************************************************
 * Function:        void tmtoa(rtccTimeDate *td,char *str)
 *
 * PreCondition:    None
 *
 * Input:           str - pointer to the zero terminated string
 *					dt  - TimeDate struct storing time value
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Convert rtcc Date to ANSI time (24 hour) format HH-MM-SS
 *					to the given zero terminated string
 *
 * Note:            None
 *****************************************************************************/
void tmtoa(rtccTimeDate *td,char *str)
{
	btoh(td->f.hour,str);
	str[2]=':';
	btoh(td->f.min,str+3);
	str[5]=':';
	btoh(td->f.sec,str+6);
}

/******************************************************************************
 * Function:        void bcdadd1(unsigned char *bcd)
 *
 * PreCondition:    None
 *
 * Input:           bcd - pointer to the bcd number
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        increase by 1 and align bcd number
 *
 * Note:            None
 *****************************************************************************/
void bcdadd1(unsigned char *bcd)
{
	if((++(*bcd)&0xf)==0x0a) *bcd+=6;
}

/******************************************************************************
 * Function:        void bcdsub1(unsigned char *bcd)
 *
 * PreCondition:    None
 *
 * Input:           bcd - pointer to the bcd number
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        decrease by 1 and align bcd number
 *
 * Note:            None
 *****************************************************************************/
void bcdsub1(unsigned char *bcd)
{
	if((--(*bcd)&0x0f)==0x0f) *bcd-=6;
}

/******************************************************************************
 * Function:        void sec2coord(int s)
 *
 * PreCondition:    None
 *
 * Input:           s - seconds value (1-60)
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Calculate the clock's secondhand endpoint coordinate from
 *                  the given second value. It uses the coordh helper array
 *					which stores the eighth arc's calculated sine and cosine
 *					values for a 25 pixel radius circle.
 *
 * Note:            Using static variable for less code size
 *					Using global variables for output for less code size
 *                  I'm tried, which version produces smaller code, this
 *					with the small 16 byte helper array, or the very simple
 *					one with the 120 byte signed char array. This won. (and also nicer)
 *****************************************************************************/
void sec2coord(int s)
{
	static unsigned char i,j,k;
	i=s%15;
	j=s/15;
	k=(i&0x08)?15-i:i;
	if(((i&0x08)>>3)^(j&1))
	{
		cy=coordh[k][0];
		cx=coordh[k][1];
	}
	else
	{
		cx=coordh[k][0];
		cy=coordh[k][1];
	}
	if(!j||(j==3)) cy=-cy;
	if(j>1) cx=-cx;
}

/******************************************************************************
 * Function:        void printanalog()
 *
 * PreCondition:    prevh,prevm,prevs must be contain true values
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    cx,cy global variables changed
 *
 * Overview:        Prints the anagog clock's small, big and secondhand
 *					from the calculated "second" values. The small hand is
 *					0.6 scale in lengt and 3 pixel widht, the big hand is
 *					full in length and 2 pixel in width, the second hand is
 *					a normal line.
 *
 * Note:            It's prints mode 2 (xored) the lines, twice in a second
 *					int the main loop, first clear the previous, then
 *					writes the new
 *****************************************************************************/
void printanalog()
{
	sec2coord(prevh);
	oledLine(32,36,32+cx*3/5,36+cy*3/5,2,3);
	sec2coord(prevm);
	oledLine(32,36,32+cx,36+cy,2,2);
	sec2coord(prevs);
	oledLine(32,36,32+cx,36+cy,2,1);
}

/******************************************************************************
 * Function:        void printdial()
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    cx,cy global variables changed
 *
 * Overview:        Prints the analog clock's dial 12 small line at every 
 *					5 second
 *
 * Note:            Using static variable for less code size
 *****************************************************************************/
void printdial()
{
	static unsigned char i;
	static int k;
	k=11;
	for(i=0;i<12;i++)
	{
		sec2coord(i*5);
		oledLine(32+cx,36+cy,32+(cx*k/10),36+(cy*k/10),1,1);
	}
}


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
	static unsigned char cerr,nl,n,i,b;
	static unsigned int p,q;
	static rtccTimeDate td;
	
	// Soft Start the APP_VDD
    if(AppPowerReady() == FALSE) return;

/* --------------------
 * User input section, handles touchbuttons, and the menu button
 * for less cpu stress it runs on every 2000th loop.
 * buttonstate bits flags if the butoon pressed or released for
 * exclude repetition. position and screen variables points the
 * cursors place, and the action depends on it (incrementing,
 * decrementing variables even the position and screen variable.
 * if the screen variable changed, the screenvalid become false
 * pointing the drawing section to refresh screen.
 * if the positon variable changed, the positionchanged become
 * true pointing that the cursor moves. 	
 * -------------------- */	
	if(executionTick++>2000)
	{
		executionTick=0;
		p=mTouchReadButton(0);									// button 0 : Right (accept)
		if(buttonstate.bits.b0)
			{ if(p>750) buttonstate.bits.b0=0; }				// released
		else
			if(p<600)											// pressed
			{
				buttonstate.bits.b0=1;
				if((screen==3)&&(position<16)) position++;
				if((screen==4)&&(position<16))
				{
					if(position!=6) position++;
					else switch(mRtccGetAlarmRpt())
						{
							case 2: position=16;
								break;
							case 3: position=15;
								break;
							case 4: position=14;
								break;
							case 5: position=13;
								break;
							case 6: position=11;
								break;
							case 7: position=10;
								break;
							case 8: position=9;
								break;
							case 9: position=7;
						}
				}
				positionchanged=1;
			}  
		p=mTouchReadButton(3);									// button 0 : Left (cancel)
		if(buttonstate.bits.b2)
			{ if(p>750) buttonstate.bits.b2=0; }				// released
		else
			if(p<600)											// pressed
			{
				buttonstate.bits.b2=1;
				if((screen==3)&&position) position--;
				if((screen==4)&&position)
				{
					if(position<9) position--;
					else 
					{
						n=mRtccGetAlarmRpt();
						if((n==2)||((n==3)&&(position==15))||((n==4)&&(position==14))||
							((n==5)&&(position==13))||((n==6)&&(position==11))||
							((n==7)&&(position==10))||((n==8)&&(position==9))) position=6;
						else position--;
					}
				}
				positionchanged=1;
			}  
		p=mTouchReadButton(1);
		q=mTouchReadButton(2);
		if(buttonstate.bits.b1)									// button 1,2 : Scroller
			{ if((p>750)&&(q>750)) buttonstate.bits.b1=0;}		// released
		else
			if((p<700)||(q<700))
			{
				buttonstate.bits.b1=1;
				if(p<q)											// up
				{
					if((position==0)&&(screen)) screen--,screenvalid=0;
					if(screen==3)
					{
						if((position>0)&&(position<13))
						{
							switch(position)
							{
								case 1: screentd.f.year+=0x10;
										break;
								case 2: bcdadd1(&screentd.f.year);
										break;
								case 3: screentd.f.mon+=0x10;
										break;
								case 4: bcdadd1(&screentd.f.mon);
										break;
								case 5: screentd.f.mday+=0x10;
										break;
								case 6: bcdadd1(&screentd.f.mday);
										break;	
								case 7: screentd.f.hour+=0x10;
										break;
								case 8: bcdadd1(&screentd.f.hour);
										break;
								case 9: screentd.f.min+=0x10;
										break;
								case 10: bcdadd1(&screentd.f.min);
										break;
								case 11: screentd.f.sec+=0x10;
										break;
								case 12: bcdadd1(&screentd.f.sec);
							}
							RtccWriteTimeDate(&screentd,0);
						}
						if((position==13)&&!mRtccIsOn()) mRtccOn();
						if(position>13)
						{
							switch(position)
							{
								case 14: if(screencalibration<27) screencalibration+=100;
										break;
								case 15: if(screencalibration<117) screencalibration+=10;
										break;
								case 16: if(screencalibration<127) screencalibration++;
										break;
							}
							RtccSetCalibration(screencalibration);
						}
					}
					if(screen==4)
					{
						if((position==1)&&(!mRtccIsAlrmEnabled())) mRtccAlrmEnable();
						else if((position==2)&&(!mRtccGetChimeEnable())) RtccSetChimeEnable(TRUE,TRUE);	
						else if((position>2)&&(position<6))
						{
							b=mRtccGetAlarmRptCount();
							switch(position)
							{
								case 3: if(b<155) b+=100;
										break;
								case 4: if(b<245) b+=10;
										break;
								case 5: if(b<254) b++;	 
							}
							RtccSetAlarmRptCount(b,TRUE);
						}
						else if((position==6)&&(mRtccGetAlarmRpt()<9)) RtccSetAlarmRpt(mRtccGetAlarmRpt()+1,TRUE);
						else
						{
							RtccReadAlrmTimeDate(&td);
							switch(position)
							{
								case 7: td.f.mon+=0x10;
										break;
								case 8: bcdadd1(&td.f.mon);
										break;
								case 9: td.f.mday+=0x10;
										break;
								case 10: if(mRtccGetAlarmRpt()==7) td.f.wday++;
										else bcdadd1(&td.f.mday);
										break;	
								case 11: td.f.hour+=0x10;
										break;
								case 12: bcdadd1(&td.f.hour);
										break;
								case 13: td.f.min+=0x10;
										break;
								case 14: bcdadd1(&td.f.min);
										break;
								case 15: td.f.sec+=0x10;
										break;
								case 16: bcdadd1(&td.f.sec);
							}
							RtccWriteAlrmTimeDate(&td);
						}
					}
				}
				else											// down
				{
				if((position==0)&&(screen<4)) screen++,screenvalid=0;
					if(screen==3)
					{
						if((position>0)&&(position<13))
						{
							switch(position)
							{
								case 1: screentd.f.year-=0x10;
										break;
								case 2: bcdsub1(&screentd.f.year);
										break;
								case 3: screentd.f.mon-=0x10;
										break;
								case 4: bcdsub1(&screentd.f.mon);
										break;
								case 5: screentd.f.mday-=0x10;
										break;
								case 6: bcdsub1(&screentd.f.mday);
										break;	
								case 7: screentd.f.hour-=0x10;
										break;
								case 8: bcdsub1(&screentd.f.hour);
										break;
								case 9: screentd.f.min-=0x10;
										break;
								case 10: bcdsub1(&screentd.f.min);
										break;
								case 11: screentd.f.sec-=0x10;
										break;
								case 12: bcdsub1(&screentd.f.sec);
							}
							RtccWriteTimeDate(&screentd,0);
						}
						if((position==13)&&mRtccIsOn()) mRtccOff();
						if(position>13)
						{
							switch(position)
							{
								case 14: if(screencalibration>-27) screencalibration-=100;
										break;
								case 15: if(screencalibration>-117) screencalibration-=10;
										break;
								case 16: if(screencalibration>-127) screencalibration--;
										break;
							}
							RtccSetCalibration(screencalibration);
						}
					}
					if(screen==4)
					{
						if((position==1)&&(mRtccIsAlrmEnabled())) mRtccAlrmDisable();
						else if((position==2)&&(mRtccGetChimeEnable())) RtccSetChimeEnable(FALSE,TRUE);	
						else if((position>2)&&(position<6))
						{
							b=mRtccGetAlarmRptCount();
							switch(position)
							{
								case 3: if(b>99) b-=100;
										break;
								case 4: if(b>9) b-=10;
										break;
								case 5: if(b) b--;	 
							}
							RtccSetAlarmRptCount(b,TRUE);
						}
						else if((position==6)&&(mRtccGetAlarmRpt())) RtccSetAlarmRpt(mRtccGetAlarmRpt()-1,TRUE);
						else
						{
							RtccReadAlrmTimeDate(&td);
							switch(position)
							{
								case 7: td.f.mon-=0x10;
										break;
								case 8: bcdsub1(&td.f.mon);
										break;
								case 9: td.f.mday-=0x10;
										break;
								case 10: if(mRtccGetAlarmRpt()==7) td.f.wday--;
										else bcdsub1(&td.f.mday);
										break;	
								case 11: td.f.hour-=0x10;
										break;
								case 12: bcdsub1(&td.f.hour);
										break;
								case 13: td.f.min-=0x10;
										break;
								case 14: bcdsub1(&td.f.min);
										break;
								case 15: td.f.sec-=0x10;
										break;
								case 16: bcdsub1(&td.f.sec);
							}
							RtccWriteAlrmTimeDate(&td);
						}
					}	
				}
				positionchanged=1;
			}
		if(buttonstate.bits.b3)									// button RB0 : Menu
			{ 
				if(PORTBbits.RB0) 
				{
					resetcounter=0;
					screenvalid=0;
					buttonstate.bits.b3=0;
				}
				else  
				{
					strcpypgm2ram(cbuf,(const rom far char *)"Reset ");
					if(!(resetcounter++%10)||(!resetcounter))
					{
						itoa(10-resetcounter/10,cbuf+6);
						rjustify(cbuf+6,2);
						oledPutStringINV((unsigned char *)cbuf,7,0);
					}
					if(resetcounter>105)
					{
						_asm
						RESET
						_endasm
					}						
				}
			}
		else
			if(!PORTBbits.RB0)
				buttonstate.bits.b3=1;
	}

/* --------------------
 * Drawing section: draws the screens depending on controller state and given
 * variables.
 * If the screenvalid variable is false, the whole screen is cleared and redrawn
 * otherwise depends on screen
 * for example on the startup, clock and setup screen is redraw the clock, if
 * the rtcc clock differs, from the stored in screentd
 * the screens drawing depend on position variable, which pointing the user input
 * whith inverse characters.
 * On clock screen the the clokss variable counts 10 seconds, and after hides
 * the header row as a "screensaver".
 * -------------------- */	

	if(!screenvalid)
	{
		if(!devicereset) 
        {
            ResetDevice();
            devicereset++;
        }
		FillDisplay(0x00);
		oledPutROMString((ROM_STRING)"screen",7,48);
		itoa(screen,cbuf);
		if(position) oledPutString((unsigned char *)cbuf,7,120);
		else oledPutStringINV((unsigned char *)cbuf,7,120);
	}
	switch(screen)
	{
		case 0:
			if(!executionTick||!screenvalid)
			{
				RtccReadTimeDate(&td);
				if((td.l[0]!=screentd.l[0])||(td.l[1]!=screentd.l[1])||!screenvalid)
				{
					screentd.l[0]=td.l[0];
					screentd.l[1]=td.l[1];
					dttoa(&screentd,cbuf);
					cbuf[10]=' ';
					tmtoa(&screentd,cbuf+11);
					oledPutString((unsigned char *)cbuf,3,6);
				}
			}
			if(!screenvalid)
			{
	            oledPutROMString((ROM_STRING)"Startup",7,0);
	            oledPutROMString((ROM_STRING)"CDC Clock Demo",1,18);
	            oledPutROMString((ROM_STRING)"Touch Down to switch",5,0);
	            oledPutROMString((ROM_STRING)"to help screen",6,18);
    	        screenvalid=1;
			}
			break;
		case 1:
			if(!screenvalid)
			{
				oledPutROMString((ROM_STRING)"Help",7,0);
				for(i=0;i<6;i++) oledPutROMString(helpstring[i],i+1,0);
				screenvalid=1;
			}
			break;
		case 2:
			if(!executionTick||!screenvalid)
			{
				RtccReadTimeDate(&td);
				if((td.l[0]!=screentd.l[0])||(td.l[1]!=screentd.l[1])||!screenvalid)
				{
					if(screenvalid) printanalog();
					else printdial();
					if(clockss<10) clockss++;
					if(clockss==10) FillDisplay(0x00),printdial(),clockss=11;
					screentd.l[0]=td.l[0];
					screentd.l[1]=td.l[1];
					dttoa(&screentd,cbuf);
					oledPutString((unsigned char *)cbuf,2,64);
					tmtoa(&screentd,cbuf);
					oledPutString((unsigned char *)cbuf,4,70);
					prevm=(screentd.f.min&0x0f)+10*(screentd.f.min>>4);
					prevs=(screentd.f.sec&0x0f)+10*(screentd.f.sec>>4);
					prevh=(((screentd.f.hour&0x0f)+10*(screentd.f.hour>>4))%12)*5+prevm/12;
					printanalog();
				}
			}
			if(!screenvalid)
			{
				oledPutROMString((ROM_STRING)"Clock",7,0);
				clockss=0;
				screenvalid=1;
			}
			break;
		case 3:
			if(!executionTick||!screenvalid||positionchanged)
			{
				RtccReadTimeDate(&td);
				if((td.l[0]!=screentd.l[0])||(td.l[1]!=screentd.l[1])||!screenvalid||positionchanged)
				{
					screentd.l[0]=td.l[0];
					screentd.l[1]=td.l[1];
					dttoa(&screentd,cbuf+20);
					oledPutString((unsigned char *)cbuf+20,1,36);
					tmtoa(&screentd,cbuf);
					oledPutString((unsigned char *)cbuf,3,42);
					if((position>0)&&(position<13))
					{
						i=0;
						switch(position)
						{
							case 6: 
							case 12: i++;
							case 5:
							case 11: i+=2;
							case 4:
							case 10: i++;
							case 3:
							case 9: i+=2;
							case 2:
							case 8:	 i++;
						}
						n=(position>6);
						oledWriteChar1xINV((unsigned char)cbuf[i+(n?0:22)],n?0xb4:0xb2,(i+(n?7:8))*6);
					}
				}
				if(!screenvalid||positionchanged)
				{
					if(mRtccIsOn()) strcpypgm2ram(cbuf,onstr);
					else strcpypgm2ram(cbuf,offstr);
					if(position==13) oledPutStringINV((unsigned char *)cbuf,5,72);
					else oledPutString((unsigned char *)cbuf,5,72);
					itoa(screen,cbuf);
					if(position) oledPutString((unsigned char *)cbuf,7,120);
					else oledPutStringINV((unsigned char *)cbuf,7,120);
					screencalibration=mRtccGetCalibration();
					itoa((int)screencalibration,cbuf);
					rjustify(cbuf,4);
					if(position>13)
					{
						if(screencalibration<0)
						{
							if(screencalibration>-100) cbuf[1]='0';
							if(screencalibration>-10) cbuf[2]='0';
							*cbuf='-';
						}
						else
						{
							if(screencalibration<100) cbuf[1]='0';
							if(screencalibration<10) cbuf[2]='0';
						}
						i=position-13;
						oledPutString((unsigned char *)cbuf,6,72);
						oledWriteChar1xINV((unsigned char)cbuf[i],0xb7,(i+12)*6);
					}
					else oledPutString((unsigned char *)cbuf,6,72);
				}
				positionchanged=0;
			}	
			if(!screenvalid)
			{
				oledPutROMString((ROM_STRING)"Setup",7,0);
				oledPutROMString((ROM_STRING)"RTCC module",5,0);
				oledPutROMString((ROM_STRING)"Calibration",6,0);
				screenvalid=1;
			}
			break;
		case 4:
			if(!screenvalid||positionchanged)
			{
				itoa(screen,cbuf);
				if(position) oledPutString((unsigned char *)cbuf,7,120);
				else oledPutStringINV((unsigned char *)cbuf,7,120);
				if(mRtccIsAlrmEnabled()) strcpypgm2ram(cbuf,onstr);
				else strcpypgm2ram(cbuf,offstr);
				if(position==1) oledPutStringINV((unsigned char *)cbuf,1,42);
				else oledPutString((unsigned char *)cbuf,1,42);
				if(mRtccGetChimeEnable()) strcpypgm2ram(cbuf,onstr);
				else strcpypgm2ram(cbuf,offstr);
				if(position==2) oledPutStringINV((unsigned char *)cbuf,2,42);
				else oledPutString((unsigned char *)cbuf,2,42);
				p=mRtccGetAlarmRptCount();
				itoa(p,cbuf);
				rjustify(cbuf,3);
				if((position>2)&&(position<6))
				{
					if(p<100) cbuf[0]='0';
					if(p<10) cbuf[1]='0';
					i=position-3;
					oledPutString((unsigned char *)cbuf,3,42);
					oledWriteChar1xINV((unsigned char)cbuf[i],0xb4,(i+7)*6);
				}
				else oledPutString((unsigned char *)cbuf,3,42);
				n=mRtccGetAlarmRpt();
				itoa(n,cbuf);
				if(position==6) oledWriteChar1xINV((unsigned char)*cbuf,0xb5,54);
				else oledPutString((unsigned char *)cbuf,4,54);
				RtccReadAlrmTimeDate(&td);
				if(n==9)
				{
					btoh(td.f.mon,cbuf);
					oledPutString((unsigned char *)cbuf,5,48);
					if(position==7) oledWriteChar1xINV((unsigned char)*cbuf,0xb6,48);
					if(position==8) oledWriteChar1xINV((unsigned char)cbuf[1],0xb6,54);
					oledPutROMString(minusstr,5,60);
				}
				if((n==8)||(n<7))
				{
					oledPutROMString(xxstr,5,48);
					oledPutROMString(minusstr,5,60);
				}
				if(n>7)
				{
					btoh(td.f.mday,cbuf);
					oledPutString((unsigned char *)cbuf,5,66);
					if(position==9) oledWriteChar1xINV((unsigned char)*cbuf,0xb6,66);
					if(position==10) oledWriteChar1xINV((unsigned char)cbuf[1],0xb6,72);
				}
				if(n<7) oledPutROMString(xxstr,5,66);
				if(n==7)
				{
					btoh(td.f.wday,cbuf);
					oledPutROMString((ROM_STRING)"dow ",5,48);
					if(position==10) oledWriteChar1xINV((unsigned char)cbuf[1],0xb6,72);
					else oledPutString((unsigned char *)cbuf+1,5,72);
				}
				tmtoa(&td,cbuf);
				oledPutString((unsigned char *)cbuf,6,42);
				if((position>10))
				{
					i=0;
					switch(position)
					{
						case 16: i++;
						case 15: i+=2;
						case 14: i++;
						case 13: i+=2;
						case 12: i++;
					}
					oledWriteChar1xINV((unsigned char)cbuf[i],0xb7,(i+7)*6);
				}
				switch(n)
				{
					case 0:
					case 1: oledPutROMString(xxstr+1,6,84);
					case 2: oledPutROMString(xxstr+1,6,78);
					case 3: oledPutROMString(xxstr+1,6,66);
					case 4: oledPutROMString(xxstr+1,6,60);
					case 5: oledPutROMString(xxstr,6,42);
				}
				positionchanged=0;
			}
			if(!screenvalid)
			{
				oledPutROMString(alarmstr,7,0);
				oledPutROMString(alarmstr,1,0);
				oledPutROMString((ROM_STRING)"Chime",2,0);
				oledPutROMString((ROM_STRING)"Repeat",3,0);
				oledPutROMString((ROM_STRING)"Mask",4,0);
				screenvalid=1;
			}
			break;
	}

/* --------------------
 * Alarm section, if an alarm occured, a flashing signal drawn on
 * the top row for a while. At the start, and at the end the whole
 * screen will be redrawn. For example showing, the alarm count
 * decrementing.
 * -------------------- */	

	if(PIR3bits.RTCCIF==1)
	{
		alarmcnt=1;
		PIR3bits.RTCCIF=0;
		screenvalid=0;
		USBCBSendResume();
	}
	else if(alarmcnt>250)
	{
		alarmcnt=0;
		screenvalid=0;
	}
	if(!executionTick&&alarmcnt)
	{
		strcpypgm2ram(cbuf,(const far rom char *)"** ALARM **");
		if((++alarmcnt%10)>5) oledPutString((unsigned char *)cbuf,7,30);
		else oledPutStringINV((unsigned char *)cbuf,7,30);
	}	
	
/* --------------------
 * Communication section, handling serial commands, printing time, and
 * any other USB tasks. 
 * Originally I planned larger this section (e.g. setting time, or
 * alarm, querying controller state) but the memory is full with the 
 * other tasks, and dont have space for it.
 * -------------------- */	
	
	
    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	if(mUSBUSARTIsTxTrfReady())
	{
		outbuf[0]=0;
		n=getsUSBUSART(cbuf,64);
		if(n)
		{
			for(i=0;i<n;i++)
			{
				strcatc(outbuf,cbuf[i]);						// echo
				if((strlen(cmdstr) < CMDSTRLEN)&&isprint(cbuf[i])) strcatc(cmdstr,cbuf[i]);
				if(cbuf[i]=='\r')
				{
					cerr=0;										// command error
					nl=1;										// send new line after
					outbufend=strend(outbuf);
					switch(*cmdstr)
					{ 
						case 0: 
								nl=0;
								break;
						case 'v': 								// version
								strcpypgm2ram(outbufend,(const far rom char *)"clock ver 1.0");
								break;
						case 'g':								// get
								switch(cmdstr[1])
								{
									case 't':					// get time
											{
												RtccReadTimeDate(&td);
												dttoa(&td,outbufend);
												outbufend[10]=' ';
												tmtoa(&td,outbufend+11);
											}
											break;
									case 'a':					// get alarm
											{	
												
												RtccReadAlrmTimeDate(&td);
												dttoa(&td,outbufend);
												outbufend[10]=' ';
												tmtoa(&td,outbufend+11);
												outbufend[19]='\n';
												outbufend[20]='\r';
												strcpypgm2ram(outbufend+21,(const far rom char *)alarmstr);
											}
											break;
									default:
											cerr=1;				// command error
								}
								break;
						default:
								cerr=1;							// command error
								
					}
					if(cerr) strcpypgm2ram(outbufend,(const far rom char *)"ERR");
					if(nl) strcatc(outbuf,'\n'),strcatc(outbuf,'\r');
					cmdstr[0]=0;
				}
			}
		}
		if(outbuf[0])            			// if output buffer not empty
		putUSBUSART(outbuf,strlen(outbuf));
	}	
 
	CDCTxService();
}		//end ProcessIO


// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Turn off the App Vdd
	AppPowerDisable();
}
/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	while(AppPowerReady() == FALSE)
	{
		// Soft Start the APP_VDD	
	}
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.
	
	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckCDCRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    CDCInitEP();
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function should only be called when:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling USBCBWakeFromSuspend()
 
 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    USBResumeControl = 1;                // Start RESUME signaling
    
    delay_count = 1800U;                // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}


/*******************************************************************
 * Function:        void USBCBEP0DataReceived(void)
 *
 * PreCondition:    ENABLE_EP0_DATA_RECEIVED_CALLBACK must be
 *                  defined already (in usb_config.h)
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called whenever a EP0 data
 *                  packet is received.  This gives the user (and
 *                  thus the various class examples a way to get
 *                  data that is received via the control endpoint.
 *                  This function needs to be used in conjunction
 *                  with the USBCBCheckOtherReq() function since 
 *                  the USBCBCheckOtherReq() function is the apps
 *                  method for getting the initial control transfer
 *                  before the data arrives.
 *
 * Note:            None
 *******************************************************************/
#if defined(ENABLE_EP0_DATA_RECEIVED_CALLBACK)
void USBCBEP0DataReceived(void)
{
}
#endif

/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }      
    return TRUE; 
}

/** EOF main.c *************************************************/
