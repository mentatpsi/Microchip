/********************************************************************
 FileName:		main.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18,PIC24,PIC32 & dsPIC33F Microcontrollers
 Hardware:		This demo is natively intended to be used on Exp 16, LPC
 				& HPC Exp board. This demo can be modified for use on other hardware
 				platforms.
 Complier:  	Microchip C18 (for PIC18), C30 (for PIC24 & dsPIC), C32(for PIC32)
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
  Rev   Description
  ----  -----------------------------------------
  1.0      Initial release
  1.01     Modified to Support T=1 protocol
  1.02.2   Modified the usage of PPS functionality.
           Moved timer interrupts (used by smart card stack) to ISO 7816 hardware
           driver files.
           Cleaned unwanted stuff in the file.
  1.02.4   Removed 'COE_OFF' from _CONFIG1 settings of PIC24FJ256GB110 part.
********************************************************************/

#include "string.h"
#include "sc_config.h"
#include "./Smart Card/SClib.h"

#include "stdio.h"

#if defined(EXPLORER_16)
    #ifdef __PIC24FJ256GB110__ //Defined by MPLAB when using 24FJ256GB110 device
		_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & ICS_PGx2 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS1);
		_CONFIG2(IESO_OFF & PLLDIV_DIV2 & FNOSC_PRIPLL & POSCMOD_HS & FCKSM_CSDCMD & OSCIOFNC_OFF & IOL1WAY_OFF & 0xF7FF) 
		_CONFIG3(WPCFG_WPCFGDIS & WPDIS_WPDIS) 
	#elif defined(__dsPIC33F__) || defined(__PIC24H__)
		_FOSCSEL(FNOSC_PRIPLL);
		_FOSC(FCKSM_CSECMD &OSCIOFNC_OFF &POSCMD_HS);
		_FWDT(FWDTEN_OFF);
		_FICD(ICS_PGD1 & JTAGEN_OFF);
    #elif defined(__32MX795F512L__)
        #pragma config UPLLEN   = ON        	// USB PLL Enabled
        #pragma config FPLLMUL  = MUL_16        // PLL Multiplier
        #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
        #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
        #pragma config FPLLODIV = DIV_4         // PLL Output Divider
        #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
        #pragma config FWDTEN   = OFF           // Watchdog Timer
        #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
        #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
        #pragma config OSCIOFNC = OFF           // CLKO Enable
        #pragma config POSCMOD  = HS            // Primary Oscillator
        #pragma config IESO     = OFF           // Internal/External Switch-over
        #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
        #pragma config FNOSC    = PRIPLL        // Oscillator Selection
        #pragma config CP       = OFF           // Code Protect
        #pragma config BWP      = OFF           // Boot Flash Write Protect
        #pragma config PWP      = OFF           // Program Flash Write Protect
        #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
        #pragma config DEBUG    = ON            // Background Debugger Enable

	#endif
#elif defined(HPC_EXPLORER)
    #ifdef __18F46J50 //Defined by MPLAB when using PIC18F46J50 device
		#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
	    #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
	    #pragma config STVREN = ON            //stack overflow/underflow reset enabled
	    #pragma config XINST = ON          //Extended instruction set disabled
	    #pragma config CPUDIV = OSC1       //CPU system clock divide by 1
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

	#elif defined(__18F87J50)				// Configuration bits for PIC18F87J50 FS USB Plug-In Module board
        #pragma config XINST    = ON   		// Extended instruction set
        #pragma config STVREN   = ON      	// Stack overflow reset
        #pragma config PLLDIV   = 3         // (12 MHz crystal used on this board)
        #pragma config WDTEN    = OFF      	// Watch Dog Timer (WDT)
        #pragma config CP0      = OFF      	// Code protect
        #pragma config CPUDIV   = OSC1      // OSC1 = divide by 1 mode
        #pragma config IESO     = OFF      	// Internal External (clock) Switchover
        #pragma config FCMEN    = OFF      	// Fail Safe Clock Monitor
        #pragma config FOSC     = HSPLL     // Firmware must also set OSCTUNE<PLLEN> to start PLL!
        #pragma config WDTPS    = 32768
//      #pragma config WAIT     = OFF      	// Commented choices are
//      #pragma config BW       = 16      	// only available on the
//      #pragma config MODE     = MM      	// 80 pin devices in the 
//      #pragma config EASHFT   = OFF      	// family.
        #pragma config MSSPMSK  = MSK5
//      #pragma config PMPMX    = DEFAULT
//      #pragma config ECCPMX   = DEFAULT
        #pragma config CCP2MX   = DEFAULT

	#endif
