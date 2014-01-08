/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the external memory to store fonts and bitmaps.
 * 
 * To run this demo:
 * external.hex file in "Fonts and Bitmaps" folder must be programmed into external
 * flash memory installed on Graphics PICTail Plus board. To do this
 * "Graphics PICTail Board Memory Programmer" project can be used. Please refer to
 * "Getting started -> Graphics External Memory Demo" section in the help file for more information.
 *
 * To create a new hex file containing data for font and bitmap to be located in
 * the external flash memory use 'Bitmap and Font converter.exe' utility included in the Graphics Library.
 * With hex file the utility will create a C file containing structures for referencing
 * the font and bitmap in the hex file. This C file must be included in the project.
 * Please refer to the help built in the utility.
 *
 * In applications with external memory USE_BITMAP_EXTERNAL and USE_FONT_EXTERNAL
 * must be defined in GraphicsConfig.h. 
 *
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright (c) 2010 Microchip Technology Inc.  All rights reserved.
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
 * Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 09/01/10	   ...
 *****************************************************************************/
#include "MainDemo.h"
#include "drv_spi.h"
#include "ExternalMemory.h"
#include "TimeDelay.h"
#include "Graphics/DisplayDriver.h"
#include "HardwareProfile.h"

#if defined(__dsPIC33F__) || defined(__PIC24H__)
_FOSCSEL(FNOSC_PRI);
_FOSC(FCKSM_CSECMD &OSCIOFNC_OFF &POSCMD_XT);
_FWDT(FWDTEN_OFF);
#elif defined(__dsPIC33E__) 
_FOSCSEL(FNOSC_FRC);			
_FOSC(FCKSM_CSECMD & POSCMD_XT & OSCIOFNC_OFF & IOL1WAY_OFF);
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR128 & BOREN_ON & ALTI2C1_ON & ALTI2C2_ON);
_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);
#elif defined(__PIC32MX__)
    #pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FCKSM = CSECME, FPBDIV = DIV_1
    #pragma config OSCIOFNC = ON, POSCMOD = XT, FSOSCEN = ON, FNOSC = PRIPLL
    #pragma config CP = OFF, BWP = OFF, PWP = OFF
#ifdef USE_COMM_PKT_MEDIA_USB
    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
    #pragma config UPLLEN   = ON
#endif
#else
    #if defined(__PIC24FJ256GB110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ256GA110__)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2)
_CONFIG2(IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_XT & FNOSC_PRIPLL & IOL1WAY_OFF)
    #endif
    #if defined(__PIC24FJ128GA010__)
_CONFIG2(FNOSC_PRIPLL & POSCMOD_XT) // Primary XT OSC with PLL
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF)   // JTAG off, watchdog timer off
    #endif
	#if defined (__PIC24FJ256GB210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif
	#if defined (__PIC24FJ256DA210__)
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_XT & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 
	#endif	
#endif

/////////////////////////////////////////////////////////////////////////////
// SPI Channel settings
/////////////////////////////////////////////////////////////////////////////
#if defined (USE_SST25VF016)
    #ifdef __PIC32MX__
        const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 1, 0, 0, 1, 1, 0};
    #else    
        #if defined(__dsPIC33E__) 
            #if defined (MEB_BOARD)
                // since the SPI is muxed through the CPLD, we use a slower clock for MEB_BOARD
                const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
            #else
                const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 7, 0, 1, 1, 0};
            #endif
        #else
            const DRV_SPI_INIT_DATA SPI_Init_Data = {SST25_SPI_CHANNEL, 3, 6, 0, 1, 1, 0};
        #endif
    #endif
#elif defined (USE_MCHP25LC256)       
    const DRV_SPI_INIT_DATA SPI_Init_Data = {MCHP25LC256_SPI_CHANNEL, 6, 3, 0, 1, 1, 0};
#endif    

///////////////////////////// FONTS AND BITMAPS ///////////////////////////////
// This font is located in internal flash
extern const FONT_FLASH internalfont;

// This picture is located in internal flash
extern const IMAGE_FLASH   internalbitmap;

// External font and bitmap must be stored in external flash memory installed on
// Graphics PICTail Plus board 
// See "ExternalResource.h" included in MainDemo.h:
// FONT_EXTERNAL        externalfont;
// IMAGE_EXTERNAL      externalbitmap;

/////////////////////////////////////////////////////////////////////////////
// Function Prototypes
/////////////////////////////////////////////////////////////////////////////
void InitializeBoard(void);
void CheckExternalFlashHex();

#define DEMODELAY 	2000

#if defined (USE_COMM_PKT_MEDIA_USB)
    extern USB_HANDLE USBGenericInHandle;
#endif

/* */

