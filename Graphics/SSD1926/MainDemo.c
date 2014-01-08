/******************************************************************************
 * SSD1926 hardware JPEG decoder and SD card interface demo
 * The demo displays all JPEG files in the SD card root location.
 * The card must be inserted before the demo starts.
 *
 * The demo will also play video files formated as RGB565 for each frame .
 * Use ffmpeg to do the video conversion, an example ffmpeg command line is:
 * ffmpeg -i video.avi -s 320x240 -pix_fmt rgb565 video.rgb
 *
 * All display orientations are supported. Resolution of the images must be less than 2560x1920
 * (SSD1926 maximum hardware scale down factor is 8).
 *
 * FUNCTIONS IMPLEMENTED:
 *	void SSD1926SetRGB(void) - switch display to RGB,
 *  void SSD1926SetYUV(void) - switch display to YUV (most JPEG files have this format),
 *  BYTE JPEGPutImage(char* filename) - display JPEG file in the center.
 *
 * MUST BE IN PROJECT:
 *  FSIO.c - file system,
 *  DisplayDriver.c - graphics library,
 *  Primitive.c - graphics library,
 *  SSD1926_SDCard.c - SD card access functions for the file system,
 *  SSD1926_JPEG.c - JPEG decoder functions.
 ******************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    see included files below
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        C30 v2.01/C32 v0.00.18
 * Company:         Microchip Technology, Inc.
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
 *******************************************************************************/
/************************************************************
 * Includes
 ************************************************************/
#include "MainDemo.h"
#include "Graphics/SSD1926.h"

#if !(defined (GFX_PICTAIL_V3) || defined(MEB_BOARD))
    #error  Error: This demo does not work with the selected hardware setup.
#endif

/************************************************************
 * Configuration Bits
 ************************************************************/
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

/************************************************************
 * Defines
 ************************************************************/
#define START_ADD   0ul
#define FRAME_SIZE (320ul * 240ul * 2ul)
/************************************************************
 * Enumeration Structure
 ************************************************************/
typedef enum
{
    DEMO_FILE_TYPE_JPEG,
    DEMO_FILE_TYPE_RGB,
    DEMO_FILE_TYPE_OTHER
}DEMO_FILE_TYPE;

/************************************************************
 * Externs: Function Prototypes
 ************************************************************/
extern BYTE FILEget_next_cluster(FSFILE *fo, DWORD n);
extern DWORD Cluster2Sector(DISK * disk, DWORD cluster);
extern BYTE SDSectorDMARead(DWORD sector_addr, DWORD dma_addr, UINT16 num_blk);

/************************************************************
 * Function Prototypes
 ************************************************************/
DEMO_FILE_TYPE GetFileType(char *fileName);
void PlayRGB(const char * fileName);

/************************************************************
 * Variables
 ************************************************************/
DEMO_FILE_TYPE currentFileType;

/************************************************************
 * int main (void)
 ************************************************************/
int main (void)
{
    #if defined(__dsPIC33F__) || defined(__PIC24H__) || defined(__dsPIC33E__)

        // Configure Oscillator to operate the device at 40Mhz
        // Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
        #if defined(__dsPIC33E__) 
            //Fosc = 8M * 60/(2*2) = 120MHz for 8M input clock
            PLLFBD = 58;    			// M=60         
        #else
            // Fosc= 8M*40(2*2)=80Mhz for 8M input clock
            PLLFBD = 38;                    // M=40
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
    
    #elif defined(__PIC32MX__)
        SYSTEMConfig(GetSystemClock(), SYS_CFG_ALL);
        #ifdef MEB_BOARD
            CPLDInitialize();
            CPLDSetGraphicsConfiguration(GRAPHICS_HW_CONFIG);
            CPLDSetSPIFlashConfiguration(SPI_FLASH_CHANNEL);
        #endif
    #endif

    #if defined(__dsPIC33FJ128GP804__) || defined(__PIC24HJ128GP504__)
        AD1PCFGL = 0xffff;
    #elif defined(__dsPIC33E__) 
        ANSELE = 0x00;
        ANSELDbits.ANSD6 = 0;
    #endif
 
    InitGraph();                // Graphics
    FSInit();                   // File system

    currentFileType = DEMO_FILE_TYPE_RGB;
    
    while(1)
    {        
        SearchRec nextFile;
        if(-1 != FindFirst("*.*", ATTR_ARCHIVE, &nextFile))
        {
            do
            {
                if(GetFileType(nextFile.filename) == DEMO_FILE_TYPE_JPEG)   
                {
                    if(currentFileType != DEMO_FILE_TYPE_JPEG)
                    {
                        // Set YUV mode to display JPEG
                        SetColor(BLACK);    // Black in RGB is dark green in YUV
                        ClearDevice();
                        SSD1926SetYUV();    // Switching shows a little green flicker
                        currentFileType = DEMO_FILE_TYPE_JPEG;
                    }
                    
                    JPEGPutImage(nextFile.filename);
                    DelayMs(2000);
                }
                else if(GetFileType(nextFile.filename) == DEMO_FILE_TYPE_RGB)   
                {
                    if(currentFileType != DEMO_FILE_TYPE_RGB)
                    {
                        // Set YUV mode to display JPEG
                        SetColor(BLACK);    // Black in RGB is dark green in YUV
                        ClearDevice();
                        SSD1926SetRGB();    // Switching shows a little green flicker
                        currentFileType = DEMO_FILE_TYPE_RGB;
                    }
                    
                    PlayRGB(nextFile.filename);
                    
                }
                else
                {
                    // don't do anything for other file types
                }

            } while(-1 != FindNext(&nextFile));
        }

    }

    return (-1);
}
/************************************************************
 * DEMO_FILE_TYPE GetFileType(char *fileName)
 ************************************************************/