#elif defined(LOW_PIN_COUNT_USB_DEVELOPMENT_KIT)
	// PIC18F14K50
        #pragma config CPUDIV = NOCLKDIV
        #pragma config USBDIV = OFF
        #pragma config FOSC   = HS
        #pragma config PLLEN  = ON
        #pragma config FCMEN  = OFF
        #pragma config IESO   = OFF
        #pragma config PWRTEN = OFF
        #pragma config BOREN  = OFF
        #pragma config BORV   = 30
        #pragma config WDTEN  = OFF
        #pragma config WDTPS  = 32768
        #pragma config MCLRE  = ON
        #pragma config HFOFST = OFF
        #pragma config STVREN = ON
        #pragma config LVP    = OFF
        #pragma config XINST  = ON
        #pragma config BBSIZ  = OFF
        #pragma config CP0    = OFF
        #pragma config CP1    = OFF
        #pragma config CPB    = OFF
        #pragma config WRT0   = OFF
        #pragma config WRT1   = OFF
        #pragma config WRTB   = OFF
        #pragma config WRTC   = OFF
        #pragma config EBTR0  = OFF
        #pragma config EBTR1  = OFF
        #pragma config EBTRB  = OFF 

#elif defined(PICDEM_FS_USB)      // Configuration bits for PICDEM FS USB Demo Board (based on PIC18F4550)
        #pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
        #pragma config CPUDIV   = OSC1_PLL2   
        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = ON
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON      //USB Voltage Regulator
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768
        #pragma config MCLRE    = ON
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF
        #pragma config CCP2MX   = ON
        #pragma config STVREN   = ON
        #pragma config LVP      = OFF
//      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
        #pragma config XINST    = ON       // Extended Instruction Set
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF
//      #pragma config CP2      = OFF
//      #pragma config CP3      = OFF
        #pragma config CPB      = OFF
//      #pragma config CPD      = OFF
        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF
//      #pragma config WRT2     = OFF
//      #pragma config WRT3     = OFF
        #pragma config WRTB     = OFF       // Boot Block Write Protection
        #pragma config WRTC     = OFF
//      #pragma config WRTD     = OFF
        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF
//      #pragma config EBTR2    = OFF
//      #pragma config EBTR3    = OFF
        #pragma config EBTRB    = OFF

#else
 	#error No hardware board defined, see "sc_config.h"
#endif

extern void WaitMilliSec(unsigned long int ms);

// APDU Command to the Card
SC_APDU_COMMAND cardCommand;

// APDU Response from the Card
SC_APDU_RESPONSE cardResponse;

// PPS Request to the card
BYTE ppsString[3] = 
{
	0xFF,	// PPSS byte
	0x00,	// PPS0 byte
	0xFF	// PCK byte
};

#ifdef SC_PROTO_T1
	// T=1, Prologue Field
	SC_T1_PROLOGUE_FIELD prologueField;
#endif

// Store the APDU Command/Response Data in a 256 bytes register bank(RAM)
#if defined(__18CXX)
	extern BOOL delayLapsedFlag;
	void LowISR();
	#pragma udata apdu_data
		BYTE apduData[SC_APDU_BUFF_SIZE];
	#pragma udata

#else
	BYTE apduData[SC_APDU_BUFF_SIZE];
#endif