int main(void)
{
    SHORT   width, height;

    InitializeBoard();

    // Initialize graphics
    InitGraph();
    // Initialize flash installed on Graphics PICTail Plus board 3
    FLASHInit((DRV_SPI_INIT_DATA *)&SPI_Init_Data);
    
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
    AD1PCFGL = 0xffff;
    #endif

    HardwareButtonInit();

    CheckExternalFlashHex();

    while(1)
    {

        // Display text with font located in internal flash
        // Set current font
        SetFont((void *) &internalfont);

        // Get text width and height
        width = GetTextWidth("Font from internal flash", (void *) &internalfont);
        height = GetTextHeight((void *) &internalfont);

        // Put green text in the center
        SetColor(BRIGHTGREEN);
        OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Font from internal flash");

        // Wait a couple of seconds and clean screen
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        // Display text with font located in external memory
        // Set current font
        SetFont((void *) &externalfont);

        // Get text width and height
        width = GetTextWidth("Font from external flash", (void *) &externalfont);
        height = GetTextHeight((void *) &externalfont);

        // Put red text in the center
        SetColor(BRIGHTRED);
        OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Font from external flash");

        // Wait a couple of seconds and clean screen
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// reset back the font to use internal font
        SetFont((void *) &internalfont);

        // Display picture located in internal memory
        // Get bitmap width and height
        width = GetImageWidth((void *) &internalbitmap);
        height = GetImageHeight((void *) &internalbitmap);

        // Put bitmap in the center
        PutImage((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, (void *) &internalbitmap, 1);

        // Display some white text
        SetColor(WHITE);
        OutTextXY(0, 0, "Bitmap from internal flash");

        // Wait a couple of seconds and clean screen
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        // Display picture located in external memory
        // Get bitmap width and height
        width = GetImageWidth((void *) &externalbitmap);
        height = GetImageHeight((void *) &externalbitmap);

        // Put bitmap in the center
        PutImage((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, (void *) &externalbitmap, 1);

        // Display some white text
        SetColor(WHITE);
        OutTextXY(0, 0, "Bitmap from external flash");

        // Wait a couple of seconds and clean screen
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();
    }
}
/////////////////////////////////////////////////////////////////////////////
// void InitializeBoard(void)
/////////////////////////////////////////////////////////////////////////////
void InitializeBoard(void)
{
     #if defined(PIC24FJ256DA210_DEV_BOARD)
    
    _ANSG8 = 0; /* S1 */
    _ANSE9 = 0; /* S2 */
    _ANSB5 = 0; /* S3 */
        
    #else

    /////////////////////////////////////////////////////////////////////////////
    // ADC Explorer 16 Development Board Errata (work around 2)
    // RB15 should be output
    /////////////////////////////////////////////////////////////////////////////
    #ifndef MEB_BOARD
    LATBbits.LATB15 = 0;
    TRISBbits.TRISB15 = 0;
    #endif

    #endif

    /////////////////////////////////////////////////////////////////////////////
    #if defined(__dsPIC33F__) || defined(__PIC24H__) || defined(__dsPIC33E__)

        // Configure Oscillator to operate the device at 40Mhz
        // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
        #if defined(__dsPIC33E__) 
            //Fosc = 8M * 60/(2*2) = 120MHz for 8M input clock
            PLLFBD = 58;                // M=60         
        #else
        	// Fosc= 8M*40(2*2)=80Mhz for 8M input clock
        	PLLFBD = 38;                // M=40
        #endif

        CLKDIVbits.PLLPOST = 0;         // N1=2
        CLKDIVbits.PLLPRE = 0;          // N2=2
        OSCTUN = 0;                     // Tune FRC oscillator, if FRC is used

        // Disable Watch Dog Timer
        RCONbits.SWDTEN = 0;

        // Clock switching to incorporate PLL
        __builtin_write_OSCCONH(0x03);  // Initiate Clock Switch to Primary
    
        // Oscillator with PLL (NOSC=0b011)
        __builtin_write_OSCCONL(0x01);  // Start clock switching
        while(OSCCONbits.COSC != 0b011);

        // Wait for Clock switch to occur	
        // Wait for PLL to lock
        while(OSCCONbits.LOCK != 1)
        { };     
    
        #if defined(__dsPIC33F__) || defined(__PIC24H__)
            // Set PMD0 pin functionality to digital
            AD1PCFGL = AD1PCFGL | 0x1000;

        #elif defined(__dsPIC33E__) || defined(__PIC24E__)
            ANSELE = 0x00;
            ANSELDbits.ANSD6 = 0;

		    // Set all touch screen related pins to Analog mode.
	        ANSELBbits.ANSB11 = 1; 	        
	        #ifdef MEB_BOARD
	        	// Configure auxiliary PLL for USB.
		        // Configuring the auxiliary PLL, since the primary
	    		// oscillator provides the source clock to the auxiliary
	    		// PLL, the auxiliary oscillator is disabled. Note that
	    		// the AUX PLL is enabled. The input 8MHz clock is divided
	    		// by 2, multiplied by 24 and then divided by 2. Wait till 
	    		// the AUX PLL locks.	
			    ACLKCON3 = 0x24C1;   
			    ACLKDIV3 = 0x7;
			    
			    ACLKCON3bits.ENAPLL = 1;
			    while(ACLKCON3bits.APLLCK != 1); 
			    // Power USB device.
			    ANSELBbits.ANSB5 = 0;
			    TRISBbits.TRISB5 = 0;
			    LATBbits.LATB5 = 1;	

		    #endif
        #endif
       
    #elif defined(__PIC32MX__)
        INTEnableSystemMultiVectoredInt();
        SYSTEMConfigPerformance(GetSystemClock());
    #endif

    #ifdef MEB_BOARD
        CPLDInitialize();
        CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
        CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
    #endif


    //The following are PIC device specific settings for the SPI channel used. 

    //Set IOs directions for SST25 SPI
    #if defined (GFX_PICTAIL_V3) || defined (MEB_BOARD) || defined (GFX_PICTAIL_V3E) || defined (GFX_PICTAIL_LCC)  
	    #if defined SST25_CS_ANS
		    SST25_CS_ANS = 0; // Change port to digital from analog.
        #endif   
        SST25_CS_LAT = 1;
        SST25_CS_TRIS = 0;
        #ifndef __PIC32MX__
            SST25_SCK_TRIS = 0;
            SST25_SDO_TRIS = 0;
            SST25_SDI_TRIS = 1;
            #if defined(__PIC24FJ256GB210__) 
            	SST25_SDI_ANS = 0;
            #elif defined(__dsPIC33E__) 
            	SST25_SDI_ANS = 0;
	    	    SST25_SCK_ANS = 0;
	    	    SST25_SDO_ANS = 0; 
	            SST25_SDI_ANS = 0;
    	    #endif
        #endif
    
    #elif defined (PIC24FJ256DA210_DEV_BOARD)
        SST25_CS_LAT = 1;
        SST25_CS_TRIS = 0;
    
        // Set the pins to be digital 
    	SST25_SDI_ANS = 0;
        SST25_SDO_ANS = 0;

        SST25_SCK_TRIS = 0;
        SST25_SDO_TRIS = 0;
        SST25_SDI_TRIS = 1;
	    
	#endif

    // set the peripheral pin select for the PSI channel used
    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        AD1PCFGL = 0xFFFF;
        RPOR9bits.RP18R = 11;                   // assign RP18 for SCK2
        RPOR8bits.RP16R = 10;                   // assign RP16 for SDO2
        RPINR22bits.SDI2R = 17;                 // assign RP17 for SDI2	
    #elif defined(__PIC24FJ256GB110__) || defined(__PIC24FJ256GA110__) || defined (__PIC24FJ256GB210__)
        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
        RPOR10bits.RP21R = 11;                  // assign RP21 for SCK2
        RPOR9bits.RP19R = 10;                   // assign RP19 for SDO2
        RPINR22bits.SDI2R = 26;                 // assign RP26 for SDI2
        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    #elif defined(__dsPIC33EP512MU810__) 
        // nothing to be done here since SPI2 is not remappable
    #elif defined(__PIC24FJ256DA210__)

        __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    	#if (SST25_SPI_CHANNEL == 1)
    	    RPOR1bits.RP2R = 8;                 // assign RP2 for SCK1
    	    RPOR0bits.RP1R = 7;                 // assign RP1 for SDO1
    	    RPINR20bits.SDI1R = 0;              // assign RP0 for SDI1
        #elif (SST25_SPI_CHANNEL == 2)
            RPOR1bits.RP2R = 11;                // assign RP2 for SCK2
    	    RPOR0bits.RP1R = 10;                // assign RP1 for SDO2
    	    RPINR22bits.SDI2R = 0;              // assign RP0 for SDI2
    	#endif

        __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    #endif
}

/////////////////////////////////////////////////////////////////////////////
// void CheckExternalFlashHex(void)
/////////////////////////////////////////////////////////////////////////////
void CheckExternalFlashHex()
{
    typedef struct 
    {
        UINT32 mchpSignature;
        UINT32 mchpCRCData;
    } CRC_CHECK; 

    CRC_CHECK externalCRC, expectedCRC;   
    WORD textHeight;
    void *pFont;
    XCHAR *pStr = NULL;
    BOOL  setProgram = FALSE;
    
    XCHAR   msgStr1[] = {'P','r','o','g','r','a','m',' ','E','x','t','e','r','n','a','l',' ','D','a','t','a',0};
    XCHAR   msgStr2[] = {'E','x','t','e','r','n','a','l',' ','d','a','t','a',' ','i','n','v','a','l','i','d','.',0};
    XCHAR   msgStr3[] = {'P','l','e','a','s','e',' ','s','e','n','d',' ','d','a','t','a',' ','u','s','i','n','g',0};
    XCHAR   msgStr4[] = {'"','E','x','t','e','r','n','a','l',' ','M','e','m','o','r','y',0};
    XCHAR   msgStr5[] = {'P','r','o','g','r','a','m','m','e','r','"',' ','u','t','i','l','i','t','y',0};
    XCHAR   msgStr6[] = {'N','o','w',' ','w','a','i','t','i','n','g',' ','f','o','r',' ','d','a','t','a',0};
#ifdef USE_COMM_PKT_MEDIA_USB
    XCHAR   msgStr7[] = {'v','i','a',' ','U','S','B','.','.','.',0};
#else
    XCHAR   msgStr7[] = {'v','i','a',' ','U','A','R','T','.','.','.',0};
#endif

    pFont = (void*) &internalfont;
    SetFont(pFont);
    textHeight = GetTextHeight(pFont);

    // check if the CRC matches the data stored in the external flash memory
    expectedCRC.mchpCRCData = GRC_CRC32_EXTERNAL_MARKER;
    expectedCRC.mchpSignature = 0x5048434D;                // this is "MCHP"

    // check if programming is prompted     
    if(GetHWButtonProgram() == HW_BUTTON_PRESS)
    {
        pStr = msgStr1;
        setProgram = TRUE;
	} 

    if (setProgram == FALSE)
    {
        ReadArray(GRC_CRC32_EXTERNAL_ADDR, (BYTE *)&externalCRC, 8);
    
        if  ((expectedCRC.mchpCRCData != externalCRC.mchpCRCData) || \
             (expectedCRC.mchpSignature != externalCRC.mchpSignature))
        {
            // expected and read CRC does not match, proceed to programming flash first
            // run the flash programming 
            pStr = msgStr2;
            setProgram = TRUE;
        }
    }
    
    if (setProgram == TRUE)
    {
        SetColor(BLACK);
        ClearDevice();
        SetColor(WHITE);
        OutTextXY(10,10                 , pStr);
        OutTextXY(10,10 + (textHeight*2), msgStr3);
        OutTextXY(10,10 + (textHeight*3), msgStr4);
        OutTextXY(10,10 + (textHeight*4), msgStr5);
        OutTextXY(10,10 + (textHeight*5), msgStr6);
        OutTextXY(10,10 + (textHeight*6), msgStr7);

        // Call the external flash programming routine
        ProgramFlash();

#if defined (USE_COMM_PKT_MEDIA_USB)

        // check if USB has sent reply to host then do a detach
        while(!USBHandleBusy(USBGenericInHandle));
        USBDeviceDetach();
        // delay the reset to have time for the acknowledge data to be sent to the host side
        DelayMs(100);

#elif defined (USE_COMM_PKT_MEDIA_SERIAL_PORT)

        // check if UART is still busy sending replies to the host
        while(U2STAbits.TRMT);
        // delay the reset to have time for the acknowledge data to be sent to the host side
        DelayMs(10);

#endif

        // Force Reset to force the checking of the flash memory if programming was a success
        Reset();
    }
    
}	


/*********************************************************************
* Function: WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
*
* PreCondition: none
*
* Input:  memory - pointer to the bitmap or font external memory structures
*                  (FONT_EXTERNAL or BITMAP_EXTERNAL)
*         offset - data offset
*         nCount - number of bytes to be transferred to the buffer
*         buffer - pointer to the buffer
*
* Output: number of bytes were transferred.
*
* Side Effects: none
*
* Overview: this function must be implemented in application. Graphics Library calls it
*           each time the data from external memory is required. The application must copy 
*           required data to the buffer provided.
*
* Note: none
*
********************************************************************/
// If there are several memories in the system they can be selected by IDs.
// In this demo ID for memory chip installed on Graphics PICTail board is assumed to be 0.
#define SST39_MEMORY    0
/* */

WORD ExternalMemoryCallback(IMAGE_EXTERNAL *memory, LONG offset, WORD nCount, void *buffer)
{
    if(memory->ID == SST39_MEMORY)
    {

        // Read data requested into buffer provided
        SST25ReadArray(memory->address + offset, // address to read from
        (BYTE *)buffer, nCount);
    }

    return (nCount);
}