DEMO_FILE_TYPE GetFileType(char *fileName)
{
   
    while(*fileName != '.')
        fileName++;

    fileName++;

    // check to see if it is a jpeg
    if((fileName[0] == 'J') && (fileName[1] == 'P') && (fileName[2] == 'G'))
        return DEMO_FILE_TYPE_JPEG;

    if((fileName[0] == 'j') && (fileName[1] == 'p') && (fileName[2] == 'g'))
        return DEMO_FILE_TYPE_JPEG;

    // check to see if it is a rgb movie
    if((fileName[0] == 'R') && (fileName[1] == 'G') && (fileName[2] == 'B'))
        return DEMO_FILE_TYPE_RGB;

    if((fileName[0] == 'r') && (fileName[1] == 'g') && (fileName[2] == 'b'))
        return DEMO_FILE_TYPE_RGB;

    return DEMO_FILE_TYPE_OTHER;
}


/************************************************************
 * WORD StreamRGBFrame(FSFILE *fileStream, WORD numSectors)
 * This is a hack of the MDD file system to be able to use 
 * the SSD1926 SD Card DMA feature.
 ************************************************************/
WORD StreamRGBFrame(FSFILE *fileStream, WORD numSectors)
{
    DISK 		*dsk;
    DWORD       sec_sel;
    WORD       sectorsToRead;
    DWORD       currentCluster;
    DWORD       prevousCluster;
    static DWORD add = START_ADD;

    dsk    = (DISK *)fileStream->dsk;

    sec_sel = Cluster2Sector(dsk, fileStream->ccls);
    sec_sel += (WORD)fileStream->sec;      // add the sector number to it
    
    currentCluster = fileStream->ccls;
    prevousCluster = currentCluster;

    // This will be the minimum sectors that are available by the card to be read
    sectorsToRead = (WORD)dsk->SecPerClus - (WORD)fileStream->sec;

    // get as many sectors from clusters that are contiguous
    while(sectorsToRead < numSectors)
    {
        if( FILEget_next_cluster( fileStream, 1) != CE_GOOD )
            return 0xFFFF;
        
        if((prevousCluster + 1) != fileStream->ccls)
        {
            fileStream->ccls = prevousCluster;
            break;
        }

        prevousCluster++;
        sectorsToRead += dsk->SecPerClus;
    }
    
    // make sure that we are not over the bounds
    if(sectorsToRead > numSectors)
        sectorsToRead = numSectors;

    // do a DMA write of the information from the SD card to the display buffer
    if (!SDSectorDMARead(sec_sel,  add, sectorsToRead))
    {
        fileStream->ccls = currentCluster;
        return 0;
    }
    else
    {
        // update the address
        add += (dsk->sectorSize * (DWORD)sectorsToRead);
        
        if (add >= (FRAME_SIZE + START_ADD)) 
        {
            add = START_ADD;
        }
        
        // update the pointers
        fileStream->seek += (dsk->sectorSize * sectorsToRead);
        
        if (fileStream->seek > fileStream->size)
        {
            fileStream->seek = fileStream->size;
            return 0xFFFF;
        }
    }

    // get the current sector within the current cluster
    currentCluster = fileStream->sec + sectorsToRead;
    while(currentCluster > dsk->SecPerClus)
        currentCluster -= dsk->SecPerClus;

    fileStream->sec = currentCluster;
    
    // get a new cluster if necessary
    if( fileStream->sec == dsk->SecPerClus )
    {
        fileStream->sec = 0;
        if( FILEget_next_cluster( fileStream, 1) != CE_GOOD )
            return 0xFFFF;
    }

    return sectorsToRead;

}
/************************************************************
 * void PlayRGB(const char * fileName)
 ************************************************************/
void PlayRGB(const char * fileName)
{

	DWORD total_frames;
    WORD sectorsToWrite;
	FSFILE *pFile = NULL;
    int i;
    BYTE reg;

    reg = GetReg(REG_SPECIAL_EFFECTS);

    SetReg(REG_SPECIAL_EFFECTS, reg & 0xBF);

    pFile = FSfopen(fileName, "rb");

    total_frames = (pFile->size / FRAME_SIZE);

	SetColor(BLACK);
	ClearDevice();
	
	DisplayEnable();	// Aseert chip select to SSD1926

    for (i = 0; i < total_frames; i++) 
    {
        sectorsToWrite = FRAME_SIZE / pFile->dsk->sectorSize;

        while(sectorsToWrite)
        {
            WORD result = StreamRGBFrame(pFile, sectorsToWrite);

            if(result == 0xFFFF)
                break;

            sectorsToWrite -= result;
        }
    }

    // Clear screen back to black before changing the hardware SFR byte swap back
	SetColor(BLACK);
	ClearDevice();
	
    SetReg(REG_SPECIAL_EFFECTS, reg);

    DisplayDisable();       // De-assert chip select to SSD1926	
    
    FSfclose(pFile);
}