///////////////////////////////////////////////////
//////////////////////////////////////////////////
#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{
	int breakPoint = 0;	
	
    #if defined(__18F46J50) || defined(__18F87J50)
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
	#elif defined(__18F14K50)
    {
        unsigned int pll_startup_counter = 600;
        OSCTUNEbits.SPLLEN = 1;  //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
        while(pll_startup_counter--);
    }
	#elif defined(__dsPIC33F__) || defined(__PIC24H__)
    {
		OSCCONbits.NOSC = 0x03;
		CLKDIVbits.DOZE = 0x00;
		PLLFBDbits.PLLDIV = 30; // 32MHz, i.e (4Mz * 32)/4
	}
	#endif

    #if defined(__18CXX)
        ADCON1 |= 0x0F;                 // Default all pins to digital
    #elif defined(__C30__)
        AD1PCFGL = 0xFFFF;
    #elif defined(__C32__)
        AD1PCFG = 0xFFFF;
    #endif

	//Start smart card stack
	SC_Initialize();

	// Proceed further only after detecting the card
	while( !SC_CardPresent() );
		
	//Turn on power to Card and recieve and process Answer-to-Reset
	if( !SC_PowerOnATR() )
		while(1)
		{
			breakPoint++;
		}

	WaitMilliSec(20);

	// If T = 1 protocol supported
	if(SC_T1ProtocolType())
	{
		// If T=1 is defined
		#ifdef SC_PROTO_T1

			// Write Node Address, Protocol Control Byte & I-Field Length respectively
			prologueField.NAD = 0;
			prologueField.PCB = 0;
			prologueField.LENGTH = 5;

			// Write the APDU command in I field array (Get CPLC Data Command)
			apduData[0] = 0x80;
			apduData[1] = 0xCA;
			apduData[2] = 0x9F;
			apduData[3] = 0x7F;
			apduData[4] = 0x00;
			
			// Transact using T=1 protocol
			if(!SC_TransactT1(&prologueField,apduData,&cardResponse))
				while(1)
				{
					breakPoint++;
				}

		#endif
	}

	// If T = 0 protocol supported
	if(SC_T0ProtocolType())
	{
		// Note :- T =1 & T = 0 cards stop responding if any unsucessful PPS
		// exchange happens between interfacing device and the smart card. Some of 
		// the T = 0 and T = 1 cards wont support PPS exchange. If unsuccesful
		// PPS exchange happens then we need to shutdown the smart card. So it is
		// better not to do PPS if you are not sure. For demo purpose PPS request 
		// is transmitted to T = 0 cards.
		if( !SC_DoPPS(ppsString) )
			while(1)
			{
				breakPoint++;
			}

		WaitMilliSec(10);

		//Execute Card Commands ///////////////////////
		// ACOS3 Smart Card from Advanced Card Systems Ltd. is used for testing the
		// Smart Card library.
		
		// Submit Code
		cardCommand.CLA = 0x80;
		cardCommand.INS = SC_SUBMIT_CODE;
		cardCommand.P1 	= 7;
		cardCommand.P2  = 0;
		cardCommand.LC  = 8;	//tx 8 bytes of auth code
		#if defined(__PIC24F__) || defined(__dsPIC33F__) || defined(__PIC24H__) || defined(__PIC32MX__)
			memcpy( apduData, "ACOSTEST", 8 );
		#elif defined(__18F46J50) || defined(__18F87J50) || defined(__18F14K50) || defined(__18F4550)
			apduData[0] = 'A';
			apduData[1] = 'C';
			apduData[2] = 'O';
			apduData[3] = 'S';
			apduData[4] = 'T';
			apduData[5] = 'E';
			apduData[6] = 'S';
			apduData[7] = 'T';
		#else
			// Do Nothing
		#endif
		cardCommand.LE  = 0;
		
		if( !SC_TransactT0( &cardCommand, &cardResponse, apduData ) )
			while(1)
			{
				breakPoint++;
			}

		WaitMilliSec(10);

		// Select File ///////////////
		cardCommand.CLA = 0x80;
		cardCommand.INS = SC_SELECT_FILE;
		cardCommand.P1 	= 0;
		cardCommand.P2  = 0;
		cardCommand.LC  = 2;	//tx 2 bytes of file id
		apduData[0] = 0xFF;
		apduData[1] = 0x01;	//manufac File ID
		cardCommand.LE  = 0;

		if( !SC_TransactT0( &cardCommand, &cardResponse, apduData ) )
			while(1)
			{
				breakPoint++;
			}

		WaitMilliSec(10);

		//Write record to selected file
		cardCommand.CLA = 0x80;
		cardCommand.INS = SC_WRITE_RECORD;
		cardCommand.P1 	= 0; 				//rec index
		cardCommand.P2  = 0;
		cardCommand.LC  = 8;				//tx 8 bytes
		apduData[0] = 0;
		apduData[1] = 10;
		apduData[2] = 20;
		apduData[3] = 35;
		apduData[4] = 45;
		apduData[5] = 58;
		apduData[6] = 68;
		apduData[7] = 0;
		cardCommand.LE  = 0;

		if( !SC_TransactT0( &cardCommand, &cardResponse, apduData ) )
			while(1)
			{
				breakPoint++;
			}

		WaitMilliSec(10);

		//Readback the contents to verify write data
		cardCommand.CLA = 0x80;
		cardCommand.INS = SC_READ_RECORD;
		cardCommand.P1 	= 0; 				//rec index
		cardCommand.P2  = 0;
		cardCommand.LC  = 0;				
		cardCommand.LE  = 8;				//expect 8 bytes from card

		if( !SC_TransactT0( &cardCommand, &cardResponse, apduData ) )
			while(1)
			{
				breakPoint++;
			}

	}

	// Shut Down the Card when dont need anything to do with it
	SC_Shutdown();

	while(1)
	{
		breakPoint++;
	}

    #if !defined(__18CXX)
    return 0;
    #endif
}

#if defined(__18CXX)
/*********************************************************************
 * Function:        void LowVector (void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Instruction at Low Interrupt Vector Address location
 *
 * Note:            None
 ********************************************************************/

#pragma code lowhVector=0x18
void LowVector (void)
{
    _asm goto LowISR _endasm
}
#pragma code

/*********************************************************************
 * Function:        void LowISR(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Low Priority Interrupts jump to this location.
 *                  Timer 0 overflow flag is checked.
 *
 * Note:            None
 ********************************************************************/

#pragma interruptlow LowISR
void LowISR(void)
{
	//check for TMR0 overflow
	if (INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF = 0; //clear interrupt flag
		SCdrv_DisableDelayTimer();
		delayLapsedFlag = 1; //indicate timeout
	}
}

#endif
